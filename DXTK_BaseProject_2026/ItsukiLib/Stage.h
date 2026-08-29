#pragma once

#include"ItsukiLib/BoxCollider.h"
#include"../Scene/GamePlayScene/Obj.h"
#include <memory>
#include <GameContext.h>

using namespace DirectX;

class Stage:public Obj
{
public:

	Stage(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pPrimitiveBatch,
          std::unique_ptr<Itsuki::Collider> collider);

	~Stage();

	void Update(float elapsedtime)override;

	void Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                const DirectX::SimpleMath::Matrix& proj, const DirectX::SimpleMath::Vector3& eye,
                const DirectX::SimpleMath::Vector3& target);

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


    SimpleMath::Vector3 m_minPos;
    SimpleMath::Vector3 m_maxPos;
};
