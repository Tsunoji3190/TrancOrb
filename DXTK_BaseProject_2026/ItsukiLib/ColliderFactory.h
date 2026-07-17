#pragma once
#include "Collider.h"
#include<memory>

namespace Itsuki
{
    class  ColliderFactory
    {
    public:

        ColliderFactory()
        {

        }

        ~ColliderFactory() = default;

        // 当たり判定の受け渡し
        Collider* MakeCollider(SHAPE shape)
        {
            return m_colliders->find(shape)->second;
        }

        //当たり判定の追加
        void AddCollider(SHAPE shape,Collider* collider)
        {
            m_colliders->emplace(shape, collider);
        }




    private:

        //当たり判定の種類
        std::unique_ptr<std::unordered_map<SHAPE, Collider*>> m_colliders;

    };

}