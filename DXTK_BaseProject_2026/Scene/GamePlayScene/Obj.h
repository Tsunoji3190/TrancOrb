#pragma once
#include"pch.h"

class Obj
{
public:

    enum Type
    {
        NONE=-1,
        Player,
        Wall,
        Orb
    };

    Obj() {};

    virtual ~Obj() = default;

    virtual void Update(float elapsedTime) = 0;

    DirectX::SimpleMath::Vector3 GetPosition()
    {
        return m_position;
    }

    void SetPosition(DirectX::SimpleMath::Vector3 position)
    {
        m_position = position;
    }

    Collider* GetCollider()
    {
        return m_collider.get();
    }

protected:

    //位置
    DirectX::SimpleMath::Vector3 m_position;

    //当たり判定
    std::unique_ptr<Collider> m_collider;

    // オブジェのモデルハンドル
    std::unique_ptr<DirectX::Model> m_model;
};
