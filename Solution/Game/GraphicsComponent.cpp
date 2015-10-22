#include "stdafx.h"

#include "CollisionComponent.h"
#include "Constants.h"
#include "GraphicsComponent.h"
#include "Entity.h"
#include <Engine.h>
#include <EngineEnums.h>
#include <EffectContainer.h>
#include <Instance.h>
#include <Model.h>
#include "ModelLoader.h"
#include <ModelProxy.h>
#include <Scene.h>
#include <XMLReader.h>


GraphicsComponent::GraphicsComponent(Entity& aEntity)
	: Component(aEntity)
	, myInstance(nullptr)
	, myCullingRadius(10.f)
{
}

GraphicsComponent::~GraphicsComponent()
{
	if (myEntity.GetOctreeType() != Prism::eOctreeType::NOT_IN_OCTREE)
	{
		myEntity.GetScene().RemoveInstance(myInstance);
	}
	delete myInstance;
	myInstance = nullptr;
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{
	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(aModelPath
		, aEffectPath);

	std::string xmlPath(aModelPath, strlen(aModelPath) - 4);
	xmlPath += ".xml";

	XMLReader reader;
	reader.OpenDocument(xmlPath);

	reader.ForceReadAttribute(reader.ForceFindFirstChild(reader.ForceFindFirstChild("root"), "radius"), "value", myCullingRadius);
	reader.CloseDocument();

	myInstance = new Prism::Instance(*model, myEntity.myOrientation, myEntity.GetOctreeType(), myCullingRadius);

	if (myEntity.GetComponent<CollisionComponent>() != nullptr)
	{
		myEntity.GetComponent<CollisionComponent>()->SetCollisionRadius(myCullingRadius 
			* globalCollisionRadiusMultiplier);
	}
}

void GraphicsComponent::InitCube(float aWidth, float aHeight, float aDepth)
{
	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadCube(aWidth, aHeight, aDepth);

	myInstance = new Prism::Instance(*model, myEntity.myOrientation, myEntity.GetOctreeType(), myCullingRadius);

	if (myEntity.GetComponent<CollisionComponent>() != nullptr)
	{
		myEntity.GetComponent<CollisionComponent>()->SetCollisionRadius(myCullingRadius 
			* globalCollisionRadiusMultiplier);
	}
}

void GraphicsComponent::Update(float aDeltaTime)
{
	aDeltaTime;

	if (myEntity.GetShouldRotate() == true)
	{
		CU::Vector3f pos = myEntity.myOrientation.GetPos();
		if (myEntity.GetShouldRotateX() == true)
		{
			myEntity.myOrientation = CU::Matrix44f::CreateRotateAroundX(0.1f * aDeltaTime)* myEntity.myOrientation;
		}
		if (myEntity.GetShouldRotateY() == true)
		{
			myEntity.myOrientation = CU::Matrix44f::CreateRotateAroundY(0.1f * aDeltaTime) * myEntity.myOrientation;
		}
		if (myEntity.GetShouldRotateZ() == true)
		{
			myEntity.myOrientation = CU::Matrix44f::CreateRotateAroundZ(0.1f * aDeltaTime)* myEntity.myOrientation;
		}
		myEntity.myOrientation.SetPos(pos);
	}
}

void GraphicsComponent::SetPosition(const CU::Vector3<float>& aPosition)
{
	myEntity.myOrientation.SetPos(aPosition);
}

void GraphicsComponent::SetScale(const CU::Vector3<float>& aScale)
{
	myInstance->SetScale(aScale);
}
