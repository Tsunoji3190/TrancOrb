// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <cstdint>

namespace SuzukiLib::Audio
{
    /// @brief 再生中の音声インスタンスを参照する世代付きハンドル。
    /// @details オーディオプレイヤーが管理する内部スロットのインデックスと世代値を組み合わせた構造体。
    struct AudioHandle
    {
        /// @brief プレイヤー内部スロットのインデックス。
        uint32_t index = 0;

        /// @brief スロット再利用を識別する世代値。0 は無効値。
        uint32_t generation = 0;

        /// @brief ハンドルが有効か判定する。プレイヤー内で現在も再生中かは保証しない。
        [[nodiscard]] bool IsValid() const
        {
            return generation != 0;
        }

        /// @brief ハンドルの一致比較用オーバーロード。
        friend bool operator==(const AudioHandle&, const AudioHandle&) = default;
    };
}
