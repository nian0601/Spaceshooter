#include "stdafx.h"
#include "ShootingComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"
#include "InputMessage.h"
#include "PostMaster.h"
#include "BulletMessage.h"
#include <FileWatcher.h>
#include <XMLReader.h>

#define PI 3.14159265359f

ShootingComponent::ShootingComponent()
{
	myWeapons.Init(4);
	ReadFromXML("Data/script/weapon.xml");
	WATCH_FILE("Data/script/weapon.xml", ShootingComponent::ReadFromXML);
	myCurrentWeaponID = 0;
}

ShootingComponent::~ShootingComponent()
{
}

void ShootingComponent::Update(float aDeltaTime)
{
	if (myWeapons[myCurrentWeaponID].myCurrentTime >= myWeapons[myCurrentWeaponID].myCoolDownTime)
	{
		myWeapons[myCurrentWeaponID].myCurrentTime = myWeapons[myCurrentWeaponID].myCoolDownTime;
	}
	else
	{
		myWeapons[myCurrentWeaponID].myCurrentTime += aDeltaTime;
	}
}

void ShootingComponent::ReceiveMessage(const ShootMessage&)
{
	if (myWeapons[myCurrentWeaponID].myCurrentTime == myWeapons[myCurrentWeaponID].myCoolDownTime)
	{
		CU::Matrix44<float> orientation = myEntity->myOrientation;
		orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f));

		if (myWeapons[myCurrentWeaponID].mySpread > 0)
		{
			float randomSpreadX = float((rand() % (myWeapons[myCurrentWeaponID].mySpread * 2)) - myWeapons[myCurrentWeaponID].mySpread) / 100.f;
			float randomSpreadY = float((rand() % (myWeapons[myCurrentWeaponID].mySpread * 2)) - myWeapons[myCurrentWeaponID].mySpread) / 100.f;
			
			CU::Matrix44<float> rotation;
			rotation.myMatrix[8] = randomSpreadX;
			rotation.myMatrix[9] = randomSpreadY;

			CU::Vector4<float> pos = orientation.GetPos();
			orientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
			orientation = rotation * orientation;
			orientation.SetPos(pos);
		}

		PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, orientation));
		myWeapons[myCurrentWeaponID].myCurrentTime = 0.f;
	}
}

//void ShootingComponent::ReceiveMessage(const InputMessage& aMessage)
//{
//	SetCurrentWeaponID(aMessage.GetKey());
//}

void ShootingComponent::Init(CU::Vector3<float> aSpawningPointOffset)
{
	mySpawningPointOffset = aSpawningPointOffset;
}

void ShootingComponent::ReadFromXML(const std::string aFilePath)
{
	XMLReader reader;
	reader.OpenDocument(aFilePath);

	tinyxml2::XMLElement* weaponElement = reader.FindFirstChild("weapon");
	for (; weaponElement != nullptr; weaponElement = reader.FindNextElement(weaponElement))
	{

		WeaponData weaponData;

		std::string type;
		reader.ReadAttribute(reader.FindFirstChild(weaponElement, "type"), "value", type);
		reader.ReadAttribute(reader.FindFirstChild(weaponElement, "cooldown"), "value", weaponData.myCoolDownTime);
		reader.ReadAttribute(reader.FindFirstChild(weaponElement, "spread"), "value", weaponData.mySpread);
		weaponData.myCurrentTime = weaponData.myCoolDownTime;

		if (type == "testGun")
		{
			weaponData.myBulletType = eBulletType::BOX_BULLET;
		}
		else if (type == "testGun2")
		{
			weaponData.myBulletType = eBulletType::SUPER_DEATH_LASER;
		}

		myWeapons.Add(weaponData);
	}
	myCurrentWeaponID = 0;
}