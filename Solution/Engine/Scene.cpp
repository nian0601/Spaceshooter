#include "stdafx.h"

#include "DebugDataDisplay.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "PointLight.h"
#include "Scene.h"
#include "SpotLight.h"

Prism::Scene::Scene()
{
	myInstances.Init(4);
	myDirectionalLights.Init(4);
	myPointLights.Init(4);
	mySpotLights.Init(4);

	memset(&myDirectionalLightData[0], 0, sizeof(DirectionalLightData) * NUMBER_OF_DIRECTIONAL_LIGHTS);
	memset(&myPointLightData[0], 0, sizeof(PointLightData) * NUMBER_OF_POINT_LIGHTS);
	memset(&mySpotLightData[0], 0, sizeof(SpotLightData) * NUMBER_OF_SPOT_LIGHTS);
}

void Prism::Scene::Render(CU::GrowingArray<Instance*>& someBulletInstances)
{
	TIME_FUNCTION;

	for (int i = 0; i < myDirectionalLights.Size(); ++i)
	{
		myDirectionalLights[i]->Update();

		myDirectionalLightData[i].myDirection = myDirectionalLights[i]->GetCurrentDir();
		myDirectionalLightData[i].myColor = myDirectionalLights[i]->GetColor();
	}

	for (int i = 0; i < myPointLights.Size(); ++i)
	{
		myPointLights[i]->Update();

		myPointLightData[i].myColor = myPointLights[i]->GetColor();
		myPointLightData[i].myPosition = myPointLights[i]->GetPosition();
		myPointLightData[i].myRange = myPointLights[i]->GetRange();
	}

	for (int i = 0; i < mySpotLights.Size(); ++i)
	{
		mySpotLights[i]->Update();

		mySpotLightData[i].myPosition = mySpotLights[i]->GetPosition();
		mySpotLightData[i].myDirection = mySpotLights[i]->GetDir();
		mySpotLightData[i].myColor = mySpotLights[i]->GetColor();
		mySpotLightData[i].myRange = mySpotLights[i]->GetRange();
		mySpotLightData[i].myCone = mySpotLights[i]->GetCone();
	}

	for (int i = 0; i < myInstances.Size(); ++i)
	{
		myInstances[i]->UpdateDirectionalLights(myDirectionalLightData);
		myInstances[i]->UpdatePointLights(myPointLightData);
		myInstances[i]->UpdateSpotLights(mySpotLightData);
		myInstances[i]->Render(*myCamera);
	}

	for (int i = 0; i < myInstances.Size(); ++i)
	{
		myInstances[i]->UpdateDirectionalLights(myDirectionalLightData);
		myInstances[i]->UpdatePointLights(myPointLightData);
		myInstances[i]->UpdateSpotLights(mySpotLightData);
		myInstances[i]->Render(*myCamera);
	}

	for (int i = 0; i < someBulletInstances.Size(); ++i)
	{
		someBulletInstances[i]->UpdateDirectionalLights(myDirectionalLightData);
		someBulletInstances[i]->UpdatePointLights(myPointLightData);
		someBulletInstances[i]->UpdateSpotLights(mySpotLightData);
		someBulletInstances[i]->Render(*myCamera);
	}

}

void Prism::Scene::AddInstance(Instance* aInstance)
{
	myInstances.Add(aInstance);
}

void Prism::Scene::AddLight(DirectionalLight* aLight)
{
	myDirectionalLights.Add(aLight);
}

void Prism::Scene::AddLight(PointLight* aLight)
{
	myPointLights.Add(aLight);
}

void Prism::Scene::AddLight(SpotLight* aLight)
{
	mySpotLights.Add(aLight);
}

void Prism::Scene::RemoveInstance(Instance* aInstance) 
{
	myInstances.RemoveCyclic(aInstance);
}

void Prism::Scene::SetCamera(Camera* aCamera)
{
	myCamera = aCamera;
}

Prism::Camera& Prism::Scene::GetCamera()
{
	return *myCamera;
}