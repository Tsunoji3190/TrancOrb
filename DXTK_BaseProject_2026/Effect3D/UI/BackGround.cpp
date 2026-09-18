//--------------------------------------------------------------------------------------
// File: BackGround.cpp
//
// タイトル
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "BackGround.h"
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

Effect3D::BackGround::BackGround()
    : m_windowHeight(0)
    , m_windowWidth(0)
    , m_pDR(nullptr)
{
    m_userInterface.clear();
}

Effect3D::BackGround::~BackGround()
{
}

void Effect3D::BackGround::Initialize(DX::DeviceResources* pDR, int width, int height)
{
    m_pDR = pDR;
    m_windowWidth = width / 64;
    m_windowHeight = height / 64;


    for (int i = 0; i < m_windowWidth; i++)
    {
        for (int j = 0; j < m_windowHeight; j++)
        {
            //  タイトル画像を読み込む
            Add(L"Resources/Textures/Grid.png"
                , DirectX::SimpleMath::Vector2(i, j)
                , DirectX::SimpleMath::Vector2(0.025f, 0.025f)
                , Effect3D::ANCHOR::TOP_LEFT);

        }
    }
}

void Effect3D::BackGround::Render()
{
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        //  実際に表示したいアイテム画像を表示
        m_userInterface[i]->Render();
    }
}

void Effect3D::BackGround::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, Effect3D::ANCHOR anchor)
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


