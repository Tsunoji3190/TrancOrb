//--------------------------------------------------------------------------------------
// File: Menu.cpp
//
// メニュー
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Menu.h"
#include "Effect3D/UI/UserInterface.h"

#include "Effect3D/BinaryFile.h"
#include "Common/DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

Effect3D::Menu::Menu()
    : m_menuIndex(0)
    , m_windowHeight(0)
    , m_windowWidth(0)
    , m_pDR(nullptr)
{
    m_userInterface.clear();
}

Effect3D::Menu::~Menu()
{
}

void Effect3D::Menu::Initialize(DX::DeviceResources* pDR,int width,int height)
{
	m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;


    //  メニュー１画像を読み込む
    Add(L"Resources/Textures/TrancPorinStart.png"
        , DirectX::SimpleMath::Vector2(m_windowWidth/2, 400)
        , DirectX::SimpleMath::Vector2(0.8f,0.8f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);
    //  メニュー２画像を読み込む
    Add(L"Resources/Textures/TrancPorinContinue.png"
        , DirectX::SimpleMath::Vector2(m_windowWidth / 2, 520)
        , DirectX::SimpleMath::Vector2(0.8f, 0.8f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);
    //  メニュー３の画像を読み込む
    Add(L"Resources/Textures/TrancPorinExit.png"
        , DirectX::SimpleMath::Vector2(m_windowWidth / 2, 640)
        , DirectX::SimpleMath::Vector2(0.8f, 0.8f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);

}

void Effect3D::Menu::Update()
{
    DirectX::Keyboard::State keystate = DirectX::Keyboard::Get().GetState();
    m_tracker.Update(keystate);
    if (m_tracker.pressed.Down)
    {
        //  ↓キーを押したら、選択先を1つ進める
        m_menuIndex += 1;
        m_menuIndex %= m_userInterface.size();
    }
    if (m_tracker.pressed.Up)
    {
        //  ↑キーを押したら、選択先を1つ戻す。
        //  ただし、選択先のオーバーを割り算の余りで補正するため、（アイテムの最大個数 - 1）を足して必ず余りで計算する形にしておく。
        //  以下の式は、例えばメニューが4つあったら、現在値に3を足すという事。
        //  例）選択中のメニューが0～3まである内の2番目だった場合、
        //  (2 + (4 - 1)) % 4 = 1 ← 選択中の番号が1つ減った
        //  ということ

        m_menuIndex += static_cast<unsigned int>(m_userInterface.size()) - 1;
        m_menuIndex %= m_userInterface.size();
    }

    //  各アイテムに表示する画像の初期サイズを設定する
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_userInterface[i]->SetScale(m_userInterface[i]->GetBaseScale());
    }

    //  選択中の初期サイズを取得する
    DirectX::SimpleMath::Vector2 select = m_userInterface[m_menuIndex]->GetBaseScale();
    //  選択状態とするための変化用サイズを算出する
    DirectX::SimpleMath::Vector2 selectScale = DirectX::SimpleMath::Vector2::Lerp(m_userInterface[m_menuIndex]->GetBaseScale(), DirectX::SimpleMath::Vector2::One, 1);
    //  選択状態は初期状態＋30％の大きさとする
    select += selectScale * 0.3f;
    //  算出後のサイズを現在のサイズとして設定する
    m_userInterface[m_menuIndex]->SetScale(select);
}

void Effect3D::Menu::Render()
{
    for (int i = 0;i < m_userInterface.size();i++)
    {
        //  実際に表示したいアイテム画像を表示
        m_userInterface[i]->Render();
    }
}

void Effect3D::Menu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, Effect3D::ANCHOR anchor)
{
    //  メニューとしてアイテムを追加する
    std::unique_ptr<Effect3D::UserInterface> userInterface = std::make_unique<Effect3D::UserInterface>();
    //  指定された画像を表示するためのアイテムを作成する
    userInterface->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

    //  アイテムを新しく追加
    m_userInterface.push_back(std::move(userInterface));

}


