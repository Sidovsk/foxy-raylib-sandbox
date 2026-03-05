#include <raylib.h>
#include <cstddef>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "entity.h"
using namespace std;

bool DEV_MODE = false;
int TIME_LOC = 0;
int PREV_FRAME_LOC = 0;

struct ShaderManager {
	bool active;
	Shader shader;
};

Rectangle getBoxRec(Box box, Vector2 position) {
	return (Rectangle) {
		.x = position.x + box.offset.x,
		.y = position.y + box.offset.y,
		.width = box.width,
		.height = box.height,
	};
}

void drawBackground(Texture2D backgroundTexture, float parallaxFactor, Camera2D camera) {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int currentScreenStartX = camera.target.x - camera.offset.x;
	float ratio = (float)screenHeight / backgroundTexture.height;
	float currentX = -screenWidth;
	while(currentX < currentScreenStartX + screenWidth) {
		DrawTextureEx(backgroundTexture, Vector2 { .x= currentX + (camera.target.x * (1.0f - parallaxFactor)), .y=0}, 0.0f, ratio, WHITE);
		currentX += (float)backgroundTexture.width * ratio;
	}
}

void readInput(Entity* playerEntity) {
	if (IsKeyPressed(KEY_J)) {
		DEV_MODE = !DEV_MODE;
	}

	if (IsKeyPressed(KEY_Z)) {
		playerEntity->position = Vector2{ .x=100, .y=0 };
	}
	if (IsKeyPressed(KEY_SPACE) and playerEntity->onGround) {
		playerEntity->action = EntityAction::JUMP;
		return;
	}
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
		playerEntity->action = EntityAction::MOVE_LEFT;
		return;
	}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
		playerEntity->action = EntityAction::MOVE_RIGHT;
		return;
	}

	playerEntity->action = EntityAction::NONE;
}


void updateGameState(Entity* entities, ShaderManager* shaders) {
	Entity* player = &getPlayerEntity(entities);
	Vector2 playerNewPosition = Vector2 {player->position.x, player->position.y};
	int newAnimation;
	if (player->action == EntityAction::MOVE_LEFT) {
		playerNewPosition.x -= 6;
		player->direction = EntityDirection::LEFT;
		newAnimation = 1;
	} else if (player->action == EntityAction::MOVE_RIGHT) {
		playerNewPosition.x += 6;
		player->direction = EntityDirection::RIGHT;
		newAnimation = 1;
	}
	if(player->action == EntityAction::JUMP) {
		player->gravityVelocity = -25;
		player->onGround = false;
	}
	if (!player->onGround) {
		newAnimation = player->gravityVelocity <= 0 ? 2 : 3; 
		playerNewPosition.y += player->gravityVelocity;
		player->gravityVelocity += 1;
	}

	Rectangle playerCollision = getBoxRec(*player->collisionBox, playerNewPosition);
	Rectangle playerOldCollision = getBoxRec(*player->collisionBox, player->position);
	
	player->onGround = false;
	for (int i = 0; i < 25; ++i) {
		Entity* platform = &entities[i];
		if (platform->id == 0) {
			continue;
		}
		Rectangle platformCollision = getBoxRec(*platform->collisionBox, platform->position);
		if (CheckCollisionRecs(playerCollision, platformCollision)) {
			float feetTollerance = 0.1f;
			if (playerOldCollision.y + playerOldCollision.height - feetTollerance <= platformCollision.y) {
				// UP SIDE
				playerNewPosition.y = platformCollision.y  - playerOldCollision.height - player->collisionBox->offset.y + feetTollerance;
				player->onGround = true;
				player->gravityVelocity = 0;
				if (player->action != EntityAction::MOVE_LEFT and player->action != EntityAction::MOVE_RIGHT) {
					newAnimation = 0;
				}
			} else if (playerOldCollision.y > platformCollision.y + platformCollision.height) {
				// DOWN SIDE
				player->gravityVelocity = 0;
				playerNewPosition.y = platformCollision.y + platformCollision.height - player->collisionBox->offset.y;
			} else if (playerOldCollision.x + playerOldCollision.width <= platformCollision.x) {
				// LEFT SIDE
				playerNewPosition.x = platformCollision.x - playerCollision.width - player->collisionBox->offset.x;
			} else if (playerOldCollision.x >= platformCollision.x + platformCollision.width) {
				// RIGHT SIDE
				playerNewPosition.x = platformCollision.x + platformCollision.width - player->collisionBox->offset.x;
			} else {
				// UP SIDE
				playerNewPosition.y = platformCollision.y  - playerOldCollision.height - player->collisionBox->offset.y + feetTollerance;
				player->onGround = true;
				player->gravityVelocity = 0;
				if (player->action != EntityAction::MOVE_LEFT and player->action != EntityAction::MOVE_RIGHT) {
					newAnimation = 0;
				}
			}
		}
		if (platform->interactionBox != NULL) {
			Rectangle interactionBox = getBoxRec(*platform->interactionBox, platform->position);
			int shaderIndex = platform->id == 23 ? 0 : 1;
			if (CheckCollisionRecs(playerCollision, interactionBox)) {
				shaders[shaderIndex].active = true;
				resetAnimation(platform->animations[platform->currentAnimation]);
				platform->currentAnimation = 1;
			} else {
				shaders[shaderIndex].active = false;
				resetAnimation(platform->animations[platform->currentAnimation]);
				platform->currentAnimation = 0;
			}
		}
	}
	player->position = playerNewPosition;
	if (newAnimation != player->currentAnimation) {
		resetAnimation(player->animations[player->currentAnimation]);
		player->currentAnimation = newAnimation;
	}
}

