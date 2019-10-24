//=============================================================================
//
// エフェクト処理 [Effect.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "loadEffect.h"
#include "manager.h"
#include "renderer.h"
#include "particle.h"
#include "input.h"
#include "LoadText.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TIME		(10)									//インターバルタイム
#define LIFE			(50)									//ライフの設定
#define FIRST_POS		(D3DXVECTOR3(0.0f, 10.0f, 0.0f))		//初期位置
#define FIRST_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		//初期サイズ
#define BOUND_HIGHT		(4.0f)									//高さの設定
#define MAX_EXPLOTION	(2)										//爆発エフェクト最大数
#define MAX_STAR		(13)									//星のエフェクトの最大数
#define MAX_CIRCLE		(10)									//円エフェクトの最大数
#define EFFECT_NUM		(4)										//エフェクトの番号
#define EFFECT_NUM2		(3)										//エフェクトの番号
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))			

//=============================================================================
// 生成処理
//=============================================================================
CLoadEffect * CLoadEffect::Create(int nNumEffect, D3DXVECTOR3 pos, int nPriority)
{
	CLoadEffect * pLoadEffect;
	pLoadEffect = new CLoadEffect;

	//初期化処理
	if (pLoadEffect != NULL)
	{
		pLoadEffect->Init();
		pLoadEffect->SetNumEffect(nNumEffect);	//エフェクトナンバーの設定
		pLoadEffect->SetPos(pos);				//位置の設定
		pLoadEffect->SetPriority(nPriority);	//優先順位の設定
	}

	return pLoadEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadEffect::CLoadEffect()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadEffect::~CLoadEffect()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CLoadEffect::Init(void)
{
	CEffectTool::Init();

	//初期化処理
	m_pos = VECTOR_ZERO;			//位置
	m_size = VECTOR_ZERO;			//サイズ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_bRemake = false;								//再設定するかどうか
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadEffect::Uninit(void)
{
	CEffectTool::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLoadEffect::Update(void)
{
	CEffectTool::Update();

	//パーティクルの生成処理
	for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
	{
		//カウンターの加算
		m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime++;

		if ((m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].bCreateEffect == true) &&
			(m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].bLoop == true))				//生成するかどうか
		{
			if (m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime > m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nInterbalTime)
			{
				m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime = 0;				//カウンターの初期化

				if (m_bRemake == true)
				{
					CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_size, m_color, m_nPriority);	//エフェクトの生成
				}
				else if ((m_bRemake == false) && (m_nNumEffect == EFFECT_NUM))
				{
					CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size, m_color, m_nPriority);	//エフェクトの生成
				}
				else
				{
					CreateEffect(nCntParticle,												//エフェクトの生成
						m_nNumEffect,
						m_pos,
						m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size,
						m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].Color,
						m_nPriority);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLoadEffect::Draw(void)
{
	CEffectTool::Draw();
}

//=============================================================================
// 一度だけエフェクトを生成
//=============================================================================
void CLoadEffect::OnTimeEffect(void)
{

	//パーティクルの生成処理
	for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
	{
		if (m_bRemake == true)
		{
			CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_size, m_color, m_nPriority);	//エフェクトの生成
		}
		else if ((m_bRemake == false) && (m_nNumEffect == EFFECT_NUM))
		{
			CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size, m_color, m_nPriority);	//エフェクトの生成
		}
		else
		{
			CreateEffect(nCntParticle,												//エフェクトの生成
				m_nNumEffect,
				m_pos,
				m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size,
				m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].Color,
				m_nPriority);
		}
	}
}