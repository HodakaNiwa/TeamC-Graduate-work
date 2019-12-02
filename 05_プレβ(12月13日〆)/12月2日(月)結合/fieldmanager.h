//=============================================================================
//
// 地面マネージャー処理 [fieldmanager.h]
// Author : 佐藤安純 Sato_Asumi & Hodaka Niwa
//
//=============================================================================
#ifndef _FIELDMANAGER_H_
#define _FIELDMANAGER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_FIELD		(32 * 32)
#define LINE			(32)
#define BLOCK_SIZE		(D3DXVECTOR3(62.5f, 0.0f, 62.5f))
#define FIELD_TEX		(4)

//=============================================================================
// 前方宣言
//=============================================================================
class CField;
class CFieldShader;
class CFieldRender;

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

	// 地面描画用列挙型
	typedef enum
	{
		FIELD_TYPE_NONE = 0,
		FIELD_TYPE_GROUND,
		FIELD_TYPE_MAIN,
		FIELD_TYPE_COUNTDOWN,
		FIELD_TYPE_MAX
	}FIELD_TYPE;

	CFieldManager();
	~CFieldManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetState(STATE state);
	CFieldRender *GetFieldRender(const int nIdx) { return m_pFieldRender[nIdx]; }

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

	// 地面描画用変数
	CFieldShader *m_pFieldShader;					// 地面描画用のシェーダー
	CFieldRender *m_pFieldRender[FIELD_TYPE_MAX];	// 地面描画クラスへのポインタ

	// 地面描画用関数
	void LoadShader(void);
	void MakeFieldRender(void);
	void ReleaseFieldShader(void);
	void ReleaseFieldRender(void);
	void SetFieldData_None(void);
	void SetFieldData_Ground(void);
	void SetFieldData_Main(void);
	void SetFieldData_CountDown(void);

	void SetFieldVertex_None(void);
	void SetFieldMtxWorld_None(void);
	void SetFieldColor_None(void);

	void SetFieldVertex_Ground(void);
	void SetFieldMtxWorld_Ground(void);
	void SetFieldColor_Ground(void);

	void SetFieldVertex_Main(void);
	void SetFieldMtxWorld_Main(void);
	void SetFieldColor_Main(void);

	void SetFieldVertex_CountDown(void);
	void SetFieldMtxWorld_CountDown(void);
	void SetFieldColor_CountDown(void);
};
#endif
