#pragma once
#include"Collider.h"

namespace Itsuki {

	class BoxCollider :public Collider
	{
	private:

		//箱の大きさ
		DirectX::SimpleMath::Vector3 m_dimensions_min;
		DirectX::SimpleMath::Vector3 m_dimensions_max;
	public:

		//コンストラクタ
        BoxCollider(Itsuki::CollisionParams param
			);

        void Render() override;

		//位置と四角の中で一番小さい座標を足したものを返す
		DirectX::SimpleMath::Vector3 GetMin() const
		{
            return m_dimensions_min + m_position;
		}

        // 位置と四角の中で一番大きい座標を足したものを返す
        DirectX::SimpleMath::Vector3 GetMax() const
        {
            return m_dimensions_max + m_position;
        }

		void SetMin(DirectX::SimpleMath::Vector3 PosMin)
		{
            m_dimensions_min = PosMin;
		}

		void SetMax(DirectX::SimpleMath::Vector3 PosMax)
        {
            m_dimensions_max = PosMax;
        }

	};

}

