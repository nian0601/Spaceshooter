#include "stdafx.h"
#include "Camera.h"
#include "FileWatcher.h"
#include "Frustum.h"
#include <xnamath.h>
#include <XMLReader.h>

namespace Prism
{
	Camera::Camera(CU::Matrix44f& aPlayerMatrix)
		: myOrientation(aPlayerMatrix)
		, myNear(0.1f)
		, myFar(2500.f)
	{
		myFrustum = new Frustum(aPlayerMatrix, myNear, myFar);
		WATCH_FILE("Data/Setting/SET_camera.xml", Camera::ReadXML);
		ReadXML("Data/Setting/SET_camera.xml");
	}


	Camera::~Camera()
	{
		delete myFrustum;
	}

	void Camera::ReadXML(const std::string& aFileName)
	{
		Sleep(10);
		XMLReader reader;
		reader.OpenDocument(aFileName);
		tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("camera");
		reader.ForceReadAttribute(levelElement, "fov", myFOV);
		myFOV *= 3.14159f / 180.f;
		OnResize(Engine::GetInstance()->GetWindowSize().x, Engine::GetInstance()->GetWindowSize().y);
		reader.CloseDocument();
	}

	void Camera::OnResize(const int aWidth, const int aHeight)
	{
		myProjectionMatrix = CU::Matrix44<float>::CreateProjectionMatrixLH(myNear, myFar, static_cast<float>(aHeight) / static_cast<float>(aWidth), myFOV);
	}

	const CU::Matrix44<float>& Camera::GetOrientation() const
	{
		return myOrientation;
	}

	const CU::Matrix44<float>& Camera::GetProjection() const
	{
		return myProjectionMatrix;
	}

	void Camera::Update()
	{
		myFrustum->Update();
	}
	void Camera::SetOrientation(const CU::Matrix44<float>& aOrientation)
	{
		myOrientation = aOrientation;
	}

	void Camera::SetPosition(const CU::Vector3<float>& aPosition)
	{
		myOrientation.SetPos(aPosition);
	}

	void Camera::RotateX(const float aDegrees)
	{
		TIME_FUNCTION

		CU::Vector3<float> position = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(position);
	}

	void Camera::RotateY(const float aDegrees)
	{
		TIME_FUNCTION

		CU::Vector3<float> position = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(position);
	}

	void Camera::RotateZ(const float aDegrees)
	{
		TIME_FUNCTION

		CU::Vector3<float> position = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(position);
	}

	void Camera::MoveForward(const float aDistance)
	{
		TIME_FUNCTION

		myOrientation.SetPos(myOrientation.GetPos() + myOrientation.GetForward() * aDistance);
	}

	void Camera::MoveRight(const float aDistance)
	{
		TIME_FUNCTION

		myOrientation.SetPos(myOrientation.GetPos() + myOrientation.GetRight() * aDistance);
	}
}