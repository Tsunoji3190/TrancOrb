#pragma once
#include "ItsukiLib/Collider.h"

namespace Itsuki
{
    struct Ray
    {
        DirectX::SimpleMath::Vector3 origin;
        DirectX::SimpleMath::Vector3 direction;
    };

    class CollisionChecker
    {
    public:
        // 他との当たり判定を確認するための関数
        bool CheckCollision(const Collider& collider1, const Collider& collider2) const;

         // カメラの視点との当たり判定
        bool CheckRayCollision(const Ray& ray, const Collider& collider, float& distance) const;

        // ステージ（ボックスコライダー）との押し出しを行う関数
        bool GetSeparation(const Collider& obj, const Collider& stage, DirectX::SimpleMath::Vector3& separation) const;

    private:
        bool SphireCheck(const Collider& collider1, const Collider& collider2) const;

        bool BoxCheck(const Collider& collider1, const Collider& collider2) const;

        bool BoxSphireCheck(const Collider& collider1, const Collider& collider2) const;
    };
}
