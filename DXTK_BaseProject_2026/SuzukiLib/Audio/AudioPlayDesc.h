// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include "AudioChannel.h"

namespace SuzukiLib::Audio
{
    /// @brief 音の再生時に指定するパラメータ指定用構造体。
    struct AudioPlayDesc
    {
        /// @brief 音を再生するチャンネル。音量の計算に使用される。
        /// 
        /// デフォルトは AudioChannel::Se（効果音チャンネル）。
        AudioChannel channel = AudioChannel::Se;

        /// @brief サウンドインスタンスの音量。0.0f から 1.0f にクランプされる。
        ///
        /// 値は最終的な音量計算において、マスター音量およびチャンネルごとの音量と乗算される。
        /// 範囲は 0.0f（無音）から 1.0f（最大音量）。デフォルトは 1.0f（最大音量）。
        float volume = 1.0f;

        /// @brief サウンドインスタンスのピッチ。
        /// 
        /// ピッチシフトの値。0.0f で元のピッチ、正の値で高く、負の値で低くなる。
        /// デフォルトは 0.0f（ピッチ変更なし）。
        float pitch = 0.0f;

        /// @brief サウンドインスタンスのパン。
        /// 
        /// ステレオパンニングの値を指定する。
        /// 通常、負の値で左寄り、0.0f で中央、正の値で右寄りとなる。
        /// デフォルトは 0.0f（中央）。
        float pan = 0.0f;

        /// @brief true の場合はループ再生を有効にする。PlayOneShot() では無視される。
        /// 
        /// true の場合、音声は終端に達すると自動的に先頭から再生を繰り返す。
        /// デフォルトは false（ループなし）。
        bool loop = false;
    };
}
