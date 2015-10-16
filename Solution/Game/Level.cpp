#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "DirectionalLight.h"
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
#include "GUINote.h"
#include "HealthComponent.h"
#include "Instance.h"
#include <InputWrapper.h>
#include "InputComponent.h"
#include "Level.h"
#include "MissionManager.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "PhysicsComponent.h"
#include "PointLight.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PropComponent.h"
#include <Scene.h>
#include "ShieldComponent.h"
#include "ShootingComponent.h"
#include "SpawnEnemyMessage.h"
#include <SpotLight.h>
#include "WeaponFactory.h"
#include <XMLReader.h>

Level::Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper)
	: myEntities(16)
	, myComplete(false)
	, mySkySphere(nullptr)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_ENEMY, this);
	myScene = new Prism::Scene();
	myWeaponFactory = new WeaponFactory();
	myWeaponFactory->LoadWeapons("Data/Script/LI_list_weapon.xml");
	myWeaponFactory->LoadProjectiles("Data/Script/LI_list_projectile.xml");
	myEntityFactory = new EntityFactory(myWeaponFactory);
	myEntityFactory->LoadEntites("Data/Script/LI_list_entity.xml");
	myInputWrapper = aInputWrapper;
	myShowPointLightCube = false;

	myCollisionManager = new CollisionManager();
	myBulletManager = new BulletManager(*myCollisionManager, *myScene);
	myBulletManager->LoadFromFactory(myWeaponFactory, myEntityFactory, "Data/Script/LI_list_projectile.xml");

	myDirectionalLights.Init(4);
	myPointLights.Init(4);
	mySpotLights.Init(4);

	Prism::DirectionalLight* dirLight = new Prism::DirectionalLight();
	dirLight->SetColor({ 0.5f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, 0.5f, -1.f });
	myDirectionalLights.Add(dirLight);
	
	LoadPlayer();

	WATCH_FILE(aFileName, Level::ReadXML);

	ReadXML(aFileName);

	//Entity* cube = new Entity(eEntityType::PROP, *myScene, "this is a cube");
	//cube->AddComponent<GraphicsComponent>()->Init("Data/resources/model/Primitives/cube.fbx"
	//	, "Data/effect/NoTextureEffect.fx");
	//cube->myOrientation.SetPos({ 300.f, 300.f, 300.f, 1.f });
	//cube->Update(1.f / 30.f);
	//myEntities.Add(cube);

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i]->GetComponent<AIComponent>() != nullptr)
		{
			std::string targetName = myEntities[i]->GetComponent<AIComponent>()->GetTargetName();
			Entity* target = GetEntityWithName(targetName);
			myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(myPlayer);
			if (target != nullptr)
			{
				myEntities[i]->GetComponent<AIComponent>()->SetEntityToFollow(target);
			}
		}
	}

	myMissionManager->Init();
	myScene->SetCamera(myCamera);

	for (int i = 0; i < myDirectionalLights.Size(); ++i)
	{
		myScene->AddLight(myDirectionalLights[i]);
	}

	for (int i = 0; i < myPointLights.Size(); ++i)
	{
		myScene->AddLight(myPointLights[i]);
	}

	for (int i = 0; i < mySpotLights.Size(); ++i)
	{
		myScene->AddLight(mySpotLights[i]);
	}

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myScene->AddInstance(gfxComp->GetInstance());
		}
	}
	myRenderStuff = true;
}


Level::~Level()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_ENEMY, this);
	delete myCamera;
	myEntities.DeleteAll();

	delete mySkySphere;
	delete myEntityFactory;
	delete myWeaponFactory;
	delete myBulletManager;
	delete myCollisionManager;
	delete myMissionManager;
	delete myEventManager;

	myDirectionalLights.DeleteAll();
	myPointLights.DeleteAll();
	mySpotLights.DeleteAll();

	delete myScene;
	Prism::Engine::GetInstance()->GetFileWatcher()->Clear();
}

void Level::SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName)
{
	Prism::ModelProxy* skySphere = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		aModelFilePath, aEffectFileName);
	delete mySkySphere;
	mySkySphere = new Prism::Instance(*skySphere, mySkySphereOrientation, Prism::eOctreeType::NOT_IN_OCTREE);
}

