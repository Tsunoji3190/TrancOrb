#include "pch.h"
#include "ThrowableObject.h"
#include "BoxCollider.h"
#include "ItsukiMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ThrowableObject::ThrowableObject(ID3D11DeviceContext* context, std::unique_ptr<Itsuki::Collider> collider,
                                 float size, int requiredCarryPower)
    : m_size(size)
    , m_requiredCarryPower(requiredCarryPower)
{
    // 当たり判定をセットする
    SetCollider(std::move(collider));

    // 位置の設定
    SetPosition(m_collider->GetParam().pos);

    m_primitive = GeometricPrimitive::CreateCube(context);
}

void ThrowableObject::Update(float elapsedTime)
{
    if (m_isHeld)
        return;

    m_velocity.y -= Itsuki::Physics::GRAVITY * elapsedTime;

    m_position += m_velocity * elapsedTime;

    //万が一すり抜けで下まで落ちてしまったら
    if (m_position.y < -10.0f)
    {
        //リスポーンさせる
        m_position = {0.0, 0.0, 0.0};
        m_velocity = {0.0, 0.0, 0.0};
    }
    //位置の設定
    m_collider->SetPosition(m_position);
}

void ThrowableObject::Render(const Matrix& view, const Matrix& projection, const Vector3& eye,
                             const Vector3& forward, const Vector3& up)
{
    Vector3 position = m_position;
    if (m_isHeld)
    {
        Vector3 right = forward.Cross(up);
        right.Normalize();
        position = eye + forward * 1.25f + right * 0.55f - up * 0.42f;
    }
    const Matrix world = Matrix::CreateScale(m_size) * Matrix::CreateTranslation(position);
    m_primitive->Draw(world, view, projection, m_isHeld ? Colors::White : Colors::SandyBrown);
}

bool ThrowableObject::IsGetRay(const Itsuki::Ray& ray, const Itsuki::CollisionChecker& checker, float& distance) const
{
    return checker.CheckRayCollision(ray, *m_collider, distance);
}

void ThrowableObject::Throw(const Vector3& position, const Vector3& velocity)
{
    m_isHeld = false;
    m_position = position;
    m_velocity = velocity;
    m_collider->SetPosition(m_position);
}

void ThrowableObject::SetHeldPosition(const Vector3& position)
{
    if (!m_isHeld)
        return;
    m_position = position;
    m_collider->SetPosition(m_position);
}
