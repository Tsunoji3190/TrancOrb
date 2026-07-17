//--------------------------------------------------------------------------------------
// File: ShootingGameScene.cpp
//
// シューティングゲームシーン
//
// Date: 2026.2.24
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ShootingGameScene.h"
#include "Task/PlayerTask.h"
#include "Task/EnemyGeneratorTask.h"
#include "../BaseScene/BaseScene.h"

using namespace DirectX;

// 更新
void ShootingGameScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	auto& debugRenderer = gameContext.debugRenderer;

	// 経過時間
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

	Keyboard::KeyboardStateTracker& tracker = gameContext.keyboardTracker;

	// スペースキーが押された
	if (tracker.pressed.Space)
	{
		// 次のシーンへ
		sceneController.RequestSwitch(SceneId::BaseScene);
	}

	// タスクの更新
	m_taskSystem.Update(elapsedTime);

	// 当たり判定
	m_collisionManager.Check(m_taskSystem);

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ShootingGameScene");
}

// 描画
void ShootingGameScene::Render(GameContext& gameContext)
{
	auto& states = gameContext.commonStates;

	// スプライトバッチの設定
	m_spriteBatch->Begin(
		SpriteSortMode_Deferred,	// Endでまとめて描画
		states.NonPremultiplied(),	// 半透明
		states.PointWrap()			// サンプラー
	);

	// タスクの描画
	m_taskSystem.Render();

	// 登録されたスプライトの描画
	m_spriteBatch->End();
}

// シーン切り替え時に呼び出される関数
void ShootingGameScene::OnEnter(GameContext& gameContext)
{
	auto device = gameContext.deviceResources.GetD3DDevice();
	auto context = gameContext.deviceResources.GetD3DDeviceContext();

	// スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// テクスチャのロード
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ShootingGame.dds", nullptr, m_texture.ReleaseAndGetAddressOf())
	);

	// プレイヤータスクを生成
	RECT rect = gameContext.deviceResources.GetOutputSize();
	m_taskSystem.GetRoot()->AddChild<PlayerTask>(
		&gameContext,
		m_spriteBatch.get(),
		m_texture.Get(),
		SimpleMath::Vector2((rect.right - PlayerTask::SIZE) / 2.0f, 600.0f)	// 初期位置（画面中央）
	);

	// 敵ジェネレータタスクを生成
	m_taskSystem.GetRoot()->AddChild<EnemyGeneratorTask>(&gameContext, m_spriteBatch.get(), m_texture.Get());
}
