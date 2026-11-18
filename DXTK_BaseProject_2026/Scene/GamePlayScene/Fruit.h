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

    //落ちる際に反応する関数
    void Fall(float Yvel);

    void SetVelocity(SimpleMath::Vector3 vel)
    {
        m_velocity = vel;
    }

private:

    //大きさ
    static constexpr float SCALE = 0.3f;
    //加速
    DirectX::SimpleMath::Vector3 m_velocity = DirectX::SimpleMath::Vector3::Zero;

    //落ちるかどうか
    bool m_isFall=false;

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
