//
// 
// 描画するためのもの
// 
//

#include"ItsukiLib/SphereCollider.h"
#include "ItsukiLib/BoxCollider.h"

#pragma once

namespace Itsuki
{

	class ColliderRenderer
	{

	public:

		 ColliderRenderer(ID3D11Device* pDevice,
			ID3D11DeviceContext* pContext,
			DirectX::CommonStates* pStates,
			DirectX::FXMVECTOR color = DirectX::Colors::Blue,
			int segments = SEGMENTS
		);

		void Render(ID3D11DeviceContext* pContext,
			const DirectX::SimpleMath::Matrix& view,
            const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider);

	private:

		void SphereRender(
			ID3D11DeviceContext* pContext,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider
		);

		void BoxRender(ID3D11DeviceContext* pContext,
			const DirectX::SimpleMath::Matrix& view,
            const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider);

	private:

		//球に使われる分割
		static constexpr int SEGMENTS = 12;

	private:

		// 共通ステートへのポインタ
		DirectX::CommonStates* m_pStates;

		// ベーシックエフェクトへのポインタ
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

		// プリミティブバッチへのポインタ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		// 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		// セグメント数
		int m_segments;

		// 表示色
		DirectX::SimpleMath::Color m_color;



	};

}
