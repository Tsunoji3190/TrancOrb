//--------------------------------------------------------------------------------------
//	File: ParticleManager.cpp
//
//	パーティクルマネージャクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ParticleManager.h"

#include "Effect3D/BinaryFile.h"
#include <random>

using namespace DirectX;

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Effect3D::ParticleManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

///	<summary>
///	コンストラクタ
///	</summary>
Effect3D::ParticleManager::ParticleManager(int count)
	:m_pDR(nullptr)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-5, 5);
	for (int i = 0; i < count; i++)
	{
		//登録用のパーティクルポジションを設定する
		std::unique_ptr<Effect3D::ParticlePosition> position = std::make_unique<Effect3D::ParticlePosition>();

		////初期位置をrandomに設定する
		//float posX = static_cast<float>(dist(gen));
		//float posZ = static_cast<float>(dist(gen));
		//SimpleMath::Vector3 pos = SimpleMath::Vector3(posX, 0, posZ);

		////初期位置をrandomにセットする
		//position->SetPosition(pos);

		position->SetPositionRawColumn(i, 5,1.0f);

		//position->SetVelocityCircle(i, count, 0.01f);
		//position->SetVelocityRandom(-5,5);

		//配列に登録する
		m_particlePosition.push_back(std::move(position));
	}

}

///	<summary>
///	デストラクタ
///	</summary>
Effect3D::ParticleManager::~ParticleManager()
{
}

///	<summary>
///	WICテクスチャリソース読み込み関数
///	</summary>
///	<param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void Effect3D::ParticleManager::LoadTextureWIC(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}
///	<summary>
///	DDSテクスチャリソース読み込み関数
///	</summary>
///	<param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void Effect3D::ParticleManager::LoadTextureDDS(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateDDSTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}
///	<summary>
///	生成関数
///	</summary>
///	<param name="pDR">ユーザーリソース等から持ってくる</param>
void Effect3D::ParticleManager::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	ID3D11Device1* device = pDR->GetD3DDevice();

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み
	LoadTextureWIC(L"Resources/Textures/image01.png");
	LoadTextureWIC(L"Resources/Textures/yoshiko.jpg");
	LoadTextureDDS(L"Resources/Textures/shadow.dds");

	//	プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPosition>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

///	<summary>
///	Shader作成部分だけ分離した関数
///	</summary>
void Effect3D::ParticleManager::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	kHorikawa::BinaryFile VSData = kHorikawa::BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	kHorikawa::BinaryFile GSData = kHorikawa::BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	kHorikawa::BinaryFile PSData = kHorikawa::BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{//	エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{//	エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{//	エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

void Effect3D::ParticleManager::Update(float elapsedTime)
{
	for (auto itr = m_particlePosition.begin();
		itr != m_particlePosition.end();
		itr++)
	{
		(*itr).get()->Update(elapsedTime);
	}

}

///	<summary>
///	描画関数
///	</summary>
///	<param name="view">ビュー行列</param>
///	<param name="proj">射影行列</param>
void Effect3D::ParticleManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Matrix billboard)
{



	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();


	//	頂点情報(板ポリゴンの４頂点の座標情報）
	std::vector<VertexPosition> vertex;

	for (auto itr = m_particlePosition.begin();
		itr != m_particlePosition.end();
		itr++)
	{
		VertexPosition v;
		v.position = (*itr).get()->GetPosition();

		vertex.push_back(v);
	}


	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = (billboard).Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//	カリングは左周り
	context->RSSetState(m_states->CullNone());

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	m_batch->End();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);


}

void Effect3D::ParticleManager::Sort(DirectX::SimpleMath::Vector3 eyePos)
{
	for (std::vector<std::unique_ptr<ParticlePosition>>::iterator itr = m_particlePosition.begin();
		itr != m_particlePosition.end();
		itr++)
	{
		DirectX::SimpleMath::Vector3 eyePosition = eyePos;
		(*itr)->SetDistance(eyePosition);
	}

	std::sort(m_particlePosition.begin(), m_particlePosition.end(),
		[](const std::unique_ptr<ParticlePosition>& a, const std::unique_ptr<ParticlePosition>& b)
		{
			return (a->GetDistance() > b->GetDistance());
		}
	);


}