bool Level::LogicUpdate(float aDeltaTime)
{
	myCollisionManager->CleanUp();

	if (myPlayer->GetAlive() == false)
	{
		return true;
	}

	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->GetAlive() == false)
		{
			GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

			if (gfxComp != nullptr)
			{
				myScene->RemoveInstance(gfxComp->GetInstance());
			}

			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(aDeltaTime);

		/*if (myEntities[i]->GetType() == eEntityType::POWERUP)
		{
			myPlayer->SendNote<WaypointNote>(WaypointNote(myEntities[i]->myOrientation.GetPos()));
		}*/

		if (myEntities[i]->GetType() == eEntityType::ENEMY)
		{
			myPlayer->SendNote<GUINote>(GUINote(myEntities[i]->myOrientation.GetPos(), eGUINoteType::ENEMY));
		}
	}

	mySkySphereOrientation.SetPos(myPlayer->myOrientation.GetPos());

	UpdateDebug();

	myCollisionManager->Update();
	myBulletManager->Update(aDeltaTime);
	myMissionManager->Update(aDeltaTime);
	myCamera->Update();
	return myComplete;
}

void Level::Render()
{
	Prism::Engine::GetInstance()->DisableZBuffer();
	mySkySphere->Render(*myCamera);
	Prism::Engine::GetInstance()->EnableZBuffer();

	if (myRenderStuff)
	{
		myScene->Render(myBulletManager->GetInstances());
	}

	myPlayer->GetComponent<GUIComponent>()->Render(Prism::Engine::GetInstance()->GetWindowSize(), myInputWrapper->GetMousePosition());


	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().x), CU::Vector2<float>(0, 0));
	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().y), CU::Vector2<float>(0, -30));
	Prism::Engine::GetInstance()->PrintDebugText(static_cast<float>(myPlayer->myOrientation.GetPos().z), CU::Vector2<float>(0, -60));

	Prism::Engine::GetInstance()->PrintDebugText(std::to_string(myPlayer->GetComponent<HealthComponent>()->GetHealth()), { 0, -100.f });
	Prism::Engine::GetInstance()->PrintDebugText(std::to_string(myPlayer->GetComponent<ShieldComponent>()->GetCurrentShieldStrength()), { 0, -120.f });

	Prism::Engine::GetInstance()->PrintDebugText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().x, { 0, -140.f });
	Prism::Engine::GetInstance()->PrintDebugText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().y, { 0, -160.f });
	Prism::Engine::GetInstance()->PrintDebugText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().z, { 0, -180.f });
}


void Level::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

Entity* Level::AddTrigger(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	Entity* newEntity = new Entity(eEntityType::TRIGGER, *myScene, Prism::eOctreeType::NOT_IN_OCTREE);
	float entityRadius;
	aReader.ForceReadAttribute(aElement, "radius", entityRadius);
	myEntityFactory->CopyEntity(newEntity, "trigger");

	newEntity->GetComponent<CollisionComponent>()->SetRadius(entityRadius);

	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "position");
	CU::Vector3<float> triggerPosition;
	aReader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
	aReader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
	aReader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
	newEntity->myOrientation.SetPos(triggerPosition*10.f);

	myEntities.Add(newEntity);
	myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::TRIGGER);

	return myEntities.GetLast();
}

