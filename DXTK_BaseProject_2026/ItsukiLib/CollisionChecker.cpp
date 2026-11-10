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
        case Itsuki::SHAPE::BOX:

            if (collider2.GetShape() == Itsuki::SHAPE::BOX)
            {
                return BoxCheck(collider1,collider2);
            }
            else if (collider2.GetShape() == Itsuki::SHAPE::SPHERE)
            {
                return BoxSphireCheck(collider1, collider2);
            }

            break;

        case Itsuki::SHAPE::SPHERE:

            if (collider2.GetShape() == Itsuki::SHAPE::BOX)
            {
                return BoxSphireCheck(collider2, collider1);

            }
            else if (collider2.GetShape() == Itsuki::SHAPE::SPHERE)
            {
                return SphireCheck(collider1, collider2);
            }
            break;

        case Itsuki::SHAPE::NONE:

            return false;
            break;

        default:

            assert("no kind!");
            return false;
    }
    
}

bool CollisionChecker::CheckRayCollision(const Ray& ray, const Collider& collider, float& distance) const
{
    distance = 0.0f;
    auto direction = ray.direction;

    if (direction.LengthSquared() <= 0.000001f)
        return false;

    direction.Normalize();

    if (collider.GetShape() == SHAPE::SPHERE)
    {
        const auto* sphere = dynamic_cast<const SphereCollider*>(&collider);

        if (!sphere)
            return false;

        const auto toCenter = sphere->GetPosition() - ray.origin;
        const float projected = toCenter.Dot(direction);
        const float discriminant =
            projected * projected - (toCenter.LengthSquared() - sphere->GetRadius() * sphere->GetRadius());

        if (discriminant < 0.0f)
            return false;

        const float nearDistance = projected - std::sqrt(discriminant);

        const float farDistance = projected + std::sqrt(discriminant);

        distance = nearDistance >= 0.0f ? nearDistance : farDistance;

        return distance >= 0.0f;
    }

    if (collider.GetShape() == SHAPE::BOX)
    {
        const auto* box = dynamic_cast<const BoxCollider*>(&collider);

        if (!box)
            return false;

        float nearDistance = 0.0f;
        float farDistance = (std::numeric_limits<float>::max)();
        const auto min = box->GetMin();
        const auto max = box->GetMax();
        const auto component = [](const SimpleMath::Vector3& value, int axis)
        {
            switch (axis)
            {
                case 0:
                    return value.x;
                case 1:
                    return value.y;
                default:
                    return value.z;
            }
        };
        for (int axis = 0; axis < 3; ++axis)
        {
            const float origin = component(ray.origin, axis);
            const float dir = component(direction, axis);
            const float minimum = component(min, axis);
            const float maximum = component(max, axis);
            if (std::abs(dir) <= 0.000001f)
            {
                if (origin < minimum || origin > maximum)
                    return false;
                continue;
            }
            float a = (minimum - origin) / dir;
            float b = (maximum - origin) / dir;
            if (a > b)
                std::swap(a, b);
            nearDistance = (std::max)(nearDistance, a);
            farDistance = (std::min)(farDistance, b);
            if (nearDistance > farDistance)
                return false;
        }
        distance = nearDistance;
        return farDistance >= 0.0f;
    }
    return false;
}


bool Itsuki::CollisionChecker::GetSeparation(const Collider& obj, const Collider& stage,
                                             DirectX::SimpleMath::Vector3& separation) const
{
    separation = SimpleMath::Vector3::Zero;

    // ステージがボックスコライダーじゃないのなら判定しない
    const auto* stageBox = dynamic_cast<const BoxCollider*>(&stage);
    if (!stageBox)
        return false;

    // ステージの大きさの取得
    const SimpleMath::Vector3 stageMin = stageBox->GetMin();
    const SimpleMath::Vector3 stageMax = stageBox->GetMax();

    // 球コライダー用の処理
    if (const auto* sphere = dynamic_cast<const SphereCollider*>(&obj))
    {
        // 位置の取得
        const SimpleMath::Vector3 center = sphere->GetPosition();

        // 球からもっとも近い座標を作成する
        SimpleMath::Vector3 closest;
        closest.x = std::clamp(center.x, stageMin.x, stageMax.x);
        closest.y = std::clamp(center.y, stageMin.y, stageMax.y);
        closest.z = std::clamp(center.z, stageMin.z, stageMax.z);

        // 球から近い点のベクトル
        const SimpleMath::Vector3 diff = center - closest;

        // ベクトルの二乗を取る（平方根を取るより早い）
        const float distanceSq = diff.LengthSquared();

        // 半径を取得
        const float radius = sphere->GetRadius();

        // ベクトルが半径より大きい（地形にめり込んでいない）なら
        if (distanceSq > radius * radius)
            return false;

        // 球の中心が箱の外側にある場合
        if (distanceSq > 0.000001f)
        {
            // 平方根をとる（やむなし）
            const float distance = std::sqrt(distanceSq);

            // 押し戻し量の計算
            separation = diff / distance * (radius - distance);

            // 押し戻しを伝える
            return true;
        }

        // 球の中心が AABB の内部にある場合（dist = 0）
        if (distanceSq == 0.0f)
        {
            // とりあえず 上方向に押し出す（簡易処理）
            separation = SimpleMath::Vector3(0, 1, 0);
        }

        return true;
    }

    // ボックスコライダー用の処理
    if (const auto* objBox = dynamic_cast<const BoxCollider*>(&obj))
    {
        // オブジェクトの大きさを取得
        const SimpleMath::Vector3 objMin = objBox->GetMin();
        const SimpleMath::Vector3 objMax = objBox->GetMax();

        // それぞれの座標の重なりを取得
        const float overlapX = std::min(objMax.x, stageMax.x) - std::max(objMin.x, stageMin.x);
        const float overlapY = std::min(objMax.y, stageMax.y) - std::max(objMin.y, stageMin.y);
        const float overlapZ = std::min(objMax.z, stageMax.z) - std::max(objMin.z, stageMin.z);

        // 3点すべてが重なっていないなら何もしない
        if (overlapX <= 0.0f || overlapY <= 0.0f || overlapZ <= 0.0f)
            return false;

        // それぞれの中心を取る
        const SimpleMath::Vector3 movingCenter = (objMin + objMax) * 0.5f;
        const SimpleMath::Vector3 stageCenter = (stageMin + stageMax) * 0.5f;

        // 重なりが一番小さい軸に沿って押し出す
        if (overlapX <= overlapY && overlapX <= overlapZ)
        {
            separation.x = movingCenter.x < stageCenter.x ? -overlapX : overlapX;
        }
        else if (overlapY <= overlapZ)
        {
            separation.y = movingCenter.y < stageCenter.y ? -overlapY : overlapY;
        }
        else
        {
            separation.z = movingCenter.z < stageCenter.z ? -overlapZ : overlapZ;
        }

        return true;
    }

    // 何らかの形で抜けたらfalseを返す
    return false;
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
