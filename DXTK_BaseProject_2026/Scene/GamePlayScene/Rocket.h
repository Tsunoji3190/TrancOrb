#pragma once

#include "../../ItsukiLib/Obj.h"
#include <GameContext.h>

using namespace DirectX;

class Rocket : public Obj
{
public:
    Rocket(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel,
        std::unique_ptr<Itsuki::Collider> collider)
        : m_gameContext(gameContext)
        , m_view(view)
        , m_projection(projection)
        , m_pModel(pModel)
    {
        // 当たり判定を設定する
        SetCollider(std::move(collider));

        // 位置の設定
        SetPosition(m_collider->GetParam().pos);

    }

    void Update(float elapsedtime) override
    {

    }

    void Render()
    {
        SimpleMath::Matrix world = SimpleMath::Matrix::CreateTranslation(m_position);

        // モデルの描画
        m_pModel->Draw(m_gameContext.deviceResources.GetD3DDeviceContext(), m_gameContext.commonStates, world, m_view,
                       m_projection);

    }

    private:

    // ゲームコンテキスト
    const GameContext& m_gameContext;

    // ビュー行列
    const DirectX::SimpleMath::Matrix& m_view;

    // プロジェクション行列
    const DirectX::SimpleMath::Matrix& m_projection;

    // モデルへのポインタ
    DirectX::Model* m_pModel = nullptr;
};