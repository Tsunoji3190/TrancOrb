//--------------------------------------------------------------------------------------
// File: NodeTestScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "NodeTestScene.h"

using namespace DirectX;

// 更新
void NodeTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
    // 経過時間を取得する
    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

	//キーボードの取得
	auto kb = Keyboard::Get().GetState();

	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({540, 300.0f}, L"Please Push to Space");

	m_node.Update(elapsedTime,gameContext);
}

// 描画
void NodeTestScene::Render(GameContext& gameContext)
{
	gameContext;
    m_node.Render();
}

// シーン切り替え時に呼び出される関数
void NodeTestScene::OnEnter(GameContext& gameContext)
{
	gameContext;

	m_node.SetNode(gameContext, nullptr, {100, 100}, [this] { void(); }, 1, L"Resources/Textures/テストオーブ.png");
}
