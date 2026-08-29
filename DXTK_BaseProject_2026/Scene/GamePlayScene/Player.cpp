#include "pch.h"
#include "Player.h"

using namespace DirectX;

Player::Player(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view,
               const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel,
               std::unique_ptr<Itsuki::Collider> collider)
    : m_gameContext(gameContext)
    , m_view(view)
    , m_projection(projection)
    , m_pModel(pModel)
    , m_position(0,1.0f,2.0f) 
    , m_velocity(.0f,.0f,.0f)
    , m_speed(MOVE_SPEED)
    , m_jump(JUMP_SPEED)
    , m_facingAngleRad(.0f,.0f,.0f)
    , m_isGround{true}
{
    //当たり判定を設定する
    SetCollider(std::move(collider));

    m_collider->SetPosition(m_position);

    Mouse::Get().SetMode(Mouse::MODE_RELATIVE);
}

Player::~Player()
{
}

void Player::Initialze()
{

}

void Player::Update(float elapsedTime)
{


    auto kb = Keyboard::Get().GetState();
    
    // マウスの取得
    auto state = Mouse::Get().GetState();
    

// マウスの相対移動量（Relative モード時）
    float dx = state.x;
    float dy = state.y;

    const float mouseSensitivity = 0.005f; // 調整用

    // Yaw（左右回転）
    m_facingAngleRad.y -= dx * mouseSensitivity;

    // Pitch（上下回転）
    m_facingAngleRad.x -= dy * mouseSensitivity;
    // ピッチの制限（上下向きすぎ防止）
    m_facingAngleRad.x = std::clamp(m_facingAngleRad.x, XMConvertToRadians(-80.0f), XMConvertToRadians(80.0f));

    // 回転行列を作成する
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_facingAngleRad.y);


    if (kb.W)
        m_direction += SimpleMath::Vector3::Forward;
    if (kb.S)
        m_direction += SimpleMath::Vector3::Backward;
    if (kb.A)
        m_direction += SimpleMath::Vector3::Left;
    if (kb.D)
        m_direction += SimpleMath::Vector3::Right;

    if (m_direction.LengthSquared() > 0)
    {
        m_direction.Normalize();
        SimpleMath::Vector3 worldDir = SimpleMath::Vector3::Transform(m_direction, rotY);

        m_velocity.x = worldDir.x * m_speed * elapsedTime;
        m_velocity.z = worldDir.z * m_speed * elapsedTime;
    }

    //動かないなら
    if (!(kb.W || kb.S || kb.A || kb.D))
    {
        m_velocity = {0, m_velocity.y, 0};
    }

    //スペースキーでジャンプ
    if (m_gameContext.keyboardTracker.pressed.Space && m_isGround)
    {
        m_gameContext.audio.PlayOneShot("Jump");
        m_velocity.y = m_jump;
        m_isGround = false;

    }
    else if (m_velocity.y>=0 && !m_isGround)
    {
        m_velocity.y -= 0.05 * elapsedTime;
    }


    m_position += m_velocity - (SimpleMath::Vector3(0, Itsuki::Physics::GRAVITY, 0) * elapsedTime);    

    if (m_position.y <= -10)
    {
        m_position = {0, 1, 0};
    }

    // 当たり判定も移動させる
    m_collider->SetPosition(m_position);
}

void Player::Render()
{
    SimpleMath::Matrix world;

    // ワールド行列を作成する
    world = SimpleMath::Matrix::CreateRotationY(m_facingAngleRad.y) * 
            SimpleMath::Matrix::CreateTranslation(m_position);

    // モデルの描画
    m_pModel->Draw(m_gameContext.deviceResources.GetD3DDeviceContext(), m_gameContext.commonStates, world, m_view,
                   m_projection);

       Imase::DebugRenderer& debugRenderer = m_gameContext.debugRenderer;

   //向きを知る
   //std::wstring textX = L"Xradius: " + std::to_wstring(m_facingAngleRad.x);

   //debugRenderer.DrawText({0, 500.0f},textX);

}
