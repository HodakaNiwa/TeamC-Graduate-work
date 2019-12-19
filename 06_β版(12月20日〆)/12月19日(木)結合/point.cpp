//=============================================================================
//
// ポイントの処理 [point.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "point.h"
#include "manager.h"
#include "scene.h"
#include "game.h"
#include "numberBillborad.h"
#include "sceneBillBorad.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HIGHT	(60.0f)
#define SIZE	(D3DXVECTOR3(22.0f, 37.0f, 0.0f))
#define SIZE1	(D3DXVECTOR3(30.0f, 60.0f, 0.0f))
#define SIZE2	(D3DXVECTOR3(40.0f, 70.0f, 0.0f))
#define LIFE	(60)
#define TEX_NAME ("data/TEXTURE/game/plus.png")

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPoint::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CPoint * CPoint::Create(int nPoint, D3DXVECTOR3 pos, D3DXCOLOR Color)
{
	CPoint * pScore = NULL;

	pScore = new CPoint;
	pScore->m_pos = pos;
	pScore->m_Color = Color;
	pScore->m_nPoint = nPoint;
	pScore->Init();
	return pScore;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPoint::CPoint():CScene(10, CScene::OBJTYPE_BILLBORAD){}

//=============================================================================
// デストラクタ
//=============================================================================
CPoint::~CPoint(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPoint::Init(void)
{
	//変数の初期化
	m_pPlus = NULL;
	m_nLife = LIFE;
	m_bDeth = false;

	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		m_nNumber[nCnt] = 0;
		m_pNumber[nCnt] = NULL;
	}

	//ナンバーのサイズ設定
	D3DXVECTOR3 Size = INITIALIZE_VECTOR3;
	float fMiddle = 0.0f;
	if (m_nPoint < 4)
	{
		Size = SIZE;
		fMiddle = 32.0f;
	}
	else if (m_nPoint < 8)
	{
		Size = SIZE1;
		fMiddle = 40.0f;
	}
	else if (m_nPoint > 7)
	{
		Size = SIZE2;
		fMiddle = 50.0f;
	}

	//ナンバーの生成
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		m_pos.y = HIGHT;
		m_pos.x += nCnt * fMiddle;
		m_pNumber[nCnt] = CNumberBillborad::Create(m_pos, Size);
		m_pNumber[nCnt]->SetColor(m_Color);
	}

	//ポイントの繁栄
	AddScore(m_nPoint);

	//プラスの表示
	if (m_nNumber[0] == 0)
	{
		m_pos.x -= fMiddle * 2;
		m_pPlus = CSceneBillborad::Create(m_pos);
	}
	else
	{
		m_pos.x -= fMiddle * MAX_NUMBER_POINT + 1;
		m_pPlus = CSceneBillborad::Create(m_pos);
	}

	//プラスの数値設定
	if (m_pPlus != NULL)
	{
		m_pPlus->BindTexture(m_pTexture);						//テクスチャの設定
		m_pPlus->SetSize(D3DXVECTOR3(Size.x, Size.x, 0.0f));	//サイズの設定
		m_pPlus->SetColor(m_Color);								//色の設定
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPoint::Uninit(void)
{
	//数字の破棄
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			m_pNumber[nCnt]->Uninit();
			delete m_pNumber[nCnt];
			m_pNumber[nCnt] = NULL;
		}
	}

	m_pPlus = NULL;

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPoint::Update(void)
{
	//表示時間を減らす
	m_nLife--;

	//プラスの位置、色の設定
	if (m_pPlus != NULL)
	{	
		//位置の設定
		D3DXVECTOR3 pos = m_pPlus->GetPos();
		D3DXCOLOR col = m_pPlus->GetColor();
		bool bDeth = false;
		pos.y++;
		m_pPlus->SetPos(pos);	

		if (m_nLife < 0)
		{
			col.a -= 0.05f;

			//値の修正
			if (col.a < 0.0f) { col.a = 0.0f; }
			m_pPlus->SetColor(col);
		}
	}

	//位置、色の更新
	D3DXCOLOR Color = INITIALIZE_VECTOR4;
	D3DXVECTOR3 Pos = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			//位置更新
			Pos = m_pNumber[nCnt]->GetPos();
			Pos.y++;
			m_pNumber[nCnt]->SetPos(Pos);

			//表示時間がなくなったら			
			if (m_nLife > 0) { continue; }
			Color = m_pNumber[nCnt]->GetColor();
			Color.a -= 0.05f;

			if (Color.a < 0.0f)
			{
				Color.a = 0.0f;
				m_bDeth = true;
			}

			//色の設定
			m_pNumber[nCnt]->SetColor(Color);
		}
	}

	//死亡フラグが有効だったら
	if (m_bDeth)
	{
		if (m_pPlus != NULL)
		{
			m_pPlus->Uninit();
			m_pPlus = NULL;
		}

		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPoint::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (nCnt == 0 && m_nNumber[0] == 0) { continue; }
		m_pNumber[nCnt]->Draw();
	}

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// スコアの加算
//=============================================================================
void CPoint::AddScore(int nScore)
{
	//変数宣言
	int nCntNum;			//スコアの桁数のカウンター
	int nNum = 1;
	int nNum2 = 1;

	//スコアの加算
    m_nPoint = nScore;

	//桁数の計算
	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT; nCntNum++)
	{
		m_nNumber[nCntNum] = (m_nPoint % nNum) / nNum2;	//格納する数字を計算
		float fUV = m_nNumber[nCntNum] * 0.1f;
		m_pNumber[nCntNum]->SetUV(D3DXVECTOR2(0.0f + fUV, 0.0f), D3DXVECTOR2(0.1f + fUV, 1.0f));

		//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// スコアの取得
//=============================================================================
int CPoint::GetScore(void)
{
	return m_nPoint;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CPoint::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPoint::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}