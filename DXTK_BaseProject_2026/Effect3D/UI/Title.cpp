//--------------------------------------------------------------------------------------
// File: Title.cpp
//
// タイトル
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Title.h"
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

Effect3D::Title::Title()
    : m_titleIndex(0)
    , m_windowHeight(0)
    , m_windowWidth(0)
    , m_pDR(nullptr)
    ,m_timer(0)
{
    m_userInterface.clear();
}

Effect3D::Title::~Title()
{
}

void Effect3D::Title::Initialize(DX::DeviceResources* pDR, int width, int height)
{
    m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    //  タイトル画像を読み込む
    Add(L"Resources/Textures/TrancOrbTitle.png"
        , DirectX::SimpleMath::Vector2(m_windowWidth / 2, 150)
        , DirectX::SimpleMath::Vector2(1.2f, 1.2f)
        , Effect3D::ANCHOR::MIDDLE_CENTER);
}

void Effect3D::Title::Update(float elapsedtime)
{ 
    m_timer += elapsedtime;
    m_cos = DirectX::XMScalarCos(m_timer);


    for (int i = 0; i < m_userInterface.size(); i++)
    {

        float Y = m_userInterface[i]->GetPosition().y + (m_cos / 4);

        DirectX::SimpleMath::Vector2 pos = {m_userInterface[i]->GetPosition().x,Y};

        //  実際に表示したいアイテム画像を表示
        m_userInterface[i]->SetPosition(pos);
    }

}

void Effect3D::Title::Render()
{
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        //  実際に表示したいアイテム画像を表示
        m_userInterface[i]->Render();
    }
}

void Effect3D::Title::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, Effect3D::ANCHOR anchor)
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


