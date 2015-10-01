#include "stdafx.h"

#include "Camera.h"
#include <Fov90Frustum.h>
#include <xnamath.h>


namespace Prism
{
	Camera::Camera(CU::Matrix44f& aPlayerMatrix)
		: myOrientation(aPlayerMatrix)
	{
		//OnResize(800, 600);

		//myOrientation.SetPos(CU::Vector3<float>(0.f, 0.f, -25.f));

		myFrustum = new CU::Intersection::Fov90Frustum(0.1f, 1000.f);
	}


	Camera::~Camera()
	{
		delete myFrustum;
		myFrustum = nullptr;
	}

	void Camera::OnResize(const int aWidth, const int aHeight)
	{
		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(aWidth)
			, static_cast<float>(aHeight), 0.1f, 1000.f);


		XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PI * 0.5f, static_cast<float>(aWidth) / aHeight, 0.1f, 1000.f);
		XMFLOAT4X4 proj;
		XMStoreFloat4x4(&proj, projection);
		myProjectionMatrix.Init(reinterpret_cast<float*>(proj.m));
	}

	const CU::Matrix44<float>& Camera::GetOrientation() const
	{
		return myOrientation;
	}

	const CU::Vector3<float>& Camera::GetPosition() const
	{
		return myPosition;
	}

	const CU::Matrix44<float>& Camera::GetProjection() const
	{
		return myProjectionMatrix;
	}

	const CU::Matrix44<float>& Camera::GetOrthogonal() const
	{
		return myOrthogonalMatrix;
	}

	void Camera::SetOrientation(const CU::Matrix44<float>& aOrientation)
	{
		myOrientation = aOrientation;
	}

	void Camera::SetPosition(const CU::Vector3<float>& aPosition)
	{
		myPosition = aPosition;
		myOrientation.SetPos(aPosition);
	}

	void Camera::RotateX(const float aDegrees)
	{
		TIME_FUNCTION;

		myPosition = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(myPosition);
	}

	void Camera::RotateY(const float aDegrees)
	{
		TIME_FUNCTION;

		myPosition = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(myPosition);
	}

	void Camera::RotateZ(const float aDegrees)
	{
		TIME_FUNCTION;

		myPosition = myOrientation.GetPos();
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
		myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aDegrees * XM_PI / 180.f) * myOrientation;
		myOrientation.SetPos(myPosition);
	}

	void Camera::MoveForward(const float aDistance)
	{
		TIME_FUNCTION;

		myPosition += myOrientation.GetForward() * aDistance;
		myOrientation.SetPos(myPosition);
	}

	void Camera::MoveRight(const float aDistance)
	{
		TIME_FUNCTION;

		myPosition += myOrientation.GetRight() * aDistance;
		myOrientation.SetPos(myPosition);
	}
}