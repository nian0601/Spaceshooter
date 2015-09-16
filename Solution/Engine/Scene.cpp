#include "stdafx.h"

#include "DebugDataDisplay.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "PointLight.h"
#include "Scene.h"



Scene::Scene()
{
	myInstances.Init(4);
	myDirectionalLights.Init(4);
	myPointLights.Init(4);

	memset(&myDirectionalLightDirections[0], 0, sizeof(CU::Vector4<float>) * 1);
	memset(&myDirectionalLightColors[0], 0, sizeof(CU::Vector4<float>) * 1);

	memset(&myPointLightPositions[0], 0, sizeof(CU::Vector4<float>) * 3);
	memset(&myPointLightColors[0], 0, sizeof(CU::Vector4<float>) * 3);
	memset(&myPointLightRanges[0], 0, sizeof(float) * 3);
}


Scene::~Scene()
{
}

void Scene::Render()
{
	TIME_FUNCTION

	for (int i = 0; i < 0; ++i)
	{
		myDirectionalLights[i]->Update();
		myDirectionalLightDirections[i] = myDirectionalLights[i]->GetCurrentDir();
		myDirectionalLightColors[i] = myDirectionalLights[i]->GetColor();
	}

	for (int i = 0; i < 1; ++i)
	{
		myPointLights[i]->Update();
		myPointLightColors[i] = myPointLights[i]->GetColor();
		myPointLightPositions[i] = myPointLights[i]->GetPosition();
		myPointLightRanges[i] = myPointLights[i]->GetRange();
	}

	for (int i = 0; i < myInstances.Size(); ++i)
	{
		myInstances[i]->UpdateDirectionalLights(myDirectionalLightDirections, myDirectionalLightColors);
		myInstances[i]->UpdatePointLights(myPointLightPositions, myPointLightColors, myPointLightRanges);
		myInstances[i]->Render(*myCamera);
	}
}

void Scene::AddInstance(Instance* aInstance)
{
	myInstances.Add(aInstance);
}

void Scene::AddLight(DirectionalLight* aLight)
{
	myDirectionalLights.Add(aLight);
}

void Scene::AddLight(PointLight* aLight)
{
	myPointLights.Add(aLight);
}


void Scene::SetCamera(Camera* aCamera)
{
	myCamera = aCamera;
}

Camera& Scene::GetCamera()
{
	return *myCamera;
}