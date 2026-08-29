#pragma once
#include<SimpleMath.h>

namespace Itsuki
{

    // 当たり判定の種類
    enum SHAPE
    {
        NONE = -1,
        BOX,
        SPHERE
    };

    //初期化の際に使われる情報
    struct CollisionParams
    {
        //現在の位置
        DirectX::SimpleMath::Vector3 pos;
        //現在の位置からの最小のXYZ座標
        DirectX::SimpleMath::Vector3 min;
        //現在の位置からの最大のXYZ座標
        DirectX::SimpleMath::Vector3 max;
        //半径
        float radius;
    };


	class Collider
    {
    public:

        //コンストラクタ
        Collider(Itsuki::CollisionParams param) 
            : m_param{param}
        {

        }

        // デストラクタ
        virtual ~Collider() = default;

        virtual void Render() = 0;

        // 当たり判定の位置を取得
        DirectX::SimpleMath::Vector3 GetPosition() const
        {
            return m_position;
        }

        // 当たり判定の位置の設定
        void SetPosition(DirectX::SimpleMath::Vector3 position)
        {
            m_position = position;
        }

        // 当たり判定の形を返す
        int GetShape() const
        {
            return m_KindShape;
        }

        //当たり判定の情報を返す
        CollisionParams GetParam()
        {
            return m_param;
    }

    public:
        // 当たり判定の位置
        DirectX::SimpleMath::Vector3 m_position;

        // 当たり判定の種類（最初はなにも入れない）
        int m_KindShape = NONE;

        //パラメータ
        Itsuki::CollisionParams m_param;
    };

}
