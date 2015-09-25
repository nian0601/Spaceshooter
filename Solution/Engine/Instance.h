#pragma once

#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <StaticArray.h>

namespace Prism
{
	class Camera;
	class Light;
	class Model;

	class Instance
	{
	public:
		Instance(Model& aModel);

		void Render(Camera& aCamera);
		void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera);

		void SetPosition(const CU::Vector3<float>& aPosition);
		CU::Vector3<float>& GetPosition();
		CU::Matrix44<float>& GetOrientation();
		void SetOrientation(const CU::Matrix44<float>& aOrientation);

		void SetEffect(const std::string& aEffectFile);

		void PerformRotationLocal(CU::Matrix44<float>& aRotation);
		void PerformRotationWorld(CU::Matrix44<float>& aRotation);
		void PerformTransformation(CU::Matrix44<float>& aTransformation);

		void UpdateDirectionalLights(
			const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData);
		void UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData);
		void UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData);

		void SetOrientationPointer(CU::Matrix44<float>& aOrientation);

	private:
		void operator=(Instance&) = delete;

		Model& myModel;
		CU::Matrix44<float> myOrientation;
		CU::Matrix44<float>* myOrientationPointer;
		CU::Vector3<float> myPosition;
	};
}