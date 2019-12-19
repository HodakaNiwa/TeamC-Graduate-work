//=============================================================================
//
// エフェクトツール処理 [Effecttool.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "effecttool.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "LoadText.h"
#include "game.h"
#include "tutorial.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define FIRST_POS		(D3DXVECTOR3(0.0f, 10.0f, 0.0f))		//初期位置

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CEffectTool::Effect * CEffectTool::m_pEffect = NULL;
int CEffectTool::m_nMaxEffect = 0;

//=============================================================================
// 生成処理
//=============================================================================
CEffectTool * CEffectTool::Create(void)
{//インスタンスの生成

	CEffectTool * pEffect;
	pEffect = new CEffectTool;

	//初期化処理
	if (pEffect != NULL)
	{
		pEffect->Init();
	}

	return pEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEffectTool::CEffectTool(){}

//=============================================================================
// デストラクタ
//=============================================================================
CEffectTool::~CEffectTool(){}

//=============================================================================
// エフェクトの読み込み
//=============================================================================
void CEffectTool::LoadEffect(void)
{
	//エフェクト情報の生成・初期化
	if (m_pEffect == NULL)
	{
		//読み込んだ情報を取得する
		CLoadTextEffect * pLoadTextEffect = {};

		CManager::MODE mode = CManager::GetMode();
		if (mode == CManager::MODE_GAME)
		{
			pLoadTextEffect = CGame::GetEffectLoad();
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			pLoadTextEffect = CManager::GetTutorial()->GetEffectLoad();
		}
		m_nMaxEffect = pLoadTextEffect->GetMaxEffect();							//エフェクトの最大数を取得する
		Effect * pEffect = pLoadTextEffect->GetEffect();						//エフェクト情報ポインタの取得

																				//メモリの動的確保
		m_pEffect = new Effect[m_nMaxEffect];

		if (m_pEffect != NULL)
		{
			for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
			{
				//エフェクト情報の初期化
				if (m_pEffect[nCntEffect].EffectInfo != NULL)
				{
					m_pEffect[nCntEffect].EffectInfo = NULL;
				}

				//エフェクト情報のメモリを動的確保
				if (m_pEffect[nCntEffect].EffectInfo == NULL)
				{
					m_pEffect[nCntEffect].EffectInfo = new Effect_Info[MAX_EFFECT];
				}

				//読み込んだ情報の取得
				if (m_pEffect[nCntEffect].EffectInfo != NULL)
				{
					for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
					{
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect = pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect;		//エフェクトを使用ているかどうか
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop = pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop;						//ループするかどうか
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].pos = FIRST_POS;																	//位置
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].size = pEffect[nCntEffect].EffectInfo[nCntParticle].size;						//サイズ
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum = pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum;					//テクスチャ番号
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType = pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType;				//描画状態
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].Type = pEffect[nCntEffect].EffectInfo[nCntParticle].Type;						//更新状態
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].Color = pEffect[nCntEffect].EffectInfo[nCntParticle].Color;						//色
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur = pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur;				//Zバッファの状態
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nLife = pEffect[nCntEffect].EffectInfo[nCntParticle].nLife;						//表示時間
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a = pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a;		//透明度の変化量
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity = pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity;			//重力を使用するかどうか
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime = pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime;		//生成時間の間隔
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate = pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate;			//一度の生成量
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed = pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed;					//スピード
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight = pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight;				//高さの最小値
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight = pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight;				//高さの最大値
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity = pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity;				//重力の重さ
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nCountTime = 0;																	//時間のカウンター
					}
				}
			}
		}
	}
}

//=============================================================================
// エフェクトの破棄
//=============================================================================
void CEffectTool::UnloadEffect(void)
{
	//エフェクト情報の開放
	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{
		if (m_pEffect[nCntEffect].EffectInfo != NULL)
		{
			delete[] m_pEffect[nCntEffect].EffectInfo;
			m_pEffect[nCntEffect].EffectInfo = NULL;
		}
	}
	if (m_pEffect != NULL)
	{
		delete[] m_pEffect;
		m_pEffect = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
void CEffectTool::Init(void)
{
	m_bBuffZ = true;								//Zバッファの有効状態
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffectTool::Uninit(void){}

//=============================================================================
// 更新処理
//=============================================================================
void CEffectTool::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CEffectTool::Draw(void){}

//=============================================================================
// エフェクトの生成
//=============================================================================
void CEffectTool::CreateEffect(int nCntParticle, int nNumEffect, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nPriority)
{
	//変数宣言
	bool bBuffZ;

	for (int nCnt = 0; nCnt < m_pEffect[nNumEffect].EffectInfo[nCntParticle].nNumCreate; nCnt++)
	{
		//Zバッファの有効設定
		if (m_bBuffZ != m_pEffect[nNumEffect].EffectInfo[nCntParticle].bZbuffur)
		{
			bBuffZ = m_bBuffZ;
		}
		else
		{
			bBuffZ = m_pEffect[nNumEffect].EffectInfo[nCntParticle].bZbuffur;
		}

		//パーティクルの生成
		CParticle * pParticle = CParticle::Create(pos,														//位置情報
												  size,														//サイズ
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nTexNum,	//テクスチャ番号
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].DrawType,	//描画状態
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].Type,		//処理タイプ
												  col,														//色
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nLife,		//表示時間
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].fSpeed,	//スピード
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nMinHight,	//高さの最小値
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nMaxHight,	//高さの最大値
												  bBuffZ,													//Zバッファの状態
												  nPriority);												//優先順位

		if (pParticle != NULL)
		{
			if (m_pEffect[nNumEffect].EffectInfo->Type == CParticle::TYPE_SPEW || m_pEffect[nNumEffect].EffectInfo->Type == CParticle::TYPE_CICLE)
			{
				pParticle->SetChangeCol_a(m_pEffect[nNumEffect].EffectInfo[nCntParticle].fChangeCol_a);				//透明度の変化量の設定
				pParticle->SetUseGravity(m_pEffect[nNumEffect].EffectInfo[nCntParticle].bUseGravity);				//重力の状態を設定する
				pParticle->SetGravity(m_pEffect[nNumEffect].EffectInfo[nCntParticle].fGravity);						//重力の設定
			}
		}
	}
}