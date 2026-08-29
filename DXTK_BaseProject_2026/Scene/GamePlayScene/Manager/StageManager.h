#pragma once
#include <vector>
#include <memory>
#include "ItsukiLib/Stage.h"

class StageManager
{
public:

    StageManager(GameContext* pGameContext, DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pPrimitiveBatch)
        : m_pGameContext{pGameContext}, m_pPrimitiveBatch{pPrimitiveBatch}
        , m_pStages{}
    { 

    };

    ~StageManager() = default;

    void Update();

    void Render();

    void AddStage( std::unique_ptr<Itsuki::Collider> collider)
    {
        
        std::unique_ptr<Stage> stage = std::make_unique<Stage>(m_pGameContext, m_pPrimitiveBatch, std::move(collider));
        
        m_pStages.push_back(std::move(stage));
    }


    //指定されたステージを返す
    Stage* GetStage(int Index)
    {
        return m_pStages[Index].get();
    }

    //ステージの数を返す
    int GetNumStages()
    {
        return m_pStages.size();
    }

private:

    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // スプライトバッチへのポインタ
    DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* m_pPrimitiveBatch;


    //ステージを格納する箱
    std::vector<std::unique_ptr<Stage>> m_pStages = {};

};


