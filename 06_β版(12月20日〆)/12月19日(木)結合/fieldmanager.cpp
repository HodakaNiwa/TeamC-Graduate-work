//=============================================================================
//
// 地面マネージャー処理 [fieldmanager.cpp]
// Author : 佐藤安純 Sato_Asumi & Hodaka Niwa
//
//=============================================================================
#include "fieldmanager.h"
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "fieldRender.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAIN_SIZE						(D3DXVECTOR3(2000.0f, 0.0f, 2000.0f))
#define TEX_NAME						("data/TEXTURE/Field/Stage000.jpg")
#define TEX_NAME2						("data/TEXTURE/Field/Stage_blue.png")
#define TEX_NAME3						("data/TEXTURE/Field/Stage_Red.jpg")
#define TEX_NAME4						("data/TEXTURE/Field/Stage_Yellow.jpg")
#define START_X							(-2000.0f)
#define START_Y							(-2000.0f)
#define TIME_X							(30)
#define TIME_RANDUM						(20)
#define TIME_RANDUM2					(180)
#define TIME_WAVE						(10)
#define TIME_WAVE2						(50)
#define TIME_WAVE3						(90)
#define TIME_WAVE4						(130)
#define TIME							(90)
#define TEX_GRAUND						(0)		//地面
#define TEX_BLUE						(1)		//青
#define TEX_RED							(2)		//赤
#define TEX_YELLOW						(3)		//黄色

// 地面シェーダー用
#define FIELDRENDER_PRIORITY        	(3)
#define FIELDRENDER_SHADER_FILENAME 	"data/field.hlsl"

// 通常
#define FIELD_NONE_NUMINSTANCE			(MAX_FIELD)
#define FIELD_NONE_TEXIDX				(1)
#define FIELD_NONE_DISP					(false)
#define FIELD_NONE_LIGHTING				(false)

// 補間用
#define FIELD_GROUND_NUMINSTANCE		(MAX_FIELD)
#define FIELD_GROUND_TEXIDX				(0)
#define FIELD_GROUND_DISP				(false)
#define FIELD_GROUND_LIGHTING			(true)

// メイン
#define FIELD_MAIN_NUMINSTANCE			(1)
#define FIELD_MAIN_TEXIDX				(0)
#define FIELD_MAIN_DISP					(false)
#define FIELD_MAIN_LIGHTING				(true)

// カウントダウン
#define FIELD_COUNTDOWN_NUMINSTANCE		(1)
#define FIELD_COUNTDOWN_TEXIDX			(1)
#define FIELD_COUNTDOWN_DISP			(false)
#define FIELD_COUNTDOWN_LIGHTING		(false)

// イベント用
#define FIELD_EVENT_NUMINSTANCE			(1)
#define FIELD_EVENT_TEXIDX				(1)
#define FIELD_EVENT_DISP				(false)
#define FIELD_EVENT_LIGHTING			(true)

//=============================================================================
// コンストラクタ
//=============================================================================
CFieldManager::CFieldManager() {}

