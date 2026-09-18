//--------------------------------------------------------------------------------------
//	File: ParticlePosition.h
//
//	パーティクルの座標クラス
//	未拡張の場合はPosition情報のみ
//
//-------------------------------------------------------------------------------------

#pragma once

#include "Common/StepTimer.h"
#include "Common/DeviceResources.h"
#include "pch.h"
#include <vector>


namespace Effect3D
{
	class ParticlePosition
	{
	public:

	private:
		DirectX::SimpleMath::Vector3 m_position;	//パーティクル座標

		DirectX::SimpleMath::Vector3 m_velocity;	//パーティクルの速度

		float m_cameraDistance;

	public:

		ParticlePosition();
		~ParticlePosition();

		void Update(float elapsedTime);
	
		void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
		DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

		void SetDistance(DirectX::SimpleMath::Vector3& eyePosition) { m_cameraDistance = (m_position - eyePosition).Length(); }

		float GetDistance() { return m_cameraDistance; }

		//
		void SetPositionRandom(float min, float max);
		void SetPositionRawColumn(int index, int raw, float range);

		//
		void SetPositionCircle(int index, int max, float range);
		void SetPositionCircleRandom(float range);

		//
		void SetVelocityRandom(float min, float max);
		void SetVelocityCircle(int index, int max, float range);



	};
}