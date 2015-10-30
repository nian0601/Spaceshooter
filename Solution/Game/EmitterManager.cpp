#include "stdafx.h"

#include "Entity.h"

#include "EmitterComponent.h"
#include "EmitterInstance.h"
#include "EmitterManager.h"
#include "DestroyEmitterMessage.h"
#include "PostMaster.h"
EmitterManager::EmitterManager()
	: myEmitters(32)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DESTORY_EMITTER, this);
}

EmitterManager::~EmitterManager()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DESTORY_EMITTER, this);
}

void EmitterManager::AddEmitter(EmitterComponent* anEmitter)
{
	myEmitters.Add(anEmitter);
}

void EmitterManager::UpdateEmitters(float aDeltaTime)
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{

		if (myEmitters[i]->GetEntity().GetAlive() == true && myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Update(aDeltaTime);
		}
		else if (myEmitters[i]->GetEntity().GetAlive() == false)
		{
			myEmitters.RemoveNonCyclicAtIndex(i);
		}
	}
}

void EmitterManager::RenderEmitters()
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		if (myEmitters[i]->GetEmitter()->GetIsActive() == true)
		{
			myEmitters[i]->Render();
		}
	}
}

void EmitterManager::ReceiveMessage(const DestroyEmitterMessage& aMessage)
{
	myEmitters.RemoveNonCyclic(aMessage.myEmitterComponent);
}
