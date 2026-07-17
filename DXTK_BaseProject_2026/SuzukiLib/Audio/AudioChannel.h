// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once

namespace SuzukiLib::Audio
{
    /// @brief 音量ミキサーが再生時のサウンドカテゴリ分類とチャンネル音量の適用に使用するenum。
    /// 
    /// このenumは、オーディオシステムで異なる種類のサウンドを分類し、
    /// それぞれに独立した音量制御やエフェクト設定を適用するために使用する。
    enum class AudioChannel
    {
        /// @brief BGMや環境音など、持続的に再生されるサウンドに使用する。
        Bgm,

        /// @brief ボタンクリック音、爆発音、衝突音など、短い単発サウンドに使用する。
        Se,
    };
}
