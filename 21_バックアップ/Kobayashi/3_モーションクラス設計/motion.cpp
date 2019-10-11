//=============================================================================
//
// モーションの処理 [motion.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "model.h"
#include "LoadText.h"
//#include "sound.h"
#include "character.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define NUTORAL_MOTION	(0)		//ニュートラルモーション
#define KICK_MOTION		(5)		//キックモーションナンバー
#define KICK_KEY		(5)		//対象キー番号

//=============================================================================
// 生成処理
//=============================================================================
CMotion * CMotion::Create(void)
{
	//インスタンスの生成
	CMotion * pMotion;
	pMotion = new CMotion;

	//初期化処理
	pMotion->Init();

	return pMotion;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	m_pModel = NULL;								//モデル情報
	m_nMaxModel = 0;								//モデルの最大数
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_nMaxMotion = 0;								//モーションの最大数
	m_pMotionInfo = NULL;							//モーション情報
	m_nNumMotion = 0;								//ニュートラルモーション
	m_nKey = 0;										//現在のキー
	m_nCountMotion = 0;								//モーションカウンター
	m_nNextNumMotion = -1;							//次のモーション番号
	m_bBlend = false;								//次のモーションの状態設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMotion::Init(void)
{
	//変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		//移動
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			//向き

	//ニュートラルモーションの最初の向きを設定する
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		m_pModel[nCntModel]->SetRot(D3DXVECTOR3(m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotX, m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotY, m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotZ));
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMotion::Uninit(void)
{
	//モデルデータの破棄
	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}

	//モーションデータの破棄
	if (m_pMotionInfo != NULL)
	{
		m_pMotionInfo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
int CMotion::Update(void)
{
	//MotionSetLode();

	//変数宣言
	KEY * pKey, *pNextKey;		//現在のキーと次のキー
	D3DXVECTOR3 ModelRot;		//モデルの向き

	//カウンターを進める
	m_nCountMotion++;

	//キックモーションなら音を出す
	if ((m_nNumMotion == KICK_MOTION) && 
		(m_nKey == KICK_KEY) && 
		m_nCountMotion == 1)
	{
		//CSound * pSound = CManager::GetSound();
		//pSound->PlaySoundA(CSound::SOUND_LABEL_SE_KICK);
	}

	//フレーム判定
	if (m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].nFrame < m_nCountMotion)
	{
		m_nCountMotion = 0;	//カウンターをリセットする

		if (m_bBlend == true)
		{//モーションブレンドが有効だったら
			m_nKey = 0;										//キーを初期化する
			m_nNumMotion = m_nNextNumMotion;				//現在のモーションを次のモーションに入れ替える
			m_bBlend = false;								//モーションブレンドを無効にする
		}
		else
		{//モーションブレンドが無効だったら
			m_nKey = (m_nKey + 1) % m_pMotionInfo[m_nNumMotion].nNumKey;			//キーを進める
		}
	}

	//モーションのキー設定・差分を求める
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		pKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].aKey[nCntModel];		//現在のキーを設定する
		pNextKey = SetKey(nCntModel);	//次のキーを設定する
		SetMotionDifference(pNextKey, pKey, nCntModel);
	}

	return m_nNumMotion;
}

//=============================================================================
// 描画処理
//=============================================================================
void CMotion::Draw(void){}

//=============================================================================
// キー設定	(※CMotion更新で使っている)
//=============================================================================
CMotion::KEY * CMotion::SetKey(int nCntModel)
{
	//変数宣言
	KEY * pNextKey;

	if (m_nKey != m_pMotionInfo[m_nNumMotion].nNumKey - 1)
	{//最後のキーじゃなかったら

		if (m_bBlend == false)
		{//現在のモーションの次のキーを設定する
			pNextKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey + 1].aKey[nCntModel];
		}
		else
		{//次のモーションのキーに設定する
			pNextKey = &m_pMotionInfo[m_nNextNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//最初のキーに設定する
		}
	}
	else
	{//最後のキーだったら
		if (m_pMotionInfo[m_nNumMotion].m_bLoop == true)
		{
			pNextKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//最初のキーに設定する
		}
		else
		{
			m_nNextNumMotion = NUTORAL_MOTION;	//ニュートラルモーションにする
			pNextKey = &m_pMotionInfo[m_nNextNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//最初のキーに設定する
			m_bBlend = true;
		}
	}

	return pNextKey;
}

//=============================================================================
// モーションの差分を求める	(※CMotion更新で使っている)
//=============================================================================
void CMotion::SetMotionDifference(KEY * pNextKey, KEY * pKey, int nCntModel)
{
	//変数宣言
	float fRateMotion;			//相対値
	KEY * pDiffKey = new KEY;	//差分

	fRateMotion = (float)m_nCountMotion / (float)m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].nFrame;	//相対値を求める

	pDiffKey->fPosX = pNextKey->fPosX - pKey->fPosX;		//現在のキーと次のキーの差分を求める
	pDiffKey->fPosY = pNextKey->fPosY - pKey->fPosY;
	pDiffKey->fPosZ = pNextKey->fPosZ - pKey->fPosZ;
	pDiffKey->fRotX = pNextKey->fRotX - pKey->fRotX;
	pDiffKey->fRotY = pNextKey->fRotY - pKey->fRotY;
	pDiffKey->fRotZ = pNextKey->fRotZ - pKey->fRotZ;

	//変化量を求める
	D3DXVECTOR3 ModelPos;
	D3DXVECTOR3 ModelRot;
	ModelPos.x = pKey->fPosX + (pDiffKey->fPosX * fRateMotion);
	ModelPos.y = pKey->fPosY + (pDiffKey->fPosY * fRateMotion);
	ModelPos.z = pKey->fPosZ + (pDiffKey->fPosZ * fRateMotion);
	ModelRot.x = pKey->fRotX + (pDiffKey->fRotX * fRateMotion);
	ModelRot.y = pKey->fRotY + (pDiffKey->fRotY * fRateMotion);
	ModelRot.z = pKey->fRotZ + (pDiffKey->fRotZ * fRateMotion);

	//計算結果をモデルに渡す
	m_pModel[nCntModel]->SetRot(ModelRot);
	if (!m_bBlend) { m_pModel[nCntModel]->AddPos(ModelPos); }

	//原点の位置を変更する
	if (nCntModel == 0)
	{
		m_move.x = ModelPos.x;
		m_move.y = ModelPos.y;
		m_move.z = ModelPos.z;
	}

	//キー情報を破棄する
	delete pDiffKey;
	pDiffKey = NULL;
}

//=============================================================================
// モーションの設定
//=============================================================================
void CMotion::SetNumMotion(int NumMotion)
{
	m_nNumMotion = NumMotion;
	m_nKey = 0;
}
