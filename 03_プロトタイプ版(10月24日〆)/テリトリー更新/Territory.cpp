//=============================================================================
//
// テリトリー処理 [territory.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "Territory.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "game.h"
#include "Player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SaveTextName ("data/TEXT/SaveTerritory.txt")		//保存するテキスト名
#define OBJ_PRIORITY (3)
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//赤
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//青
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//緑
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//白
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//黒
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//黄
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//オレンジ
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//紫
#define UPDATE_LENGTH	(80.0f)								//更新状態を有効にする範囲
#define GET_LENGTH		(40.0f)								//取得範囲		
#define MIN_TERRITORY	(3)									//取得テリトリーの最低数

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 * CTerritory::m_ppTexture = NULL;
LPD3DXMESH * CTerritory::m_pMesh = NULL;
LPD3DXBUFFER * CTerritory::m_pBuffMat = NULL;
DWORD *	CTerritory::m_pNumMat = NULL;
D3DXVECTOR3 * CTerritory::m_pVtxMax = NULL;
D3DXVECTOR3 * CTerritory::m_pVtxMin = NULL;
int CTerritory::m_nMaxTex = 0;
int CTerritory::m_nMaxModel = 0;
int CTerritory::m_nCountObj = 0;
int CTerritory::m_nMaxObj = 0;
D3DXCOLOR CTerritory::m_DefaultColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

//国の色の設定
D3DXCOLOR CTerritory::m_CountryColor[CPlayer::TYPE_MAX] = 
{
	COLOR_WHITE,	//日本
	COLOR_BULE,		//ロシア
	COLOR_RED,		//アメリカ
	COLOR_YELLOW,	//ブラジル
	COLOR_ORENGE,	//イギリス
	COLOR_PAPULE,	//イタリア
	COLOR_BRACK,	//ニュージーランド
	COLOR_GREEN,	//南アフリカ
};

