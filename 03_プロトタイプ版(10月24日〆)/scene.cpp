//=============================================================================
//
// オブジェクトの処理 [renderer.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "scene.h"
#include "input.h"
#include "manager.h"
#include "collision.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScene * CScene::m_pTop[MAX_LAYER] = {};
CScene * CScene::m_pCur[MAX_LAYER] = {};
int CScene::m_nNumAll = 0;
bool CScene::m_bPause = false;
int CScene::m_nCountDraw = 0;

//デバック用変数
int CScene::m_anNumOBJ[MAX_LAYER] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE ObjType)
{
	//当たり判定ポインタ
	m_pColliderManger = NULL;

	//死亡フラグ
	m_bDeth = false;

	//描画するかどうか
	m_bDraw = false;

	//初期位置の保存
	if (m_pTop[nPriority] == NULL)
	{
		m_pTop[nPriority] = this;		//初めの位置を保存する
	}

	//前のオブジェクト（位置）のポインタを保存する
	if (m_pCur[nPriority] != NULL)
	{
		m_pPrev = m_pCur[nPriority];
	}
	else
	{
		m_pPrev = NULL;
	}

	//最後尾のポインタを保存する
	m_pCur[nPriority] = this;

	//最後尾の次のオブジェクトをNULL状態にする
	m_pNext = NULL;			

	//前回のオブジェクトのm_pNextに現在位置（最後尾）のポインタを渡す
	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pCur[nPriority];
	}

	m_nID = m_nNumAll;			//番号の保存
	m_nNumAll++;				//総数の加算
	m_anNumOBJ[nPriority]++;	//各レイヤーのオブジェクト数の加算
	m_ObjType = ObjType;		//オブジェクトタイプの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
//	全メモリの開放
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//トップの位置を保存する
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				pScene->Uninit();
				pScene = pScene->m_pNext;	//スタート位置を次のオブジェクトに進める
			}

			//トップの位置を保存する
			pScene = m_pTop[nCntLayer];

			//死亡フラグのみ破棄する
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する

				if (pScene->m_bDeth == true)
				{
					pScene->Deth(nCntLayer);	//消す
				}
				pScene = pSceneNext;			//ポインタを進める
			}

			m_pTop[nCntLayer] = NULL;	//始まりの位置を初期化する
			m_pCur[nCntLayer] = NULL;	//最後尾を初期化する
		}
	}

	m_nNumAll = 0;	//値の初期化

	//コライダーの全破棄
	CCollider::ReleaseAll();
}

//=============================================================================
//	フェード以外のメモリを開放する
//=============================================================================
void CScene::ReleaseFade(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//トップの位置を保存する
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する
				if (pScene->m_ObjType != OBJTYPE_FADE)
				{
					pScene->Uninit();
				}
				pScene = pSceneNext;					//ポインタを進める
			}
		}
	}

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//トップの位置を保存する
			CScene * pScene = m_pTop[nCntLayer];

			//死亡フラグのみ破棄する
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する

				if (pScene->m_bDeth == true)
				{
					if (pScene->m_ObjType != OBJTYPE_FADE)
					{
						pScene->Deth(nCntLayer);	//消す
					}
					else
					{
						pScene->m_bDeth = false;	//死なない状態にする
					}
				}
				pScene = pSceneNext;				//ポインタを進める
			}
		}
	}
}

//=============================================================================
//	メモリの開放
//=============================================================================
void CScene::Release(void)
{	
	m_bDeth = true;
}

//=============================================================================
//	全ての更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	//デバック表示の取得
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	//変数宣言
	CScene * pScene = NULL;

	if (m_bPause == false)
	{
		for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
		{
			if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
			{
				//トップの位置を保存する
				CScene * pScene = m_pTop[nCntLayer];

				while (pScene != NULL)
				{
					CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する
					pScene->Update();
					pScene = pSceneNext;					//ポインタを進める
				}
			}
		}
	}

	//コライダーの更新処理
	CCollider::UpdateAll();

	//衝突チェック
	CheckCollisionAll();

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//トップの位置を保存する
			pScene = m_pTop[nCntLayer];

			//死亡フラグのみ破棄する
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する

				if (pScene->m_bDeth == true)
				{
					pScene->Deth(nCntLayer);	//消す
				}
				pScene = pSceneNext;					//ポインタを進める
			}
		}
	}

	m_nCountDraw = 0;
}

//=============================================================================
//	全ての描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//トップの位置を保存する
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する

				if (pScene->m_bDraw == false)
				{
					pScene->Draw();
					m_nCountDraw++;
				}

				pScene = pSceneNext;					//ポインタを進める
			}
		}
	}

	//コライダーの描画処理
	CCollider::DrawAll();
}

//=============================================================================
//	死亡フラグの設定
//=============================================================================
void CScene::Deth(int nCntLayer)
{
	//コライダーのあたり判定管轄クラスの破棄
	if (m_pColliderManger != NULL)
	{
		m_pColliderManger->Uninit();
		m_pColliderManger = NULL;
	}

	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pNext;	//前のオブジェクトに現在位置のm_pNextを代入する
	}
	else
	{
		m_pTop[nCntLayer] = m_pNext;			//スタート位置を次のオブジェクトに移動する
	}

	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		m_pCur[nCntLayer] = m_pPrev;			//スタート位置を次のオブジェクトに移動する
	}

	m_anNumOBJ[nCntLayer]--;	//各レイヤーのオブジェクトの総数を減らす
	m_nNumAll--;				//オブジェクトの総数を減らす
	delete this;	
}

//=============================================================================
// コライダーの衝突判定チェック
//=============================================================================
void CScene::CheckCollisionAll(void)
{
	//デバック表示の取得
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	//変数宣言
	CScene * pScene = NULL;

	if (m_bPause == false)
	{
		for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
		{
			if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
			{
				//トップの位置を保存する
				CScene * pScene = m_pTop[nCntLayer];

				while (pScene != NULL)
				{
					CScene * pSceneNext = pScene->m_pNext;	//次のオブジェクトのポインタを保存する
					pScene->Collision();
					pScene = pSceneNext;					//ポインタを進める
				}
			}
		}
	}

	CCollider::DeathCheck();
}

//=============================================================================
// コリジョン
//=============================================================================
void CScene::Collision(void){}