#pragma once
#include"pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

using namespace DirectX;

namespace Itsuki
{

    namespace Physics
    {
        static constexpr float GRAVITY = 9.8f;
    }

    namespace Easing
    {
        //徐々に遅くなるイージング
        const float easeOutQuint(float x) 
        {
            return 1 - std::pow(1 - x, 5);
        }

        //バウンドするようなイージング
        const float easeOutBounce(float x)
        {
            const auto n1 = 7.5625;
            const auto d1 = 2.75;

            if (x < 1 / d1)
            {
                return n1 * x * x;
            }
            else if (x < 2 / d1)
            {
                return n1 * (x -= 1.5 / d1) * x + 0.75;
            }
            else if (x < 2.5 / d1)
            {
                return n1 * (x -= 2.25 / d1) * x + 0.9375;
            }
            else
            {
                return n1 * (x -= 2.625 / d1) * x + 0.984375;
            }
        }
    }


    class ItsukiMath
    {
    public:

        //もっとも近い座標を取得
        SimpleMath::Vector3 ReturnClosestPoint(Itsuki::SphereCollider sphere,Itsuki::BoxCollider aabb)
        {
            // もっとも近い座標の作成
            SimpleMath::Vector3 closestPoint = {0, 0, 0};

            // それぞれの座標で球に最も近い点を取る
            closestPoint.x = std::clamp(sphere.GetPosition().x, aabb.GetMin().x, aabb.GetMax().x);
            closestPoint.y = std::clamp(sphere.GetPosition().y, aabb.GetMin().y, aabb.GetMax().y);
            closestPoint.z = std::clamp(sphere.GetPosition().z, aabb.GetMin().z, aabb.GetMax().z);


            return closestPoint;

        }



    };

}