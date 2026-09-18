//--------------------------------------------------------------------------------------
// File: Drawer.h
//
// 画像表示クラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "Common/StepTimer.h"
#include "UserInterface.h"
#include "Common/DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
namespace Effect3D
{
	class Drawer
	{
	public:
		//	データ受け渡し用コンスタントバッファ(送信側)
		struct ConstBuffer
		{
			DirectX::SimpleMath::Vector4	windowSize;
			DirectX::SimpleMath::Matrix		rotationMatrix;
		};
	private:
		//	変数
		DX::DeviceResources* m_pDR;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

		DX::StepTimer                           m_timer;
		//	入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		//	プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
		//	コモンステート
		std::unique_ptr<DirectX::CommonStates> m_states;
		//	テクスチャハンドル
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
		//	頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		//	ピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		//	ジオメトリシェーダ
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

		int m_windowWidth , m_windowHeight;
		int m_textureWidth, m_textureHeight;

		DirectX::SimpleMath::Vector2 m_scale;
		DirectX::SimpleMath::Vector2 m_baseScale;
		DirectX::SimpleMath::Vector2 m_position;

		ANCHOR m_anchor;

		float m_renderRatio;

		DirectX::SimpleMath::Matrix m_rotationMatrix;

	public:
		//	関数
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		Drawer();
		~Drawer();

		void LoadTexture(const wchar_t* path);
		
		void Create(DX::DeviceResources* pDR
			, const wchar_t* path
			, DirectX::SimpleMath::Vector2 position
			,DirectX::SimpleMath::Vector2 scale
			,ANCHOR anchor);

		void Render();

		void SetWindowSize(const int &width,const int& height);

		void SetScale(DirectX::SimpleMath::Vector2 scale);
		DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
		DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }
		void SetPosition(DirectX::SimpleMath::Vector2 position);
		DirectX::SimpleMath::Vector2 GetPosition() { return m_position;}
		void SetAnchor(Effect3D::ANCHOR anchor);
		Effect3D::ANCHOR GetAnchor() { return m_anchor; }

		void SetRenderRatio(float ratio);
		float GetRenderRatio() { return m_renderRatio; }
	
		void SetRotationMatrix(DirectX::SimpleMath::Matrix rotation);
		DirectX::SimpleMath::Matrix GetRotationMatrix() { return m_rotationMatrix; }
	private:

		void CreateShader();
	};
}