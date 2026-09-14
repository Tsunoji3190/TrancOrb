#pragma once

class Button
{
public:

	//当たり判定の大きさを設定する
    void SetRect(DirectX::XMUINT2 size)
	{
        m_size = size;
	}
	//画像の大きさを当たり判定として設定する
    void SetRectImage();

	//クリックされた瞬間
	bool IsPushed()
	{
	}

private:

	DirectX::XMUINT2 m_size;


};