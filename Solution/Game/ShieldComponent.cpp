#include "stdafx.h"

#include "Entity.h"
#include "Enums.h"
#include "PowerUpNote.h"
#include "ShieldComponent.h"
#include "ShieldNote.h"


ShieldComponent::ShieldComponent(Entity& aEntity)
	: Component(aEntity)
{
}

void ShieldComponent::Init()
{

	myShieldOvercharged = false;
	
	myReachargeTime = 5.f;
	myCooldown = 0.f;

	myOvercharge = 0;
	myMaxShieldStrength = 100;
	myShieldStrength = myMaxShieldStrength;

}

void ShieldComponent::ReceiveNote(const PowerUpNote& aNote)
{
	if (aNote.myType == ePowerUpType::SHIELDBOOST)
	{
		myShieldOvercharged = true;
		myShieldStrength = static_cast<int>(aNote.myValue);
	}
}

void ShieldComponent::DamageShield(int someDamage)
{
	myCooldown = 0;
	if (someDamage >= myShieldStrength)
	{
		myShieldStrength = 0;
	}
	else
	{
		myShieldStrength -= someDamage;
	}

	myEntity.SendNote(ShieldNote(myShieldStrength, myMaxShieldStrength));
}

void ShieldComponent::Update(float aDelta)
{
	if (myShieldOvercharged == false)
	{
		if (myShieldStrength <= 0)
		{
			COMPONENT_LOG("Shield is depleted, setting to 0");
			myShieldStrength = 0;
		}

		if (myShieldStrength < myMaxShieldStrength)
		{
			COMPONENT_LOG("Recharging shield strenght!");
			myCooldown += aDelta;

			if (myCooldown >= myReachargeTime)
			{
				if (myShieldStrength >= myMaxShieldStrength)
				{
					COMPONENT_LOG("Shield Strength reached 100. During recharge time.");
					myShieldStrength = myMaxShieldStrength;
					return;
				}
				myShieldStrength += 1;
			}
		}
	}

	if (myShieldOvercharged == true)
	{
		COMPONENT_LOG("Shield is overcharged.");
		POWERUP_LOG("Shield is overcharged.");
		if (myShieldStrength <= myMaxShieldStrength)
		{
			COMPONENT_LOG("Shield is 100 or lower, shield is no longer overcharged!");
			myShieldOvercharged = false;
		}
	}

}

