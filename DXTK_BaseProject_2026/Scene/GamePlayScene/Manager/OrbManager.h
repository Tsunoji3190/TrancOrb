#pragma once
#include <vector>
#include <memory>
#include "../Orb.h"
#include"../../../ItsukiLib/ColliderFactory.h"

class OrbManager
{
public:

    OrbManager(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch,
        ID3D11ShaderResourceView* pTexture)
        : m_pGameContext{pGameContext}
        , m_pPrimitiveBatch{pPrimitiveBatch}
        , m_pTexture{pTexture}
        , m_pOrbs{},
          m_orbValues{FIRST_ORBVALUE}
    { 
        
        for (size_t i = 0; i < FIRST_ORB; i++)
        {
            AddOrb();
        }

    };

    ~OrbManager() = default;

    void Update();

    void Render();

    void AddOrb()
    {
        // 当たり判定工場の作成
        Itsuki::ColliderFactory& colF = Itsuki::ColliderFactory::GetRefInstance();

        /// 球
        Itsuki::CollisionParams orbCol = {{0.0f, 0.0f, .0f}, {-.15f, -.15f, -.15f}, {0.15f, 0.15f, 0.15f}, {0.3}};


        std::unique_ptr<Orb> orb = std::make_unique<Orb>(m_pGameContext, m_pPrimitiveBatch, m_pTexture,
                                                         colF.MakeCollider(Itsuki::SHAPE::SPHERE, orbCol));
        
        m_pOrbs.push_back(std::move(orb));
    }


    //指定されたオーブを返す
    Orb* GetOrb(int Index)
    {
        return m_pOrbs[Index].get();
    }

    //オーブの数を返す
    int GetNumOrbs()
    {
        return m_pOrbs.size();
    }

    void Sort(DirectX::SimpleMath::Vector3 eyePos)
    {
        for (auto& orb:m_pOrbs)
        {
            DirectX::SimpleMath::Vector3 eyePosition = eyePos;
            orb->SetCameraDistance(eyePosition);
        }

        std::sort(m_pOrbs.begin(), m_pOrbs.end(),
                  [](const std::unique_ptr<Orb>& a, const std::unique_ptr<Orb>& b)
                  { return (a->GetCameraDistance() > b->GetCameraDistance()); });
    }

    // オーブの価値を追加する
    void SetOrbValue(int value)
    {
        m_orbValues += value;
    }

    // オーブの価値を返す
    int GetOrbValue()
    {
        return m_orbValues;
    }

private:

    //オーブの数
    static constexpr int FIRST_ORB = 1;

    static constexpr int FIRST_ORBVALUE = 1;

private:
    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // スプライトバッチへのポインタ
    DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_pPrimitiveBatch;

    // テクスチャへのポインタ
    ID3D11ShaderResourceView* m_pTexture;

    //オーブを格納する箱
    std::vector<std::unique_ptr<Orb>> m_pOrbs;

    //オーブの基本価格
    int m_orbValues;
};


