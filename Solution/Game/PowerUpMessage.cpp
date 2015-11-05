#include "stdafx.h"
#include "PowerUpMessage.h"


PowerUpMessage::PowerUpMessage(ePowerUpType aType, CU::Vector3<float> aPos, float aRadius, float aTime)
	: Message(eMessageType::POWER_UP)
	, myPowerupType(aType)
	, myPosition(aPos)
	, myRadius(aRadius)
	, myTime(aTime)
	, myUpgrade("")
	, myPickupMessage("")
	, myUpgradeID(-1)
{
}

PowerUpMessage::PowerUpMessage(ePowerUpType aType, std::string anUpgrade, std::string aPickupMessage, int anUpgradeID)
	: Message(eMessageType::POWER_UP)
	, myPowerupType(aType)
	, myPosition({0.f, 0.f, 0.f})
	, myRadius(0.f)
	, myTime(0.f)
	, myUpgrade(anUpgrade)
	, myUpgradeID(anUpgradeID)
	, myPickupMessage(aPickupMessage)
{
}