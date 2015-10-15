#include "stdafx.h"
#include "Engine.h"
#include "KillXEnemiesMission.h"
#include "Level.h"
#include "XMLReader.h"


KillXEnemiesMission::KillXEnemiesMission(Level& aLevel, XMLReader& aReader, tinyxml2::XMLElement* aElement)
	: Mission(aReader, aElement)
	, myLevel(aLevel)
{
	tinyxml2::XMLElement* enemiesToKillElement = aReader.ForceFindFirstChild(aElement, "enemiestokill");
	aReader.ForceReadAttribute(enemiesToKillElement, "value", myEnemiesToKill);
}

bool KillXEnemiesMission::Update(float aDeltaTime)
{
	aDeltaTime;
	Prism::Engine* engine = Prism::Engine::GetInstance();
	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
	engine->PrintDebugText("Current mission: Kill all enemies!", { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });


	return myEnemiesToKill > myLevel.GetEnemiesAlive();
}

void KillXEnemiesMission::Start()
{

	myStartEnemyCount = myLevel.GetEnemiesAlive();
}