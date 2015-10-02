#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include <DL_Debug.h>
#include "EffectContainer.h"
#include "EffectListener.h"
#include "Effect.h"
#include "Engine.h"
#include "Texture.h"

Prism::Effect::Effect()
{
	myEffectListeners.Init(2);
}

bool Prism::Effect::Init(const std::string& aEffectFile)
{
	//if (myEffect != nullptr)
	//{
	//	myEffect->Release();
	//	myEffect = nullptr;
	//}

	if (ReloadShader(aEffectFile) == false)
	{
		return false;
	}

	for (int i = 0; i < myEffectListeners.Size(); ++i)
	{
		myEffectListeners[i]->OnEffectLoad();
	}

	return true;

}

void Prism::Effect::SetScaleVector(const CU::Vector3<float>& aScaleVector)
{
	myScaleVectorVariable->SetFloatVector(&aScaleVector.x);
}

void Prism::Effect::SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix)
{
	myWorldMatrixVariable->SetMatrix(&aWorldMatrix.myMatrix[0]);
}

void Prism::Effect::SetViewMatrix(const CU::Matrix44<float>& aViewMatrix)
{
	myViewMatrixVariable->SetMatrix(&aViewMatrix.myMatrix[0]);
}

void Prism::Effect::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
{
	myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
}

void Prism::Effect::SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask)
{
	Engine::GetInstance()->GetContex()->OMSetBlendState(aBlendState, aBlendFactor, aSampleMask);
}

void Prism::Effect::UpdateDirectionalLights(
	const CU::StaticArray<Prism::DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData)
{
	if (myDirectionalLightVariable != nullptr)
	{
		myDirectionalLightVariable->SetRawValue(&someDirectionalLightData[0], 0, 
			sizeof(DirectionalLightData) * NUMBER_OF_DIRECTIONAL_LIGHTS);
	}
}

void Prism::Effect::UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData)
{
	if (myPointLightVariable != nullptr)
	{
		myPointLightVariable->SetRawValue(&somePointLightData[0], 0, sizeof(PointLightData) * NUMBER_OF_POINT_LIGHTS);
	}
}

void Prism::Effect::UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData)
{
	if (mySpotLightVariable != nullptr)
	{
		mySpotLightVariable->SetRawValue(&someSpotLightData[0], 0, sizeof(SpotLightData) * NUMBER_OF_SPOT_LIGHTS);
	}
}

void Prism::Effect::UpdateTime(const float aDeltaTime)
{
	if (myTotalTimeVariable != nullptr)
	{
		float newTime = 0.f;
		myTotalTimeVariable->GetFloat(&newTime);
		newTime += aDeltaTime;
		myTotalTimeVariable->SetFloat(newTime);
	}
}


bool Prism::Effect::ReloadShader(const std::string& aFile)
{
	Sleep(100);

	myFileName = aFile;

	HRESULT hr;
	unsigned int shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	hr = D3DX11CompileFromFile(myFileName.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader
		, &compilationMsgs, 0);
	if (hr != S_OK)
	{
		if (compilationMsgs != nullptr)
		{
			DL_MESSAGE_BOX((char*)compilationMsgs->GetBufferPointer(), "Effect Error", MB_ICONWARNING);
			myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect(
				"Data/effect/SuperUglyDebugEffect.fx")->myEffect;
		}
	}
	if (compilationMsgs != nullptr)
	{
		compilationMsgs->Release();
	}

	if (hr == S_OK)
	{
		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL
			, Engine::GetInstance()->GetDevice(), &myEffect);

		if (FAILED(hr))
		{
			DL_MESSAGE_BOX("Cant Create Effect", "Effect Error", MB_ICONWARNING);
			return false;
		}

		compiledShader->Release();
	}
	

	myTechnique = myEffect->GetTechniqueByName("Render");
	if (myTechnique->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get Technique", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myScaleVectorVariable = myEffect->GetVariableByName("Scale")->AsVector();
	if (myScaleVectorVariable->IsValid() == false)
	{
		myScaleVectorVariable = nullptr;
	}

	myWorldMatrixVariable = myEffect->GetVariableByName("World")->AsMatrix();
	if (myWorldMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get WorldMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myViewMatrixVariable = myEffect->GetVariableByName("View")->AsMatrix();
	if (myViewMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ViewMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
	if (myProjectionMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myTotalTimeVariable = nullptr;
	myTotalTimeVariable = myEffect->GetVariableByName("Time")->AsScalar();
	if (myTotalTimeVariable->IsValid() == false)
	{
		myTotalTimeVariable = nullptr;
	}

	myDirectionalLightVariable = myEffect->GetVariableByName("DirectionalLights");
	if (myDirectionalLightVariable->IsValid() == false)
	{
		myDirectionalLightVariable = nullptr;
	}

	myPointLightVariable = myEffect->GetVariableByName("PointLights");
	if (myPointLightVariable->IsValid() == false)
	{
		myPointLightVariable = nullptr;
	}

	mySpotLightVariable = myEffect->GetVariableByName("SpotLights");
	if (mySpotLightVariable->IsValid() == false)
	{
		mySpotLightVariable = nullptr;
	}

	return true;
}