#pragma once

class Entity;
class WeaponFactory;
class XMLReader;

namespace tinyxml2
{
	class XMLElement;
}

namespace Prism
{
	class Scene;
	enum class eOctreeType;
}

enum class eEntityDataGraphicsType
{
	MODEL,
	CUBE
};


struct EntityData
{
	EntityData(Prism::Scene& aDummyScene);

	Entity* myEntity;

	std::string myEffectFile;
	std::string myModelFile;
	std::string myTargetName;
	std::string myWeaponType;

	CU::Vector3f myScale;

	float myCollisionSphereRadius;
	float myDepth;
	float myHeight;
	float myMaxTime;
	float myMinSpeed;
	float myMaxSpeed;
	float myMinTimeToNextDecision;
	float myMaxTimeToNextDecision;
	float myWidth;

	int myChanceToFollow;

	int myLife;
	int myDamage;

	float myDuration;
	int myShieldStrength;
	int myHealthToRecover; 
	int myFireRateMultiplier;
	ePowerUpType myPowerUpType;

	eEntityDataGraphicsType myGraphicsType;
	Prism::eOctreeType myType;
};

class EntityFactory
{
public:
	EntityFactory(WeaponFactory* aWeaponFactory);
	~EntityFactory();

	void LoadEntites(const std::string& aEntityRootPath);

	void CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag);
	void ReloadEntity(const std::string&);
private:
	void LoadEntity(const std::string& aEntityPath);

	void LoadAIComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aAIComponentElement);
	void LoadBulletComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aBulletComponentElement);
	void LoadCollisionComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aCollisionComponenetElement);
	void LoadGraphicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aGraphicsComponentElement);
	void LoadHealthComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aHealthComponentElement);
	void LoadShootingComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aShootingComponenetElement);
	void LoadPhysicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aPhysicsComponentElement);
	void LoadPowerUpComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aPowerUpComponent);

	std::unordered_map<std::string, EntityData> myEntities;

	Prism::Scene* myDummyScene;
	WeaponFactory* myWeaponFactoryPointer;
};

