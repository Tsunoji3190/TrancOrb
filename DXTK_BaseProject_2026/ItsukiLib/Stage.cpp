#include "pch.h"
#include "Stage.h"

Stage::Stage(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pPrimitiveBatch,
             std::unique_ptr<Itsuki::Collider> collider)
    :m_pPrimitiveBatch{pPrimitiveBatch} ,m_pGameContext{pGameContext}
{
    //位置と当たり判定の形を設定する
    SetCollider(std::move(collider));
   
    //位置の設定
    SetPosition(m_collider->GetPosition());

    //現在は立方体のステージしか考えていないので直接minとmaxを取ってしまう
    m_minPos = m_collider->GetParam().min;
    m_maxPos = m_collider->GetParam().max;

    auto device = m_pGameContext->deviceResources.GetD3DDevice();

    // ベーシックエフェクトの作成
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
    // 色の使用
    m_basicEffect->SetVertexColorEnabled(true);
    // 光の当たり具合
    m_basicEffect->SetLightingEnabled(false);
    // 画像の使用
    m_basicEffect->SetTextureEnabled(false);

    	// 入力レイアウトの作成
    DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColorTexture>(device, m_basicEffect.get(),
                                                                              m_inputLayout.ReleaseAndGetAddressOf()));
}

Stage::~Stage()
{

}

void Stage::Update(float elapsedtime)
{
}

void Stage::Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
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
    SimpleMath::Matrix world;
    m_basicEffect->SetWorld(world);
    m_basicEffect->SetView(view);
    m_basicEffect->SetProjection(proj);

    // エフェクトを適用する
    m_basicEffect->Apply(device);
    //-----------------------------


    // 前面の頂点データ
    VertexPositionColor d1[4] = {
        {SimpleMath::Vector3{m_minPos.x + m_position.x, m_maxPos.y + m_position.y,m_maxPos.z + m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)},  // 3
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 1

    };

    // 右面の頂点データ
    VertexPositionColor d2[4] = {
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 3
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 1

    };

    // 下面の頂点データ
    VertexPositionColor d3[4] = {
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 3
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 1

    };

    // 上面の頂点データ
    VertexPositionColor d4[4] = {
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 3
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_maxPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 1

    };

    // 左面の頂点データ
    VertexPositionColor d5[4] = {
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_maxPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_maxPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 3
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 1

    };

    // 後面の頂点データ
    VertexPositionColor d6[4] = {
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 0
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_maxPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.95, .95, .95, 1)}, // 2
        {SimpleMath::Vector3{m_minPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 3
        {SimpleMath::Vector3{m_maxPos.x+m_position.x, m_minPos.y+m_position.y, m_minPos.z+m_position.z}, SimpleMath::Vector4(.75, .75, .75, 1)}, // 1

    };
    // 頂点の描画順（インデックスデータ）
    uint16_t indexses[6] = {0, 1, 2, 0, 2, 3};


        // プリミティブバッジで描画する
    m_pPrimitiveBatch->Begin();

    // 描画
    // m_pPrimitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,v,3);

    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d1, 4);
    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d2, 4);
    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d3, 4);
    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d4, 4);
    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d5, 4);
    m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indexses, 6, d6, 4);

    m_pPrimitiveBatch->End();
}

