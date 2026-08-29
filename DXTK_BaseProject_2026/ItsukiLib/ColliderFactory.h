#pragma once
#include "Collider.h"
#include"BoxCollider.h"
#include"SphereCollider.h"
#include<memory>
#include <cassert>

using  Vec3=DirectX::SimpleMath::Vector3;

namespace Itsuki
{
    class  ColliderFactory
    {
    public:

        ColliderFactory()
        {
        }
        ~ColliderFactory() = default;

        // ColliderFactoryのインスタンスを取得する
        static ColliderFactory* GetInstance()
        {
            // インスタンスが生成されていない場合
            if (s_ColFactory == nullptr)
            {
                // ColliderFactoryのインスタンスを生成し、インスタンスをm_graphicsに格納する
                s_ColFactory = new ColliderFactory();
            }
            // ColliderFactoryのインスタンスを返す
            return s_ColFactory;
        }

        // ColliderFactoryの参照インスタンスを取得する
        static ColliderFactory& GetRefInstance()
        {
            // インスタンスが生成されていない場合
            if (s_ColFactory == nullptr)
            {
                // ColliderFactoryのインスタンスを生成し、インスタンスをm_graphicsに格納する
                s_ColFactory = new ColliderFactory();
            }
            // ColliderFactoryのインスタンスを返す
            return *s_ColFactory;
        }


        template <typename Col>
        //当たり判定の追加
        void AddCollider(SHAPE shape)
        {
            //すでに何か格納されていたら止める
            assert(m_colliders[shape] == nullptr && "既にあります！");

            m_colliders[shape] = [](Itsuki::CollisionParams param) { return std::make_unique<Col>(param); };
        }

        // 当たり判定の受け渡し
        std::unique_ptr<Itsuki::Collider> MakeCollider(SHAPE shape, Itsuki::CollisionParams param)
        {
            //shapeに格納されている当たり判定を渡す
            return m_colliders[shape](param);
        }

    private:

        // 当たり判定の種類を格納する
        std::unordered_map<SHAPE, std::function<std::unique_ptr<Itsuki::Collider>(Itsuki::CollisionParams)>> m_colliders;

        // インスタンスのポインタ
        static ColliderFactory* s_ColFactory;
    };

    // インスタンスのポインタの初期化
    inline ColliderFactory* ColliderFactory::s_ColFactory = nullptr;
}