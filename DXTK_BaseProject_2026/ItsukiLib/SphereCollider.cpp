#include"pch.h"
#include "SphereCollider.h"

Itsuki::SphereCollider::SphereCollider(float radius)
    :m_radius{radius}
{
    //当たり判定の種類を球にする
    m_KindShape = SHAPE::SPHERE;
}

void Itsuki::SphereCollider::Render()
{
}
