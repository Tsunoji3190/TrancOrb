#pragma once

class BaseState
{
public:

    // 初期化する
    virtual void Initialize() = 0;

    //デストラクタ
    ~BaseState() = default;
    // 状態を実行する
    virtual void Update(float elapsedtime) = 0;
    // 状態を描画する
    virtual void Render() = 0;
};

