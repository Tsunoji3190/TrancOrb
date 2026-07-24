#include "pch.h"
#include "CollisionChecker.h"

#include "ItsukiLib/SphereCollider.h"
#include "ItsukiLib/BoxCollider.h"
#include <SimpleMath.h>

using namespace Itsuki;

bool CollisionChecker::CheckCollision(const Collider& collider1, const Collider& collider2) const
{

    switch (collider1.GetShape())
    {
        case SHAPE::BOX:

            if (collider2.GetShape() == SHAPE::BOX)
            {
                return BoxCheck(collider1,collider2);
            }
            else if (collider2.GetShape() == SHAPE::SPHERE)
            {
                return BoxSphireCheck(collider1, collider2);
            }

            break;

        case SHAPE::SPHERE:

            if (collider2.GetShape() == SHAPE::BOX)
            {
                return BoxSphireCheck(collider2, collider1);

            }
            else if (collider2.GetShape() == SHAPE::SPHERE)
            {
                return SphireCheck(collider1, collider2);
            }
            break;

        case SHAPE::NONE:

        
            return false;
    }
    
}

bool CollisionChecker::SphireCheck(const Collider& collider1, const Collider& collider2) const
{
    // Sphere型のColliderを作成
    const SphereCollider* sphere1 = dynamic_cast<const SphereCollider*>(&collider1);
    const SphereCollider* sphere2 = dynamic_cast<const SphereCollider*>(&collider2);

    // あったら判定をする
    if (sphere1 && sphere2)
    {

        // 絶対値を入れる
        float dist = (sphere1->GetPosition() - sphere2->GetPosition()).Length();

        // 球と球の距離が半径より大きいならfalse、小さいならtrue
        return dist <= (sphere1->GetRadius() + sphere2->GetRadius());
    }

    return false;
}

bool CollisionChecker::BoxCheck(const Collider& collider1, const Collider& collider2) const
{
    const BoxCollider* aabb1 = dynamic_cast<const BoxCollider*>(&collider1);
    const BoxCollider* aabb2 = dynamic_cast<const BoxCollider*>(&collider2);

    // あったら判定をする
    if (aabb1 && aabb2)
    {
        return (aabb1->GetMax().x >= aabb2->GetMin().x && aabb1->GetMin().x <= aabb2->GetMax().x) &&
               (aabb1->GetMax().y >= aabb2->GetMin().y && aabb1->GetMin().y <= aabb2->GetMax().y) &&
               (aabb1->GetMax().z >= aabb2->GetMin().z && aabb1->GetMin().z <= aabb2->GetMax().z);
    }

    return false;
}

bool CollisionChecker::BoxSphireCheck(const Collider& collider1, const Collider& collider2) const
{
    using namespace DirectX;

    const BoxCollider* aabb = dynamic_cast<const BoxCollider*>(&collider1);
    const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(&collider2);


    SimpleMath::Vector3 closestPoint = {0, 0, 0};


    //どちらかがない場合は判定しない
    if (!aabb || !sphere)
    {
        return false;
    }

    //それぞれの座標で球に最も近い点を取る
    closestPoint.x = std::clamp(sphere->GetPosition().x,aabb->GetMin().x,aabb->GetMax().x);
    closestPoint.y = std::clamp(sphere->GetPosition().y,aabb->GetMin().y,aabb->GetMax().y);
    closestPoint.z = std::clamp(sphere->GetPosition().z,aabb->GetMin().z,aabb->GetMax().z);

    //ベクトルの二乗を取る（ベクトルはsinとcosの平方根を足して求められる。が、平方根の計算は重いから）
    float distSq = (closestPoint - sphere->GetPosition()).LengthSquared();

    //ベクトルの二乗が半径の二乗より小さかったら当ってることを返す
    return distSq <= (sphere->GetRadius() * sphere->GetRadius());
}
