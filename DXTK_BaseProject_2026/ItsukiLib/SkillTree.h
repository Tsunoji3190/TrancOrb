#pragma once
#include "SkillNode.h"

namespace Itsuki
{
	using namespace Itsuki;

	class SkillTree
    {
    public:

        //SkillTree()
        //{
        //    //ノードを作っていく
        //    m_node[0]->SetNode(nullptr,)
        //}
        

        //オーブの数の追加
        void AddOrb(int num);

        //オーブの価値増加
        void AddOrbCost(int num);

        //オーブを取得できる範囲増加
        void Magnet(int num);

        //価値が2倍オーブの追加
        void LuckOrb(float num);

        //いいことが起きる確率を増加させる
        void Luck(float num);

        //脚の速さ増加
        void Speed(int num);

        //スキルを追加
        void GetSkill(int num);

        //オーブのインターバル減少
        void IntervalDecrease(float time);


    private:

        //スキルノードを数分入れる
        SkillNode* m_node[15];
    };

}