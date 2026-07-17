#pragma once
#include "ItsukiLib/Collider.h"

namespace Itsuki
{
    class CollisionChecker
    {
    public:
        // 他との当たり判定を確認するための関数
        bool CheckCollision(const Collider& collider1, const Collider& collider2) const;

    private:
        bool SphireCheck(const Collider& collider1, const Collider& collider2) const;

        bool BoxCheck(const Collider& collider1, const Collider& collider2) const;

        bool BoxSphireCheck(const Collider& collider1, const Collider& collider2) const;
    };
}
