//--------------------------------------------------------------------------------------
// File: EnemyGeneratorTask.cpp
//
// 敵生成タスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "EnemyGeneratorTask.h"
#include "EnemyTask.h"

using namespace DirectX;

// コンストラクタ
EnemyGeneratorTask::EnemyGeneratorTask(
    GameContext* pGameContext,
    DirectX::SpriteBatch* pSpriteBatch,
    ID3D11ShaderResourceView* pTexture
)
    : m_pGameContext(pGameContext)
    , m_pSpriteBatch(pSpriteBatch)
    , m_pTexture(pTexture)
{
    // タグを設定
    SetTag(L"EnemyGenerator");
}

// 更新
bool EnemyGeneratorTask::Update(float elapsedTime)
{
    // 敵の出現用のタイマーの加算
    m_spawnTimer += elapsedTime;
 
    // 一定の間隔で敵を出現させる
    if (m_spawnTimer >= SPAWN_INTERVAL)
    {
        // 敵の出現用のタイマーの初期化
        m_spawnTimer = 0.0f;

        // 画面サイズ取得
        RECT rect = m_pGameContext->deviceResources.GetOutputSize();

        // 敵タスクを生成（画面上から出現）
        AddChild<EnemyTask>(
            m_pGameContext,
            m_pSpriteBatch,
            m_pTexture,
            SimpleMath::Vector2(static_cast<float>(std::rand() % (rect.right - EnemyTask::SIZE)), -EnemyTask::SIZE)
        );
    }

    return true;
}
