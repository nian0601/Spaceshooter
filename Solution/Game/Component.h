#pragma once
#include "Enums.h"

class Entity;

struct CollisionNote;
struct GUINote;
struct InputNote;
struct MissionNote;
struct PowerUpNote;
struct ShootNote;


class Component
{
public:
	Component(Entity& aEntity);

	virtual void Init();
	virtual void Update(float aDeltaTime);

	virtual void ReceiveNote(const CollisionNote& aMessage);
	virtual void ReceiveNote(const InputNote& aMessage);
	virtual void ReceiveNote(const MissionNote& aMessage);
	virtual void ReceiveNote(const PowerUpNote& aNote);
	virtual void ReceiveNote(const ShootNote& aMessage);
	virtual void ReceiveNote(const GUINote& aNote);

	Entity& GetEntity();

	static eComponentType GetType();

protected:
	void operator=(Component&) = delete;

	Entity& myEntity;
};

inline eComponentType Component::GetType()
{
	return eComponentType::NOT_USED;
}

inline Entity& Component::GetEntity()
{
	return myEntity;
}