#include "pch.h"
#include "Fruit.h"
#include <random>

Fruit::Fruit(GameContext* pGameContext, const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel,
        std::unique_ptr<Itsuki::Collider> collider)
    : m_pGameContext{pGameContext}        
    , m_view(view)
    , m_projection(projection)
    , m_pModel(pModel)

{
    // 当たり判定をセットする
    SetCollider(std::move(collider));

    auto device = m_pGameContext->deviceResources.GetD3DDevice();

    SetRandom();
}

void Fruit::Update(float elapsedTime)
{
}

void Fruit::Render()
{
    using namespace DirectX::SimpleMath;


    SimpleMath::Matrix world =SimpleMath::Matrix::CreateScale(0.3) * SimpleMath::Matrix::CreateTranslation(m_position);
    // モデルの描画
    m_pModel->Draw(m_pGameContext->deviceResources.GetD3DDeviceContext(), m_pGameContext->commonStates, world, m_view,
                   m_projection);
}

void Fruit::SetRandom()
{
    // 非決定論的な乱数の種（利用できない環境では固定値になる場合あり）
    std::random_device rd;

    // 乱数生成器に種を設定
    std::mt19937 gen(rd());

    // 1〜100の整数を生成する分布
    std::uniform_real_distribution<float> distXZ(-3, 3);
    std::uniform_real_distribution<float> distY(3, 4);

    // 乱数を10個生成
    DirectX::SimpleMath::Vector3 Pos = {distXZ(gen), distY(gen), distXZ(gen)};

    // ランダムに位置を設定する
    SetPosition(Pos);

    // 当たり判定の位置も設定
    m_collider->SetPosition({Pos.x,Pos.y+0.5f,Pos.z});
}

void Fruit::Fall()
{
}
