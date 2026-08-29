#include "pch.h"
#include "BoxCollider.h"

Itsuki::BoxCollider::BoxCollider(Itsuki::CollisionParams param) 
    : Collider{param},
    m_dimensions_max{param.max},
    m_dimensions_min{param.min}
{
    m_KindShape = SHAPE::BOX;

    SetPosition(param.pos);

}

void Itsuki::BoxCollider::Render()
{
}

