#pragma once

enum class eMessageType
{
	ACTIVATE_BULLET,
	FADE,
	GAME_STATE,
	SPAWN_ENEMY,
	START_EVENT,
	ENEMY_KILLED,
	POWER_UP,
	CONVERSATION,
	EVENT_QUEUE_EMPTY,
	DEFEND,
	RESIZE,
	BULLET_COLLISION_TO_GUI,
	SPAWN_POWERUP,
	KILL_STRUCTURE,
	DESTORY_EMITTER,
	EMP,
	SPAWN_EXPLOSION_ON_UNIT_DEATH,
	SPAWN_EXPLOSION_ON_ASTROID_DEATH,
	SPAWN_EXPLOSION_ON_PROP_DEATH,
	SPAWN_EXPLOSION_ON_ROCKET_DEATH,
	SPAWN_EFFECT_ON_HIT,
	LEVEL_SCORE,
	SPAWN_EFFECT_ON_ASTROID_HIT,
	SPAWN_ON_FINAL_STRUCTURE_1,
	SPAWN_ON_FINAL_STRUCTURE_2,
	SPAWN_ON_FINAL_STRUCTURE_3,
	COUNT,
};

enum class eBulletType
{
	MACHINGUN_BULLET_LEVEL_1,
	MACHINGUN_BULLET_LEVEL_2,
	MACHINGUN_BULLET_LEVEL_3,
	SHOTGUN_BULLET_LEVEL_1,
	SHOTGUN_BULLET_LEVEL_2,
	SHOTGUN_BULLET_LEVEL_3,
	ROCKET_MISSILE_LEVEL_1,
	ROCKET_MISSILE_LEVEL_2,
	ROCKET_MISSILE_LEVEL_3,
	ENEMY_BULLET_DEFAULT,
	ENEMY_BULLET_FAST,
	ENEMY_BULLET_SLOW,
	ENEMY_BULLET_TURRET,
	ENEMY_BULLET_HOMING,
	COUNT,
};

enum eEntityType //Collision manager needs this as ints!
{
	NOT_USED = -1,
	PLAYER = 1,
	ENEMY = 2,
	PLAYER_BULLET = 4,
	ENEMY_BULLET = 8,
	TRIGGER = 16,
	PROP = 32,
	POWERUP = 64,
	DEFENDABLE = 128,
	STRUCTURE = 256,
	EMP = 512,
	ALLY = 1024,
	ALLY_BULLET = 2048,
};

enum class eCollisionType
{
	NORMAL,
	PLANET,
};

enum class ePowerUpType
{
	NO_POWERUP,
	FIRERATEBOOST,
	SHIELDBOOST,
	HEALTHKIT,
	WEAPON_UPGRADE,
	EMP,
	HOMING,
	INVULNERABLITY
};

enum class eMissionType
{
	WAYPOINT,
	KILL_ALL,
};

enum class eMissionCategory
{
	REQUIRED,
	NOT_REQUIRED,
	DUMMY
};

enum class eMissionEvent
{
	START,
	END,
};

enum class eGUINoteType
{
	WAYPOINT,
	ENEMY,
	POWERUP,
	STEERING_TARGET,
	HOMING_TARGET
};

enum class eSoundNoteType
{
	PLAY,
	STOP
};

enum class eComponentType
{
	NOT_USED,
	AI,
	BULLET,
	BULLET_AI,
	COLLISION,
	GRAPHICS,
	GUI,
	HEALTH,
	INPUT,
	PHYSICS,
	POWERUP,
	PROP,
	SHEILD,
	SHOOTING,
	WAY_POINT,
	PARTICLE_EMITTER,
	STREAK_EMITTER,
	SOUND,
	POINT_LIGHT,
	_COUNT,
};

enum class eAITargetPositionMode
{
	NOT_USED,
	KEEP_DISTANCE,
	ESCAPE_THEN_RETURN,
	KAMIKAZE,
	MINE,
	TURRET
};

enum class eDifficult
{
	EASY,
	NORMAL,
	HARD,
	_COUNT
};

enum class eLevelScoreMessageType
{
	PLAYER_SHOT,
	PLAYER_HIT_ENEMY,
	OPTIONAL_MISSION_ADDED,
	OPTIONAL_MISSION_COMPLETED
};