//=============================================================================
// デストラクタ
//=============================================================================
CFieldManager::~CFieldManager(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFieldManager::Init(void)
{
	// 値の初期化
	for (int nCntTex = 0; nCntTex < FIELD_TEX; nCntTex++)
	{
		m_pTexture[nCntTex] = NULL;
	}

	for (int nCntRender = 0; nCntRender < FIELD_TYPE_MAX; nCntRender++)
	{
		m_pFieldRender[nCntRender] = NULL;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture[0]);	//地面
	D3DXCreateTextureFromFile(pDevice, TEX_NAME2, &m_pTexture[1]);	//青
	D3DXCreateTextureFromFile(pDevice, TEX_NAME3, &m_pTexture[2]);	//赤
	D3DXCreateTextureFromFile(pDevice, TEX_NAME4, &m_pTexture[3]);	//黄

	//変数の初期化
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_pField[nCnt] = NULL;
		m_pFieldGraund[nCnt] = NULL;
	}
	m_nCountTime = 0;
	m_nCountY = 1;
	m_nCountX = 0;
	m_nCountY2 = 1;
	m_nCountX2 = 0;
	m_nCountY3 = 1;
	m_nCountX3 = 0;
	m_nCountY4 = 1;
	m_nCountX4 = 0;
	m_state = STATE_NORMAL;
	m_pMainField = NULL;
	m_pCountDownField = NULL;
	m_pEventField = NULL;
	m_fCol_a = 1.0f;
	m_pFieldShader = NULL;

	//デジタル地面の生成
	int nCountField = 0;
	for (int nCntY = 0; nCntY < LINE; nCntY++)
	{
		float PosY = ((BLOCK_SIZE.z * 2.0f) * nCntY) + START_Y;

		for (int nCntX = 0; nCntX < LINE; nCntX++)
		{
			float PosX = ((BLOCK_SIZE.x * 2.0f) * nCntX) + START_X;
			m_pField[nCountField] = CField::Create(D3DXVECTOR3(PosX, 0.0f, PosY), BLOCK_SIZE, CField::TYPE_BLOCK);
			m_pField[nCountField]->BindTexture(m_pTexture[TEX_BLUE]);
			m_pField[nCountField]->SetLight(true);
			m_pField[nCountField]->SetDraw(false);
			nCountField++;
		}
	}

	//グランド地面
	int nCountField2 = 0;
	for (int nCntY = 0; nCntY < LINE; nCntY++)
	{
		float PosY = ((BLOCK_SIZE.z * 2.0f) * nCntY) + START_Y;

		for (int nCntX = 0; nCntX < LINE; nCntX++)
		{
			float PosX = ((BLOCK_SIZE.x * 2.0f) * nCntX) + START_X;
			m_pFieldGraund[nCountField2] = CField::Create(D3DXVECTOR3(PosX, 0.0f, PosY), BLOCK_SIZE, CField::TYPE_BLOCK);
			m_pFieldGraund[nCountField2]->BindTexture(m_pTexture[TEX_GRAUND]);
			m_pFieldGraund[nCountField2]->SetRot(D3DXVECTOR3(/*D3DX_PI*/ 0.0f, 0.0f, 0.0f));
			m_pFieldGraund[nCountField2]->SetDraw(false);

			nCountField2++;
		}
	}

	//カウントダウン用のテクスチャ
	/*if (m_pCountDownField == NULL)
	{
		m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.1f, START_Y), D3DXVECTOR3(MAIN_SIZE.x, 0.0f, BLOCK_SIZE.z * 2.0f), CField::TYPE_COUNTDOWN);
		m_pCountDownField->BindTexture(m_pTexture[4]);
		m_pCountDownField->SetDivision(D3DXVECTOR2(LINE / 2.0f, 1));
		m_pCountDownField->SetLight(true);
		m_pCountDownField->SetState(CField::STATE_MOVE_Z);
	}*/

	//大きな地面を生成する
	if (m_pMainField == NULL)
	{
		m_pMainField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.01f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
		m_pMainField->BindTexture(m_pTexture[TEX_GRAUND]);
		m_pMainField->SetDivision(D3DXVECTOR2(LINE, LINE));
		m_pMainField->SetDraw(true);
	}

	// イベント用の地面を生成する
	if (m_pEventField == NULL)
	{
		m_pEventField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.0f, -BLOCK_SIZE.z), MAIN_SIZE * 0.5f, CField::TYPE_MAINFIELD);
		m_pEventField->BindTexture(m_pTexture[TEX_BLUE]);
	}

	//状態の設定
	SetState(m_state);

	// シェーダー読み込み
	LoadShader();

	// 地面描画クラスの生成
	MakeFieldRender();

	// ゲーム画面以外だったらメインの地面を描画
	if (CManager::GetMode() != CManager::MODE_GAME && m_pFieldRender[FIELD_TYPE_MAIN] != NULL)
	{
		m_pFieldRender[FIELD_TYPE_NONE]->SetDisp(false);
		m_pFieldRender[FIELD_TYPE_GROUND]->SetDisp(false);
		m_pFieldRender[FIELD_TYPE_MAIN]->SetDisp(true);
		m_pFieldRender[FIELD_TYPE_COUNTDOWN]->SetDisp(false);
		m_pFieldRender[FIELD_TYPE_EVENT]->SetDisp(false);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFieldManager::Uninit(void)
{
	//変数の初期化
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_pField[nCnt] = NULL;
		m_pFieldGraund[nCnt] = NULL;
	}

	//テクスチャの開放
	for (int nCnt = 0; nCnt < FIELD_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}

	// シェーダーの開放
	ReleaseFieldShader();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFieldManager::Update(void)
{
	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_XTURN:  UpdateXTurn(); break;	//X軸の回転
	case STATE_RANDOM: UpdateRamdom(); break;	//ランダム
	case STATE_WAVE:   UpdateWave();   break;	//波
	case STATE_COUNTDOWN: UpdateCountDown(); break;	//カウントダウン
	case STATE_TIME_2:
	case STATE_TIME_1:
	case STATE_TIME_30: UpdateTime(); break;	//制限時間時の表示
	}

	// 地面の描画データを登録
	SetFieldData_None();
	SetFieldData_Ground();
	SetFieldData_Main();
	SetFieldData_CountDown();
	SetFieldData_Event();
}

//=============================================================================
// 回転の更新処理
//=============================================================================
void CFieldManager::UpdateXTurn(void)
{
	if (m_nCountY > LINE)
	{
		m_state = STATE_NORMAL;
		m_nCountTime = 0;
		m_nCountX = 0;
		m_nCountY = 1;
		ChangeField(true);	//地面表示の切り替え
		return;
	}

	m_nCountTime++;

	if ((m_nCountTime % TIME_X) == 0)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = (int)(m_nCountX + nCnt);

			if (m_pField[nBlock] != NULL)
			{
				m_pField[nBlock]->SetState(CField::STATE_X_TURN);
			}
		}

		m_nCountX = m_nCountY * LINE;
		m_nCountY++;
	}
}

