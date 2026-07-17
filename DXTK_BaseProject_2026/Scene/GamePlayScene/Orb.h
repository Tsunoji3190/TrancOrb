#pragma once
#include"Obj.h"
#include "../../GameContext.h"

class Orb:public Obj
{
public:

    Orb(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch,
        ID3D11ShaderResourceView* pTexture);

    void Update(float elapsedTime) override;

    void Render(ID3D11DeviceContext* pContext, const DirectX::SimpleMath::Matrix& view,
                const DirectX::SimpleMath::Matrix& proj, const DirectX::SimpleMath::Vector3& eye,
                const DirectX::SimpleMath::Vector3& target);

    Itsuki::SphereCollider* GetCollider()
    {
        return m_collider.get();
    }

    void SetRandom();

    //オーブの価値を追加する 
    void AddOrbValue(int value)
    {
        m_OrbValue += value;
    }

    //オーブの価値を返す
    int GetOrbValue()
    {
        return m_OrbValue;
    }

    //カメラとの距離を設定する
    void SetCameraDistance(DirectX::SimpleMath::Vector3& eyePosition)
    {
        m_cameraDistance = (m_position - eyePosition).Length();
    }

    //カメラとの距離を設定する
    float GetCameraDistance()
    {
        return m_cameraDistance;
    }

private:
    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // ベーシックエフェクトへのポインタ
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

    // スプライトバッチへのポインタ
    DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_pPrimitiveBatch;

    // テクスチャへのポインタ
    ID3D11ShaderResourceView* m_pTexture;

    // 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    // 当たり判定
    std::unique_ptr<Itsuki::SphereCollider> m_collider;

    //オーブの価値
    int m_OrbValue = 1;

    //カメラとの距離
    float m_cameraDistance;
};
