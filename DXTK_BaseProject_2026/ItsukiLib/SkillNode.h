#pragma once
#include <functional>
#include <string>
#include <memory>
#include <Windows.h>
#include <GameContext.h>
#include <WICTextureLoader.h>

namespace Itsuki
{
    class SkillNode
    {
    public:
        SkillNode()
            : m_parentNode{nullptr},
            m_skillUp{},
            m_steak{0},
            m_isGet{false}
        {
        }

        ~SkillNode()
        {
        }

        void Render()
        {

        }


        void SetNode(GameContext & gameContext,SkillNode * parent, std::function<void()> skillup, int steak,
                     std::wstring skillimage)
        {
            m_parentNode = parent;
            m_skillUp = skillup;
            m_steak = steak;
            m_skillImage = skillimage;

            // DirectX3Dのデバイスを取得する
            auto device = gameContext.deviceResources.GetD3DDevice();

            // DirectX3Dのデバイスコンテキストを取得する
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            m_states = std::make_unique<DirectX::CommonStates>(device);

            DirectX::CreateWICTextureFromFile(device, m_skillImage.c_str(), nullptr,
                                              m_texture.ReleaseAndGetAddressOf());


        }

        //取得済みかどうか設定する
        void SetIsGet(bool isget)
        {
            m_isGet = isget;
        }

        //親のノードを返す
        SkillNode* GetPrevNode()
        {
            return m_parentNode;
        }

        //実行する関数を返す
        std::function<void()> GetSkillUp()
        {
            return m_skillUp;
        }

        //必要なオーブ数を返す
        int GetSteak()
        {
            return m_steak;
        }

        //画像名を返す
        std::wstring GetImage()
        {
            return m_skillImage;
        }

        //取得済みか返す
        bool GetIsGet()
        {
            return m_isGet;
        }

    private:

        //自分の前となるノード
        SkillNode* m_parentNode;

        //実行する関数
        std::function<void()> m_skillUp;

        //獲得するのに必要なオーブ数
        int m_steak;

        //スキルの画像名
        std::wstring m_skillImage;

        //このスキルが既に取得済みか
        bool m_isGet;

        //当たり判定の大きさ
        RECT HitDetection;

        //スプライトバッヂ
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        //コモンステート
        std::unique_ptr<DirectX::CommonStates> m_states;

        //画像
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    };

}