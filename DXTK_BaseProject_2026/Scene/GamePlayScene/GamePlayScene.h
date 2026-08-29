//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
// モデルテスト用シーン
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include "ImaseLib/DebugCamera.h"

#include "Player.h"
#include "ItsukiLib/ColliderRenderer.h"
#include"ItsukiLib/CollisionChecker.h"
#include"ItsukiLib/ColliderFactory.h"

#include"Manager/StageManager.h"
#include"Manager/OrbManager.h"
#include"Status.h"

#include"ItsukiLib/Camera/NormalCamera.h"

class GamePlayScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// コンストラクタ
    GamePlayScene();

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;


private:

	static constexpr int FIRST_TIMER = 10;

	static constexpr int FIRST_ORB = 5;

	
	
private:

	// ビュー行列
    DirectX::SimpleMath::Matrix m_view;

	// プロジェクション行列
    DirectX::SimpleMath::Matrix m_projection;

	// スプライトバッチ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_CprimitiveBatch;

	// ベーシックエフェクトへのポインタ
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

    // 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    // シェーダーリソースビュー
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_orbTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	// デバッグカメラ
    std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// プロジェクション行列を作成する関数
    DirectX::SimpleMath::Matrix CreateProjectionMatrix(GameContext& gameContext);

	// ウインドウサイズ変更時に呼び出される関数
    void OnWindowSizeChanged(GameContext& gameContext) override;

	//当った際に当る前の状態に戻す関数（現在は球と立方体の形のみに適応）
    void ResolveCollision(Itsuki::Collider* col1, Itsuki::Collider* col2);

	// モデルハンドル
    std::unique_ptr<DirectX::Model> m_model;


	enum class CameraMode
	{
		Title,
		GamePlay,
	};

	// カメラモード
    CameraMode m_cameraMode = CameraMode::Title;

	// 一人称カメラ
    void GamePlayCamera(float elapsedTime); 
	
	// 三人称カメラ
    void ThirdCamera(float elapsedTime);

	// カメラのY軸に対する回転角度（タイトル用）
    float m_titleAngleRad = 0.0f;

	// タイトル用カメラの回転角度（１秒間あたりの回転角度）
    static constexpr float TITLE_CAMERA_MOVE_ANGLE_DEG = 10.0f;

	// モデルハンドル（矢印）
    std::unique_ptr<DirectX::Model> m_arrowModel;

	// ---- オイラー角テスト用 ----- //
    float m_angleRad_X = 0.0f;
    float m_angleRad_Y = 0.0f;
    float m_angleRad_Z = 0.0f;

	// １秒間あたりの回転角度
    static constexpr float ROTATE_ANGLE_DEG = 90.0f;

	// クォータニオン
    DirectX::SimpleMath::Quaternion m_quaternion;

	// モデルハンドル（ターゲット）
    std::unique_ptr<DirectX::Model> m_targetModel;

	// ターゲットの位置
    DirectX::SimpleMath::Vector3 m_targetPosition = { 0.0f, 0.0f, -2.0f };

	//プレイヤーの作成
	std::unique_ptr<Player> m_player;


		enum class ControlCamera
    {
        P1,
        P2
    };


	ControlCamera m_controlCamera = ControlCamera::P1;

	//ステージを格納
    std::unique_ptr<StageManager> m_stageManager;

	//オーブを格納
    std::unique_ptr<OrbManager> m_orbManager;

	//
    std::unique_ptr<Status> m_status;

	//----------やむを得ず----------

    // 時間制限
    float m_timer = 10;

	//----------やむを得ず----------

	//当たり判定を描画するもの
	std::unique_ptr<Itsuki::ColliderRenderer> m_renderer;

	//当たり判定同氏がぶつかったか判定するもの
	std::unique_ptr<Itsuki::CollisionChecker> m_collisionChecker;

	//当たり判定Factory

	//一人称のカメラ
    Itsuki::NormalCamera m_camera;

	//BGM
	SuzukiLib::Audio::AudioHandle m_bgmHandle;

};

