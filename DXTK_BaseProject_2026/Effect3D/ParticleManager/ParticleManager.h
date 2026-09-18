//--------------------------------------------------------------------------------------
//	File: ParticleManager.h
//
//	パーティクルマネージャクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "Common/StepTimer.h"
#include "Common/DeviceResources.h"
#include "pch.h"
#include <vector>

#include "Effect3D/ParticleManager/ParticlePosition.h"


namespace Effect3D
{
	class ParticleManager
	{
	public:
		//	データ受け渡し用コンスタントバッファ(送信側)
		struct ConstBuffer
		{
			DirectX::SimpleMath::Matrix		matWorld;
			DirectX::SimpleMath::Matrix		matView;
			DirectX::SimpleMath::Matrix		matProj;
			DirectX::SimpleMath::Vector4	Diffuse;
		};

	private:
		//	変数
		DX::DeviceResources* m_pDR;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

		DX::StepTimer                           m_timer;
		//	入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		//	プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPosition>> m_batch;
		//	コモンステート
		std::unique_ptr<DirectX::CommonStates> m_states;
		//	テクスチャハンドル
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
		//	テクスチャハンドル
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
		//	頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		//	ピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		//	ジオメトリシェーダ
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_proj;

		std::vector<std::unique_ptr<Effect3D::ParticlePosition>>	m_particlePosition;


	public:
		//	関数
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		ParticleManager(int count);
		~ParticleManager();

		void LoadTextureWIC(const wchar_t* path);
		void LoadTextureDDS(const wchar_t* path);

		void Create(DX::DeviceResources* pDR);

		void Update(float elapsedTime);
		void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Matrix billboard = DirectX::SimpleMath::Matrix::Identity);
		void Sort(DirectX::SimpleMath::Vector3 eyePos);

	private:

		void CreateShader();
	};
}