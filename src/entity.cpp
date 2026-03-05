#include <raylib.h>
#include <cstddef>
#include "entity.h"

const int PLAYER_ID = 0;

void updateAnimation(Animation *self) {
  float dt = GetFrameTime();
  self->elapsedTime += dt;

  if (self->elapsedTime > self->frameTime) {
    self->elapsedTime = 0;
    self->frameIndex += 1;

    if (self->frameIndex >= self->frameCount) {
			self->frameIndex = self->oneshot ? self->frameIndex - 1 : 0;
    }
  }
}

Rectangle getAnimationFrame(Animation *self, EntityDirection direction) {
  return (Rectangle) {
		.x = (float)self->frameIndex * self->baseFrame.width,
		.y = self->baseFrame.y,
		.width = self->baseFrame.width * direction,
		.height = self->baseFrame.height,
	};
}

void resetAnimation(Animation *animation) {
	animation->frameIndex = animation->firstFrame;
	animation->elapsedTime = 0;
	animation->baseFrame.x = 0;
}


Entity& getPlayerEntity(Entity* entities) {
	for (int i = 0; i < 25; ++i) {
		if (entities[i].id == PLAYER_ID) {
			return entities[i];
		}
	}
	return entities[0];
}

Entity* LoadEntities() {
	Entity* entities = new Entity[25];
	
	// LOAD PLAYER ENTITY
	entities[PLAYER_ID] = Entity {
		.id = PLAYER_ID,
		.position = { .x=100, .y=0 },
		.zIndex = 100,
		.sprite = { .offset = {-33 * 2, -43 * 2}, .texture=LoadTexture("assets/sprites/foxy-sprites.png")},
		.collisionBox = new Box {
			.offset = { -18 * 2, -21 * 2 },
			.width = 16 * 4,
			.height = 21 * 4,
		},
		.interactionBox = NULL,
		.scale = 4,
		.action = NONE,
		.currentAnimation = 1,
		.animations = new Animation*[4],
		.sounds = new Sound[3],
		.gravityVelocity = 8,
		.direction = RIGHT,
	};
	
	// LOAD PLAYER ANIMATIONS
	entities[PLAYER_ID].animations[0] = new Animation{
		.frameIndex= 0,
		.firstFrame=0,
		.frameCount=4,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=false,
		.baseFrame={.x=0, .y=0, .width=33, .height=32}
	};
	entities[PLAYER_ID].animations[1] = new Animation{
		.frameIndex= 0,
		.firstFrame=0,
		.frameCount=6,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=false,
		.baseFrame={.x=0, .y=32, .width=33, .height=32}
	};
	entities[PLAYER_ID].animations[2] = new Animation{
		.frameIndex= 0,
		.firstFrame=0,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=0, .y=32*5, .width=33, .height=32}
	};
	entities[PLAYER_ID].animations[3] = new Animation{
		.frameIndex= 1,
		.firstFrame=1,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=33, .y=32*5, .width=33, .height=32}
	};
	
	// LOAD PLAYER SOUNDS
	entities[PLAYER_ID].sounds[0] = LoadSound("assets/sounds/jump.wav");
	entities[PLAYER_ID].sounds[1] = LoadSound("assets/sounds/walk-grass.wav");

	// LOAD PLATFORM ENTITIES
	Texture2D platformTexture1 = LoadTexture("assets/sprites/tile1.png");
	Texture2D platformTexture2 = LoadTexture("assets/sprites/tile2.png");
	for (int i = 1; i < 22; ++i){
		float scale = 2.3;
		float tileWidth = scale * 64;
		Vector2 position = {.x = (float) tileWidth * (i-1), .y = 670 };
		entities[i] = {
			.id = i,
			.position = position,
			.zIndex = 40,
			.sprite = { .offset = {-tileWidth/2, -36 * scale - 30}, .texture=i % 2 == 0 ? platformTexture1 : platformTexture2},
			.collisionBox = new Box {
				.offset = { -tileWidth/2, -36 * scale },
				.width = (float) tileWidth,
				.height = (float)(72 * scale) - 30,
			},
			.interactionBox = NULL,
			.scale = scale,
			.action = NONE,
			.currentAnimation = 0,
			.animations = NULL,
			.gravityVelocity = 0,
		};
	}

	float scale = 2.3;
	float tileWidth = scale * 64;
	entities[22] = {
		.id = 22,
		.position = {.x = 760, .y = 480 },
		.zIndex = 50,
		.sprite = { .offset = {-tileWidth/2, -36 * scale - 30}, .texture=LoadTexture("assets/sprites/tile1.png") },
		.collisionBox = new Box {
			.offset = { -tileWidth/2, -36 * scale },	
			.width = (float) tileWidth,
			.height = (float)(72 * scale) - 30,
		},
		.interactionBox = NULL,
		.scale = scale,
		.action = NONE,
		.currentAnimation = 0,
		.animations = NULL,
		.gravityVelocity = 0,
	};

	// LOAD OBJECTS ENTITIES
	entities[23] = {
		.id = 23,
		.position = {.x = 340, .y = 560 },
		.zIndex = 98,
		.sprite = { .offset = {-tileWidth*0.85f, -36 * 3.7}, .texture=LoadTexture("assets/sprites/gray-detonator.png") },
		.collisionBox = new Box {
			.offset = { -tileWidth*0.50f, -60.28f },
			.width = (float) tileWidth*0.8f,
			.height = (float)(90),
		},
		.interactionBox = new Box {
			.offset = { -tileWidth/2.83f, -132 },
			.width = (float) tileWidth*0.54f,
			.height = (float)(70.85f),
		},
		.scale = 3.1,
		.action = NONE,
		.currentAnimation = 0,
		.animations = new Animation*[2],
		.gravityVelocity = 0,
		.direction = RIGHT,	
	};
	entities[23].animations[0] = new Animation{
		.frameIndex= 0,
		.firstFrame=0,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=0, .y=18, .width=61, .height=66}
	};
	entities[23].animations[1] = new Animation{
		.frameIndex= 1,
		.firstFrame=1,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=0, .y=18, .width=61, .height=66}
	};

	entities[24] = {
		.id = 24,
		.position = {.x = 540, .y = 560 },
		.zIndex = 98,
		.sprite = { .offset = {-tileWidth*0.85f, -34 * 3.7}, .texture=LoadTexture("assets/sprites/danger-detonator.png") },
		.collisionBox = new Box {
			.offset = { -tileWidth*0.50f, -60.28f },
			.width = (float) tileWidth*0.8f,
			.height = (float)(90),
		},
		.interactionBox = new Box {
			.offset = { -tileWidth/2.83f, -132 },
			.width = (float) tileWidth*0.54f,
			.height = (float)(70.85f),
		},
		.scale = 3.1,
		.action = NONE,
		.currentAnimation = 0,
		.animations = new Animation*[2],
		.gravityVelocity = 0,
		.direction = RIGHT,	
	};
	entities[24].animations[0] = new Animation{
		.frameIndex= 0,
		.firstFrame=0,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=0, .y=18, .width=61, .height=66}
	};
	entities[24].animations[1] = new Animation{
		.frameIndex= 1,
		.firstFrame=1,
		.frameCount=1,
		.frameTime=0.2,
		.elapsedTime=0,
		.oneshot=true,
		.baseFrame={.x=0, .y=20, .width=61, .height=64}
	};
  return entities;
}
