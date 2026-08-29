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
                                      const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider)
{

    switch (collider.GetShape())
    {
        case Itsuki::SHAPE::BOX:
            
            BoxRender(pContext,view,proj,collider);
            break;

        case Itsuki::SHAPE::SPHERE:
            
            SphereRender(pContext, view, proj, collider);
            break;
    }
}

void Itsuki::ColliderRenderer::SphereRender(ID3D11DeviceContext* pContext, 
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider)
{

    m_color = Colors::Blue;

    const SphereCollider* Sphere = dynamic_cast<const SphereCollider*>(&collider);


    //半径を取得
    float r = Sphere->GetRadius();

    //円の中心点を取得
    SimpleMath::Vector3 center = Sphere->GetPosition();


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

    //XZ平面に円を描く
    for (int j = 0; j < SEGMENTS; j++)
    {
        //Φを作成
        float phi = XM_2PI * j / SEGMENTS; 


        for (int i = 0; i < m_segments; i++)
        {
            // 円周に存在する一つの点
            float t0 = XM_2PI * i / m_segments;
            // 円周に存在するt0より値の大きな一つの点
            float t1 = XM_2PI * (i + 1) / m_segments;


            //X
            float x0p = r * cosf(phi) * sin(t0);
            float x1p = r * cosf(phi) * sin(t1);

            // Y
            float y0p = r * sin(phi);
            float y1p = r * sin(phi);


            //Z
            float z0p = r * cosf(phi) * cos(t0);
            float z1p = r * cosf(phi) * cos(t1);


            SimpleMath::Vector3 p0 = center + SimpleMath::Vector3(x0p, y0p, z0p);
            SimpleMath::Vector3 p1 = center + SimpleMath::Vector3(x1p, y1p, z1p);

            m_primitiveBatch->DrawLine({p0, m_color}, {p1, m_color});

        }

        // YZ平面に円を描く
        for (int i = 0; i < m_segments; i++)
        {
            //円周に存在する一つの点
            float t0 = XM_2PI * i / m_segments;
            //円周に存在するt0より値の大きな一つの点
            float t1 = XM_2PI * (i + 1) / m_segments;

            // YZ上の円

            float z0 = cosf(t0) * r;
            float y0 = sinf(t0) * r;

            float z1 = cosf(t1) * r;
            float y1 = sinf(t1) * r;

            // Y軸回転
            float x0p = z0 * cosf(phi);
            float z0p = z0 * sinf(phi);

            float x1p = z1 * cosf(phi);
            float z1p = z1 * sinf(phi);

            SimpleMath::Vector3 p0 = center + SimpleMath::Vector3(x0p, y0, z0p);
            SimpleMath::Vector3 p1 = center + SimpleMath::Vector3(x1p, y1, z1p);

            m_primitiveBatch->DrawLine({p0, m_color}, {p1, m_color});
        }

    }


    // 球の描画終わり
    m_primitiveBatch->End();
}

void Itsuki::ColliderRenderer::BoxRender(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                                         const DirectX::SimpleMath::Matrix& proj, Itsuki::Collider& collider)
{
    m_color = Colors::Green;

    using namespace DirectX::SimpleMath;

    const BoxCollider* Box = dynamic_cast<const BoxCollider*>(&collider);


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
    Vector3 min = Box->GetMin();

    // 四角の当たり判定の一番大きい座標を取る
    Vector3 max = Box->GetMax();

    //四角の角八点を作成
    Vector3 p[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z},
    };



    // 12本のエッジを描く
    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;

        m_primitiveBatch->DrawLine({p[i], m_color}, {p[j], m_color});
    }


    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;

        m_primitiveBatch->DrawLine({p[i+4], m_color}, {p[j+4], m_color});
    }

    for (int i = 0; i < 4; i++)
    {
        int j = i + 1;

        m_primitiveBatch->DrawLine({p[i], m_color}, {p[j + 3], m_color});
    }



    //---------------網目状の表現--------------------
    int divX = (max.x - min.x) * 1;
    int divY = (max.y - min.y) * 1;
    int divZ = (max.z - min.z) * 1;

    //全X軸の網の描画
    for (int i = 1; i < divX; i++)
    {

        float t = (float)i / divX;

        // 四角の上面の縦線
        float x1 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x1, max.y, min.z), m_color}, {Vector3(x1, max.y, max.z), m_color});
        // 四角の下面の縦線
        float x2 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x2, min.y, min.z), m_color}, {Vector3(x2, min.y, max.z), m_color});
        // 四角の前面の縦線
        float x3 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x3, min.y, min.z), m_color}, {Vector3(x3, max.y, min.z), m_color});
        // 四角の後面の縦線
        float x4 = min.x + (max.x - min.x) * t;
        m_primitiveBatch->DrawLine({Vector3(x4, min.y, max.z), m_color}, {Vector3(x2, max.y, max.z), m_color});

    }

    // 全Y軸の網の描画
    for (int i = 1; i < divY; i++)
    {
        float t = (float)i / divY;

        // 四角の前面の横線
        float y1 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y1, max.z), m_color}, {Vector3(max.x, y1, max.z), m_color});

        // 四角の後面の横線
        float y2 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y2, min.z), m_color}, {Vector3(max.x, y2, min.z), m_color});

        // 四角の右側面の横線
        float y3 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(max.x, y3, min.z), m_color}, {Vector3(max.x, y3, max.z), m_color});

        // 四角の左側面の横線
        float y4 = min.y + (max.y - min.y) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, y4, min.z), m_color}, {Vector3(min.x, y2, max.z), m_color});

    }

    // 全Z軸の網の描画
    for (int i = 1; i < divZ; i++)
    {
        float t = (float)i / divZ;

        // 四角の右側面縦線
        float z1 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(max.x, min.y, z1), m_color}, {Vector3(max.x, max.y, z1), m_color});

        // 四角の左側面のZの縦線
        float z2 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, min.y, z2), m_color}, {Vector3(min.x, max.y, z2), m_color});

        // 四角の上面のZの横線
        float z3 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, max.y, z3), m_color}, {Vector3(max.x, max.y, z1), m_color});

        // 四角の下面のZの横線
        float z4 = min.z + (max.z - min.z) * t;
        m_primitiveBatch->DrawLine({Vector3(min.x, min.y, z4), m_color}, {Vector3(max.x, min.y, z1), m_color});


    }


    m_primitiveBatch->End();
}
