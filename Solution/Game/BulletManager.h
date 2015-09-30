#pragma once
#include "Entity.h"
#include "BulletMessage.h"
#include "Subscriber.h"

struct BulletData // holds the data for one type of bullet
{
	CU::GrowingArray<Entity*> myBullets;
	eBulletType myType;
	float mySpeed;
	int myBulletCounter;
	int myMaxBullet;
};

namespace Prism
{
	class Camera;
	class Instance;
}

namespace tinyxml2
{
	class XMLElement;
}

class CollisionManager;

class BulletManager : public Subscriber
{
public:
	BulletManager();
	~BulletManager();

	void ReadFromXML(const std::string aFilePath);
	void Update(float aDeltaTime);
	void ReceiveMessage(const BulletMessage& aMessage) override;

	void SetCollisionManager(CollisionManager* aCollisionManager);

	CU::GrowingArray<Prism::Instance*>& GetInstances();

private:

	void ActivateBullet(BulletData* aWeaponData, const CU::Matrix44<float>& anOrientation);
	void UpdateBullet(BulletData* aWeaponData, const float& aDeltaTime);

	void DeleteWeaponData(BulletData* aWeaponData);

	CU::GrowingArray<BulletData*> myBulletDatas;
	BulletData* myMachinegunBulletData;
	BulletData* mySniperBulletData;
	BulletData* myPlasmaBulletData;

	CollisionManager* myCollisionManager;

	// temp!!
	CU::GrowingArray<Prism::Instance*> myInstances;

};

inline void BulletManager::SetCollisionManager(CollisionManager* aCollisionManager)
{
	myCollisionManager = aCollisionManager;
}