//=============================================================================
// ランダム
//=============================================================================
void CFieldManager::UpdateRamdom(void)
{
	if (m_nCountY2 > LINE)
	{
		m_state = STATE_NORMAL;
		m_nCountTime = 0;
		m_nCountX = 0;
		m_nCountY = 1;
		m_nCountX2 = 0;
		m_nCountY2 = 1;
		ChangeField(false);	//地面表示の切り替え
		return;
	}
	m_nCountTime++;

	if ((m_nCountTime % TIME_RANDUM) != 0) { return; }

	if (m_nCountY < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = (int)(m_nCountX + nCnt);

			if (m_pField[nBlock] == NULL) { continue; }

			if ((m_nCountY % 2) != 0)
			{
				if ((nBlock % 2) != 0)
				{
					m_pField[nBlock]->SetState(CField::STATE_X_TURN);
					m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
				}
			}
			else
			{
				if ((nBlock % 2) == 0)
				{
					m_pField[nBlock]->SetState(CField::STATE_X_TURN);
					m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
				}
			}
		}

		m_nCountX = m_nCountY * LINE;
		m_nCountY++;
	}

	if (m_nCountTime < TIME_RANDUM2) { return; }

	for (int nCnt = 0; nCnt < LINE; nCnt++)
	{
		int nBlock = (int)(m_nCountX2 + nCnt);

		if (m_pField[nBlock] == NULL) { continue; }

		if ((m_nCountY2 % 2) != 0)
		{
			if ((nBlock % 2) == 0)
			{
				m_pField[nBlock]->SetState(CField::STATE_X_TURN);
				m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
			}
		}
		else
		{
			if ((nBlock % 2) != 0)
			{
				m_pField[nBlock]->SetState(CField::STATE_X_TURN);
				m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
			}
		}
	}

	m_nCountX2 = m_nCountY2 * LINE;
	m_nCountY2++;
}

//=============================================================================
// 波の更新処理
//=============================================================================
void CFieldManager::UpdateWave(void)
{
	m_nCountTime++;

	if ((m_nCountTime % TIME_WAVE) != 0) { return; }

	if (m_nCountY < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_RED]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY++;
	}

	if (m_nCountTime < TIME_WAVE2) { return; }

	if (m_nCountY2 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY2 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_RED]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY2++;
	}

	if (m_nCountTime < TIME_WAVE3) { return; }

	if (m_nCountY3 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY3 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_BLUE]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY3++;
	}

	if (m_nCountTime < TIME_WAVE4) { return; }

	if (m_nCountY4 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY4 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_GRAUND]);
			m_pFieldGraund[nBlock]->SetLight(false);
		}

		m_nCountY4++;
	}
	else
	{
		ChangeField(true);
		m_state = STATE_NORMAL;
	}
}

//=============================================================================
// カウントダウン処理
//=============================================================================
void CFieldManager::UpdateCountDown(void)
{
	if (m_pCountDownField == NULL) { return; }

	D3DXVECTOR3 Pos = m_pCountDownField->GetPos();

	if (Pos.z > MAIN_SIZE.z)
	{
		SetState(STATE_NORMAL);
	}
}

//=============================================================================
// 制限時間の表示処理
//=============================================================================
void CFieldManager::UpdateTime(void)
{
	//時間の可算
	m_nCountTime++;
	if (m_nCountTime < TIME) { return; }

	bool bDeth = false;
	m_fCol_a -= 0.02f;

	if (m_fCol_a < 0.0f)
	{
		m_fCol_a = 0;
		bDeth = true;
	}

	//色の設定
	if (m_pCountDownField != NULL) { m_pCountDownField->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_a)); }

	//状態の設定を元に戻す
	if (bDeth) { SetState(STATE_NORMAL); }
}

//=============================================================================
// 地面の表示切替
//=============================================================================
void CFieldManager::ChangeField(bool bChangeMain)
{
	bool bFieldDraw = bChangeMain ? false : true;

	if (m_pMainField != NULL) { m_pMainField->SetDraw(bChangeMain); }	//メインの地面を描画する

	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pField[nCnt] != NULL)
		{
			m_pField[nCnt]->SetDraw(bFieldDraw);
			m_pField[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			/*m_pField[nCnt]->Uninit();
			m_pField[nCnt] = NULL;*/
		}

		if (m_pFieldGraund[nCnt] != NULL)
		{
			m_pFieldGraund[nCnt]->SetDraw(bFieldDraw);
			m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
		/*	m_pFieldGraund[nCnt]->Uninit();
			m_pFieldGraund[nCnt] = NULL;*/
		}
	}

	if (m_pFieldRender[FIELD_TYPE_NONE] != NULL) { m_pFieldRender[FIELD_TYPE_NONE]->SetDisp(bFieldDraw); }
	if (m_pFieldRender[FIELD_TYPE_GROUND] != NULL) { m_pFieldRender[FIELD_TYPE_GROUND]->SetDisp(bFieldDraw); }
	if (m_pFieldRender[FIELD_TYPE_MAIN] != NULL) { m_pFieldRender[FIELD_TYPE_MAIN]->SetDisp(bChangeMain); }
}