void playSounds(Entity* entities) {
	Entity player = getPlayerEntity(entities);
	if (player.action == EntityAction::JUMP) {
		PlaySound(player.sounds[0]);
	}
	if ((player.action == EntityAction::MOVE_LEFT or player.action == EntityAction::MOVE_RIGHT) and player.onGround) {
		if (!IsSoundPlaying(player.sounds[1])) {
			PlaySound(player.sounds[1]);
		}
	}
}

void render(Entity* entities, Texture2D backTexture, Texture2D backMiddleTexture, RenderTexture2D* screen, ShaderManager* shaders) {
	Entity player = getPlayerEntity(entities);
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	Camera2D camera = Camera2D {
		.offset = Vector2 { (float)screenWidth/2, (float)screenHeight/2 },
		.target = Vector2 { player.position.x, (float)screenHeight/2 },
		.rotation = 0,
		.zoom = 1,
	};
	sort(entities, entities + 25, [](const Entity& a, const Entity& b) {
    return a.zIndex < b.zIndex;
	});
	bool hasActiveShader = false;
	for (int i = 0; i < 2; ++i) {
		if (shaders[i].active) {
			hasActiveShader = true;
			break;
		}
	}
	
  if(hasActiveShader) {
		BeginTextureMode(*screen);
	} else {
		BeginDrawing();
	}
		ClearBackground(BLACK);
		BeginMode2D(camera);
			drawBackground(backTexture, 0.5f, camera);
			drawBackground(backMiddleTexture, 0.7f, camera);
			for (int i = 0; i < 25; ++i) {
				Entity entity = entities[i];

				if (entity.animations) {
					Animation* animation = entity.animations[entity.currentAnimation];
					DrawTexturePro(
						entity.sprite.texture, 
						getAnimationFrame(animation, entity.direction), 
						Rectangle {
							entity.position.x + entity.sprite.offset.x, 
							entity.position.y + entity.sprite.offset.y, 
							animation->baseFrame.width * entity.scale, 	
							animation->baseFrame.height * entity.scale 
						},
						{ 0, 0 }, 
						1,
						WHITE
					);
					updateAnimation(animation);
				} else {
					DrawTextureEx(entity.sprite.texture, Vector2 {entity.position.x + entity.sprite.offset.x, entity.position.y + entity.sprite.offset.y}, 0.0f, entity.scale, WHITE);
				}
				if (DEV_MODE) {
					DrawCircle(entity.position.x, entity.position.y, 3, BLUE);
					Rectangle collisionBox = getBoxRec(*entity.collisionBox, entity.position);
					DrawRectangleLines(collisionBox.x, collisionBox.y, collisionBox.width, collisionBox.height, BLUE);
					if (entity.interactionBox != NULL) {
						Rectangle interactionBox = getBoxRec(*entity.interactionBox, entity.position);
						DrawRectangleLines(interactionBox.x, interactionBox.y, interactionBox.width, interactionBox.height, RED);
					}
				}
			}
		EndMode2D();
  if(hasActiveShader) {
		EndTextureMode();
	} else {
		EndDrawing();
	}
	static float time = 0.0f;
	if (hasActiveShader) {
		for (int i = 0; i < 2; ++i) {
			if (shaders[i].active) {
				BeginShaderMode(shaders[i].shader);
					if (i == 1) {
						time += GetFrameTime();
						SetShaderValue(shaders[i].shader, TIME_LOC, &time, SHADER_UNIFORM_FLOAT);
						SetShaderValueTexture(shaders[i].shader, PREV_FRAME_LOC, screen->texture);
					}
					BeginDrawing();
						DrawTextureRec(
							screen->texture,
							(Rectangle){0, 0,
								(float)screen->texture.width,
								-(float)screen->texture.height},
							(Vector2){0, 0},
							WHITE
						);
					EndDrawing();
				EndShaderMode();
			}
		}
	} else {
		time = 0.0f;
	}
}

int main() {
	InitWindow(1280, 720, "foxy-raylib-sandbox");
	InitAudioDevice();
	SetMasterVolume(1.0f);
	SetTargetFPS(60);
	Texture2D backgroundTexture = LoadTexture("assets/sprites/back.png");
	Texture2D backMiddleTexture = LoadTexture("assets/sprites/middle.png");
	RenderTexture2D screen = LoadRenderTexture(1280, 720);
	Entity* entities = LoadEntities();

	ShaderManager shaders[2];
	shaders[0] = ShaderManager { .active = false, .shader = LoadShader(0, "shaders/grayscale.fs")};
	shaders[1] = ShaderManager { .active = false, .shader = LoadShader(0, "shaders/fall.fs")};
	TIME_LOC = GetShaderLocation(shaders[1].shader, "time");
	PREV_FRAME_LOC = GetShaderLocation(shaders[1].shader, "prevFrame");

	while (!WindowShouldClose()) {
		readInput(&getPlayerEntity(entities));
		updateGameState(entities, shaders);
		playSounds(entities);
		render(entities, backgroundTexture, backMiddleTexture, &screen, shaders);
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