//=============================================================================
// 生成処理
//=============================================================================
CTerritory * CTerritory::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, int nPoint)
{
	//インスタンスの生成
	CTerritory * pTerritory = new CTerritory;

	//初期化処理
	pTerritory->Init();
	pTerritory->SetPos(pos);						//位置の設定
	pTerritory->SetRot(rot);						//向きの設定
	pTerritory->SetvtxMax(m_pVtxMax[ModelNum]);		//最大値
	pTerritory->SetvtxMin(m_pVtxMin[ModelNum]);		//最小値
	pTerritory->SetScale(scale);					//大きさの設定
	pTerritory->SetTexNum(nTexNum);					//テクスチャ番号
	pTerritory->SetModelType(ModelNum);				//モデル番号
	pTerritory->SetPoint(nPoint);					//ポイントの保存
	pTerritory->CreateCollider();

	//モデル・テクスチャの割り当て
	pTerritory->BindModel(m_pMesh[ModelNum], m_pBuffMat[ModelNum], m_pNumMat[ModelNum], m_ppTexture[nTexNum]);

	return pTerritory;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTerritory::CTerritory() : CSceneX(3, OBJTYPE_TERRITORY)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTerritory::CTerritory(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// デストラクタ
//=============================================================================
CTerritory::~CTerritory(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTerritory::Init(void)
{
	//変数の初期化
	m_nTex = 0;
	m_nModelType = 0;
	m_nPoint = 1;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_OldColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_State = STATE_NORMAL;							//状態
	m_nNumPlayer = -1;								//プレイヤー番号
	m_nOldNumPlayer = -1;							//前回のプレイヤー番号
	m_fLength = 0.0f;								//長さ
	m_bGetTerritory = false;						//テリトリーを取得したかどうか
	m_pNext = NULL;									//リストの次のポインタ
	m_pPrev = NULL;									//リストの前のポインタ

	CSceneX::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTerritory::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTerritory::Update(void)
{
	CSceneX::Update();

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		CPlayer * pPlayer = CManager::GetGame()->GetPlayer(nCntPlayer);
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

		//プレイヤーとの距離を求める
		m_fLength = CFunctionLib::Vec2Length(m_pos.x, m_pos.z, PlayerPos.x, PlayerPos.z);

		//更新範囲にいるかどうか
		if (m_fLength < UPDATE_LENGTH)
		{
			//状態別更新処理
			switch (m_State)
			{
			case STATE_NORMAL: UpdateNormal(pPlayer, nCntPlayer); break;	//通常状態
			case STATE_START:  UpdateStart(pPlayer, nCntPlayer);  break;	//始点の状態
			case STATE_PASS:   UpdatePass(pPlayer, nCntPlayer);   break;	//通過点の状態
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTerritory::Draw(void)
{
	SetColor(m_Color);
	CSceneX::Draw();
	SetColor(m_DefaultColor);
}

//=============================================================================
//　通常状態の更新処理
//=============================================================================
void CTerritory::UpdateNormal(CPlayer * pPlayer, int nCntPlayer)
{
	//取得範囲内にいるのかどうか
	if (m_fLength < GET_LENGTH)
	{
		if (m_bGetTerritory == true) { return; }	//テリトリーを取得状態なら更新しない

		//プレイヤー番号の保存
		m_nOldNumPlayer = m_nNumPlayer;
		m_nNumPlayer = nCntPlayer;

		//色の保存
		m_OldColor = m_Color;	
		m_Color = m_CountryColor[(int)pPlayer->GetType()];	

		//プレイヤーが始点を保持していたら
		if (pPlayer->GetTop() != NULL)
		{//通過点状態にする
			m_State = STATE_PASS;

			//リストに追加にする（最後尾）
			CTerritory * pCur = pPlayer->GetCur();	//現在のポインタ取得
			pCur->m_pNext = this;					//現在の次のポインタ代入

			//前回のテリトリー情報を保存
			m_pPrev = pCur;

			//最後尾に今回の情報を追加する
			pPlayer->SetCur(this);
		}
		else
		{//始点状態にする
			m_State = STATE_START;

			//リストに追加
			pPlayer->SetTop(this);	//先頭
			pPlayer->SetCur(this);	//現在
		}
	}
	else
	{//テリトリーを取得していない状態にする
		m_bGetTerritory = false;
	}
}

//=============================================================================
// 始点の更新処理
//=============================================================================
void CTerritory::UpdateStart(CPlayer * pPlayer, int nCntPlayer)
{
	//取得範囲内にいるのかどうか
	if (m_fLength < GET_LENGTH)
	{
		CTerritory * pTop;	//先頭ポインタ保存用

		if (nCntPlayer == m_nNumPlayer)
		{//テリトリーの完成
			 //ノード内のリストのポイントをすべてプレイヤーに加算する
			CTerritory * pTerritory = pPlayer->GetTop();	//リストの先頭を取得
			pTop = pTerritory;
			int nCountTerrotpry = 0;

			//リスト内にテリトリーが3つ以上あるかどうか
			if (false == ChackList(pTop, &nCountTerrotpry)) { return; }

			//位置保存用変数のメモリを確保
			D3DXVECTOR3 * pPos = new D3DXVECTOR3[nCountTerrotpry];

			int nCnt = 0;
			m_bGetTerritory = true;									//テリトリーを取得した状態にする

			//テリトリーの取得処理
			while (pTerritory != NULL)
			{
				CTerritory * pTerritoryNext = pTerritory->m_pNext;		//次のオブジェクトのポインタを保存する

				//位置を保存
				pPos[nCnt] = pTerritory->m_pos;

				//ポイントを加算する
				if (pTerritory->m_nNumPlayer != pTerritory->m_nOldNumPlayer)	//前回と同じテリトリーを取ったらポイントは加算されない
				{
					//前回のプレイヤーのポイントを減らす
					if (m_nOldNumPlayer != -1)
					{
						CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nOldNumPlayer);
					}
				}

				//通常状態にする
				pTerritory->m_State = STATE_NORMAL;
				pTerritory = pTerritoryNext;		//ポインタを進める
				nCnt++;
			}

			//範囲内にテリトリーがあるかどうか
			ChackIn(pPos, nCountTerrotpry, m_nNumPlayer);

			//プレイヤーで保存しているリストを初期化
			pPlayer->SetTop(NULL);
			pPlayer->SetCur(NULL);

			//メモリの開放
			if (pPos != NULL)
			{
				delete[] pPos;
				pPos = NULL;
			}
		}
		else
		{//テリトリーの破壊
			pTop = DestoroyTerritoryStart();
		}

		//ノードをNULLにする
		ResetTerritory(pTop);
	}
}

//=============================================================================
// 通過点の更新処理
//=============================================================================
void CTerritory::UpdatePass(CPlayer * pPlayer, int nCntPlayer)
{
	//取得範囲内にいるのかどうか
	if (m_fLength < GET_LENGTH)
	{
		if (nCntPlayer != m_nNumPlayer)
		{//テリトリーの破壊
			CTerritory * pTop = DestoroyTerritory();

			//ノードをNULLにする
			ResetTerritory(pTop);
		}
	}
}

//=============================================================================
// テリトリーの破壊(スタート地点用)
//=============================================================================
CTerritory * CTerritory::DestoroyTerritoryStart(void)
{
	CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nNumPlayer);
	CTerritory * pTop = pPlayer->GetTop();
	CTerritory * pTerritory = pTop;

	//プレイヤーで保存しているリストを初期化
	pPlayer->SetTop(NULL);
	pPlayer->SetCur(NULL);

	//データを初期化していく
	while (pTerritory != NULL)
	{
		CTerritory * pTerritoryNext = pTerritory->m_pNext;		//次のオブジェクトのポインタを保存する

		pTerritory->m_nNumPlayer = pTerritory->m_nOldNumPlayer;	//すべての領地を前回のプレイヤーに戻す
		pTerritory->m_Color = pTerritory->m_OldColor;			//前回の色に戻す
		pTerritory->m_State = STATE_NORMAL;						//通常状態にする

		pTerritory = pTerritoryNext;		//ポインタを進める
	}

	return pTop;
}

//=============================================================================
// テリトリーの破壊
//=============================================================================
CTerritory * CTerritory::DestoroyTerritory(void)
{
	CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nNumPlayer);
	CTerritory * pTop = pPlayer->GetTop();
	CTerritory * pTerritory = pTop;
	int nCount = 0;

	//プレイヤーで保存しているリストを初期化
	pPlayer->SetCur(pTop);

	//データを初期化していく
	while (pTerritory != NULL)
	{
		CTerritory * pTerritoryNext = pTerritory->m_pNext;		//次のオブジェクトのポインタを保存する

		if (nCount != 0)
		{
			pTerritory->m_nNumPlayer = pTerritory->m_nOldNumPlayer;	//すべての領地を前回のプレイヤーに戻す
			pTerritory->m_Color = pTerritory->m_OldColor;				//前回の色に戻す
			pTerritory->m_State = STATE_NORMAL;						//通常状態にする
		}

		pTerritory = pTerritoryNext;		//ポインタを進める
		nCount++;
	}

	return pTop;
}

//=============================================================================
// テリトリーのノードを初期化
//=============================================================================
void CTerritory::ResetTerritory(CTerritory * pTop)
{
	//トップの位置を保存する
	CTerritory * pTerritory = pTop;

	//pPrev, pNextをNULLにする
	while (pTerritory != NULL)
	{
		CTerritory * pSceneNext = pTerritory->m_pNext;	//次のオブジェクトのポインタを保存する
		
		//pPrev, pNextをNULLにする
		pTerritory->m_pPrev = NULL;
		pTerritory->m_pNext = NULL;

		//次のポインタに進める
		pTerritory = pSceneNext;
	}
}

//=============================================================================
// リスト内の状態チェック
//=============================================================================
bool CTerritory::ChackList(CTerritory * pTop, int * nCountTerritory)
{
	//変数宣言
	bool bClear = false;

	CTerritory * pTerritory = pTop;

	while (pTerritory)
	{
		CTerritory * pSceneNext = pTerritory->m_pNext;	//次のオブジェクトのポインタを保存する
		
		//カウンターを進める
		*nCountTerritory += 1;

		//次のポインタに進める
		pTerritory = pSceneNext;
	}

	if (*nCountTerritory >= MIN_TERRITORY)
	{
		bClear = true;
	}

	return bClear;
}

//=============================================================================
// 範囲内にテリトリーがあるかどうか
//=============================================================================
void CTerritory::ChackIn(D3DXVECTOR3 * pPos, int nMaxPos, int nNumPlayer)
{
	//ベクトルの数分メモリを確保
	D3DXVECTOR3 * pVec = new D3DXVECTOR3[nMaxPos];

	//ベクトルの方向を求める
	for (int nCntVec = 0; nCntVec < nMaxPos; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % nMaxPos) == 0) { nCnt = 0; }

		pVec[nCntVec] = pPos[nCnt] - pPos[nCntVec];
	}

	//敵のオブジェクトポインタの取得
	CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //オブジェクトポインタの先頭を取得			
	CScene * pScene = pSceneTop;						 //トップの位置を保存する
	CTerritory * m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//次のオブジェクトのポインタを保存する

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if ((pTerritory->m_State == STATE_NORMAL) && (pTerritory->m_nNumPlayer != nNumPlayer))
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = 0; nCnt < nMaxPos; nCnt++)
				{
					D3DXVECTOR3 VecB = TerritoryPos - pPos[nCnt];

					//外積を求める
					float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVec[nCnt].x, pVec[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

					//ベクトルより大きいかどうか
					if (fCross >= 0) { nCountEnter++; }
				}

				if (nCountEnter == nMaxPos)
				{//範囲内に入っていたら

					//プレイヤー情報の取得
					CPlayer * pPlayer = CManager::GetGame()->GetPlayer(nNumPlayer);

					pTerritory->m_Color = m_CountryColor[(int)pPlayer->GetType()];	//色の書き換え

					//相手プレイヤーの点数を減点する
					if ((pTerritory->m_nNumPlayer != -1))
					{
						CPlayer * pOtherPlayer = CManager::GetGame()->GetPlayer(pTerritory->m_nNumPlayer);
					}

					pTerritory->m_State = STATE_NORMAL;		//通常状態にする
					pTerritory->m_nNumPlayer = nNumPlayer;	//プレイヤー番号の置き換え
				}
			}
		}

		pScene = pSceneNext;	//ポインタを進める
	}

	//メモリの破棄
	if (pVec != NULL)
	{
		delete pVec;
		pVec = NULL;
	}
}

