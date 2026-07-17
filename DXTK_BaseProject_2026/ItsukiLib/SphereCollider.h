#pragma once
#include"Collider.h"

using namespace DirectX;

namespace Itsuki {

	class SphereCollider :public Collider
	{
    public:
        // コンストラクタ
        SphereCollider(float radius = RADIUS);

        void Render() override;

        float GetRadius() const
        {
            return m_radius;
        }

        void SetRadius(float radius)
        {
            m_radius = radius;
        }


    private:

        static constexpr float RADIUS = 0.5f;

	private:

		//半径
		float m_radius;


	};

}