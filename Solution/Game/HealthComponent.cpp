#include "stdafx.h"

#include "HealthComponent.h"
#include "Entity.h"
#include "Enums.h"
#include "PowerUpNote.h"
#include "ShieldComponent.h"

HealthComponent::HealthComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void HealthComponent::Init(const unsigned short& aMaxHealth, const bool& anIsInvulnerable)
{
	myMaxHealth = aMaxHealth;
	myCurrentHealth = myMaxHealth;
	myIsInvulnerable = anIsInvulnerable;
}

void HealthComponent::AddHealth(const unsigned short& aHealthToAdd)
{
	myCurrentHealth += aHealthToAdd;

	if (myIsInvulnerable == false) // testing
	{
		if (myCurrentHealth > myMaxHealth)
		{
			myCurrentHealth = myMaxHealth;
		}
	}
}

void HealthComponent::RemoveHealth(const unsigned short& aHealthToRemove)
{
	if (myIsInvulnerable == false)
	{
		if (myCurrentHealth <= aHealthToRemove)
		{
			myCurrentHealth = 0;
			myEntity.Kill();
		}
		else
		{
			myCurrentHealth -= aHealthToRemove;
		}
	}
}

void HealthComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::HEALTHKIT)
	{
		AddHealth(aNote.myHealthRecover);
	}
}