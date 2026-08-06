//--------------------------------------------------------------------------------------
// File: ClearScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ClearScene.h"

using namespace DirectX;

// 更新
void ClearScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	//キーボードの取得
	auto kb = Keyboard::Get().GetState();

	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({540, 300.0f}, L"Game Clear!!!");
	debugRenderer.DrawText({540, 350.0f}, L"Please Push to Space");

	if (gameContext.keyboardTracker.pressed.Space)
	{
        sceneController.RequestSwitch(SceneId::TitleScene);
	}

}

// 描画
void ClearScene::Render(GameContext& gameContext)
{
	gameContext;
}

// シーン切り替え時に呼び出される関数
void ClearScene::OnEnter(GameContext& gameContext)
{
	gameContext;
}
