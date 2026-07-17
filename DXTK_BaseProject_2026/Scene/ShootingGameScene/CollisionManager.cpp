//--------------------------------------------------------------------------------------
// File: CollisionManager.cpp
//
// 当たり判定を行うクラス
//
// Date: 2026.4.11
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "CollisionManager.h"
#include "Task/PlayerTask.h"
#include "Task/EnemyTask.h"
#include "Task/BulletTask.h"
#include "Task/GameTypes.h"

// 矩形同士の衝突判定
bool CollisionManager::IsColliding(RECT a, RECT b)
{
    if ( (a.right >= b.left)
      && (a.left <= b.right)
      && (a.bottom >= b.top)
      && (a.top <= b.bottom)
       )
    {
        return true;
    }
    return false;
}

// プレイヤーの弾と敵の当たり判定
void CollisionManager::PlayerBulletVsEnemy(Imase::TaskSystem& taskSystem)
{
    // 弾のリスト
    std::vector<Imase::Task*> bullets = taskSystem.FindByTag(L"Bullet");

    // 敵のリスト
    std::vector<Imase::Task*> enemies = taskSystem.FindByTag(L"Enemy");

    for (auto* bullet : bullets)
    {
        BulletTask* bulletTask = dynamic_cast<BulletTask*>(bullet);
        for (auto* enemy : enemies)
        {
            EnemyTask* enemyTask = dynamic_cast<EnemyTask*>(enemy);
            // プレイヤーの発射した弾
            if (bulletTask->GetFaction() == Faction::Player)
            {
                // 矩形同士の衝突判定を行う
                if (IsColliding(bulletTask->GetBoundingRect(), enemyTask->GetBoundingRect()))
                {
                    // 敵が爆発する
                    enemyTask->Explotion();
                    // 弾と敵を消す
                    bulletTask->Kill();
                    enemyTask->Kill();
                }
            }
        }
    }
}

// 敵の弾とプレイヤーの当たり判定
void CollisionManager::EnemyBulletVsPlayer(Imase::TaskSystem& taskSystem)
{
    // 弾のリスト
    std::vector<Imase::Task*> bullets = taskSystem.FindByTag(L"Bullet");

    // プレイヤーのリスト
    std::vector<Imase::Task*> players = taskSystem.FindByTag(L"Player");

    // プレイヤーがいない
    if (players.size() == 0) return;

    // プレイヤータスクを取得
    PlayerTask* playerTask = dynamic_cast<PlayerTask*>(players[0]);

    for (auto* bullet : bullets)
    {
        BulletTask* bulletTask = dynamic_cast<BulletTask*>(bullet);
        // 敵の発射した弾
        if (bulletTask->GetFaction() == Faction::Enemy)
        {
            // 矩形同士の衝突判定を行う
            if (IsColliding(bulletTask->GetBoundingRect(), playerTask->GetBoundingRect()))
            {
                // プレイヤーが爆発する
                playerTask->Explotion();
                // プレイヤーと弾を消す
                bulletTask->Kill();
                playerTask->Kill();
                return;
            }
        }
    }
}

// 当たり判定
void CollisionManager::Check(Imase::TaskSystem& taskSystem)
{
    // プレイヤーの弾と敵の当たり判定
    PlayerBulletVsEnemy(taskSystem);

    // 敵の弾とプレイヤーの当たり判定
    EnemyBulletVsPlayer(taskSystem);
}