//=============================================================================
//　取得状態の更新処理
//=============================================================================
void CTerritory::SetColor(D3DXCOLOR Color)
{
	//マテリアル情報の取得
	LPD3DXBUFFER ModelMat = CSceneX::GetBuffMat();
	DWORD nNumMat = CSceneX::GetNumMat();

	//マテリアルポインタの取得
	D3DXMATERIAL * pMat = (D3DXMATERIAL*)ModelMat->GetBufferPointer();

	//マテリアルカラーの変更
	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse = Color;
	}
}

//=============================================================================
// 当たり判定の生成
//=============================================================================
void CTerritory::CreateCollider(void)
{
	// コライダーの生成
	CColliderManager *pColManager = CColliderManager::Create(1);
	if (pColManager == NULL) { return; }
	CBoxCollider *pBox = CBoxCollider::Create(m_pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 27.0f, 68.0f, 20.0f, true);
	pBox->SetParent(this);
	//pBox->SetParentMtxWorld(&GetMtx());
	pColManager->SetCollider(pBox, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CTerritory::LoadTex(CLoadTextTerritory::TEX_INFO TexInfo)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの最大数
	m_nMaxTex = TexInfo.nMaxTex;

	//テクスチャメモリを動的確保
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTex];
	}

	//テクスチャを生成する
	for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, &TexInfo.ppFileName[nCntTex][0], &m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTerritory::UnloadTex(void)
{
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}

		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=============================================================================
