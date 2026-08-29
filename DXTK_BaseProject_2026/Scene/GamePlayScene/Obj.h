#pragma once
#include"pch.h"
#include <ItsukiLib/Collider.h>

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

    //当たり判定を返す関数
    Itsuki::Collider* GetCollider()
    {
        return m_collider.get();
    }

    // 当たり判定を設定する関数
    void SetCollider(std::unique_ptr<Itsuki::Collider> coll)
    {
        m_collider = std::move(coll);
    }

protected:

    //位置
    DirectX::SimpleMath::Vector3 m_position;

    //当たり判定
    std::unique_ptr<Itsuki::Collider> m_collider;

    // オブジェのモデルハンドル
    std::unique_ptr<DirectX::Model> m_model;
};