void Level::ReadXML(const std::string& aFile)
{
	Sleep(10);
	XMLReader reader;
	reader.OpenDocument(aFile);
	tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("root");
	levelElement = reader.ForceFindFirstChild(levelElement, "scene");
	std::string skySphere;
	std::string cubeMap;
	std::string missionXML;
	std::string eventXML;

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "missionxml"), "source", missionXML);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "eventxml"), "source", eventXML);

	myMissionManager = new MissionManager(*this, *myPlayer, missionXML);
	myEventManager = new EventManager(eventXML);

	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "skysphere"), "source", skySphere);
	reader.ReadAttribute(reader.ForceFindFirstChild(levelElement, "cubemap"), "source", cubeMap);
	
	Prism::Engine::GetInstance()->GetEffectContainer()->SetCubeMap(cubeMap);

	SetSkySphere(skySphere, "Data/Resource/Shader/S_effect_skybox.fx");

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "directionallight"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "directionallight"))
	{
		tinyxml2::XMLElement* directionalElement = reader.ForceFindFirstChild(entityElement, "rotation");

		Prism::DirectionalLight* newDirLight = new Prism::DirectionalLight();

		CU::Vector3<float> lightDirection;
		reader.ForceReadAttribute(directionalElement, "X", lightDirection.x);
		reader.ForceReadAttribute(directionalElement, "Y", lightDirection.y);
		reader.ForceReadAttribute(directionalElement, "Z", lightDirection.z);
		newDirLight->SetDir(lightDirection);

		directionalElement = reader.ForceFindFirstChild(entityElement, "color");

		CU::Vector4<float> lightColor;
		reader.ForceReadAttribute(directionalElement, "R", lightColor.myR);
		reader.ForceReadAttribute(directionalElement, "G", lightColor.myG);
		reader.ForceReadAttribute(directionalElement, "B", lightColor.myB);
		reader.ForceReadAttribute(directionalElement, "A", lightColor.myA);
		newDirLight->SetColor(lightColor);

		//myScene->AddLight(newDirLight);
		myDirectionalLights.Add(newDirLight);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "prop"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "prop"))
	{
		Entity* newEntity = new Entity(eEntityType::PROP, *myScene, Prism::eOctreeType::STATIC);
		std::string propType;
		reader.ForceReadAttribute(entityElement, "propType", propType);
		myEntityFactory->CopyEntity(newEntity, propType);

		tinyxml2::XMLElement* propElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> propPosition;
		reader.ForceReadAttribute(propElement, "X", propPosition.x);
		reader.ForceReadAttribute(propElement, "Y", propPosition.y);
		reader.ForceReadAttribute(propElement, "Z", propPosition.z);
		newEntity->myOrientation.SetPos(propPosition*10.f);

		propElement = reader.ForceFindFirstChild(entityElement, "rotation");
		CU::Vector3<float> propRotation;
		reader.ForceReadAttribute(propElement, "X", propRotation.x);
		reader.ForceReadAttribute(propElement, "Y", propRotation.y);
		reader.ForceReadAttribute(propElement, "Z", propRotation.z);

		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundX(propRotation.x) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundY(propRotation.y) * newEntity->myOrientation;
		newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundZ(propRotation.z) * newEntity->myOrientation;

		newEntity->AddComponent<PropComponent>();

		int health = 30;
		newEntity->AddComponent<HealthComponent>()->Init(health);

		myEntities.Add(newEntity);
		myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::PROP);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "trigger"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "trigger"))
	{
		AddTrigger(reader, entityElement);
	}

	for (tinyxml2::XMLElement* entityElement = reader.FindFirstChild(levelElement, "powerup"); entityElement != nullptr;
		entityElement = reader.FindNextElement(entityElement, "powerup"))
	{
		Entity* newEntity = new Entity(eEntityType::POWERUP, *myScene, Prism::eOctreeType::STATIC);

		tinyxml2::XMLElement* triggerElement = reader.ForceFindFirstChild(entityElement, "position");
		CU::Vector3<float> triggerPosition;
		reader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
		reader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
		reader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
		newEntity->myOrientation.SetPos(triggerPosition*10.f);


		triggerElement = reader.ForceFindFirstChild(entityElement, "type");
		std::string powerUp;
		reader.ForceReadAttribute(triggerElement, "powerup", powerUp);
		std::string powerType = CU::GetSubString(CU::ToLower(powerUp).c_str(), '_', false);

		if (powerType == "healthkit")
		{
			newEntity->SetPowerUp(ePowerUpType::HEALTHKIT);
		}
		else if (powerType == "shield")
		{
			newEntity->SetPowerUp(ePowerUpType::SHIELDBOOST);
		}
		else if (powerType == "firerate")
		{
			newEntity->SetPowerUp(ePowerUpType::FIRERATEBOOST);
		}
		else if (powerType == "emp")
		{
			newEntity->SetPowerUp(ePowerUpType::EMP);
		}
		else
		{
			std::string errorMessage = "[Level] There is no powerup named " + powerType;
			DL_ASSERT(errorMessage.c_str());
		}

		myEntityFactory->CopyEntity(newEntity, powerUp);
		newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myPlayer);
		myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);

		//newEntity->AddComponent<PowerUpComponent>()->Init(newEntity->GetPowerUpType());

		myEntities.Add(newEntity);
	}
	reader.CloseDocument();
}

Entity* Level::GetEntityWithName(const std::string& aName)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (CU::ToLower(myEntities[i]->GetName()) == CU::ToLower(aName))
		{
			return myEntities[i];
		}
	}
	return nullptr;
}

