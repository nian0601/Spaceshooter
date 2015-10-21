#include "stdafx.h"
#include "AIComponent.h"
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "ConversationManager.h"
#include "DefendMessage.h"
#include "DirectionalLight.h"
#include "HealthComponent.h"
#include <EmitterData.h>
#include <EmitterInstance.h>
#include "EffectContainer.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include "EventManager.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "InputComponent.h"
#include "Instance.h"
#include "LevelFactory.h"
#include "Level.h"
#include "ModelLoader.h"
#include "MissionManager.h"
#include "PointLight.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PropComponent.h"
#include "PhysicsComponent.h"
#include <Scene.h>
#include "ShieldComponent.h"
#include "ShootingComponent.h"
#include <SpotLight.h>
#include "WeaponFactory.h"
#include <XMLReader.h>

LevelFactory::LevelFactory(const std::string& aLevelListPath, CU::InputWrapper* anInputWrapper)
	: myInputWrapper(anInputWrapper)
	, myCurrentLevel(nullptr)
	, myLevelPaths(8)
	, myCurrentID(0)
	, myDirectionalLights(4)
	, myPointLights(4)
	, mySpotLights(4)
{
	LoadLevelListFromXML(aLevelListPath);
}

LevelFactory::~LevelFactory()
{
}

Level* LevelFactory::LoadLevel(const int& anID)
{
	if (myLevelPaths.find(anID) == myLevelPaths.end())
	{
		DL_ASSERT("[LevelFactory] Non-existing ID in LoadLevel! ID must correspond with levelList.xml");
	}
	myCurrentID = anID;

	return LoadCurrentLevel();
}

Level* LevelFactory::LoadCurrentLevel()
{
	delete myCurrentLevel;
	myCurrentLevel = nullptr;

	myCurrentLevel = new Level(myInputWrapper);
	ReadXML(myLevelPaths[myCurrentID]);

	return myCurrentLevel;
}

Level* LevelFactory::LoadNextLevel()
{
	if (IsLastLevel() == true)
	{
		return myCurrentLevel;
	}

	return LoadLevel(myCurrentID + 1);
}

bool LevelFactory::IsLastLevel() const
{
	if (myLevelPaths.find(myCurrentID + 1) == myLevelPaths.end())
	{
		return true;
	}
	return false;
}

void LevelFactory::LoadLevelListFromXML(const std::string& aXMLPath)
{
	myLevelPaths.clear();
	XMLReader reader;
	reader.OpenDocument(aXMLPath);
	std::string levelPath = "";
	int ID = 0;
	int lastID = ID - 1;

	tinyxml2::XMLElement* levelElement = reader.FindFirstChild("level");
	for (; levelElement != nullptr; levelElement = reader.FindNextElement(levelElement))
	{
		lastID = ID;
		
		reader.ForceReadAttribute(levelElement, "ID", ID);
		reader.ForceReadAttribute(levelElement, "path", levelPath);
		myLevelPaths[ID] = levelPath;

		if (ID - 1 != lastID)
		{
			DL_ASSERT("[LevelFactory] Wrong ID-number in levelList.xml! The numbers should be counting up, in order.");
		}
	}
	reader.CloseDocument();
}

