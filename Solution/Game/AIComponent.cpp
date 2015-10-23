#include "stdafx.h"
#include "AIComponent.h"
#include "Constants.h"
#include "DefendMessage.h"
#include <Engine.h>
#include "Entity.h"
#include <MathHelper.h>
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include <sstream>
#include <Vector.h>

AIComponent::AIComponent(Entity& aEntity)
	: ControllerComponent(aEntity)
	, myPhysicsComponent(nullptr)
	, myAvoidanceDistance(300.f)
	, myPrevEntityToFollow(nullptr)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);

}

AIComponent::~AIComponent()
{
	if (myTargetPositionMode != eAITargetPositionMode::KAMIKAZE)
	{
		PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	}
}

void AIComponent::Init(float aSpeed, float aTimeBetweenDecisions, const std::string& aTargetName
	, float aAvoidanceDistance, const CU::Vector3<float>& aAvoidancePoint
	, eAITargetPositionMode aTargetPositionMode)
{
	myEntityToFollow = nullptr;

	myTargetName = aTargetName;

	myTimeToNextDecision = aTimeBetweenDecisions;
	myMovementSpeed = aSpeed;
	myVelocity = myEntity.myOrientation.GetForward() * myMovementSpeed;

	myCanMove = true;

	myAvoidanceDistance = aAvoidanceDistance;
	myFollowingOffset = aAvoidancePoint;

	myTargetPositionMode = aTargetPositionMode;

	DL_ASSERT_EXP(myTargetPositionMode != eAITargetPositionMode::NOT_USED, "No AIMode was set!");
}

void AIComponent::Init(float aSpeed, eAITargetPositionMode aTargetPositionMode)
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	myTargetPositionMode = aTargetPositionMode;
	myMovementSpeed = aSpeed;
	myVelocity = myEntity.myOrientation.GetForward() * myMovementSpeed;
	myEntityToFollow = nullptr;
	myTimeToNextDecision = 0.f;
	myAvoidanceDistance = 0.f;
	myFollowingOffset = 0.f;
	myCanMove = true;
	DL_ASSERT_EXP(myTargetPositionMode != eAITargetPositionMode::NOT_USED, "No AIMode was set!");
}

void AIComponent::Update(float aDeltaTime)
{
	DL_ASSERT_EXP(myEntityToFollow != nullptr, "AI needs an entity to follow.");

	if (myPhysicsComponent == nullptr)
	{
		myPhysicsComponent = myEntity.GetComponent<PhysicsComponent>();
		DL_ASSERT_EXP(myPhysicsComponent != nullptr, "AI component needs physics component for movement."); // remove later
	}

	myVelocity = myPhysicsComponent->GetVelocity();


	

	if (myCanMove == true)
	{
		myTimeToNextDecision -= aDeltaTime;
		FollowEntity(aDeltaTime);

		if (myTargetPositionMode != eAITargetPositionMode::KAMIKAZE 
			&& myTargetPositionMode != eAITargetPositionMode::MINE)
		{
			CU::Vector3<float> toTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();

			CU::Normalize(toTarget);
			if (CU::Dot(myEntity.myOrientation.GetForward(), toTarget) > 0.72f && myTimeToNextDecision < 0)
			{
				myTimeToNextDecision = myTimeBetweenDecisions;
				Shoot(myPhysicsComponent->GetVelocity());
			}
		}
	}
	else
	{
		myVelocity -= myVelocity * aDeltaTime * 0.01f;
		myTimeBeforeMovement -= aDeltaTime;

		if (myTimeBeforeMovement <= 0.f)
		{
			myCanMove = true;
		}

		RotateX(aDeltaTime / 10);
		RotateZ(aDeltaTime / 5);
	}

	myPhysicsComponent->SetVelocity(myVelocity);
}

void AIComponent::SetEntityToFollow(Entity* aEntity)
{
	myEntityToFollow = aEntity;
}

void AIComponent::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myPrevEntityToFollow = myEntityToFollow;
		myEntityToFollow = aMessage.myEntity;
	}
	else if (aMessage.myType == DefendMessage::eType::COMPLETE)
	{
		myEntityToFollow = myPrevEntityToFollow;
	}
}

void AIComponent::FollowEntity(float aDeltaTime)
{
	if (myTargetPositionMode != eAITargetPositionMode::MINE)
	{
		CalculateToTarget(myTargetPositionMode);
		myVelocity += myToTarget * aDeltaTime;

		CU::Normalize(myVelocity);

		CU::Vector3<float> up(0, 1.f, 0);
		up = up * myEntity.myOrientation;

		CU::Normalize(myToTarget);

		up = up + myToTarget * aDeltaTime;

		CU::Normalize(up);

		CU::Vector3<float> right = CU::Cross(up, myVelocity);
		up = CU::Cross(myVelocity, right);

		right = CU::Cross(up, myVelocity);

		myEntity.myOrientation.myMatrix[0] = right.x;
		myEntity.myOrientation.myMatrix[1] = right.y;
		myEntity.myOrientation.myMatrix[2] = right.z;
		myEntity.myOrientation.myMatrix[3] = 0;

		myEntity.myOrientation.myMatrix[4] = up.x;
		myEntity.myOrientation.myMatrix[5] = up.y;
		myEntity.myOrientation.myMatrix[6] = up.z;
		myEntity.myOrientation.myMatrix[7] = 0;

		myEntity.myOrientation.myMatrix[8] = myVelocity.x;
		myEntity.myOrientation.myMatrix[9] = myVelocity.y;
		myEntity.myOrientation.myMatrix[10] = myVelocity.z;
		myEntity.myOrientation.myMatrix[11] = 0;

		myVelocity *= myMovementSpeed;
	}
	else
	{

	}
}

void AIComponent::CalculateToTarget(eAITargetPositionMode aMode)
{
	if (aMode == eAITargetPositionMode::KEEP_DISTANCE)
	{
		myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
		float distToTarget = CU::Length(myToTarget);

		if (distToTarget < myAvoidanceDistance)
		{
			float distCoef = 1.f - (distToTarget / myAvoidanceDistance);
			myToTarget += myFollowingOffset * distCoef;
		}
	}

	else if (aMode == eAITargetPositionMode::ESCAPE_THEN_RETURN)
	{
		if (myIsEscaping == false)
		{
			myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
			float distToTarget = CU::Length(myToTarget);

			if (distToTarget < myAvoidanceDistance)
			{
				float distCoef = 1.f - (distToTarget / (myAvoidanceDistance));
				myToTarget += myFollowingOffset * distCoef;
				myEscapePosition = myEntityToFollow->myOrientation.GetPos() + CU::Math::RandomVector(-myFollowingOffset, myFollowingOffset);
				myIsEscaping = true;
			}
		}
		else
		{
			myToTarget = myEscapePosition - myEntity.myOrientation.GetPos();
			float distToTarget = CU::Length(myToTarget);

			if (distToTarget < 20)
			{
				myIsEscaping = false;
			}
		}
	}
	else if (aMode == eAITargetPositionMode::KAMIKAZE)
	{
		myToTarget = myEntityToFollow->myOrientation.GetPos() - myEntity.myOrientation.GetPos();
	}
}
