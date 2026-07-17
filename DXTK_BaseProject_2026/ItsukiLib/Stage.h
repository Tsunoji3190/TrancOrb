#pragma once

#include"ItsukiLib/BoxCollider.h"
#include"../Scene/GamePlayScene/Obj.h"
#include <memory>
#include <GameContext.h>

using namespace DirectX;

class Stage:public Obj
{
public:

	Stage(GameContext* pGameContext,
          DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pPrimitiveBatch ,SimpleMath::Vector3 min,
          SimpleMath::Vector3 max, SimpleMath::Vector3 pos);

	~Stage();

	void Update(float elapsedtime)override;

	void Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                const DirectX::SimpleMath::Matrix& proj, const DirectX::SimpleMath::Vector3& eye,
                const DirectX::SimpleMath::Vector3& target);

	Itsuki::BoxCollider* GetCollider()
	{
        return m_collider.get();
	}


private:

    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // ベーシックエフェクトへのポインタ
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

    // スプライトバッチへのポインタ
    DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* m_pPrimitiveBatch;

    // テクスチャへのポインタ
    ID3D11ShaderResourceView* m_pTexture;

    // 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    std::unique_ptr<Itsuki::BoxCollider> m_collider;

    SimpleMath::Vector3 m_minPos;
    SimpleMath::Vector3 m_maxPos;
};