void LevelFactory::ReadXML(const std::string& aFilePath)
{
	myCurrentLevel->myScene = new Prism::Scene();
	myCurrentLevel->myWeaponFactory = new WeaponFactory();
	myCurrentLevel->myWeaponFactory->LoadWeapons("Data/Script/LI_list_weapon.xml");
	myCurrentLevel->myWeaponFactory->LoadProjectiles("Data/Script/LI_list_projectile.xml");
	myCurrentLevel->myEntityFactory = new EntityFactory(myCurrentLevel->myWeaponFactory);
	myCurrentLevel->myEntityFactory->LoadEntites("Data/Script/LI_list_entity.xml");
	myCurrentLevel->myCollisionManager = new CollisionManager();
	myCurrentLevel->myBulletManager = new BulletManager(*myCurrentLevel->myCollisionManager, *myCurrentLevel->myScene);
	myCurrentLevel->myBulletManager->LoadFromFactory(myCurrentLevel->myWeaponFactory, myCurrentLevel->myEntityFactory, "Data/Script/LI_list_projectile.xml");

	myDirectionalLights.DeleteAll();
	myPointLights.DeleteAll();
	mySpotLights.DeleteAll();

	LoadPlayer();

	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFilePath);
	tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("root");
	levelElement = reader.ForceFindFirstChild(levelElement, "scene");
	std::string skySphere;
	std::string cubeMap;
	std::string missionXML;
	std::string eventXML;
	std::string conversationXML;

	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "missionxml"), "source", missionXML);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "eventxml"), "source", eventXML);
	reader.ForceReadAttribute(reader.ForceFindFirstChild(levelElement, "conversationxml"), "source", conversationXML);

	myCurrentLevel->myConversationManager = new ConversationManager(conversationXML);
	myCurrentLevel->myMissionManager = new MissionManager(*myCurrentLevel, *myCurrentLevel->myPlayer, missionXML);
	myCurrentLevel->myEventManager = new EventManager(eventXML, *myCurrentLevel->myConversationManager);

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "skysphere"), "source", skySphere);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "cubemap"), "source", cubeMap);

	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap(cubeMap);

	SetSkySphere(skySphere, "Data/Resource/Shader/S_effect_skybox.fx");

	LoadLights(reader, levelElement);
	LoadProps(reader, levelElement);
	LoadTriggers(reader, levelElement);
	LoadPowerups(reader, levelElement);
	
	reader.CloseDocument();

	Prism::EmitterData data;
	data.LoadDataFile("Data/Resource/Particle/P_default_emitter.xml");

	myCurrentLevel->myEmitter = new Prism::EmitterInstance();
	myCurrentLevel->myEmitter->Initiate(data);
	myCurrentLevel->myEmitter->SetPosition({ 5, 5, 5 });

	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		if (myCurrentLevel->myEntities[i]->GetComponent<AIComponent>() != nullptr)
		{
			std::string targetName = myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->GetTargetName();
			Entity* target = myCurrentLevel->GetEntityWithName(targetName);
			myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(myCurrentLevel->myPlayer);
			if (target != nullptr)
			{
				myCurrentLevel->myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(target);
			}
		}
	}

	AddToScene();

	myCurrentLevel->myMissionManager->Init();
}

void LevelFactory::LoadLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	LoadDirectionalLights(aReader, aLevelElement);



}

void LevelFactory::LoadDirectionalLights(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "directionallight"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "directionallight"))
	{
		tinyxml2::XMLElement* directionalElement = aReader.ForceFindFirstChild(entityElement, "rotation");

		Prism::DirectionalLight* newDirLight = new Prism::DirectionalLight();

		CU::Vector3<float> lightDirection;
		aReader.ForceReadAttribute(directionalElement, "X", lightDirection.x);
		aReader.ForceReadAttribute(directionalElement, "Y", lightDirection.y);
		aReader.ForceReadAttribute(directionalElement, "Z", lightDirection.z);
		newDirLight->SetDir(lightDirection);

		directionalElement = aReader.ForceFindFirstChild(entityElement, "color");

		CU::Vector4<float> lightColor;
		aReader.ForceReadAttribute(directionalElement, "R", lightColor.myR);
		aReader.ForceReadAttribute(directionalElement, "G", lightColor.myG);
		aReader.ForceReadAttribute(directionalElement, "B", lightColor.myB);
		aReader.ForceReadAttribute(directionalElement, "A", lightColor.myA);
		newDirLight->SetColor(lightColor);

		myDirectionalLights.Add(newDirLight);
	}
}

void LevelFactory::LoadProps(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "prop"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity(eEntityType::PROP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		aReader.ForceReadAttribute(entityElement, "propType", propType);
		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, propType);

		std::string defendName;
		aReader.ReadAttribute(entityElement, "defendName", defendName);
		defendName = CU::ToLower(defendName);

		tinyxml2::XMLElement* propElement = aReader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> propPosition;
		aReader.ForceReadAttribute(propElement, "X", propPosition.x);
		aReader.ForceReadAttribute(propElement, "Y", propPosition.y);
		aReader.ForceReadAttribute(propElement, "Z", propPosition.z);
		newEntity->myOrientation.SetPos(propPosition*10.f);

		propElement = aReader.ForceFindFirstChild(entityElement, "rotation");
		CU::Vector3<float> propRotation;
		aReader.ForceReadAttribute(propElement, "X", propRotation.x);
		aReader.ForceReadAttribute(propElement, "Y", propRotation.y);
		aReader.ForceReadAttribute(propElement, "Z", propRotation.z);

		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundX(propRotation.x) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundY(propRotation.y) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundZ(propRotation.z) * newEntity->myOrientation;

		newEntity->AddComponent<PropComponent>()->Init(defendName);

		int health = 30;
		newEntity->AddComponent<HealthComponent>()->Init(health);

		myCurrentLevel->myEntities.Add(newEntity);
		myCurrentLevel->myCollisionManager->Add(myCurrentLevel->myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::PROP);
	}
}