//=============================================================================
// 状態設定
//=============================================================================
void CFieldManager::SetState(STATE state)
{
	//前回の状態の後処理
	switch (m_state)
	{
	case STATE_RANDOM:	//ランダム
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pField[nCnt] != NULL)
			{
				m_pField[nCnt]->SetDraw(false);
				m_pField[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}

			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(false);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}
		}
		if (m_pFieldRender[FIELD_TYPE_NONE] != NULL) { m_pFieldRender[FIELD_TYPE_NONE]->SetDisp(false); }
		if (m_pFieldRender[FIELD_TYPE_GROUND] != NULL) { m_pFieldRender[FIELD_TYPE_GROUND]->SetDisp(false); }
		if (m_pFieldRender[FIELD_TYPE_MAIN] != NULL) { m_pFieldRender[FIELD_TYPE_MAIN]->SetDisp(true); }
		break;

	case STATE_WAVE:	//波
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(false);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}
		}
		break;

	case STATE_COUNTDOWN:	//カウントダウン
		if(m_pCountDownField != NULL)
		{
			m_pCountDownField->Uninit();
			m_pCountDownField = NULL;
		}
		break;

	case STATE_TIME_2:
	case STATE_TIME_1:
	case STATE_TIME_30:
		if (m_pMainField != NULL)
		{
			m_pMainField->BindTexture(m_pTexture[TEX_GRAUND]);
			m_pMainField->SetLight(false);
		}

		//カウントダウン用
		if (m_pCountDownField != NULL)
		{
			m_pCountDownField->Uninit();
			m_pCountDownField = NULL;
		}
		if (m_pFieldRender[FIELD_TYPE_COUNTDOWN] != NULL) { m_pFieldRender[FIELD_TYPE_COUNTDOWN]->SetDisp(false); }
		break;
	}

	//状態の更新
	m_state = state;

	//現在の状態の設定
	switch (m_state)
	{
	case STATE_RANDOM:	//ランダム
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pField[nCnt] != NULL)
			{
				m_pField[nCnt]->SetDraw(true);
				m_pField[nCnt]->BindTexture(m_pTexture[TEX_BLUE]);
				m_pField[nCnt]->SetLight(true);
			}

			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(true);
				m_pFieldGraund[nCnt]->BindTexture(m_pTexture[TEX_GRAUND]);
				m_pFieldGraund[nCnt]->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
			}
		}

		//メインの地面
		if (m_pMainField != NULL) { m_pMainField->SetDraw(false); }

		if (m_pFieldRender[FIELD_TYPE_NONE] != NULL) { m_pFieldRender[FIELD_TYPE_NONE]->SetDisp(true); }
		if (m_pFieldRender[FIELD_TYPE_GROUND] != NULL) { m_pFieldRender[FIELD_TYPE_GROUND]->SetDisp(true); }
		if (m_pFieldRender[FIELD_TYPE_MAIN] != NULL) { m_pFieldRender[FIELD_TYPE_MAIN]->SetDisp(false); }
		break;

	case STATE_WAVE:	//波
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(true);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NORMAL);
				m_pFieldGraund[nCnt]->BindTexture(m_pTexture[TEX_GRAUND]);
				m_pFieldGraund[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		//メインの地面
		if (m_pMainField != NULL) { m_pMainField->SetDraw(false); }
		break;

	case STATE_COUNTDOWN:	//カウントダウン
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.1f, START_Y), D3DXVECTOR3(MAIN_SIZE.x, 0.0f, BLOCK_SIZE.z * 2.0f), CField::TYPE_COUNTDOWN);
			m_pCountDownField->BindTexture(m_pTexture[TEX_RED]);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE / 2.0f, 1));
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetState(CField::STATE_MOVE_Z);
		}

		//メインの地面
		if (m_pMainField != NULL) { m_pMainField->SetDraw(true); }
		break;

	case STATE_TIME_2:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_BLUE]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE /** 0.5f*/, LINE/* * 0.5f*/));
		}
		if (m_pFieldRender[FIELD_TYPE_COUNTDOWN] != NULL)
		{
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->SetDisp(true);
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->BindTexture(m_pTexture[TEX_BLUE]);
		}
		break;

	case STATE_TIME_1:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_YELLOW]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE/* * 0.5f*/, LINE/* * 0.5f*/));
		}
		if (m_pFieldRender[FIELD_TYPE_COUNTDOWN] != NULL)
		{
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->SetDisp(true);
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->BindTexture(m_pTexture[TEX_YELLOW]);
		}
		break;

	case STATE_TIME_30:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_RED]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE/* * 0.5f*/, LINE/* * 0.5f*/));
		}
		if (m_pFieldRender[FIELD_TYPE_COUNTDOWN] != NULL)
		{
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->SetDisp(true);
			m_pFieldRender[FIELD_TYPE_COUNTDOWN]->BindTexture(m_pTexture[TEX_RED]);
		}
		break;
	}
}


//*****************************************************************************
//     地面描画用関数
//     Author : Hodaka Niwa
//*****************************************************************************
//=============================================================================
//    シェーダーを読み込む処理
//=============================================================================
void CFieldManager::LoadShader(void)
{
	if (m_pFieldShader != NULL) { return; }
	m_pFieldShader = CFieldShader::Create(FIELDRENDER_SHADER_FILENAME);
}

