#pragma once

namespace Itsuki
{
    class GameTimer
    {
    public:
        GameTimer() : m_timer{0}, m_maxTimer{0}, m_isTimer{true}
        {
        }

        void Initialize()
        {
            m_maxTimer = FIRST_TIMER;
            m_timer = m_maxTimer;
            m_isTimer = true;
        }

        void Update(float elapsedtime)
        {

            // 時間が０より下になっていなかったら
            if (m_timer > 0)
            {
                // 時間を減らしていく
                RemoveTimer(elapsedtime);

                // タイマーが0以下になったら
                if (m_timer <= 0)
                {
                    m_timer = 0;
                }
            }
        }

        // 時間を初期化する関数
        void ResetTimer()
        {
            m_timer = m_maxTimer;
        }

        // 現在の時間を取得する関数
        float GetTimer()
        {
            return m_timer;
        }

        // 時間を減らす関数
        void RemoveTimer(float timer)
        {
            m_timer -= timer;
        }

        // 最大時間を増やす関数
        void AddMaxTimer(float timer)
        {
            m_maxTimer += timer;
        }

        // 時間を動かすかどうか設定する関数
        void SetIsTimer(bool istime)
        {
            m_isTimer = istime;
        }

        // 時間が動いてるかを返す関数
        bool GetITimer()
        {
            return m_isTimer;
        }

    private:
        // 最初の時間
        static constexpr float FIRST_TIMER = 1;

    private:
        // 経過時間
        float m_timer;

        // 動ける最大時間
        float m_maxTimer;

        // 時間を稼働させるかどうか
        bool m_isTimer;
    };

}