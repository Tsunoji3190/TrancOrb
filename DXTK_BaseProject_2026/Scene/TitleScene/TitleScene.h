//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include "Effect3D/Particle.h"

#include "Effect3D/ParticleManager/ParticleManager.h"
#include "Effect3D/UI/Menu.h"

#include "Effect3D/UI/Title.h"
#include "Effect3D/UI/BackGround.h"

class TitleScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:

	   // // パーティクル
    //std::unique_ptr<Effect3D::Particle> m_particle;

    //// パーティクルマネージャー
    //std::unique_ptr<Effect3D::ParticleManager> m_particleManager;

    //// タイトル
    //std::unique_ptr<Effect3D::Title> m_title;

    //// メニュー
    //std::unique_ptr<Effect3D::Menu> m_menu;

    //// 背景
    //std::unique_ptr<Effect3D::BackGround> m_background;

    //int m_isMenu;
};

