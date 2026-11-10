#pragma once
#include <ItsukiLib/Collider.h>
#include "CollisionChecker.h"
#include <GeometricPrimitive.h>

#include "Obj.h"

class ThrowableObject:public Obj
{
public:

    ThrowableObject(ID3D11DeviceContext* context, std::unique_ptr<Itsuki::Collider> collider,
                    float size, int requiredCarryPower);

    //更新関数
    void Update(float elapsedTime);

    //描画関数
    void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection,
                const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& forward,
                const DirectX::SimpleMath::Vector3& up);

    bool IsGetRay(const Itsuki::Ray& ray, const Itsuki::CollisionChecker& checker, float& distance) const;

    //投げる関数
    void Throw(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity);

    //持たれた際の位置の変更
    void SetHeldPosition(const DirectX::SimpleMath::Vector3& position);

    
    //当たり判定の取得
    Itsuki::Collider* GetCollider() const
    { 
        return m_collider.get();
    }

    // 持っているか判定する関数
    bool IsHeld() const
    {
        return m_isHeld;
    }

    // どのくらいの重さなのか返す関数
    int GetRequiredCarryPower() const
    {
        return m_requiredCarryPower;
    }

    // 持ち上げた際の変更
    void PickUp()
    {
        m_isHeld = true;
        m_velocity = DirectX::SimpleMath::Vector3::Zero;
    }

    // 速度を取得する関数
    DirectX::SimpleMath::Vector3 GetVelocity() const
    {
        return m_velocity;
    }

    // 速度を設定する関数
    void SetVelocity(DirectX::SimpleMath::Vector3 velocity)
    {
        m_velocity = velocity;
    }

private:


    std::unique_ptr<DirectX::GeometricPrimitive> m_primitive;

    DirectX::SimpleMath::Vector3 m_velocity = DirectX::SimpleMath::Vector3::Zero;

    float m_size;

    int m_requiredCarryPower;

    bool m_isHeld = false;

};
