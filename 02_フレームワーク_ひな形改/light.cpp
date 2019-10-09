//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 佐藤安純 Sato_Asumi 
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TIME	(60 * 2)

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight(){}
	
//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight(){}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	//変数の初期化
	m_bSwich = true;
	m_bSetLight = false;
	m_type = TYPE_NORMAL;
	m_nNumLight = 0;
	m_nNumDir = 0;
	m_nCntTime = 0;
	m_Col = D3DXCOLOR(0.7f, 0.75f, 1.0f, 1.0f);
	m_AimCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	m_light[0].Diffuse = D3DXCOLOR(0.7f, 0.75f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.30f, 0.35f, 0.40f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.30f, 0.35f, 0.40f, 1.0f);

	// ライトの方向の設定
	m_vecDir[0] = D3DXVECTOR3(0.2f, -0.74f, 0.64f);		//光を当たる方向
	m_vecDir[1] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);	//光を当たる方向
	m_vecDir[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);	//光を当たる方向

	SetLight();		//ライトの設定
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
	//変数宣言
	bool bSet = false;

	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	if (pInputKeyboard->GetKeyboardTrigger(DIK_7) == true)
	{
		m_bSwich = m_bSwich ? false : true;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0) == true)
	{
		// ライトの拡散光を設定
		m_light[0].Diffuse = D3DXCOLOR(0.90f, 0.95f, 1.0f, 1.0f);
		m_light[1].Diffuse = D3DXCOLOR(0.40f, 0.40f, 0.40f, 1.0f);
		m_light[2].Diffuse = D3DXCOLOR(0.40f, 0.40f, 0.40f, 1.0f);
		SetLight();
	}

	if (m_bSwich == true)
	{//ライトの更新
		ChangeNumber(&m_nNumLight, pInputKeyboard);				//数字の切り替え
		bSet = SetValue(&m_light[m_nNumLight].Diffuse.r,		//数値の設定
				 &m_light[m_nNumLight].Diffuse.g,
				 &m_light[m_nNumLight].Diffuse.b,
				 pInputKeyboard);
	}
	else
	{//ライトの向きの設定
		ChangeNumber(&m_nNumDir, pInputKeyboard);		//数字の切り替え
		bSet = SetValue(&m_vecDir[m_nNumDir].x,				//数値の設定
				 &m_vecDir[m_nNumDir].y,
				 &m_vecDir[m_nNumDir].z,
				 pInputKeyboard);
	}

	if (bSet == true) { SetLight(); }						//ライトの設定
}

//=============================================================================
// ライトの設定
//=============================================================================
void CLight::SetLight(void)
{
	//ライトを設定する
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		D3DXVec3Normalize(&m_vecDir[nCntLight], &m_vecDir[nCntLight]);		//正規化する
		m_light[nCntLight].Direction = m_vecDir[nCntLight];				//決めた大きさを代入する

																		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}


//=============================================================================
// 色の設定
//=============================================================================
void  CLight::SetColor(void)
{
	//色の割当て
	m_light[0].Diffuse = D3DXCOLOR(m_AimCol.r, m_AimCol.g, m_AimCol.b, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);

	SetLight();	//ライトの設定
}

//=============================================================================
// 数字の切り替え
//=============================================================================
void CLight::ChangeNumber(int * nNum, CInputKeyboard * pInputKeyboard)
{
	//数字の切り替え
	if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPADPLUS) == true)
	{
		*nNum = (*nNum + (MAXLIGHT - 1)) % MAXLIGHT;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPADMINUS) == true)
	{
		*nNum = (*nNum + 1) % MAXLIGHT;
	}
}

//=============================================================================
// 数値の切り替え
//=============================================================================
bool CLight::SetValue(float * fValue_X, float * fValue_Y, float * fValue_Z, CInputKeyboard * pInputKeyboard)
{
	//変数宣言
	bool bSet = false;

	//fValue_Xの数値の切り替え
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD7) == true)
	{
		*fValue_X += 0.01f;
		if (*fValue_X > 1.0f) { *fValue_X = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD4) == true)
	{
		*fValue_X -= 0.01f;
		if (*fValue_X < 0.0f) { *fValue_X = 0.0f; }
		bSet = true;
	}

	//fValue_Yの数値の切り替え
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD8) == true)
	{
		*fValue_Y += 0.01f;
		if (*fValue_Y > 1.0f) { *fValue_Y = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD5) == true)
	{
		*fValue_Y -= 0.01f;
		if (*fValue_Y < 0.0f) { *fValue_Y = 0.0f; }
		bSet = true;
	}

	//fValue_Zの数値の切り替え
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD9) == true)
	{
		*fValue_Z += 0.01f;
		if (*fValue_Z > 1.0f) { *fValue_Z = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD6) == true)
	{
		*fValue_Z -= 0.01f;
		if (*fValue_Z < 0.0f) { *fValue_Z = 0.0f; }
		bSet = true;
	}

	return bSet;
}

//=============================================================================
// ライトの明るさを調整
//=============================================================================
void CLight::UpdateLight(void){}