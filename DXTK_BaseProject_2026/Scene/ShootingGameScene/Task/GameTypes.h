//--------------------------------------------------------------------------------------
// File: GameType.h
//
// ゲームのタイプの定義ヘッダ
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

// 当たり判定用のグループ
enum class Faction
{
    Player, // プレイヤー側
    Enemy,  // 敵側
    Neutral // 中立
};

// 当たり判定用のグループを持つためのインターフェイスクラス
class IFaction
{
public:
    // インターフェイスは純粋仮想デストラクタを必ず書く
    virtual ~IFaction() = default;

    // 当たり判定用のグループを取得する関数
    virtual Faction GetFaction() const = 0;
};
