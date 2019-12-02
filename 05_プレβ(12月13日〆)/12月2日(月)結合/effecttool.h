//=============================================================================
//
// エフェクトツール処理 [Effecttool.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _EFFECTTOOL_H_
#define _EFFECTTOOL_H_

#include "main.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EFFECT	(19)		//エフェクトの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CSaveText;

//=============================================================================
// エフェクトクラス
//=============================================================================
class CEffectTool
{
public:
	typedef struct
	{
		bool					bCreateEffect;	//エフェクトを生成
		bool					bLoop;			//ループするかどう
		D3DXVECTOR3				pos;			//位置
		D3DXVECTOR3				size;			//サイズ
		int						nTexNum;		//テクスチャ番号
		CParticle::DRAWTYPE		DrawType;		//描画タイプ
		CParticle::TYPE			Type;			//パーティクルのタ
		D3DXCOLOR				Color;			//色
		bool					bZbuffur;		//Zバッファのオンオフ
		int						nLife;			//ライフ
		int						nInterbalTime;	//生成時間
		float					fChangeCol_a;	//透明度の変化量
		bool					bUseGravity;	//重力
		float					fGravity;		//重力の設定
		int						nNumCreate;		//生成量
		float					fSpeed;			//スピードの設定
		int						nMinHight;		//高さの最小値
		int						nMaxHight;		//高さの最大数
		int						nCountTime;		//時間のカウンター
	}Effect_Info;

	typedef struct
	{
		Effect_Info * EffectInfo;		//エフェクト情報
	}Effect;

	CEffectTool();
	~CEffectTool();
	static CEffectTool * Create(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void LoadEffect(void);
	static void UnloadEffect(void);
	void CreateEffect(int nCntParticle, int nNumEffect, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nPriority);
	void SetBuffZ(bool bBuffZ) { m_bBuffZ = bBuffZ; };					//Zバッファの有効設定

protected:
	static Effect *		m_pEffect;		//エフェクト情報

private:
	static int			m_nMaxEffect;	//エフェクトの最大数
	bool				m_bBuffZ;		//Zバッファのオンオフ
};
#endif