#include "pch.h"
#include "ColliderRenderer.h"
using namespace DirectX;
using namespace DirectX::DX11;

Itsuki::ColliderRenderer::ColliderRenderer(ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext, 
	DirectX::CommonStates* pStates, 
	DirectX::FXMVECTOR color, 
	int segments
)
	: m_pStates(pStates)
	, m_color(color)
	,m_segments(segments)
{
    // ブレンドステートの設定（不透明）
    pContext->OMSetBlendState(m_pStates->Opaque(), nullptr, 0xFFFFFFFF);

    // カリングの設定（カリングなし）
    pContext->RSSetState(m_pStates->CullNone());

	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(pContext);

	// ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(pDevice);

	//色の設定
	m_basicEffect->SetVertexColorEnabled(true);

	//光の設定
	m_basicEffect->SetLightingEnabled(false);

	//テクスチャの設定
	m_basicEffect->SetTextureEnabled(false);

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			pDevice,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

}

void Itsuki::ColliderRenderer::Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                                      const DirectX::SimpleMath::Matrix& proj, Collider& collider)
{

    //switch (Collider.GetShape())
    //{
    //    case Collider::SHAPE::BOX:
    //           
    //    const BoxCollider* Box = dynamic_cast<const BoxCollider*>(&Collider);


    //}
}

void Itsuki::ColliderRenderer::Render(ID3D11DeviceContext* pContext, 
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj,
    SphereCollider& sphereCollider
)
{

    //半径を取得
    float r = sphereCollider.GetRadius();

    //円の中心点を取得
    SimpleMath::Vector3 center = sphereCollider.GetPosition();


    //------------------------------Beginをする際に必要となるもの
    // 深度バッファの設定（通常）
    pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);

    // 各行列の設定
    SimpleMath::Matrix world;
    m_basicEffect->SetWorld(world);
    m_basicEffect->SetView(view);
    m_basicEffect->SetProjection(proj);

    // エフェクトを適用する
    m_basicEffect->Apply(pContext);
    //-----------------------------


    // 入力レイアウトを設定
    pContext->IASetInputLayout(m_inputLayout.Get());

    // 球を描画
    m_primitiveBatch->Begin();


    // XY平面に円を描く
    for (int i = 0; i < m_segments; i++)
    {
        float t0 = XM_2PI * i / m_segments;
        float t1 = XM_2PI * (i + 1) / m_segments;

        SimpleMath::Vector3 p0 = center + SimpleMath::Vector3(cosf(t0) * r, sinf(t0) * r, 0);
        SimpleMath::Vector3 p1 = center + SimpleMath::Vector3(cosf(t1) * r, sinf(t1) * r, 0);

        m_primitiveBatch->DrawLine({ p0, m_color }, { p1, m_color });
    }

    // XZ平面に円を描く
    for (int i = 0; i < m_segments; i++)
    {
        float t0 = XM_2PI * i / m_segments;
        float t1 = XM_2PI * (i + 1) / m_segments;

        SimpleMath::Vector3 p0 = center + SimpleMath::Vector3(cosf(t0) * r, 0, sinf(t0) * r);
        SimpleMath::Vector3 p1 = center + SimpleMath::Vector3(cosf(t1) * r, 0, sinf(t1) * r);

        m_primitiveBatch->DrawLine({ p0, m_color }, { p1, m_color });
    }

    // YZ平面に円を描く
    for (int i = 0; i < m_segments; i++)
    {
        float t0 = XM_2PI * i / m_segments;
        float t1 = XM_2PI * (i + 1) / m_segments;

        SimpleMath::Vector3 p0 = center + SimpleMath::Vector3(0, cosf(t0) * r, sinf(t0) * r);
        SimpleMath::Vector3 p1 = center + SimpleMath::Vector3(0, cosf(t1) * r, sinf(t1) * r);

        m_primitiveBatch->DrawLine({ p0, m_color }, { p1, m_color });
    }

    // 球の描画終わり
    m_primitiveBatch->End();
}

void Itsuki::ColliderRenderer::Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                                      const DirectX::SimpleMath::Matrix& proj, BoxCollider& boxCollider)
{

    using namespace DirectX::SimpleMath;

    //------------------------------Beginをする際に必要となるもの
    // 深度バッファの設定（通常）
    pContext->OMSetDepthStencilState(m_pStates->DepthDefault(), 0);

    // 各行列の設定
    SimpleMath::Matrix world;
    m_basicEffect->SetWorld(world);
    m_basicEffect->SetView(view);
    m_basicEffect->SetProjection(proj);

    // エフェクトを適用する
    m_basicEffect->Apply(pContext);
    //-----------------------------
    
    // 入力レイアウトを設定
    pContext->IASetInputLayout(m_inputLayout.Get());


    m_primitiveBatch->Begin();

    //四角の当たり判定の一番小さい座標を取る
    Vector3 min = boxCollider.GetMin();

    // 四角の当たり判定の一番大きい座標を取る
    Vector3 max = boxCollider.GetMax();

    //四角の角八点を作成
    Vector3 p[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z},
    };



    // 12本のエッジを描く
    auto c = Colors::Black;

    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;

        m_primitiveBatch->DrawLine({p[i], c}, {p[j], c});
    }


    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;

        m_primitiveBatch->DrawLine({p[i+4], c}, {p[j+4], c});
    }

    for (int i = 0; i < 4; i++)
    {
        int j = i + 1;

        m_primitiveBatch->DrawLine({p[i], c}, {p[j + 3], c});
    }



    //---------------網目状の表現--------------------
    int divX = (max.x - min.x) * 1.5;
    int divY = (max.y - min.y) * 1.5;
    int divZ = (max.z - min.z) * 1.5;

    //全X軸の網の描画
    for (int i = 1; i < divX; i++)
    {

        float t = (float)i / divX;

        // 四角の上面の縦線
        float x1 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x1, max.y, min.z), c}, {Vector3(x1, max.y, max.z), c});
        // 四角の下面の縦線
        float x2 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x2, min.y, min.z), c}, {Vector3(x2, min.y, max.z), c});
        // 四角の前面の縦線
        float x3 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x3, min.y, min.z), c}, {Vector3(x3, max.y, min.z), c});
        // 四角の後面の縦線
        float x4 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x4, min.y, max.z), c}, {Vector3(x2, max.y, max.z), c});

    }

    // 全Y軸の網の描画
    for (int i = 1; i < divY; i++)
    {
        float t = (float)i / divY;

        // 四角の前面の横線
        float y1 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y1, max.z), c}, {Vector3(max.x, y1, max.z), c});

        // 四角の後面の横線
        float y2 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y2, min.z), c}, {Vector3(max.x, y2, min.z), c});

        // 四角の右側面の横線
        float y3 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(max.x, y3, min.z), c}, {Vector3(max.x, y3, max.z), c});

        // 四角の左側面の横線
        float y4 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y4, min.z), c}, {Vector3(min.x, y2, max.z), c});

    }

    // 全Z軸の網の描画
    for (int i = 1; i < divZ; i++)
    {
        float t = (float)i / divZ;

        // 四角の右側面縦線
        float z1 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(max.x, min.y, z1), c}, {Vector3(max.x, max.y, z1), c});

        // 四角の左側面のZの縦線
        float z2 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, min.y, z2), c}, {Vector3(min.x, max.y, z2), c});

        // 四角の上面のZの横線
        float z3 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, max.y, z3), c}, {Vector3(max.x, max.y, z1), c});

        // 四角の下面のZの横線
        float z4 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, min.y, z4), c}, {Vector3(max.x, min.y, z1), c});


    }


    m_primitiveBatch->End();
}
