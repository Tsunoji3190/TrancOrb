#pragma once
#include <vector>
#include <memory>
#include "../Orb.h"

class OrbManager
{
public:

    OrbManager(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch,
        ID3D11ShaderResourceView* pTexture,
               const int firstorb = FIRST_ORB)
        : m_pGameContext{pGameContext}
        , m_pPrimitiveBatch{pPrimitiveBatch}
        , m_pTexture{pTexture}
        , m_pOrbs{}
    { 
        
        for (size_t i = 0; i < firstorb; i++)
        {
            AddOrb();
        }

    };

    ~OrbManager() = default;

    void Update();

    void Render();

    void AddOrb()
    {
        
        std::unique_ptr<Orb> orb = std::make_unique<Orb>(m_pGameContext,m_pPrimitiveBatch,m_pTexture);
        
        m_pOrbs.push_back(std::move(orb));
    }


    //指定されたステージを返す
    Orb* GetOrb(int Index)
    {
        return m_pOrbs[Index].get();
    }

    //ステージの数を返す
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

private:

    //オーブの数
    static constexpr int FIRST_ORB = 5;

    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // スプライトバッチへのポインタ
    DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_pPrimitiveBatch;

    // テクスチャへのポインタ
    ID3D11ShaderResourceView* m_pTexture;

    //ステージを格納する箱
    std::vector<std::unique_ptr<Orb>> m_pOrbs = {};

};


