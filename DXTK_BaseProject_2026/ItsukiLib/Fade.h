#pragma once

namespace Itsuki
{
    //フェードクラス
	class Fade
    {
    public:

        enum class State
        {
            Clear,      //
            FadeIn,     //
            FadeOut,    //
            BlackOut,   //
        };

        void Initialize()
        {
        }

        void Update(float elapsedtime)
        {
            switch (m_state)
            {
                case State::FadeIn:
                    break;

                case State::FadeOut:
                    break;

                case State::BlackOut:
                    break;

                case State::Clear:
                    break;
            }
        }

        //ステートの設定
        void SetState(State state, float duration = 0.5f)
        {
            m_state = state;
            m_duration = duration;
        }

    private:

        //フェードインの行動
        void FadeInBehavior(float elapsedtime)
        {

        }

        //フェードアウトの行動
        void FadeOutBehavior(float elapsedtime)
        {

        }

        //ブラックアウトの行動
        void BlackOutBehavior()
        {

        }


    private:


        //現在の状態を占めす
        State m_state;

        //間隔を示す
        float m_duration;
    };

}