//=============================================================================
//    地面描画クラスを生成する処理
//=============================================================================
void CFieldManager::MakeFieldRender(void)
{
	int nNumInstance[FIELD_TYPE_MAX] =
	{
		FIELD_NONE_NUMINSTANCE,
		FIELD_GROUND_NUMINSTANCE,
		FIELD_MAIN_NUMINSTANCE,
		FIELD_COUNTDOWN_NUMINSTANCE,
		FIELD_EVENT_NUMINSTANCE,
	};
	int nTexIdx[FIELD_TYPE_MAX] =
	{
		FIELD_NONE_TEXIDX,
		FIELD_GROUND_TEXIDX,
		FIELD_MAIN_TEXIDX,
		FIELD_COUNTDOWN_TEXIDX,
		FIELD_EVENT_TEXIDX
	};
	bool bDisp[FIELD_TYPE_MAX] =
	{
		FIELD_NONE_DISP,
		FIELD_GROUND_DISP,
		FIELD_MAIN_DISP,
		FIELD_COUNTDOWN_DISP,
		FIELD_EVENT_DISP,
	};
	bool bLighting[FIELD_TYPE_MAX] =
	{
		FIELD_NONE_LIGHTING,
		FIELD_GROUND_LIGHTING,
		FIELD_MAIN_LIGHTING,
		FIELD_COUNTDOWN_LIGHTING,
		FIELD_EVENT_LIGHTING
	};
	for (int nCntRender = 0; nCntRender < FIELD_TYPE_MAX; nCntRender++)
	{
		// 地面描画クラスの生成
		if (m_pFieldRender[nCntRender] != NULL) { continue; }
		m_pFieldRender[nCntRender] = CFieldRender::Create(nNumInstance[nCntRender], FIELDRENDER_PRIORITY);
		if (m_pFieldRender[nCntRender] == NULL) { continue; }

		// テクスチャの設定
		m_pFieldRender[nCntRender]->BindTexture(m_pTexture[nTexIdx[nCntRender]]);

		// シェーダーへのポインタを設定
		m_pFieldRender[nCntRender]->SetFieldShader(m_pFieldShader);

		// 描画するかしないかを設定
		m_pFieldRender[nCntRender]->SetDisp(bDisp[nCntRender]);

		// ライティングするかしないかを設定
		m_pFieldRender[nCntRender]->SetLighting(bLighting[nCntRender]);
	}
}

//=============================================================================
//    地面描画シェーダークラスを開放する処理
//=============================================================================
void CFieldManager::ReleaseFieldShader(void)
{
	if (m_pFieldShader != NULL)
	{
		m_pFieldShader->Uninit();
		delete m_pFieldShader;
		m_pFieldShader = NULL;
	}
}

//=============================================================================
//    地面描画クラスを開放する処理
//=============================================================================
void CFieldManager::ReleaseFieldRender(void)
{
	for (int nCntRender = 0; nCntRender < FIELD_TYPE_MAX; nCntRender++)
	{
		if (m_pFieldRender[nCntRender] != NULL)
		{
			m_pFieldRender[nCntRender]->Uninit();
			m_pFieldRender[nCntRender] = NULL;
		}
	}
}

//=============================================================================
//    地面の描画データを登録する処理( None ver )
//=============================================================================
void CFieldManager::SetFieldData_None(void)
{
	if (m_pFieldRender[FIELD_TYPE_NONE] == NULL) { return; }
	if (m_pFieldRender[FIELD_TYPE_NONE]->GetDisp() == false) { return; }

	// 頂点情報
	SetFieldVertex_None();

	// ワールドマトリックス情報
	SetFieldMtxWorld_None();

	// 頂点カラー情報
	SetFieldColor_None();
}

//=============================================================================
//    地面の描画データを登録する処理( Ground ver )
//=============================================================================
void CFieldManager::SetFieldData_Ground(void)
{
	if (m_pFieldRender[FIELD_TYPE_GROUND] == NULL) { return; }
	if (m_pFieldRender[FIELD_TYPE_GROUND]->GetDisp() == false) { return; }

	// 頂点情報
	SetFieldVertex_Ground();

	// ワールドマトリックス情報
	SetFieldMtxWorld_Ground();

	// 頂点カラー情報
	SetFieldColor_Ground();
}

//=============================================================================
//    地面の描画データを登録する処理( Main ver )
//=============================================================================
void CFieldManager::SetFieldData_Main(void)
{
	if (m_pFieldRender[FIELD_TYPE_MAIN] == NULL) { return; }
	if (m_pFieldRender[FIELD_TYPE_MAIN]->GetDisp() == false) { return; }

	// 頂点情報
	SetFieldVertex_Main();

	// ワールドマトリックス情報
	SetFieldMtxWorld_Main();

	// 頂点カラー情報
	SetFieldColor_Main();
}

//=============================================================================
//    地面の描画データを登録する処理( CountDown ver )
//=============================================================================
void CFieldManager::SetFieldData_CountDown(void)
{
	if (m_pFieldRender[FIELD_TYPE_COUNTDOWN] == NULL) { return; }
	if (m_pFieldRender[FIELD_TYPE_COUNTDOWN]->GetDisp() == false) { return; }

	// 頂点情報
	SetFieldVertex_CountDown();

	// ワールドマトリックス情報
	SetFieldMtxWorld_CountDown();

	// 頂点カラー情報
	SetFieldColor_CountDown();
}

