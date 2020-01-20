//=============================================================================
//
// 風船処理 [barun.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "barun.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEX_NAME	("data/TEXTURE/object/barun.png")

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBarun::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CBarun * CBarun::Create(const D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move)
{
	//インスタンスの生成
	CBarun * pSceneBillboard;
	pSceneBillboard = new CBarun;

	//初期化処理
	pSceneBillboard->Init(nType, move);

	//設定処理
	pSceneBillboard->Set(pos, D3DXVECTOR3(10.0f, 20.0f, 0.0f));

	return pSceneBillboard;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBarun::CBarun() : CSceneBillborad(3, OBJTYPE_BILLBORAD)
{
	m_move = INITIALIZE_VECTOR3;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBarun::CBarun(int nPriority, OBJTYPE ObjType) : CSceneBillborad(nPriority, ObjType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CBarun::~CBarun(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBarun::Init(int nType, D3DXVECTOR3 move)
{
	//ビルボードの初期化
	CSceneBillborad::Init();
	CSceneBillborad::BindTexture(m_pTexture);
	CSceneBillborad::SetUV(D3DXVECTOR2(0.0f + (nType * 0.2f), 0.0f), D3DXVECTOR2(0.2f + (nType * 0.2f), 1.0f));

	//変数宣言
	m_move = move;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBarun::Uninit(void)
{
	CSceneBillborad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBarun::Update(void)
{
	D3DXVECTOR3 Pos = CSceneBillborad::GetPos();

	Pos += m_move;

	CSceneBillborad::SetPos(Pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CBarun::Draw(void)
{
	//描画処理
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CSceneBillborad::Draw();

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); 

}

//=============================================================================
// テクスチャの読みこみ処理
//=============================================================================
void CBarun::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//風船テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CBarun::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}