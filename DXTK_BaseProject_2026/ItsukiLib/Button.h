#pragma once

namespace Itsuki
{
    class Button
    {
    public:
        // 当たり判定の大きさを設定する
        void SetRect(DirectX::SimpleMath::Vector2 size)
        {
            m_size = size;
        }

        //位置を設定する
        void Setpositon(DirectX::SimpleMath::Vector2 position)
        {
            m_position = position;
        }

        // クリックされた瞬間
        bool IsPushed(DirectX::Mouse::State mouse)
        {
            if (mouse.leftButton)
            {
                auto mx = mouse.x;
                auto my = mouse.y;

                auto width = m_size.x + m_position.x;
                auto height = m_size.y + m_position.y;

                //ボタンの範囲内で押されていたらtrueを返す
                return (mx < width && my < height && mx > m_position.x && my > m_position.y) ? true : false;

            }

            return false;
        }

        //カーソルが重なったか返す関数
        bool IsCursored(DirectX::Mouse::State mouse)
        {
            auto mx = mouse.x;
            auto my = mouse.y;

            auto width = m_size.x + m_position.x;
            auto height = m_size.y + m_position.y;

            // ボタンの範囲内にカーソルがあったらtrueを返す
            return (mx < width && my < height && mx > m_position.x && my > m_position.y) ? true : false;
        }

    private:

        //大きさ
        DirectX::SimpleMath::Vector2 m_size;

        //位置
        DirectX::SimpleMath::Vector2 m_position;
    };
}
