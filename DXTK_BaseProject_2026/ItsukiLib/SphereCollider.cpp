#include"pch.h"
#include "SphereCollider.h"

Itsuki::SphereCollider::SphereCollider(Itsuki::CollisionParams param) 
    : Collider{param}
    ,m_radius{param.radius}
{
    //当たり判定の種類を球にする
    m_KindShape = SHAPE::SPHERE;
}

void Itsuki::SphereCollider::Render()
{
}
