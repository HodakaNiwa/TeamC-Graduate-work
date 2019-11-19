//=============================================================================
//
// 地面マネージャー処理 [fieldmanager.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _FIELDMANAGER_H_
#define _FIELDMANAGER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_FIELD	(32 * 32)
#define LINE		(32)
#define BLOCK_SIZE	(D3DXVECTOR3(62.5f, 0.0f, 62.5f))
#define FIELD_TEX	(4)

//=============================================================================
// 前方宣言
//=============================================================================
class CField;

//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class CFieldManager
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_XTURN,
		STATE_RANDOM,
		STATE_WAVE,
		STATE_COUNTDOWN,
		STATE_TIME_2,
		STATE_TIME_1,
		STATE_TIME_30,
		STATE_MAX,
	}STATE;

	CFieldManager();
	~CFieldManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetState(STATE state);
	
private:
	void UpdateXTurn(void);
	void UpdateRamdom(void);
	void ChangeField(bool bChangeMain);
	void UpdateWave(void);
	void UpdateTime(void);
	void UpdateCountDown(void);

	CField *			m_pField[MAX_FIELD];		//地面ポインタ
	CField *			m_pFieldGraund[MAX_FIELD];	//地面ポインタ
	CField *			m_pCountDownField;			//カウントダウン用変数
	CField *			m_pMainField;				//メインの地面
	LPDIRECT3DTEXTURE9	m_pTexture[FIELD_TEX];		// テクスチャへのポインタ
	STATE				m_state;					// 状態
	int					m_nCountTime;				// 時間の加算
	int					m_nCountY;					// Y軸のカウンター
	int					m_nCountX;					// X軸のカウンター
	int					m_nCountY2;					// Y軸のカウンター
	int					m_nCountX2;					// X軸のカウンター
	int					m_nCountY3;					// Y軸のカウンター
	int					m_nCountX3;					// X軸のカウンター
	int					m_nCountY4;					// Y軸のカウンター
	int					m_nCountX4;					// X軸のカウンター
	float				m_fCol_a;					// 色のα値
};
#endif