//=============================================================================
//    地面の描画データを登録する処理( Event ver )
//=============================================================================
void CFieldManager::SetFieldData_Event(void)
{
	if (m_pFieldRender[FIELD_TYPE_EVENT] == NULL) { return; }
	if (m_pFieldRender[FIELD_TYPE_EVENT]->GetDisp() == false) { return; }

	// 頂点情報
	SetFieldVertex_Event();

	// ワールドマトリックス情報
	SetFieldMtxWorld_Event();

	// 頂点カラー情報
	SetFieldColor_Event();
}

//=============================================================================
//    頂点情報を設定する処理
//=============================================================================
void CFieldManager::SetFieldVertex_None(void)
{
	CFieldRender::VERTEXDATA *pVtx = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	// データを頂点バッファに書き込む
	pVtxBuff = m_pFieldRender[FIELD_TYPE_NONE]->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	D3DXVECTOR3 Size = m_pField[0]->GetSize();
	pVtx[0].pos = D3DXVECTOR3(-Size.x, 0.0f, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, 0.0f, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, 0.0f, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, 0.0f, -Size.z);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CFieldManager::SetFieldMtxWorld_None(void)
{
	CFieldRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = NULL;

	// データを頂点バッファに書き込む
	pWorldBuff = m_pFieldRender[FIELD_TYPE_NONE]->GetWorldBuff();
	if (pWorldBuff == NULL) { return; }

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pField[nCnt] == NULL) { continue; }
		pos = m_pField[nCnt]->GetPos();
		rot = m_pField[nCnt]->GetRot();
		float fSinPitch = sinf(rot.x);
		float fCosPitch = cosf(rot.x);
		float fSinYaw = sinf(rot.y);
		float fCosYaw = cosf(rot.y);
		float fSinRoll = sinf(rot.z);
		float fCosRoll = cosf(rot.z);
		pWorld[nCnt].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		pWorld[nCnt].m1[1] = fSinRoll * fCosPitch;
		pWorld[nCnt].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		pWorld[nCnt].m1[3] = 0.0f;
		pWorld[nCnt].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		pWorld[nCnt].m2[1] = fCosRoll * fCosPitch;
		pWorld[nCnt].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		pWorld[nCnt].m2[3] = 0.0f;
		pWorld[nCnt].m3[0] = fCosPitch * fSinYaw;
		pWorld[nCnt].m3[1] = -fSinPitch;
		pWorld[nCnt].m3[2] = fCosPitch * fCosYaw;
		pWorld[nCnt].m3[3] = 0.0f;
		pWorld[nCnt].m4[0] = pos.x;
		pWorld[nCnt].m4[1] = pos.y;
		pWorld[nCnt].m4[2] = pos.z;
		pWorld[nCnt].m4[3] = 1.0f;
	}

	// ワールドマトリックスバッファをアンロックする
	pWorldBuff->Unlock();
}

//=============================================================================
//    頂点カラーを設定する処理
//=============================================================================
void CFieldManager::SetFieldColor_None(void)
{
	CFieldRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = NULL;

	// データを頂点バッファに書き込む
	pColorBuff = m_pFieldRender[FIELD_TYPE_NONE]->GetColorBuff();
	if (pColorBuff == NULL) { return; }

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	D3DXCOLOR col = INITIALIZE_VECTOR4;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pField[nCnt] == NULL) { continue; }
		col = m_pField[nCnt]->GetCol();
		pColor[nCnt].col = col;
	}

	// 頂点カラーバッファをアンロックする
	pColorBuff->Unlock();
}

//=============================================================================
//    頂点情報を設定する処理
//=============================================================================
void CFieldManager::SetFieldVertex_Ground(void)
{
	CFieldRender::VERTEXDATA *pVtx = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	// データを頂点バッファに書き込む
	pVtxBuff = m_pFieldRender[FIELD_TYPE_GROUND]->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	D3DXVECTOR3 Size = m_pFieldGraund[0]->GetSize();
	pVtx[0].pos = D3DXVECTOR3(-Size.x, 0.0f, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, 0.0f, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, 0.0f, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, 0.0f, -Size.z);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CFieldManager::SetFieldMtxWorld_Ground(void)
{
	CFieldRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = NULL;

	// データを頂点バッファに書き込む
	pWorldBuff = m_pFieldRender[FIELD_TYPE_GROUND]->GetWorldBuff();
	if (pWorldBuff == NULL) { return; }

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pFieldGraund[nCnt] == NULL) { continue; }
		pos = m_pFieldGraund[nCnt]->GetPos();
		rot = m_pFieldGraund[nCnt]->GetRot();
		float fSinPitch = sinf(rot.x);
		float fCosPitch = cosf(rot.x);
		float fSinYaw = sinf(rot.y);
		float fCosYaw = cosf(rot.y);
		float fSinRoll = sinf(rot.z);
		float fCosRoll = cosf(rot.z);
		pWorld[nCnt].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		pWorld[nCnt].m1[1] = fSinRoll * fCosPitch;
		pWorld[nCnt].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		pWorld[nCnt].m1[3] = 0.0f;
		pWorld[nCnt].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		pWorld[nCnt].m2[1] = fCosRoll * fCosPitch;
		pWorld[nCnt].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		pWorld[nCnt].m2[3] = 0.0f;
		pWorld[nCnt].m3[0] = fCosPitch * fSinYaw;
		pWorld[nCnt].m3[1] = -fSinPitch;
		pWorld[nCnt].m3[2] = fCosPitch * fCosYaw;
		pWorld[nCnt].m3[3] = 0.0f;
		pWorld[nCnt].m4[0] = pos.x;
		pWorld[nCnt].m4[1] = pos.y;
		pWorld[nCnt].m4[2] = pos.z;
		pWorld[nCnt].m4[3] = 1.0f;
	}

	// ワールドマトリックスバッファをアンロックする
	pWorldBuff->Unlock();
}

