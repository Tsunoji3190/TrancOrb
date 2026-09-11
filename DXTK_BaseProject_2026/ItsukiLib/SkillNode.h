#pragma once
#include <functional>

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


        void SetNode(SkillNode* parent, std::function<void()> skillup,int steak,wchar_t skillimage)
        {
            m_parentNode = parent;
            m_skillUp = skillup;
            m_steak = steak;
            m_skillImage = skillimage;
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
        wchar_t GetImage()
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
        wchar_t m_skillImage;


        //このスキルが既に取得済みか
        bool m_isGet;
    };

}