//=============================================================================
//
// タイムの処理 [time.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "scene2D.h"
#include "number2D.h"
#include "scene.h"
#include "fade.h"
#include "timerlogo.h"
#include "game.h"
#include "fieldmanager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TIME	(180)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//タイマークラス
int CTimer::m_nTimeData[MAX_TIMENUMBER] = {};
int CTimer::m_nTime = 0;

//カウントダウンタイマークラス
int CCountDownTimer::m_nNumber[MAX_TIMENUMBER] = {};

//ゲームタイマークラス
int CGameTimer::m_nNumber[MAX_TIMENUMBER] = {};

//=============================================================================
// 生成処理
//=============================================================================
CTimer * CTimer::Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTimer * pTimer = NULL;

	if (type == TYPE_GAME)
	{
		if (pTimer == NULL)
		{
			pTimer = new CGameTimer(Priority, OBJTYPE_2DPOLYGON);

			if (pTimer != NULL)
			{
				pTimer->SetSizePos(pos, size);
				pTimer->Init();
			}
		}
	}
	if (type == TYPE_COUNTER)
	{
		if (pTimer == NULL)
		{
			pTimer = new CCountDownTimer(Priority, OBJTYPE_2DPOLYGON);

			if (pTimer != NULL)
			{
				pTimer->SetSizePos(pos, size);
				pTimer->Init();
			}
		}
	}
	return pTimer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTimer::CTimer(int nPriority,OBJTYPE objType) : CScene(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CTimer::~CTimer() {}

//=============================================================================
// タイムの代入
//=============================================================================
void CTimer::AddTime(int nTime)
{
	//変数宣言
	int nCntNum;			//スコアの桁数のカウンター
	int nNum = 1;
	int nNum2 = 1;

	//スコアの加算
	m_nTime = nTime;

	//桁数の計算
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = (m_nTime % nNum) /  nNum2;	//格納する数字を計算

															//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// タイムの取得
//=============================================================================
int CTimer::GetTime(void)
{
	return m_nTime;
}



//=============================================================================
// コンストラクタ
//=============================================================================
CCountDownTimer::CCountDownTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority,objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CCountDownTimer::~CCountDownTimer() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCountDownTimer::Init()
{
	//変数宣言
	int nCntNum;
	int nCntNumber;

	//数字のポインタの初期化
	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//数字の初期化
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	//初期値
	m_nCountDown = 3;
	m_nCounter = 0;
	m_nDeathCount = 0;
	m_bDeath = false;
	m_bCountStop = false;
	AddTime(m_nCountDown);
	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCountDownTimer::Uninit(void)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();
			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nTime = 0;

	for (int nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCountDownTimer::Update(void)
{
	//変数宣言
	int nCntNumber;

	//カウントダウンの更新
	TimeCountUpdate();

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nTimeData[nCntNumber]);
		}
	}
}

//=============================================================================
// カウントダウンの更新処理
//=============================================================================
void CCountDownTimer::TimeCountUpdate(void)
{
	int nState = CManager::GetGameState();

	if (nState == 1)
	{//ゲーム開始時のカウントの時
		m_nCounter--;

		if (m_nCounter % 60 == 0)
		{
			m_nCountDown--;
			CTimer::AddTime(m_nCountDown);

			if (m_nCountDown == 0)
			{
				//初期化する
				m_nCountDown = 0;
				m_bDeath = true;

				//ゲーム状態にする
				CManager::SetGameState(CManager::GAMESTATE_GAME);
			}
		}
	}

	if (m_bDeath == true)
	{
		CTimer::AddTime(1);

		m_nDeathCount++;

		if (m_nDeathCount == 5)
		{
			//初期化する
			m_nDeathCount = 0;
			m_bDeath = false;

			//タイマーの終了処理
			Uninit();

			//スタートロゴの生成
			CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_START);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCountDownTimer::Draw(void)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CCountDownTimer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}


//=============================================================================
// コンストラクタ
//=============================================================================
CGameTimer::CGameTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority,objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CGameTimer::~CGameTimer() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameTimer::Init()
{
	//変数宣言
	int nCntNum;
	int nCntNumber;

	//数字のポインタの初期化
	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//数字の初期化
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	//初期値
	m_nTimeCounter = 0;
	m_nTime = MAX_TIME;
	m_bTimeStop = false;
	m_nDeathGameCount = 0;
	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameTimer::Uninit(void)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();

			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nTime = 0;

	for (int nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameTimer::Update(void)
{
	//変数宣言
	int nCntNumber;

	//ゲームタイムの更新
	TimeGameUpdate();

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nTimeData[nCntNumber]);
		}
	}
}

//=============================================================================
// カウントダウンの更新処理
//=============================================================================
void CGameTimer::TimeGameUpdate(void)
{
	int nState = CManager::GetGameState();

	if (nState == 2)
	{//ゲーム状態の時
		m_nTimeCounter--;

		if (m_nTimeCounter % 60 == 0)
		{
			m_nTime--;
			CTimer::AddTime(m_nTime);

			if (m_nTime == 120)
			{//残り2分になったらテクスチャを生成
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_2MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_2);
			}
			if (m_nTime == 60)
			{//残り1分になったらテクスチャを生成
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_1MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_1);
			}
			if (m_nTime == 30)
			{ //残り30秒になったらテクスチャを生成
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_30SEC);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_30);
			}
			if (m_nTime == 0)
			{
				//値を初期化する
				m_bTimeStop = true;
				m_nTime = 0;

				//ゲームを終了状態にする
				CManager::SetGameState(CManager::GAMESTATE_FIN);
			}
		}
	}

	if (m_bTimeStop == true)
	{
		CTimer::AddTime(1);

		m_nDeathGameCount++;

		if (m_nDeathGameCount == 5)
		{
			//フィニッシュロゴの生成
			CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_FIN);

			//タイマーを透明にする
			m_pNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		if (m_nDeathGameCount >= 190)
		{
			//初期化する
			m_nDeathGameCount = 0;
			m_bTimeStop = false;

			//タイマーが0になったらリザルトにフェードする
			CFade::SetFade(CManager::MODE_RESULT);

			//タイマーの終了処理
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameTimer::Draw(void)
{
	//変数宣言
	int nCntNumber;

	if (m_nTime <= 5)
	{//ゲームタイマーが5秒になったら描画する
		for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->Draw();
			}
		}
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CGameTimer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}

