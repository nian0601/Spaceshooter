#pragma once

#include <D3DX11.h>
#include <GrowingArray.h>
#include <Matrix.h>

namespace Prism
{
	class Effect;
	class Surface;

	struct IndexBufferWrapper;
	struct MeshData;
	struct VertexBufferWrapper;
	struct VertexDataWrapper;
	struct VertexIndexWrapper;

	class Model
	{
		friend class FBXFactory;
	public:
		Model();
		~Model();

		void Init();
		void InitPolygon();
		void InitCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f);
		void AddChild(Model* aChild);
		void InitGeometry(const MeshData& aMeshData);

		Effect* GetEffect();
		void SetEffect(Effect* aEffect);

		void Render(const CU::Matrix44<float>& aOrientation);

	private:
		void InitVertexBaseData(int aNumberOfVertices, VertexType aVertexType, int aVertexSize, char* aVertexData);
		void InitIndexBaseData(DXGI_FORMAT aFormat, int aNumberOfIndices, char* aIndexData);

		bool InitVertexBuffer();
		bool InitIndexBuffer();

		Effect* myEffect;
		ID3D11InputLayout* myVertexLayout;
		CU::GrowingArray<VertexPosNormUV> myVertices;
		CU::GrowingArray<int> myVerticeIndices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myVertexFormat;
		bool myIsNULLObject;

		VertexIndexWrapper* myIndexBaseData;
		IndexBufferWrapper* myIndexBuffer;
		VertexDataWrapper* myVertexBaseData;
		VertexBufferWrapper* myVertexBuffer;

		CU::GrowingArray<Surface*> mySurfaces;
		CU::GrowingArray<Model*> myChilds;
		CU::GrowingArray<CU::Matrix44f> myChildTransforms;
		CU::Matrix44f myOrientation;
	};
}