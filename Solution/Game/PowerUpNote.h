#pragma once
#include "Enums.h"


struct PowerUpNote
{
	PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier);
	PowerUpNote(ePowerUpType someType, float someDuration);
	PowerUpNote(ePowerUpType someType, std::string someUpgrade);

	const ePowerUpType myType;
	const std::string myUpgradeType;
	const float myDuration;
	const int myShieldStrength;
	const int myHealthRecover;
	const int myFireRateMultiplier;

private:
	void operator=(PowerUpNote&) = delete;
};

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration, int someShieldStrength
		, int someHealthRecover, int someFireRateMultiplier)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(someShieldStrength)
	, myHealthRecover(someHealthRecover)
	, myFireRateMultiplier(someFireRateMultiplier)
	, myUpgradeType("")
{
	DL_ASSERT_EXP(myHealthRecover >= 0, "Can't recover negative health.");
}

inline PowerUpNote::PowerUpNote(ePowerUpType someType, float someDuration)
	: myType(someType)
	, myDuration(someDuration)
	, myShieldStrength(0)
	, myHealthRecover(0)
	, myFireRateMultiplier(0)
	, myUpgradeType("")
{
}

inline PowerUpNote::PowerUpNote(ePowerUpType someType, std::string someUpgrade)
	: myType(someType)
	, myDuration(0)
	, myShieldStrength(0)
	, myHealthRecover(0)
	, myFireRateMultiplier(0)
	, myUpgradeType(someUpgrade)
{
}