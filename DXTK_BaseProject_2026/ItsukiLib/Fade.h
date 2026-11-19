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
            BlackOUt,   //
        };

        void Initialize()
        {

        }

        void Update(float elapsedtime)
        {

        }

        //ステートの設定
        void SetState(State state, float duration = 0.5f)
        {

        }
 
    private:


        //現在の状態を占めす
        State m_state;


    };

}