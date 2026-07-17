#pragma once
#include<SimpleMath.h>

// 当たり判定の種類
enum SHAPE
{
    NONE = -1,
    BOX,
    SPHERE
};

class Collider
{
public:


	//当たり判定の位置
	DirectX::SimpleMath::Vector3 m_position;

	//当たり判定の種類（最初はなにも入れない）
	int m_KindShape = NONE;

public:

	//デストラクタ
	virtual ~Collider() = default;

	virtual void Render() = 0;

	//当たり判定の位置を取得
	DirectX::SimpleMath::Vector3 GetPosition() const
    {
        return m_position;
    }

	// 当たり判定の位置の設定
    void SetPosition(DirectX::SimpleMath::Vector3 position)
    {
        m_position = position;
    }

	//当たり判定の場所を設定する
	void SetCollider(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	//当たり判定の形を返す
    int GetShape() const
    {
        return m_KindShape;
    }
};

