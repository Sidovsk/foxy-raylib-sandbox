enum EntityDirection {
	LEFT = -1,
	RIGHT = 1,
};

struct Sprite {
	Vector2 offset;
	Texture2D texture;
};

struct Box {
	Vector2 offset;
	float width;          
	float height;
};

struct Animation {
	int frameIndex;
	int firstFrame;
	int frameCount;
	float frameTime;
	float elapsedTime;
	bool oneshot;
	Rectangle baseFrame;
};

enum EntityAction {
	NONE,
	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP,
};

struct Entity {
	int id;
	Vector2 position;
	int zIndex;
	Sprite sprite;
	Box* collisionBox;
	Box* interactionBox;
	float scale;
	EntityAction action;
	int currentAnimation;
	Animation** animations;
	Sound* sounds;
	int gravityVelocity;
	bool onGround;
	EntityDirection direction;
};

Entity* LoadEntities();
Entity& getPlayerEntity(Entity* entities);
void updateAnimation(Animation *self);
Rectangle getAnimationFrame(Animation *self, EntityDirection direction);
void resetAnimation(Animation *animation);