void LevelFactory::LoadTriggers(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "trigger"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "trigger"))
	{
		myCurrentLevel->AddTrigger(aReader, entityElement);
	}
}

void LevelFactory::LoadPowerups(XMLReader& aReader, tinyxml2::XMLElement* aLevelElement)
{
	for (tinyxml2::XMLElement* entityElement = aReader.FindFirstChild(aLevelElement, "powerup"); entityElement != nullptr;
		entityElement = aReader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myCurrentLevel->myScene, Prism::eOctreeType::STATIC);

		tinyxml2::XMLElement* powerUpElement = aReader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> powerUpPosition;
		aReader.ForceReadAttribute(powerUpElement, "X", powerUpPosition.x);
		aReader.ForceReadAttribute(powerUpElement, "Y", powerUpPosition.y);
		aReader.ForceReadAttribute(powerUpElement, "Z", powerUpPosition.z);
		newEntity->myOrientation.SetPos(powerUpPosition*10.f);

		powerUpElement = aReader.ForceFindFirstChild(entityElement, "type");
		std::string powerUp;
		aReader.ForceReadAttribute(powerUpElement, "powerup", powerUp);

		myCurrentLevel->myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myCurrentLevel->myPlayer);
		myCurrentLevel->myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);

		myCurrentLevel->myEntities.Add(newEntity);
	}
}

void LevelFactory::LoadPlayer()
{
	Entity* player = new Entity(eEntityType::PLAYER, *myCurrentLevel->myScene, Prism::eOctreeType::DYNAMIC);
	player->AddComponent<GraphicsComponent>()->Init("Data/Resource/Model/Player/SM_Cockpit.fbx"
		, "Data/Resource/Shader/S_effect_pbl.fx");
	player->AddComponent<InputComponent>()->Init(*myCurrentLevel->myInputWrapper);
	player->AddComponent<ShootingComponent>();
	player->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("machineGun1"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("shotgun1"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myCurrentLevel->myWeaponFactory->GetWeapon("rocket1"));
	player->GetComponent<ShootingComponent>()->SetCurrentWeaponID(0);
	player->AddComponent<CollisionComponent>()->Initiate(7.5f);
	player->AddComponent<ShieldComponent>()->Init();
	player->AddComponent<PhysicsComponent>()->Init(1, { 0, 0, 0 });

	XMLReader reader;
	reader.OpenDocument("Data/Setting/SET_player.xml");
	int health = 0;
	bool invulnerable = false;
	reader.ReadAttribute(reader.FindFirstChild("life"), "value", health);
	reader.ReadAttribute(reader.FindFirstChild("life"), "invulnerable", invulnerable);

	player->AddComponent<HealthComponent>()->Init(health, invulnerable);
	myCurrentLevel->myCollisionManager->Add(player->GetComponent<CollisionComponent>(), eEntityType::PLAYER);

	myCurrentLevel->myEntities.Add(player);
	myCurrentLevel->myCamera = new Prism::Camera(player->myOrientation);
	player->AddComponent<GUIComponent>()->SetCamera(myCurrentLevel->myCamera);
	float maxMetersToEnemies = 0;
	reader.ReadAttribute(reader.ForceFindFirstChild("maxdistancetoenemiesinGUI"), "meters", maxMetersToEnemies);

	reader.CloseDocument();

	player->GetComponent<GUIComponent>()->Init(maxMetersToEnemies);
	myCurrentLevel->myPlayer = player;
}

void LevelFactory::AddToScene()
{
	myCurrentLevel->myScene->SetCamera(myCurrentLevel->myCamera);

	for (int i = 0; i < myDirectionalLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myDirectionalLights[i]);
	}

	for (int i = 0; i < myPointLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(myPointLights[i]);
	}

	for (int i = 0; i < mySpotLights.Size(); ++i)
	{
		myCurrentLevel->myScene->AddLight(mySpotLights[i]);
	}

	for (int i = 0; i < myCurrentLevel->myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myCurrentLevel->myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myCurrentLevel->myScene->AddInstance(gfxComp->GetInstance());
		}
	}
}

void LevelFactory::SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName)
{
	Prism::ModelProxy* skySphere = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		aModelFilePath, aEffectFileName);
	delete myCurrentLevel->mySkySphere;
	myCurrentLevel->mySkySphere = new Prism::Instance(*skySphere, 
		myCurrentLevel->mySkySphereOrientation, Prism::eOctreeType::NOT_IN_OCTREE);
}