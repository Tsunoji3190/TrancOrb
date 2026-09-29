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
    : m_menuIndex(-1)
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
        , DirectX::SimpleMath::Vector2(m_windowWidth/2, 420)
        , DirectX::SimpleMath::Vector2(0.8f,0.8f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);
    //  メニュー2の画像を読み込む
    Add(L"Resources/Textures/TrancPorinExit.png"
        , DirectX::SimpleMath::Vector2(m_windowWidth / 2, 620)
        , DirectX::SimpleMath::Vector2(0.8f, 0.8f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);

    for (size_t i = 0; i < m_userInterface.size(); i++)
    {
        Itsuki::Button button;
        //画像
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = m_userInterface[i]->GetTexture();
        // 画像の大きさ
        DirectX::SimpleMath::Vector2 textureSize;

        // 画像のサイズ取得
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        texture->GetResource(resource.GetAddressOf());

        Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
        if (SUCCEEDED(resource.As(&tex2D)))
        {
            D3D11_TEXTURE2D_DESC desc = {};
            tex2D->GetDesc(&desc);
            textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height);
        }

        // ボタンの当たり判定を表示サイズに合わせて設定
        button.SetRect(textureSize);
        button.Setpositon(m_userInterface[i]->GetPosition());

        m_button.push_back(button);
    }

}

void Effect3D::Menu::Update()
{
    auto mouse = Mouse::Get().GetState();

    //  各アイテムに表示する画像の初期サイズを設定する
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_userInterface[i]->SetScale(m_userInterface[i]->GetBaseScale());
    }

    for (size_t i = 0; i < m_userInterface.size(); i++)
    { 
        if (m_button[i].IsCursored(mouse, { 0,0 }))
        {
            //  選択中の初期サイズを取得する
            DirectX::SimpleMath::Vector2 select = m_userInterface[i]->GetBaseScale();
            //  選択状態とするための変化用サイズを算出する
            DirectX::SimpleMath::Vector2 selectScale = DirectX::SimpleMath::Vector2::Lerp(
                m_userInterface[i]->GetBaseScale(), DirectX::SimpleMath::Vector2::One, 1);
            //  選択状態は初期状態＋30％の大きさとする
            select += selectScale * 0.3f;
            //  算出後のサイズを現在のサイズとして設定する
            m_userInterface[i]->SetScale(select);

            //メニューを合わせたやつと同じにする
            SetMenuState(i);

        }
    }

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


