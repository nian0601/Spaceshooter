#pragma once

class Entity;
class XMLReader;

namespace tinyxml2
{
	class XMLElement;
}

namespace Prism
{
	Scene;
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

	float myCollisionSphereRadius;
	float myDepth;
	float myHeight;
	float myMaxTime;
	float myWidth;

	int myChanceToFollow;

	unsigned short myLife;
	unsigned short myDamage;

	eEntityDataGraphicsType myGraphicsType;
};

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();

	void LoadEntites(const std::string& aEntityRootPath);

	void CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag);
private:
	void LoadEntity(const std::string& aEntityPath);

	void LoadAIComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aAIComponentElement);
	void LoadBulletComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aBulletComponentElement);
	void LoadCollisionComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aCollisionComponenetElement);
	void LoadGraphicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aGraphicsComponentElement);
	void LoadHealthComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aHealthComponentElement);
	void LoadShootingComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aShootingComponenetElement);
	void LoadPhysicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aPhysicsComponentElement);

	std::unordered_map<std::string, EntityData> myEntities;

	Prism::Scene* myDummyScene;
};

