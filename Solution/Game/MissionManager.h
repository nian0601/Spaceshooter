#pragma once
#include <GrowingArray.h>
#include <string>
class Entity;
class Level;
class Mission;

class MissionManager
{
public:
	MissionManager(Level& aLevel, Entity& aPlayer, const std::string& aFileToReadFrom);
	
	void Update(float aDeltaTime);

private:
	Level& myLevel;
	Entity& myPlayer;
	int myCurrentMission;
	CU::GrowingArray<Mission*> myMissions;
};
