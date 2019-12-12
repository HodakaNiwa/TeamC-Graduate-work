//=============================================================================
//
// パワー型の処理 [powerplayer.cpp]
// Author : 山下敦史
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "rawmouse.h"
#include "UI.h"
#include "skilicon.h"
#include "effectManager.h"
#include "debuglog.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (5)		//5
#define ROT_COUNTER (20)
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPowerPlayer::CPowerPlayer()
{
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_nRotCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bInit = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPowerPlayer::~CPowerPlayer()
{

}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//モデルの取得
	m_pModel = CCharacter::GetModel();  

	//ライン変数の初期化
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;
	m_pLoadEffect = NULL;

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}
	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z), 5);
	}
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	//初期化
	m_fSpeed = 1.0f;
	m_bWalk = true;
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSuperArmor = false;
	m_nInitCnt = 0;
	m_fInertia = GetInertia();			//慣性
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRotCounter = 0;

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CPowerPlayer::Uninit(void)
{
	//プレイヤーの破棄
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CPowerPlayer::Update(void)
{
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得
	
	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)	// イベントカメラが消えていたら
		{
			//アクションの更新
			ActionUpdate();
		}
	}
	//プレイヤーの更新
	CPlayer::Update();
}

//=============================================================================
// アクションの更新処理
//=============================================================================
void  CPowerPlayer::ActionUpdate(void)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	if (m_nControllerType == 0)
	{
		//コントローラー操作
		PlayerActionPad();
	}
	else if (m_nControllerType == 1)
	{
		//マウス操作
		PlayerActionMouse();
	}
	else
	{
		//キーボード操作
		PlayerActionKeyboard();
	}
	
	//アクション中
	if (m_bAction == true)
	{
		pSound->StopSound(CSound::SOUND_LABEL_SE018);	//足音
		m_nColliderCnt++;
		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{//

				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_nColliderTimer == 1)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE022);
					pSound->SetVolume(CSound::SOUND_LABEL_SE022, 20.0f);
					CreateColliderSphere();	//衝撃波の当たり判定を付与
				}
				if (m_nColliderTimer == 2)
				{
					m_bSuperArmor = false;
					m_bInit = true;
				}
			}
		}
	}

	if (m_bInit == true)
	{
		//値の初期化
		InitNum();
	}

	//リキャスト中
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		if (m_nRecastCounter == (60 * RECASTTIME))
		{
			m_bRecast = false;					//リキャスト終了
			m_bAction = false;					//アクションを使用できる
			m_nButtonCounter = 0;				//アクションボタンが使用できる
			m_nRecastCounter = 0;
		}
	}
}

//=============================================================================
// 値の初期化処理
//=============================================================================
void CPowerPlayer::InitNum(void)
{
	if (m_PlayerState != PLAYERSTATE_DAMAGE)
	{
		m_PlayerState = PLAYERSTATE_NONE;
	}

	m_nInitCnt = 0;
	m_nColliderTimer = 0;	//タイマーを初期化
	m_nColliderCnt = 0;
	m_bRecast = true;		//リキャスト中にする	
	m_bAction = false;		//アクションを終了
	m_bInit = false;
	m_nRotCounter = 0;

	if (m_bAction == false)
	{//アクションが終わったらスーパーアーマを解除
	}

	CDebugProc::Print("初期化 : %d\n", m_nInitCnt);
}

//=============================================================================
// PAD処理
//=============================================================================
void  CPowerPlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//ゲームカメラの取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//向きの取得
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0 
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//アクション中にする
			m_nButtonCounter = 1;							//アクションボタンを使用できないようにする
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//攻撃モーション
			m_bSuperArmor = true;
		}
	}

	//コントローラー（XInput）
	float fRot = pXPad->GetStickRot(0, m_nControllerIndx);				//スティックの取得
	if (pXPad->GetStick(0, m_nControllerIndx) == true)
	{//アクション中に向きを変えれる
		if (m_bAction == true)
		{
			m_nRotCounter++;
			if (m_nRotCounter <= ROT_COUNTER)
			{
				m_Angle.y = rot.y + fRot;
			}
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CPowerPlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//ゲームカメラの取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//向きの取得
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	//マウスの角度を取得
	int MouseX = pRawMouse->GetMouse(m_nControllerIndx).lLastX;
	int MouseY = pRawMouse->GetMouse(m_nControllerIndx).lLastY;
	//トラックボール用
	float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);

	if (pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0  
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//アクション中にする
			m_nButtonCounter = 1;							//アクションボタンを使用できないようにする
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//攻撃モーション
			m_bSuperArmor = true;
		}
	}

	if (MouseX != 0 || MouseY != 0)
	{//マウス
		if (m_bAction == true)
		{//アクション中に向きを変えれる
			m_nRotCounter++;
			if (m_nRotCounter <= ROT_COUNTER)
			{
				m_Angle.y = rot.y + Mouse;
			}
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CPowerPlayer::PlayerActionKeyboard(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//ゲームカメラの取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//向きの取得

	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//アクション中にする
			m_nButtonCounter = 1;							//アクションボタンを使用できないようにする
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//攻撃モーション
			m_bSuperArmor = true;
		}
	}

	if (m_bAction == true)
	{//アクション中に向きを変えれる
		m_nRotCounter++;
		if (m_nRotCounter <= ROT_COUNTER)
		{
			//プレイヤーの移動処理（キーボード）
			if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//ポリゴンを左上に移動
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//ポリゴンを左下に移動

					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//ポリゴンを左に移動
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
			{

				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//ポリゴンを右上に移動

					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//ポリゴンを右下に移動

					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//ポリゴンを右に移動

					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
			{
				m_Angle.y = rot.y;
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
			{
				m_Angle.y = D3DX_PI + rot.y;
			}
		}
	}
}

//=============================================================================
// コントローラーの向きの処理処理
//=============================================================================
void  CPowerPlayer::RotAction(int nType)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void  CPowerPlayer::Draw(void)
{
	//プレイヤーの描画
	CPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPowerPlayer *CPowerPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//メモリを動的確保
		pPowerplayer = new CPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULLチェック
			pPowerplayer->SetType(nType);									//ゲーム用国タイプ
			pPowerplayer->m_nCuntry[nNumPlayer] = nType;					//リザルト用の国タイプ
			pPowerplayer->m_CharcterType = charctertype;					//ゲーム用のキャラタイプ
			pPowerplayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//リザルト用のキャラタイプ
			pPowerplayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer,nType);		//プレイヤーの初期化
			pPowerplayer->m_nControllerIndx = nControllerIndx;				//使用してるコントローラーの番号
			pPowerplayer->m_nControllerType = nControllerType;				//使用してるコントローラーのタイプ
		}
		else
		{
			MessageBox(0, "プレイヤーがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "プレイヤーがNULLじゃないです", "警告", MB_OK);
	}
	return pPowerplayer;
}

//=============================================================================
//
//=============================================================================
void  CPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CPowerPlayer::CreateColliderSphere(void)
{
	//ゲームの取得
	CGame *pGame = CManager::GetGame();
	//エフェクト
	CEffectManager *pEffectManager = pGame->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100,pos.y,pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);

}
