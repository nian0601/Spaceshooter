#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "Constants.h"
#include "ConversationManager.h"
#include "DefendMessage.h"
#include "EffectContainer.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include "EmitterComponent.h"
#include "EventManager.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "Instance.h"
#include <InputWrapper.h>
#include "Level.h"
#include "MissionManager.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PowerUpMessage.h"
#include "PropComponent.h"
#include <Scene.h>
#include "ShieldComponent.h"
#include "ShootingComponent.h"
#include "SpawnEnemyMessage.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

Level::Level(CU::InputWrapper* aInputWrapper)
	: myEntities(16)
	, myComplete(false)
	, mySkySphere(nullptr)
	, myEntityFactory(nullptr)
	, myWeaponFactory(nullptr)
	, myBulletManager(nullptr)
	, myCollisionManager(nullptr)
	, myMissionManager(nullptr)
	, myEventManager(nullptr)
	, myConversationManager(nullptr)
	, myEntityToDefend(nullptr)
{
	myInputWrapper = aInputWrapper;
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_ENEMY, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
}

Level::~Level()
{
	Prism::Engine::GetInstance()->GetModelLoader()->ClearLoadJobs();
	while (Prism::Engine::GetInstance()->GetModelLoader()->IsLoading() == true)
	{
	}

	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_ENEMY, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	delete myCamera;

	for (int i = 0; i < myEntities.Size(); i++)
	{
		if (myEntities[i] != myPlayer)
		{
			delete myEntities[i];
			myEntities[i] = nullptr;
		}
	}

	delete mySkySphere;
	delete myEntityFactory;
	delete myWeaponFactory;
	delete myBulletManager;
	delete myCollisionManager;
	delete myMissionManager;
	delete myEventManager;
	delete myConversationManager;
	delete myScene;
	mySkySphere = nullptr;
	myScene = nullptr;
	Prism::Engine::GetInstance()->GetFileWatcher()->Clear();

}

bool Level::LogicUpdate(float aDeltaTime)
{
	myCollisionManager->CleanUp();

	if (myPlayer->GetAlive() == false || myEntityToDefend != nullptr && myEntityToDefend->GetAlive() == false)
	{
		return true;
	}

	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->GetAlive() == false)
		{
			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(aDeltaTime);

		if (myEntities[i]->GetType() == eEntityType::ENEMY)
		{
			myPlayer->SendNote<GUINote>(GUINote(myEntities[i], eGUINoteType::ENEMY));
		}
	}

	mySkySphereOrientation.SetPos(myPlayer->myOrientation.GetPos());

	UpdateDebug();

	myCollisionManager->Update();
	myBulletManager->Update(aDeltaTime);
	myMissionManager->Update(aDeltaTime);
	myEventManager->Update();
	myCamera->Update();
	return myComplete;
}

void Level::Render()
{
	Prism::Engine::GetInstance()->DisableZBuffer();
	mySkySphere->Render(*myCamera);
	Prism::Engine::GetInstance()->EnableZBuffer();
	
	myScene->Render(myBulletManager->GetInstances());

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (myEntities[i]->GetComponent<EmitterComponent>() == nullptr)
		{
			continue;
		}
		else
		{
			myEntities[i]->GetComponent<EmitterComponent>()->Render();

			Prism::Engine::GetInstance()->PrintDebugText(myEntities[i]->GetComponent<EmitterComponent>()->GetEmitterCount()
				, { 1600, 0 });

		}

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

	newEntity->GetComponent<CollisionComponent>()->SetCollisionRadius(entityRadius);

	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "position");
	CU::Vector3<float> triggerPosition;
	aReader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
	aReader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
	aReader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
	newEntity->myOriginalOrientation.SetPos(triggerPosition*10.f);
	newEntity->myOrientation = newEntity->myOriginalOrientation;

	myEntities.Add(newEntity);
	myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::TRIGGER);

	return myEntities.GetLast();
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

Entity* Level::GetPlayer()
{
	return myPlayer;
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

void Level::ReceiveMessage(const PowerUpMessage& aMessage)
{
	if (aMessage.GetPowerupType() == ePowerUpType::WEAPON_UPGRADE)
	{
		myPlayer->GetComponent<ShootingComponent>()->UpgradeWeapon(myWeaponFactory->GetWeapon(aMessage.GetUprgade()), aMessage.GetUpgradeID());
	}
}

void Level::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myEntityToDefend = aMessage.myEntity;
	}
	else if (aMessage.myType == DefendMessage::eType::COMPLETE)
	{
		myEntityToDefend = nullptr;
	}
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
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(myPlayer->GetComponent<HealthComponent>()->GetHealth());
	}
	if (myInputWrapper->KeyDown(DIK_V) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability();
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
	if (myInputWrapper->KeyDown(DIK_J))
	{
		myPlayer->GetComponent<ShootingComponent>()->ActivateEMP();
	}
}