//　モデルの読み込み
//=============================================================================
void CTerritory::LoadModel(CLoadTextTerritory::OBJ_INFO ObjInfo)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//モデルの最大数
	m_nMaxModel = ObjInfo.nMaxModel;

	//モデルの動的確保
	if ((m_pMesh == NULL) && (m_pBuffMat == NULL) && (m_pNumMat == NULL))
	{
		m_pMesh = new LPD3DXMESH[m_nMaxModel];
		m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
		m_pNumMat = new DWORD[m_nMaxModel];
		m_pVtxMax = new D3DXVECTOR3[m_nMaxModel];
		m_pVtxMin = new D3DXVECTOR3[m_nMaxModel];
	}

	//モデルの生成
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(&ObjInfo.ppModelFileName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_pNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		//変数宣言
		int		nNumVtx = 0;	//頂点数
		DWORD	sizeFVF = 0;	//頂点フォーマットのサイズ
		BYTE *	pVtxBuff = 0;	//頂点バッファへのポインタ

								//頂点数を取得
		nNumVtx = m_pMesh[nCntModel]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCntModel]->GetFVF());

		m_pVtxMax[nCntModel] = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	//頂点の最大値
		m_pVtxMin[nCntModel] = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);		//頂点の最小値

																			//頂点バッファをロック
		m_pMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

														//最大値を比較
			if (m_pVtxMax[nCntModel].x < vtx.x)
			{
				m_pVtxMax[nCntModel].x = vtx.x;
			}
			if (m_pVtxMax[nCntModel].y < vtx.y)
			{
				m_pVtxMax[nCntModel].y = vtx.y;
			}
			if (m_pVtxMax[nCntModel].z < vtx.z)
			{
				m_pVtxMax[nCntModel].z = vtx.z;
			}

			//最小値を比較
			if (m_pVtxMin[nCntModel].x > vtx.x)
			{
				m_pVtxMin[nCntModel].x = vtx.x;
			}
			if (m_pVtxMin[nCntModel].y > vtx.y)
			{
				m_pVtxMin[nCntModel].y = vtx.y;
			}
			if (m_pVtxMin[nCntModel].z > vtx.z)
			{
				m_pVtxMin[nCntModel].z = vtx.z;
			}
			//サイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		m_pMesh[nCntModel]->UnlockVertexBuffer();
	}
}