//=============================================================================
//    頂点カラーを設定する処理
//=============================================================================
void CFieldManager::SetFieldColor_Ground(void)
{
	CFieldRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = NULL;

	// データを頂点バッファに書き込む
	pColorBuff = m_pFieldRender[FIELD_TYPE_GROUND]->GetColorBuff();
	if (pColorBuff == NULL) { return; }

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	D3DXCOLOR col = INITIALIZE_VECTOR4;
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pFieldGraund[nCnt] == NULL) { continue; }
		col = m_pFieldGraund[nCnt]->GetCol();
		pColor[nCnt].col = col;
	}

	// 頂点カラーバッファをアンロックする
	pColorBuff->Unlock();
}

//=============================================================================
//    頂点データを設定する処理
//=============================================================================
void CFieldManager::SetFieldVertex_Main(void)
{
	CFieldRender::VERTEXDATA *pVtx = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	// データを頂点バッファに書き込む
	pVtxBuff = m_pFieldRender[FIELD_TYPE_MAIN]->GetVtxBuff();
	if (pVtxBuff == NULL || m_pMainField == NULL) { return; }

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	D3DXVECTOR3 Size = m_pMainField->GetSize();
	pVtx[0].pos = D3DXVECTOR3(-Size.x, 0.0f, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, 0.0f, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, 0.0f, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, 0.0f, -Size.z);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f * LINE, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * LINE);
	pVtx[3].tex = D3DXVECTOR2(1.0f * LINE, 1.0f * LINE);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CFieldManager::SetFieldMtxWorld_Main(void)
{
	CFieldRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = NULL;

	// データを頂点バッファに書き込む
	pWorldBuff = m_pFieldRender[FIELD_TYPE_MAIN]->GetWorldBuff();
	if (pWorldBuff == NULL) { return; }

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	if (m_pMainField != NULL)
	{
		pos = m_pMainField->GetPos();
		rot = m_pMainField->GetRot();
		float fSinPitch = sinf(rot.x);
		float fCosPitch = cosf(rot.x);
		float fSinYaw = sinf(rot.y);
		float fCosYaw = cosf(rot.y);
		float fSinRoll = sinf(rot.z);
		float fCosRoll = cosf(rot.z);
		pWorld[0].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		pWorld[0].m1[1] = fSinRoll * fCosPitch;
		pWorld[0].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		pWorld[0].m1[3] = 0.0f;
		pWorld[0].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		pWorld[0].m2[1] = fCosRoll * fCosPitch;
		pWorld[0].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		pWorld[0].m2[3] = 0.0f;
		pWorld[0].m3[0] = fCosPitch * fSinYaw;
		pWorld[0].m3[1] = -fSinPitch;
		pWorld[0].m3[2] = fCosPitch * fCosYaw;
		pWorld[0].m3[3] = 0.0f;
		pWorld[0].m4[0] = pos.x;
		pWorld[0].m4[1] = pos.y;
		pWorld[0].m4[2] = pos.z;
		pWorld[0].m4[3] = 1.0f;
	}

	// ワールドマトリックスバッファをアンロックする
	pWorldBuff->Unlock();
}

//=============================================================================
//    頂点カラーを設定する処理
//=============================================================================
void CFieldManager::SetFieldColor_Main(void)
{
	CFieldRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = NULL;

	// データを頂点バッファに書き込む
	pColorBuff = m_pFieldRender[FIELD_TYPE_MAIN]->GetColorBuff();
	if (pColorBuff == NULL) { return; }

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	if (m_pMainField != NULL)
	{
		pColor[0].col = m_pMainField->GetCol();
	}

	// 頂点カラーバッファをアンロックする
	pColorBuff->Unlock();
}

