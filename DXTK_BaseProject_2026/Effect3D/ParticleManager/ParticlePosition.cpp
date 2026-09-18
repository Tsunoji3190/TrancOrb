//--------------------------------------------------------------------------------------
//	File: ParticlePosition.cpp
//
//	パーティクルの座標クラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ParticlePosition.h"
#include <random>


using namespace DirectX;


///	<summary>
///	コンストラクタ
///	</summary>
Effect3D::ParticlePosition::ParticlePosition()
	:m_position(DirectX::SimpleMath::Vector3::Zero)
	,m_velocity(DirectX::SimpleMath::Vector3::Zero)
	,m_cameraDistance(0.0f)
{
	////チェック用
	//m_velocity.x = 0.1f;
}

///	<summary>
///	デストラクタ
///	</summary>
Effect3D::ParticlePosition::~ParticlePosition()
{
}

void Effect3D::ParticlePosition::Update(float elapsedTime)
{
	//位置の移動
	m_position += m_velocity;

	//ベクトルの長さが指定した長さを超えたら
	if (m_position.Length() > 5.0f)
	{
		m_position = SimpleMath::Vector3::Zero;
	}
}
void Effect3D::ParticlePosition::SetPositionRandom(float min, float max)
{
	//
	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_real_distribution<> dist(min, max);

	//
	float x = static_cast<float>(dist(engine));
	float z = static_cast<float>(dist(engine));

	//
	m_position = SimpleMath::Vector3(x, 0.0f, z);
}

void Effect3D::ParticlePosition::SetPositionRawColumn(int index, int raw, float range)
{
	//
	float x = range * (index % raw);
	float z = range * (index / raw);

	//
	m_position = SimpleMath::Vector3(x, 0.0f, z);
}

void Effect3D::ParticlePosition::SetPositionCircle(int index, int max, float range)
{
	//
	float radian = XM_2PI / static_cast<float>(max);

	//
	float x = range * cosf(radian * index);
	float z = range * sinf(radian * index);

	//
	m_position = SimpleMath::Vector3(x, 0.0f, z);
}

void Effect3D::ParticlePosition::SetPositionCircleRandom(float range)
{
	//
	SetPositionRandom(1.0f, 1.0f);
	SimpleMath::Vector3 v = m_position;
	v.Normalize();
	v *= range;
	m_position = v;
}

void Effect3D::ParticlePosition::SetVelocityRandom(float min, float max)
{
	//
	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_real_distribution<> dist(min, max);

	//
	float x = static_cast<float>(dist(engine));
	float z = static_cast<float>(dist(engine));

	//
	m_velocity = SimpleMath::Vector3(x, 0.0f, z);
	m_velocity.Normalize();
	m_velocity *= 0.05f;
}

void Effect3D::ParticlePosition::SetVelocityCircle(int index, int max, float range)
{
	//
	float radian = XM_2PI / static_cast<float>(max);

	//
	float x = cosf(radian * index);
	float z = sinf(radian * index);

	//
	m_velocity = SimpleMath::Vector3(x, 0.5f, z);
	m_velocity.Normalize();
	m_velocity *= range;
}