//=============================================================================
// モデルの破棄
//=============================================================================
void CTerritory::UnloadModel(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		delete[] m_pMesh;
		m_pMesh = NULL;
	}

	//バッファの破棄
	if (m_pBuffMat != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	//マテリアルの破棄
	if (m_pNumMat != NULL)
	{
		delete m_pNumMat;
		m_pNumMat = NULL;
	}

	//最大値の破棄
	if (m_pVtxMax != NULL)
	{
		delete m_pVtxMax;
		m_pVtxMax = NULL;
	}

	//最小値の破棄
	if (m_pVtxMin != NULL)
	{
		delete m_pVtxMin;
		m_pVtxMin = NULL;
	}
}

//=============================================================================
//　オブジェクト検索
//=============================================================================
CTerritory * CTerritory::RequestPointer(int nCountObj)
{
	//敵のオブジェクトポインタの取得
	CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //オブジェクトポインタの先頭を取得			
	CScene * pScene = pSceneTop;						 //トップの位置を保存する
	CTerritory * m_pTerritory = NULL;
	int nNumObject = 0;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();					//次のオブジェクトのポインタを保存する

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			if (nNumObject == nCountObj)
			{//番号が一致したらポインタを保存
				m_pTerritory = (CTerritory*)pScene;
				break;
			}

			nNumObject++;	//番号の加算
		}

		pScene = pSceneNext;		//ポインタを進める
	}

	return m_pTerritory;
}

//=============================================================================
// テキスト保存処理
//=============================================================================
void CTerritory::Save(HWND hWnd)
{
	//変数宣言
	FILE * pFile = NULL;

	//テクストの作成
	pFile = fopen(SaveTextName, "w");

	//書き出し処理
	if (pFile != NULL)
	{
		//敵のオブジェクトポインタの取得
		CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //オブジェクトポインタの先頭を取得			
		CScene * pScene = pSceneTop;						 //トップの位置を保存する

		while (pScene != NULL)
		{
			CScene * pSceneNext = pScene->GetpNext();					//次のオブジェクトのポインタを保存する

			if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
			{
				if (pScene->GetDeth() == false)
				{
					CTerritory * pObject = (CTerritory*)pScene;						//敵クラスにキャストする
					int ModelType = pObject->GetModelType();
					int nTexNum = pObject->GetTex();
					D3DXVECTOR3 Pos = pObject->GetPos();
					D3DXVECTOR3 Rot = pObject->GetRot();
					D3DXVECTOR3 Scale = pObject->GetScale();
					int nPoint = pObject->GetPoint();

					fprintf(pFile, "SET\n");
					fprintf(pFile, "\tMODELTYPE = %d\n", ModelType);							//モデルのタイプ
					fprintf(pFile, "\tTEX = %d\n", nTexNum);									//テクスチャ番号
					fprintf(pFile, "\tPOS = %.1f %.1f %.1f\n", Pos.x, Pos.y, Pos.z);			//位置
					fprintf(pFile, "\tROT = %.1f %.1f %.1f\n", Rot.x, Rot.y, Rot.z);			//向き
					fprintf(pFile, "\tSCALE = %.1f %.1f %.1f\n", Scale.x, Scale.y, Scale.z);	//スケール
					fprintf(pFile, "\tPOINT = %d\n", nPoint);									//ポイント
					fprintf(pFile, "END_SET\n");
					fprintf(pFile, "\n");
				}
			}

			pScene = pSceneNext;		//ポインタを進める
		}

		//書き込みを終了する
		fclose(pFile);

		//メッセージボックスの表示
		MessageBox(hWnd, "セーブが完了しました", "処理完了通知", MB_OK);
	}
}