int Level::GetEnemiesAlive() const
{
	return myCollisionManager->GetEnemiesAlive();
}


void Level::ReceiveMessage(const SpawnEnemyMessage& aMessage)
{
	Entity* newEntity = new Entity(eEntityType::ENEMY, *myScene, Prism::eOctreeType::DYNAMIC);
	myEntityFactory->CopyEntity(newEntity, aMessage.myType);

	newEntity->myOrientation.SetPos(aMessage.myPosition);


	newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundX(aMessage.myRotation.x) * newEntity->myOrientation;
	newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundY(aMessage.myRotation.y) * newEntity->myOrientation;
	newEntity->myOrientation = newEntity->myOrientation.CreateRotateAroundZ(aMessage.myRotation.z) * newEntity->myOrientation;

	newEntity->GetComponent<GraphicsComponent>()->SetScale(aMessage.myScale);

	myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::ENEMY);

	newEntity->GetComponent<AIComponent>()->SetEntityToFollow(myPlayer);
	myEntities.Add(newEntity);

	myScene->AddInstance(newEntity->GetComponent<GraphicsComponent>()->GetInstance());
}

void Level::LoadPlayer()
{
	Entity* player = new Entity(eEntityType::PLAYER, *myScene, Prism::eOctreeType::DYNAMIC);
	player->AddComponent<GraphicsComponent>()->Init("Data/Resource/Model/Player/SM_Cockpit.fbx"
		, "Data/Resource/Shader/S_effect_no_texture.fx");
	player->AddComponent<InputComponent>()->Init(*myInputWrapper);
	player->AddComponent<ShootingComponent>();
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("machineGun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("sniperGun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("plasmaGun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("shotgun"));
	player->GetComponent<ShootingComponent>()->AddWeapon(myWeaponFactory->GetWeapon("rocket"));
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
	myCollisionManager->Add(player->GetComponent<CollisionComponent>(), eEntityType::PLAYER);

	myEntities.Add(player);
	myCamera = new Prism::Camera(player->myOrientation);
	player->AddComponent<GUIComponent>()->SetCamera(myCamera);
	float maxMetersToEnemies = 0;
	reader.ReadAttribute(reader.ForceFindFirstChild("maxdistancetoenemiesinGUI"), "meters", maxMetersToEnemies);
	
	reader.CloseDocument();

	player->GetComponent<GUIComponent>()->Init(maxMetersToEnemies);
	//player->myOrientation.SetPos({ 306, 306, 306, 1 });
	myPlayer = player;
}

void Level::CompleteLevel()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::COMPLETE_LEVEL));
}

void Level::UpdateDebug()
{
	if (myInputWrapper->KeyDown(DIK_N) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(10);
	}
	if (myInputWrapper->KeyDown(DIK_M) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability(false);
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(myPlayer->GetComponent<HealthComponent>()->GetHealth());
	}
	if (myInputWrapper->KeyDown(DIK_V) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability(!myPlayer->GetComponent<HealthComponent>()->GetInvulnerability());
	}
	if (myInputWrapper->KeyDown(DIK_B) == true)
	{
		CompleteLevel();
	}
	if (myInputWrapper->KeyDown(DIK_C))
	{
		PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::RELOAD_LEVEL));
	}
	if (myInputWrapper->KeyDown(DIK_P))
	{
		Prism::Engine::GetInstance()->ToggleWireframe();
	}
	if (myInputWrapper->KeyDown(DIK_I))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Mute", 0);
	}
	if (myInputWrapper->KeyDown(DIK_U))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("UnMute", 0);
	}
	if (myInputWrapper->KeyDown(DIK_Y))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("LowerVolume", 0);
	}
	if (myInputWrapper->KeyDown(DIK_T))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("IncreaseVolume", 0);
	}
	if (myInputWrapper->KeyDown(DIK_L))
	{
		myPlayer->GetComponent<InputComponent>()->DisableMovement(2.f);
	}
	if (myInputWrapper->KeyDown(DIK_K))
	{
		myCollisionManager->DisableEnemiesWithinSphere(myPlayer->myOrientation.GetPos(), 100.f, 10.f);
	}
	if (myInputWrapper->KeyDown(DIK_J))
	{
		myPlayer->GetComponent<ShootingComponent>()->ActivateEMP();
	}
}