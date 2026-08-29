#include "pch.h"
#include "Orb.h"
#include <random>

Orb::Orb(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch,
         ID3D11ShaderResourceView* pTexture, std::unique_ptr<Itsuki::Collider> collider)
    : m_pGameContext{pGameContext}, m_pPrimitiveBatch{pPrimitiveBatch}, m_pTexture{pTexture}
{
    // 当たり判定をセットする
    SetCollider(std::move(collider));

    auto device = m_pGameContext->deviceResources.GetD3DDevice();

    // ベーシックエフェクトの作成
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
    //色の使用
    m_basicEffect->SetVertexColorEnabled(true);
    //光の当たり具合
    m_basicEffect->SetLightingEnabled(false);
    //画像の使用
    m_basicEffect->SetTextureEnabled(true);
    m_basicEffect->SetTexture(m_pTexture);

    
	// 入力レイアウトの作成
    DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColorTexture>(device, m_basicEffect.get(),
                                                                              m_inputLayout.ReleaseAndGetAddressOf()));


    SetRandom();
}

void Orb::Update(float elapsedTime)
{
}

void Orb::Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                 const DirectX::SimpleMath::Matrix& proj, const DirectX::SimpleMath::Vector3& eye,
                 const DirectX::SimpleMath::Vector3& target)
{
    auto device = m_pGameContext->deviceResources.GetD3DDeviceContext();
    auto& states = m_pGameContext->commonStates;

    // ブレンドステートの設定（）
    pContext->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xFFFFFFFF);
    // 深度バッファの設定（通常）
    pContext->OMSetDepthStencilState(states.DepthDefault(), 0);
    // カリングの設定（カリングなし）
    pContext->RSSetState(states.CullNone());


    //------------------------------Beginをする際に必要となるもの

    // 各行列の設定
    SimpleMath::Matrix world = world = SimpleMath::Matrix::CreateBillboard(m_position, eye, SimpleMath::Vector3::UnitY);
    m_basicEffect->SetWorld(world);
    m_basicEffect->SetView(view);
    m_basicEffect->SetProjection(proj);

    // エフェクトを適用する
    m_basicEffect->Apply(device);
    //-----------------------------

    //後ろにもテクスチャを張る
    auto sampler = states.LinearWrap();
    pContext->PSSetSamplers(0, 1, &sampler);


    // 入力レイアウトを設定
    device->IASetInputLayout(m_inputLayout.Get());

    // 板ポリゴンを描画
    DX11::VertexPositionColorTexture v[4] = {
        VertexPositionColorTexture(SimpleMath::Vector3( -0.5f, 0.0f, 0.0f), SimpleMath::Vector4(.0,0,1,1),SimpleMath::Vector2(0.0f, 1.0f)),
        VertexPositionColorTexture(SimpleMath::Vector3( -0.5f, 1.0f, 0.0f), SimpleMath::Vector4(.0, 0, 1, 1),SimpleMath::Vector2(0.0f, 0.0f)),
        VertexPositionColorTexture(SimpleMath::Vector3( 0.5f, 0.0f, 0.0f), SimpleMath::Vector4(.0, 0, 1, 1),SimpleMath::Vector2(1.0f, 1.0f)),
        VertexPositionColorTexture(SimpleMath::Vector3( 0.5f, 1.0f, 0.0f), SimpleMath::Vector4(.0, 0, 1, 1),SimpleMath::Vector2(1.0f, 0.0f)),
    };

    // スプライトバッチの設定
    m_pPrimitiveBatch->Begin();

    m_pPrimitiveBatch->Draw(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, v, 4);

    // スプライトバッチの設定
    m_pPrimitiveBatch->End();
}

void Orb::SetRandom()
{
    //-----------やむを得ず-----------
    // 非決定論的な乱数の種（利用できない環境では固定値になる場合あり）
    std::random_device rd;

    // 乱数生成器に種を設定
    std::mt19937 gen(rd());

    // 1〜100の整数を生成する分布
    std::uniform_real_distribution<float> distXZ(-2, 2);
    std::uniform_real_distribution<float> distY(1, 3);

    // 乱数を10個生成
    DirectX::SimpleMath::Vector3 Pos = {distXZ(gen), distY(gen), distXZ(gen)};

    // ランダムに位置を設定する
    SetPosition(Pos);

    // 当たり判定の位置も設定
    m_collider->SetPosition(Pos);
    //-----------やむを得ず-----------
}
