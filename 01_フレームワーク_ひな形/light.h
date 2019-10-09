//=============================================================================
//
// ライト処理 [light.h]
// Author : 佐藤安純 Sato_Asumi 
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAXLIGHT	(3)		//ライトの最大数
#define MAX_COL		(3)		//RGB

//=============================================================================
// 前方宣言
//=============================================================================
class CInputKeyboard;

//*****************************************************************************
// ライトクラス
//*****************************************************************************
class CLight
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_RED,
		TYPE_GREEN,
		TYPE_BLUE,
	}TYPE;

	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);
	D3DLIGHT9 m_light[MAXLIGHT];	//ライトの設定
	D3DXVECTOR3 GetvecDir(int nNumLight) { return m_vecDir[nNumLight]; };	//ライトのベクトルを取得

private:
	void UpdateLight(void);
	void ChangeNumber(int * nNum, CInputKeyboard * pInputKeyboard);
	bool SetValue(float * fValue_X, float * fValue_Y, float * fValue_Z, CInputKeyboard * pInputKeyboard);
	void SetLight(void);
	void SetColor(void);
	D3DXVECTOR3 m_vecDir[MAXLIGHT];
	TYPE m_type;						//状態設定
	bool m_bSwich;						//ライトの切り替え
	bool m_bSetLight;					//ライトの設定したかどうか
	int m_nNumLight;					//ライトの番号
	int m_nNumDir;						//方向の番号
	int m_nCntTime;						//時間のカウンター
	D3DXCOLOR m_Col;					//現在の色
	D3DXCOLOR m_AimCol;					//目的の色
};
#endif
