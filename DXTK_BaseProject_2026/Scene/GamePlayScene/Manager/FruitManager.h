#pragma once
#include <vector>
#include <memory>
#include "../Fruit.h"
#include"../../../ItsukiLib/ColliderFactory.h"

class FruitManager
{
public:

    FruitManager(GameContext* pGameContext, const DirectX::SimpleMath::Matrix& view,
               const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel)
        : m_pGameContext{pGameContext}
        , m_view{view}
        , m_projection{projection}
        , m_pModel{pModel}
        , m_pFruits{}
        , m_fruitValues{0}
        , m_fruitInterval{0}
        , m_saveFruitInterval{0}
        , m_count{0}
        , m_startingFruitCount{0}
    { 
        
        for (size_t i = 0; i < FIRST_ORB; i++)
        {
            AddFruit();
        }

    };

    ~FruitManager() = default;

    void Initialize()
    {
        m_fruitValues = FIRST_ORBVALUE;
        m_fruitInterval = FIRST_INTERVAL;
        m_saveFruitInterval = FIRST_INTERVAL;
        m_startingFruitCount = 0;
        m_count = 0;


    }

    void Update(float elapsedtime)
    {
        m_count += elapsedtime;

        //一定の時間で追加していく
        if (m_count >= m_fruitInterval)
        {
            AddFruit();
            m_count = 0;
        }
    }

    void Render();

    void AddFruit()
    {
        // 当たり判定工場の作成
        Itsuki::ColliderFactory& colF = Itsuki::ColliderFactory::GetRefInstance();

        /// 球
        Itsuki::CollisionParams fruitCol = {{0.0f, 0.0f, .0f}, {-.2f, -.2f, -.2f}, {0.2f, 0.2f, 0.2f}, {0.4}};


        std::unique_ptr<Fruit> fruit = std::make_unique<Fruit>(m_pGameContext, m_view, m_projection,m_pModel,
                                                         colF.MakeCollider(Itsuki::SHAPE::SPHERE, fruitCol));
        
        m_pFruits.push_back(std::move(fruit));
    }

    //指定したフルーツを消す
    void DeleteFruit(int index)
    {
        if (index < 0 || static_cast<size_t>(index) >= m_pFruits.size())
            return;
        
        m_pFruits.erase(m_pFruits.begin() + index);

    }

    //すべてのフルーツを消す
    void ResetFruit()
    {
        m_pFruits.clear();
        m_count = 0;

        for (size_t i = 0; i < m_startingFruitCount; i++)
        {
            AddFruit();
        }
    }

    //指定されたフルーツを返す
    Fruit* GetFruit(int Index)
    {
        return m_pFruits[Index].get();
    }

    //フルーツの数を返す
    int GetNumFruits()
    {
        return m_pFruits.size();
    }

    // フルーツの価値を追加する
    void SetFruitValue(int value)
    {
        m_fruitValues += value;
    }

    // フルーツの価値を返す
    int GetFruitValue()
    {
        return m_fruitValues;
    }

    //Intervalを減少させる
    void Remomveinterval(float num)
    {
        m_fruitInterval -= num;
        m_saveFruitInterval = m_fruitInterval;
    }

    //元のインターバルに戻す
    void ReturnInterval()
    {
        m_fruitInterval = m_saveFruitInterval;
    }

    //Intervalを設定する
    void SetInterval(float num)
    {
        m_fruitInterval = num;
    }



    //最初に出てくるフルーツを設定する
    void AddFirst(int num)
    {
        m_startingFruitCount += num;
    }

private:

    //フルーツの数
    static constexpr int FIRST_ORB = 1;

    //フルーツの価値
    static constexpr int FIRST_ORBVALUE = 1;

    //インターバル
    static constexpr float FIRST_INTERVAL = 5;

    //収集フェーズ始めに存在するフルーツ
    static constexpr int FIRST = 0;


private:
    // ゲームコンテキストへのポインタ
    GameContext* m_pGameContext;

    // ビュー行列
    const DirectX::SimpleMath::Matrix& m_view;

    // プロジェクション行列
    const DirectX::SimpleMath::Matrix& m_projection;

    // モデルへのポインタ
    DirectX::Model* m_pModel = nullptr;

    //フルーツを格納する箱
    std::vector<std::unique_ptr<Fruit>> m_pFruits;

    //フルーツの基本価格
    int m_fruitValues;

    //フルーツのインターバル
    float m_fruitInterval;

    //フルーツの保持インターバル
    float m_saveFruitInterval;

    //時間をカウントする
    float m_count;

    //最初にどんだけフルーツを入れるか
    int m_startingFruitCount;
};


