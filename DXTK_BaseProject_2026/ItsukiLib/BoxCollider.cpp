#include "pch.h"
#include "BoxCollider.h"



Itsuki::BoxCollider::BoxCollider(DirectX::SimpleMath::Vector3 min,
                                 DirectX::SimpleMath::Vector3 max,
								 DirectX::SimpleMath::Vector3 pos) 
	: m_dimensions_min{min}
	,m_dimensions_max{max}
{
	    // 当たり判定の種類を箱にする
    m_KindShape = SHAPE::BOX;

	SetPosition(pos);
}

void Itsuki::BoxCollider::Render()
{
}

