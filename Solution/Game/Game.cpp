#include "stdafx.h"

#include <Camera.h>
#include <DebugDataDisplay.h>
#include <DirectionalLight.h>
#include "Entity.h"
#include <Font.h>
#include "Game.h"
#include <GeometryGenerator.h>
#include "GraphicsComponent.h"
#include <GeometryGenerator.h>
#include "InputComponent.h"
#include <InputWrapper.h>
#include <Instance.h>
#include <Model.h>
#include "Player.h"
#include <PointLight.h>
#include <Scene.h>
#include <Text.h>
#include <TimerManager.h>
#include <VTuneApi.h>



Game::Game()
{
	myInputWrapper = new CU::InputWrapper();
	myPlayer = new Player(*myInputWrapper);
	myCamera = new Prism::Camera(myPlayer->GetOrientation());
}

Game::~Game()
{
	delete myCamera;
	delete myInputWrapper;
	delete myScene;
	myEntities.DeleteAll();
}

bool Game::Init(HWND& aHwnd)
{
	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myPointLight = new Prism::PointLight();
	myPointLight->SetColor({ 1.f, 0.f, 0.f, 1.f });
	myPointLight->SetPosition({ 0.f, 0.f, 0.f, 1.f });
	myPointLight->SetRange(50.f);
	myEntities.Init(4);
	
	//Entity* cube = new Entity();
	//cube->AddComponent<GraphicsComponent>()->Init("Data/resources/model/companion/companion.fbx"
	//	, "Data/effect/BasicEffect.fx");
	//cube->AddComponent<InputComponent>()->Init(*myInputWrapper);
	//myEntities.Add(cube);


	Prism::MeshData geometryData;
	Prism::GeometryGenerator::CreateGrid(500.f, 500.f, 500, 500, geometryData);

	//Entity* geometry = new Entity();
	//geometry->AddComponent<GraphicsComponent>()->InitGeometry(geometryData);
	//myEntities.Add(geometry);

	for (int i = 0; i < 100; ++i)
	{
		float width = static_cast<float>(rand() % 20) + 1.f;
		float height = static_cast<float>(rand() % 20) + 1.f;
		float depth = static_cast<float>(rand() % 20) + 1.f;

		float x = static_cast<float>(rand() % 200) - 100.f;
		float y = static_cast<float>(rand() % 200) - 100.f;
		float z = static_cast<float>(rand() % 200) - 100.f;

		Entity* cube = new Entity();
		cube->AddComponent<GraphicsComponent>()->InitCube(width, height, depth);
		cube->GetComponent<GraphicsComponent>()->GetInstance()->SetPosition({ x, y, z });
		cube->AddComponent<InputComponent>()->Init(*myInputWrapper);
		myEntities.Add(cube);
	}


	myScene = new Prism::Scene();
	myScene->SetCamera(myCamera);

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		GraphicsComponent* gfxComp = myEntities[i]->GetComponent<GraphicsComponent>();

		if (gfxComp != nullptr)
		{
			myScene->AddInstance(gfxComp->GetInstance());
		}
	}

	myScene->AddLight(myPointLight);

	myRenderStuff = true;


	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	BEGIN_TIME_BLOCK("Game::Update");

	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();
	Prism::Engine::GetInstance()->GetEffectContainer().Update(deltaTime);
	Prism::Engine::GetInstance()->GetDebugDisplay().RecordFrameTime(deltaTime);

	if (myInputWrapper->KeyDown(DIK_F5))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleFunctionTimers();
	}
	else if (myInputWrapper->KeyDown(DIK_F6))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleMemoryUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F7))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleCPUUsage();
	}
	else if (myInputWrapper->KeyDown(DIK_F8))
	{
		Prism::Engine::GetInstance()->GetDebugDisplay().ToggleFrameTime();
	}
	else if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}
	else if (myInputWrapper->KeyDown(DIK_R))
	{
		myRenderStuff = !myRenderStuff;
	}

	LogicUpdate(deltaTime);

	END_TIME_BLOCK("Game::Update");

	Render();
	
	return true;
}

void Game::Pause()
{

}

void Game::UnPause()
{

}

void Game::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

void Game::LogicUpdate(const float aDeltaTime)
{

	for (int i = 0; i < myEntities.Size(); ++i)
	{
		myEntities[i]->Update(aDeltaTime);
	}
	myPlayer->Update(aDeltaTime);
}

void Game::Render()
{
	VTUNE_EVENT_BEGIN(VTUNE::GAME_RENDER);

	BEGIN_TIME_BLOCK("Game::Render");

	if (myRenderStuff)
	{
		myScene->Render();
	}

	END_TIME_BLOCK("Game::Render");

	Prism::Engine::GetInstance()->GetDebugDisplay().Render(*myCamera);

	VTUNE_EVENT_END();
}
