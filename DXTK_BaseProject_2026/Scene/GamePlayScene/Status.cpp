#include "pch.h"
#include "Status.h"

Status::Status(GameContext& gamecontext, Player* player, OrbManager* manager) 
	: m_gameContext{gamecontext}
	, m_player{player}
    , m_orbManager{manager}
    , m_OrbCount{0}
    ,m_isClear{false}
{
}

void Status::Initialize()
{
}

Status::~Status()
{
}

void Status::Update(float& timer)
{

    if (m_gameContext.keyboardTracker.pressed.Z)
    {
        //オーブの数がSteakを超えていないなら音を出す
        if (!IsAboveSteak(m_JumpSteak))
        {
            m_gameContext.audio.PlayOneShot("Stop");

            return;
        }

        m_gameContext.audio.PlayOneShot("Buy");


        m_OrbCount -= m_JumpSteak;

        m_player->SetJump(m_JumpAdd);

        //レベルを上げる
        m_JumpLevel++;

        m_player->SetPosition({.0f, 1.0f, 2.0f});
        m_player->SetVelocityY(0);
        m_player->ResetAngle();

    }
    else if (m_gameContext.keyboardTracker.pressed.X)
    {
        // オーブの数がSteakを超えていないなら音を出す
        if (!IsAboveSteak(m_OrbNumberSteak))
        {
            m_gameContext.audio.PlayOneShot("Stop");

            return;

        }

        m_gameContext.audio.PlayOneShot("Buy");

        m_OrbCount -= m_OrbNumberSteak;

        m_orbManager->AddOrb();

         // レベルを上げる
        m_OrbNumberLevel++;

        m_player->SetPosition({.0f, 1.0f, 2.0f});
        m_player->SetVelocityY(0);
        m_player->ResetAngle();

    }
    else if (m_gameContext.keyboardTracker.pressed.C)
    {
        // オーブの数がSteakを超えていないなら音を出す
        if (!IsAboveSteak(m_OrbValueSteak))
        {
            m_gameContext.audio.PlayOneShot("Stop");

            return;

        }

        m_gameContext.audio.PlayOneShot("Buy");

        m_OrbCount -= m_OrbValueSteak;

        for (size_t i = 0; i < m_orbManager->GetNumOrbs(); i++)
        {
            auto pOrb = m_orbManager->GetOrb(i);

            pOrb->AddOrbValue(m_OrbValueAdd);

        }

        //レベルを上げる
        m_OrbValueLevel++;

        m_player->SetPosition({.0f, 1.0f, 2.0f});
        m_player->SetVelocityY(0);
        m_player->ResetAngle();

    }
    else if (m_gameContext.keyboardTracker.pressed.V)
    {
        // オーブの数がSteakを超えていないなら音を出す
        if (!IsAboveSteak(m_timerSteak))
        {
            m_gameContext.audio.PlayOneShot("Stop");

            return;

        }

        m_gameContext.audio.PlayOneShot("Buy");

        m_OrbCount -= m_timerSteak;

        FIRST_TIMER += m_timerAdd;

        // レベルを上げる
        m_timerLevel++;

        m_player->SetPosition({.0f, 1.0f, 2.0f});
        m_player->SetVelocityY(0);
        m_player->ResetAngle();
    }
    else if (m_gameContext.keyboardTracker.pressed.O)
    {
        timer = FIRST_TIMER;

        for (size_t i = 0; i < m_orbManager->GetNumOrbs(); i++)
        {
            auto pOrb = m_orbManager->GetOrb(i);

            pOrb->SetRandom();
        }


        m_player->SetPosition({.0f, 1.0f, 2.0f});
        m_player->SetVelocityY(0);
        m_player->ResetAngle();
    }
    else if (m_gameContext.keyboardTracker.pressed.Q)
    {


        // オーブの数がSteakを超えていないなら音を出す
        if (!IsAboveSteak(200))
        {
            m_gameContext.audio.PlayOneShot("Stop");

            return;
        }

        m_isClear = true;

    }
    else if (m_gameContext.keyboardTracker.pressed.Enter)
    {
        PostQuitMessage(0);
    }
}

void Status::Render()
{
    Imase::DebugRenderer& debugRenderer = m_gameContext.debugRenderer;

    std::wstring Status = L"Orb: " + std::to_wstring(m_OrbCount);

    std::wstring textS = L"Level:" + std::to_wstring(m_JumpLevel) + L"  PlayerJumpUp Cost:" +
                         std::to_wstring(m_JumpSteak) + L"  Press:Z";

    std::wstring textO = L"Level:" + std::to_wstring(m_OrbNumberLevel) + L"  Orb Plus Cost:" +
                         std::to_wstring(m_OrbNumberSteak) + L"  Press:X";

    std::wstring textOV = L"Level:" + std::to_wstring(m_OrbValueLevel) + L"  Orb Value Up Cost:" +
                          std::to_wstring(m_OrbValueSteak) + L"  Press:C";

    std::wstring textT = L"Level:" + std::to_wstring(m_timerLevel) + L"  Time Extension Cost:" +
                         std::to_wstring(m_timerSteak) + L"  Press:V";

    debugRenderer.DrawText({0, 50.0f}, Status);

    debugRenderer.DrawText({10, 100.0f}, DirectX::Colors::DarkGreen, textS);
    debugRenderer.DrawText({10, 150.0f}, DirectX::Colors::DarkGreen, textO);
    debugRenderer.DrawText({10, 200.0f}, DirectX::Colors::DarkGreen, textOV);
    debugRenderer.DrawText({10, 250.0f}, DirectX::Colors::DarkGreen, textT);
    debugRenderer.DrawText({10, 300.0f}, DirectX::Colors::DarkGreen, L"GameClear Cost:200 Press:Q");
    debugRenderer.DrawText({10, 350.0f}, DirectX::Colors::DarkGreen, L"Exit Press:O");
    debugRenderer.DrawText({10, 400.0f}, DirectX::Colors::DarkGreen, L"End This Game Press:Enter");
}
