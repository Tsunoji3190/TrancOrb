//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;

// 更新
void TitleScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	//キーボードの取得
	auto kb = Keyboard::Get().GetState();

	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({540, 300.0f}, L"Please Push to Space");

	if (kb.Space)
	{
        sceneController.RequestSwitch(SceneId::GamePlayScene);
	}

}

// 描画
void TitleScene::Render(GameContext& gameContext)
{
	gameContext;
}

// シーン切り替え時に呼び出される関数
void TitleScene::OnEnter(GameContext& gameContext)
{
	gameContext;
}