//=============================================================================
//    頂点情報を設定する処理
//=============================================================================
void CFieldManager::SetFieldVertex_CountDown(void)
{
	CFieldRender::VERTEXDATA *pVtx = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	// データを頂点バッファに書き込む
	pVtxBuff = m_pFieldRender[FIELD_TYPE_COUNTDOWN]->GetVtxBuff();
	if (pVtxBuff == NULL || m_pCountDownField == NULL) { return; }

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	D3DXVECTOR3 Size = m_pCountDownField->GetSize();
	pVtx[0].pos = D3DXVECTOR3(-Size.x, 0.0f, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, 0.0f, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, 0.0f, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, 0.0f, -Size.z);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f * LINE, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * LINE);
	pVtx[3].tex = D3DXVECTOR2(1.0f * LINE, 1.0f * LINE);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CFieldManager::SetFieldMtxWorld_CountDown(void)
{
	CFieldRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = NULL;

	// データを頂点バッファに書き込む
	pWorldBuff = m_pFieldRender[FIELD_TYPE_COUNTDOWN]->GetWorldBuff();
	if (pWorldBuff == NULL) { return; }

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	if (m_pCountDownField != NULL)
	{
		pos = m_pCountDownField->GetPos();
		rot = m_pCountDownField->GetRot();
		float fSinPitch = sinf(rot.x);
		float fCosPitch = cosf(rot.x);
		float fSinYaw = sinf(rot.y);
		float fCosYaw = cosf(rot.y);
		float fSinRoll = sinf(rot.z);
		float fCosRoll = cosf(rot.z);
		pWorld[0].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		pWorld[0].m1[1] = fSinRoll * fCosPitch;
		pWorld[0].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		pWorld[0].m1[3] = 0.0f;
		pWorld[0].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		pWorld[0].m2[1] = fCosRoll * fCosPitch;
		pWorld[0].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		pWorld[0].m2[3] = 0.0f;
		pWorld[0].m3[0] = fCosPitch * fSinYaw;
		pWorld[0].m3[1] = -fSinPitch;
		pWorld[0].m3[2] = fCosPitch * fCosYaw;
		pWorld[0].m3[3] = 0.0f;
		pWorld[0].m4[0] = pos.x;
		pWorld[0].m4[1] = pos.y;
		pWorld[0].m4[2] = pos.z;
		pWorld[0].m4[3] = 1.0f;
	}

	// ワールドマトリックスバッファをアンロックする
	pWorldBuff->Unlock();
}

//=============================================================================
//    頂点カラーを設定する処理
//=============================================================================
void CFieldManager::SetFieldColor_CountDown(void)
{
	CFieldRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = NULL;

	// データを頂点バッファに書き込む
	pColorBuff = m_pFieldRender[FIELD_TYPE_COUNTDOWN]->GetColorBuff();
	if (pColorBuff == NULL) { return; }

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	D3DXCOLOR col = INITIALIZE_VECTOR4;
	if (m_pCountDownField != NULL)
	{
		col = m_pCountDownField->GetCol();
		pColor[0].col = col;
	}

	// 頂点カラーバッファをアンロックする
	pColorBuff->Unlock();
}

//=============================================================================
//    頂点情報を設定する処理
//=============================================================================
void CFieldManager::SetFieldVertex_Event(void)
{
	CFieldRender::VERTEXDATA *pVtx = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	// データを頂点バッファに書き込む
	pVtxBuff = m_pFieldRender[FIELD_TYPE_EVENT]->GetVtxBuff();
	if (pVtxBuff == NULL || m_pEventField == NULL) { return; }

	// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	D3DXVECTOR3 Size = m_pEventField->GetSize();
	pVtx[0].pos = D3DXVECTOR3(-Size.x, 0.0f, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, 0.0f, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, 0.0f, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, 0.0f, -Size.z);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f * (LINE * 0.5f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * (LINE * 0.5f));
	pVtx[3].tex = D3DXVECTOR2(1.0f * (LINE * 0.5f), 1.0f * (LINE * 0.5f));

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CFieldManager::SetFieldMtxWorld_Event(void)
{
	CFieldRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = NULL;

	// データを頂点バッファに書き込む
	pWorldBuff = m_pFieldRender[FIELD_TYPE_EVENT]->GetWorldBuff();
	if (pWorldBuff == NULL) { return; }

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	if (m_pEventField != NULL)
	{
		pos = m_pEventField->GetPos();
		rot = m_pEventField->GetRot();
		float fSinPitch = sinf(rot.x);
		float fCosPitch = cosf(rot.x);
		float fSinYaw = sinf(rot.y);
		float fCosYaw = cosf(rot.y);
		float fSinRoll = sinf(rot.z);
		float fCosRoll = cosf(rot.z);
		pWorld[0].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		pWorld[0].m1[1] = fSinRoll * fCosPitch;
		pWorld[0].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		pWorld[0].m1[3] = 0.0f;
		pWorld[0].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		pWorld[0].m2[1] = fCosRoll * fCosPitch;
		pWorld[0].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		pWorld[0].m2[3] = 0.0f;
		pWorld[0].m3[0] = fCosPitch * fSinYaw;
		pWorld[0].m3[1] = -fSinPitch;
		pWorld[0].m3[2] = fCosPitch * fCosYaw;
		pWorld[0].m3[3] = 0.0f;
		pWorld[0].m4[0] = pos.x;
		pWorld[0].m4[1] = pos.y;
		pWorld[0].m4[2] = pos.z;
		pWorld[0].m4[3] = 1.0f;
	}

	// ワールドマトリックスバッファをアンロックする
	pWorldBuff->Unlock();
}

//=============================================================================
//    頂点カラーを設定する処理
//=============================================================================
void CFieldManager::SetFieldColor_Event(void)
{
	CFieldRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = NULL;

	// データを頂点バッファに書き込む
	pColorBuff = m_pFieldRender[FIELD_TYPE_EVENT]->GetColorBuff();
	if (pColorBuff == NULL) { return; }

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	D3DXCOLOR col = INITIALIZE_VECTOR4;
	if (m_pEventField != NULL)
	{
		col = m_pEventField->GetCol();
		pColor[0].col = col;
	}

	// 頂点カラーバッファをアンロックする
	pColorBuff->Unlock();
}