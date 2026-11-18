#pragma once
#include"../../ItsukiLib/Obj.h"
#include "../../GameContext.h"

class Fruit:public Obj
{
public:

    Fruit(GameContext* pGameContext, const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel,
        std::unique_ptr<Itsuki::Collider> collider);

    void Update(float elapsedTime) override;

    void Render();

    void SetRandom();

    //落ちる関数
    void Fall();

private:
    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // ビュー行列
    const DirectX::SimpleMath::Matrix& m_view;

    // プロジェクション行列
    const DirectX::SimpleMath::Matrix& m_projection;

    // モデルへのポインタ
    DirectX::Model* m_pModel = nullptr;

    // フルーツの価値
    int m_Value = 1;
};
