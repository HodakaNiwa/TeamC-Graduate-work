//=============================================================================
//
// タイマーの処理 [timer.cpp]
// Author : 山下敦史 & 佐藤安純
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
#define MAX_TIME		(180) //180
#define TIMERLOGO_SIZE (D3DXVECTOR3(200.0f, 80.0f, 0.0f))
#define TIMERLOGO_SIZE2 (D3DXVECTOR3(125.0f, 50.0f, 0.0f))

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//タイマークラス
int CTimer::m_nTimeData[MAX_TIMENUMBER] = {};
int CTimer::m_nTime = 0;

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
CTimer::CTimer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType) 
{
	m_size = INITIALIZE_VECTOR3;
	m_pos = INITIALIZE_VECTOR3;
}

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
		m_nTimeData[nCntNum] = (m_nTime % nNum) / nNum2;	//格納する数字を計算

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
CCountDownTimer::CCountDownTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority, objType) 
{
	m_nCounter = 0;
	m_nCountDown = 0;
	m_nCountDown = 0;
	m_bDeath = false;
	m_bCountStop = false;

	for (int nCnt = 0; nCnt < MAX_TIMENUMBER; nCnt++)
	{
		m_pNumber[nCnt] = NULL;
		m_nNumber[nCnt] = 0;
	}
}

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
	m_nCountDown = 4;
	m_nCounter = 0;
	m_nDeathCount = 0;
	m_bDeath = false;
	m_bCountStop = false;
	AddTime(3);
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

	CGame *pGame = CManager::GetGame();				// ゲームの取得←追加(よしろう)
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得←追加(よしろう)
	if (pEveCam == NULL)	// イベントカメラが消えていたら←追加(よしろう)
	{
		//カウントダウンの更新
		TimeCountUpdate();
	}

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
{//安
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	int nState = CGame::GetGameState();

	m_nCounter++;

	if (m_nCounter % 60 == 0)
	{
		m_nCountDown--;
		CTimer::AddTime(m_nCountDown);

		//カウントダウン時の音（開始時）
		pSound->PlaySound(CSound::SOUND_LABEL_SE010);

		if (m_nCountDown == 0)
		{
			//カウントダウン時の音（開始時）
			pSound->StopSound(CSound::SOUND_LABEL_SE010);
			//初期化する
			m_nCountDown = 0;
			m_bDeath = true;

			//ゲーム状態にする
			CGame::SetGameState(CGame::GAMESTATE_NORMAL);
		}
	}


	if (m_bDeath == true)
	{
		CTimer::AddTime(1);

		m_nDeathCount++;

		if (m_nDeathCount == 5)
		{
			//スタート開始音
			pSound->PlaySound(CSound::SOUND_LABEL_SE011);
			//初期化する
			m_nDeathCount = 0;
			m_bDeath = false;

			//タイマーの終了処理
			Uninit();

			//スタートロゴの生成 
			CTimerLogo::Create(m_pos, TIMERLOGO_SIZE2, CTimerLogo::TIMERLOGO_START);
		}
	}
	//音量の設定
	pSound->SetVolume(CSound::SOUND_LABEL_SE010, 50.0f);
	pSound->SetVolume(CSound::SOUND_LABEL_SE011, 30.0f);
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
CGameTimer::CGameTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority, objType)
{
	m_nTimeCounter = 0;
	m_nTime = 0;
	m_nDeathGameCount = 0;
	m_FirstSize = INITIALIZE_VECTOR3;
	m_LogoSize = INITIALIZE_VECTOR3;
	m_bDeth = false;
	m_bTimeStop = false;

	for (int nCnt = 0; nCnt < MAX_TIMENUMBER; nCnt++)
	{
		m_pNumber[nCnt] = NULL;
		m_nNumber[nCnt] = 0;
	}
}

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
	m_FirstSize = INITIALIZE_VECTOR3;	//安
	m_bDeth = false;	//安
	Set(m_pos, m_size);
	m_LogoSize = INITIALIZE_VECTOR3;	//ロゴのサイズ　安

	//ロゴのサイズを設定する　安
	int nNumPlayer = CManager::GetGame()->GetNumPlay();

	if (nNumPlayer < 2)
	{
		m_LogoSize = TIMERLOGO_SIZE;
	}
	else
	{
		m_LogoSize = TIMERLOGO_SIZE2;
	}


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
{//安
	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	int nState = CGame::GetGameState();

	if (m_bTimeStop == true)
	{
		//ゲームを終了状態にする
		CGame::SetGameState(CGame::GAMESTATE_END);

		CTimer::AddTime(1);

		m_nDeathGameCount++;

		if (m_nDeathGameCount == 5)
		{
			//ゲーム終了音
			pSound->PlaySound(CSound::CSound::SOUND_LABEL_SE014);

			//フィニッシュロゴの生成
			CTimerLogo::Create(m_pos, m_LogoSize, CTimerLogo::TIMERLOGO_FIN);

			m_pNumber[0]->SetColor(INITIALIZE_VECTOR4);
		}
		if (m_nDeathGameCount >= 190)
		{
			//初期化する
			m_nDeathGameCount = 0;
			m_bTimeStop = false;

			//タイマーが0になったらリザルトにフェードする
			CFade::SetFade(CManager::MODE_RESULT);

			//タイマーの終了処理
			m_bDeth = true;
		}
	}

	if (nState == CGame::GAMESTATE_NORMAL)
	{//ゲーム状態の時
		m_nTimeCounter--;

		if (m_nTimeCounter % 60 == 0)
		{
			m_nTime--;
			CTimer::AddTime(m_nTime);

			if (m_nTime == 120)
			{//残り2分になったらテクスチャを生成
				pSound->PlaySound(CSound::CSound::SOUND_LABEL_SE012);	//タイムアナウンス
				CTimerLogo::Create(m_pos, m_LogoSize, CTimerLogo::TIMERLOGO_2MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_2);
			}
			if (m_nTime == 115)
			{//残り1分55秒になったらイベントを生成
				StartUpEvent();
			}
			if (m_nTime == 60)
			{//残り1分になったらテクスチャを生成
				pSound->PlaySound(CSound::SOUND_LABEL_SE012);	//タイムアナウンス
				pSound->PlaySound(CSound::SOUND_LABEL_BGM037);
				//pSound->SetVolume(CSound::SOUND_LABEL_BGM037, 5.0f);
				pSound->StopSound(CSound::SOUND_LABEL_BGM001);
				CTimerLogo::Create(m_pos, m_LogoSize, CTimerLogo::TIMERLOGO_1MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_1);
			}
			if (m_nTime == 55)
			{//残り55秒になったらテクスチャを生成
				AppearRobot();
			}
			if (m_nTime == 30)
			{ //残り30秒になったらテクスチャを生成
				pSound->PlaySound(CSound::CSound::SOUND_LABEL_SE012);	//タイムアナウンス
				CTimerLogo::Create(m_pos, m_LogoSize, CTimerLogo::TIMERLOGO_30SEC);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_30);
			}
			if (m_nTime <= 5 && m_nTime >= 1)
			{//残り5秒でSEを鳴らす
				pSound->PlaySound(CSound::CSound::SOUND_LABEL_SE013);	//カウントダウン(終了時)

				//安
				for (int nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
				{//タイマーの色を変える
					if (m_pNumber[nCntNumber] != NULL)
					{
						m_pNumber[nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
				}
			}
			if (m_nTime == 0)
			{
				//値を初期化する
				m_bTimeStop = true;
				m_nTime = 0;
			}
		}
	}

	if (m_bDeth == true) { Uninit(); }
}

//=============================================================================
// イベントを起動する処理
//=============================================================================
void CGameTimer::StartUpEvent(void)
{
	// ゲームクラスの取得
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	// どちらかのイベントが起動していたら処理しない
	if (pGame->GetDivisionEventFlag() == true || pGame->GetAreaBonusEventFlag() == true) { return; }

	// 起動させるイベントをランダムで設定
	int nRandom = rand() % CGame::GAMEEVENT_MAX;

	switch (nRandom)
	{// ランダムの値によって処理わけ
	case CGame::GAMEEVENT_AREABONUS:
		pGame->StartAreaBonusEvent();
		break;
	case CGame::GAMEEVENT_DIVISION:
		pGame->StartDivisionEvent();
		break;
	}
}

//=============================================================================
// ロボットを出現させる処理
//=============================================================================
void CGameTimer::AppearRobot(void)
{
	// ゲームクラスの取得
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }
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
{//安
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_FirstSize = size /** 4.0f*/;
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (m_FirstSize.x * 2)), pos.y, pos.z), m_FirstSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}