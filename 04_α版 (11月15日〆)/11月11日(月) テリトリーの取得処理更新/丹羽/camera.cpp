//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "gamepad.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "sceneX.h"
#include "model.h"
#include "model.h"
#include "character.h"
#include "library.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DRAW_CAMERA		(3500.0f)	//カメラの描画範囲
#define CAMERA_SPEED	(2.0f)		//カメラのスピード
#define LENGTH_X		(0.0f)		//視点と注視点の距離（Ｘ）
#define LENGTH_Y		(500.0f)	//視点と注視点の距離（Ｙ）30 700.0f
#define LENGTH_Z		(180.0f)	//視点と注視点の距離（Ｚ）150
#define POSR_Y			(35.0f)		//注視点の高さ
#define BEHIND_TIME		(60)		//プレイヤーの後ろにカメラが回り込む時間
#define BEHIND_SPEED	(0.01f)		//カメラの回り込むときの慣性
#define VIEW_ANGLE		(45)		//画角

//タイトルカメラ
#define TITILE_LENGTH_X (0.0f)
#define TITILE_LENGTH_Y (90.0f)
#define TITILE_LENGTH_Z (200.0f)
#define MAX_HIGHT		(110.0f)
#define MIN_HIGHT		(15.0f)
#define CHANGE_TIME		(60 * 4)	//カメラの標示時間
#define MOVE_Y			(0.5f)		//カメラの移動量
#define MOVE_Z			(1.5f)		//移動量

//ゲームカメラ
#define MOVE_CLEAR		(0.2f)		//クリア時の移動量


//ランキングカメラ
#define WINDY			(0.05f)		//慣性
#define SPEED			(0.3f)		//カメラの移動速度
#define MOVE			(20.0f)		//移動範囲
//=============================================================================
//	コンストラクタ
//=============================================================================
CCamera::CCamera(){}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera(){}

//=============================================================================
// カメラの初期化処理
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(LENGTH_X, LENGTH_Y, -LENGTH_Z);	//視点
	m_posVDest = D3DXVECTOR3(0, 0, 0);						//目的の視点
	m_posR = D3DXVECTOR3(0, POSR_Y, 0);						//注視点
	m_posRDest = D3DXVECTOR3(0, 0, 0);						//目的の視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					//上方向ベクトル
	m_Rot =	D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);				//向き
	m_RotDest = D3DXVECTOR3(0, 0, 0);						//目的の向き
	m_Lngth = LENGTH_Z;										//長さ
	m_fAngle = 0.0f;										//角度
	m_nViewAngle = VIEW_ANGLE;								//画角
	m_bRot = false;											//カメラが回転しているかどうか
	m_bHight = false;										//高さが変化したかどうか
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	//カメラの高さが変化していない状態にする
	m_bHight = false;

	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得

	//デバッグキーの更新処理
	DebugUpdate(pInputKeyboard);

	if ((pInputKeyboard->GetKeyboardPress(DIK_Z) == true) || (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_RIGHT) == true))	 //Zキー(下)が押されたかどうか
	{
		//カメラの回転量
		m_RotDest.y += 0.01f;

		//目的の角度修正
		RemakeAngle(&m_RotDest.y);

		//回転している状態にする
		m_bRot = true;
	}
	if ((pInputKeyboard->GetKeyboardPress(DIK_C) == true) || (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_LEFT) == true))	 //Cキー(下)が押されたかどうか
	{
		//カメラの回転量
		m_RotDest.y -= 0.01f;

		//目的の角度修正
		RemakeAngle(&m_RotDest.y);

		//回転している状態にする
		m_bRot = true;
	}

	if (m_bRot == true)
	{
		m_Rot.y += m_RotDest.y;

		//目的の角度修正
		RemakeAngle(&m_Rot.y);

		//注視点更新
		m_posV.x = m_posR.x - sinf(m_Rot.y - D3DX_PI) * m_Lngth;
		m_posV.z = m_posR.z - cosf(m_Rot.y - D3DX_PI) * m_Lngth;

		//減速
		m_RotDest.y += (0.0f - m_RotDest.y) * 0.4f;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::Set()
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 描画領域を設定
	pDevice->SetViewport(&m_Viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(m_nViewAngle),							//カメラの画角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								DRAW_CAMERA);										//カメラの描画範囲

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	// ビューマトリックスを作成
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// 描画領域の設定処理
//=============================================================================
void CCamera::SetViewport(D3DVIEWPORT9 viewport)
{
	m_Viewport = viewport;
}


//=============================================================================
// カメラに写っているかどうかの判定処理
//=============================================================================
bool CCamera::ReflectObject(D3DXVECTOR3 ObjectPos)
{
	//変数宣言
	D3DXVECTOR3 Pos[4];
	D3DXVECTOR3 VecA[4];
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	int nCount = 0;

	//４頂点の位置を求める
	Pos[0] = m_posV + D3DXVECTOR3(-500.0f, 0.0f, 450.0f);
	Pos[1] = m_posV + D3DXVECTOR3(-400.0f, 0.0f, -200.0f);
	Pos[2] = m_posV + D3DXVECTOR3(450.0f, 0.0f, -200.0f);
	Pos[3] = m_posV + D3DXVECTOR3(500.0f, 0.0f, 450.0f);

	//VecAを求める
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % 4) == 0) { nCnt = 0; }

		VecA[nCntVec] = Pos[nCnt] - Pos[nCntVec];
	}

	//範囲チェック
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{
		VecB = ObjectPos - Pos[nCntVec];						//画角の終点と位置のベクトルを求める
		float fCross = (VecA[nCntVec].z * VecB.x) - (VecA[nCntVec].x * VecB.z);	//外積を求める

		//ベクトルより小さいかどうか
		if (fCross <= 0)
		{
			nCount++;
		}
		else
		{
			break;
		}
	}

	//範囲内に入っているかどうか
	if (nCount >= 4) { bDraw = true; }

	return bDraw;
}

//=============================================================================
// カメラの初期化位置に設定
//=============================================================================
void CCamera::ReSetCamera(void)
{
	m_posV = D3DXVECTOR3(LENGTH_X, LENGTH_Y, -LENGTH_Z);	//視点
	m_posR = D3DXVECTOR3(0, POSR_Y, 0);						//注視点
}

//=============================================================================
// 角度修正
//=============================================================================
void CCamera::RemakeAngle(float * pAngle)
{
	//角度の修正
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// カメラの回転を取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
// デバッグ用のカメラのキー
//=============================================================================
void CCamera::DebugUpdate(CInputKeyboard * pInputKeyboard)
{
	//カメラの動作処理
#if 1
	if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)		 //Aキー(左)が押されたかどうか
	{
		//視点更新
		m_posV.x += sinf(D3DX_PI * -0.5f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI * -0.5f + m_Rot.y) * CAMERA_SPEED;

		//注視点更新
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)	 //Dキー(右)が押されたかどうか
	{
		//視点更新
		m_posV.x += sinf(D3DX_PI * 0.5f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f + m_Rot.y) * CAMERA_SPEED;

		//注視点更新
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)	 //Wキー(上)が押されたかどうか
	{
		//視点更新
		m_posV.x += sinf(0.0f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(0.0f + m_Rot.y) * CAMERA_SPEED;

		//注視点更新
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)	 //Sキー(下)が押されたかどうか
	{
		//視点更新
		m_posV.x += sinf(D3DX_PI + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI + m_Rot.y) * CAMERA_SPEED;

		//注視点更新
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}

	if (pInputKeyboard->GetKeyboardPress(DIK_Y) == true)	 //Zキー(下)が押されたかどうか
	{
		//視点更新
		m_posV.y += 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_N) == true)	 //Cキー(下)が押されたかどうか
	{
		//視点更新
		m_posV.y -= 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_T) == true)	 //Zキー(下)が押されたかどうか
	{
		//視点更新
		m_posV.y += 1.0f;

		//注視点更新
		m_posR.y += 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_B) == true)	 //Cキー(下)が押されたかどうか
	{
		//視点更新
		m_posV.y -= 1.0f;

		//注視点更新
		m_posR.y -= 1.0f;
	}

#endif
}

//*****************************************************************************
//
// 回転カメラクラス
//
//*****************************************************************************
//=============================================================================
//	コンストラクタ
//=============================================================================
CRotationCamera::CRotationCamera()
{
	m_fRotSpeed = 0.0f;
}

//=============================================================================
//	デストラクタ
//=============================================================================
CRotationCamera::~CRotationCamera()
{

}

//=============================================================================
//	生成処理
//=============================================================================
CRotationCamera *CRotationCamera::Create(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength)
{
	// メモリ確保
	CRotationCamera *pRotCamera = new CRotationCamera;
	if (pRotCamera == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pRotCamera->Init(posVHeight, posR, fRotSpeed, fLength)))
	{
		pRotCamera->Uninit();
		delete pRotCamera;
		return NULL;
	}

	return pRotCamera;  // インスタンスのアドレスを返す
}

//=============================================================================
//	初期化処理
//=============================================================================
HRESULT CRotationCamera::Init(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength)
{
	// 各種値の設定
	m_posV.y = posVHeight;
	m_posVDest.y = posVHeight;
	m_posR = posR;
	m_fRotSpeed = fRotSpeed;
	m_Lngth = fLength;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nViewAngle = VIEW_ANGLE;

	// 視点の位置を計算
	m_posV.x = sinf(m_Rot.y) * m_Lngth;
	m_posV.z = cosf(m_Rot.y) * m_Lngth;

	return S_OK;
}

//=============================================================================
//	終了処理
//=============================================================================
void CRotationCamera::Uninit(void)
{

}

//=============================================================================
//	更新処理
//=============================================================================
void CRotationCamera::Update(void)
{
	// 回転させる
	m_Rot.y += m_fRotSpeed;
	m_Rot.y = CFunctionLib::CheckRotationOverPiX(m_Rot.y);
	m_Rot.y = CFunctionLib::CheckRotationOverMinusPiX(m_Rot.y);

	// 視点の位置を計算
	m_posV.x = m_posR.x - sinf(m_Rot.y) * m_Lngth;
	m_posV.z = m_posR.z - cosf(m_Rot.y) * m_Lngth;
}


//*****************************************************************************
//
// ゲームカメラクラス
//
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
CGameCamera::CGameCamera()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGameCamera::~CGameCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CGameCamera::Init(CPlayer *pPlayer)
{
	CCamera::Init();

	//変数の初期化
	m_pTargetPlayer = pPlayer;
	m_fOldHight = 0.0f;
	m_type = TYPE_NORMAL;
	m_bSetRdest = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameCamera::Update(void)
{
	if (m_type == TYPE_NORMAL)
	{
		CCamera::Update();
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CGameCamera::Set()
{
	CCamera::Set();

	switch (m_type)
	{
	case TYPE_NORMAL: TypeNormal(); break;
	case TYPE_CLEAR:  TypeClear();  break;
	}
}

//=============================================================================
// ノーマル状態の処理
//=============================================================================
void CGameCamera::TypeNormal(void)
{
	//変数宣言
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerOldPos;
	D3DXVECTOR3 PlayerRot;

	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	int nTypePlayer = m_pTargetPlayer->GetPlayerType();

	if (m_pTargetPlayer != NULL)
	{
		PlayerPos = m_pTargetPlayer->GetPos();
		PlayerOldPos = m_pTargetPlayer->GetPos();
		PlayerRot = m_pTargetPlayer->GetRot();
	}

	if (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_UP) == true)
	{
		//視点更新
		m_posV.y += 1.0f;
		m_bHight = true;

		if (m_posV.y > (MAX_HIGHT + PlayerPos.y))
		{
			m_posV.y = MAX_HIGHT + PlayerPos.y;
			m_bHight = false;
		}
	}
	if (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_DOWN) == true)
	{
		//視点更新
		m_posV.y -= 1.0f;
		m_bHight = true;

		if (m_posV.y < MIN_HIGHT + PlayerPos.y)
		{
			m_posV.y = MIN_HIGHT + PlayerPos.y;
			m_bHight = false;
		}
	}

	if (PlayerPos.y == 0.0f)
	{
		m_fOldHight = m_posV.y;		//高さを保存する
	}
	else
	{
		m_fOldHight = m_posV.y - PlayerPos.y;		//高さを保存する
	}

	//カメラの追尾処理
	//目的の注視点を求める
	m_posRDest.x = PlayerPos.x + sinf(PlayerRot.y) * -25.0f;
	m_posRDest.y = PlayerPos.y + POSR_Y;
	m_posRDest.z = PlayerPos.z + cosf(PlayerRot.y) * -25.0f;

	//目的の視点を求める
	m_posVDest.x = PlayerPos.x + sinf(m_Rot.y) * m_Lngth;
	m_posVDest.y = PlayerPos.y + m_fOldHight;
	m_posVDest.z = PlayerPos.z + cosf(m_Rot.y) * m_Lngth;

	//注視点の更新
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.245f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.245f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.245f;

	//視点の更新
	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.3f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;
}

//=============================================================================
// クリア状態の処理
//=============================================================================
void CGameCamera::TypeClear(void)
{
	if (m_bSetRdest == false)
	{
		m_bSetRdest = true;
		m_posRDest = D3DXVECTOR3(m_posRDest.x, m_posRDest.y + 25.0f, m_posRDest.z);
	}

	m_posR.y += MOVE_CLEAR;

	if (m_posR.y > m_posRDest.y)
	{
		m_posR.y = m_posRDest.y;
	}
}

//*****************************************************************************
//
// タイトルカメラクラス
//
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
CTitleCamera::CTitleCamera()
{
	m_nCounterTime = 0;		//カメラの時間カウンター
	m_type = TYPE_00;		//カメラのアングルタイプ
	m_bSetCamera = false;	//カメラの位置を設定したかどうか
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitleCamera::~CTitleCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CTitleCamera::Init(void)
{
	CCamera::Init();

	m_posV = D3DXVECTOR3(TITILE_LENGTH_X, TITILE_LENGTH_Y, -350.0f);	//視点
	m_posR = D3DXVECTOR3(0, 38, 0);												//注視点
	m_bSetCamera = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitleCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleCamera::Update(void)
{
	CCamera::Update();

	m_nCounterTime++;

	if (m_nCounterTime > CHANGE_TIME)
	{
		m_nCounterTime = 0;
		m_type = (TYPE)((m_type + 1) % TYPE_MAX);	//タイプの変更
		m_bSetCamera = false;
	}

	switch (m_type)
	{
	case TYPE_00:
		if (m_bSetCamera == false)
		{
			m_posV = D3DXVECTOR3(TITILE_LENGTH_X, TITILE_LENGTH_Y, -TITILE_LENGTH_Z);	//視点
			m_posR = D3DXVECTOR3(0, 38, 0);												//注視点
			m_bSetCamera = true;
		}
		m_posV.y += MOVE_Y;
		m_posR.y += MOVE_Y;
		break;

	case TYPE_01:
		if (m_bSetCamera == false)
		{
			m_posV = D3DXVECTOR3(TITILE_LENGTH_X, 150, -500);	//視点
			m_posR = D3DXVECTOR3(0, -38, 0);					//注視点
			m_bSetCamera = true;
		}

		m_posV.z += MOVE_Z;
		m_posR.z += MOVE_Z;
		break;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CTitleCamera::Set(void)
{
	CCamera::Set();
}

//*****************************************************************************
//
// ランキングカメラクラス
//
//*****************************************************************************

//=============================================================================
//	コンストラクタ
//=============================================================================
CRankingCamera::CRankingCamera()
{
	m_fSpeed = SPEED;
	m_fCountDistance = 0.0f;
	m_fMove = MOVE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRankingCamera::~CRankingCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CRankingCamera::Init(void)
{
	CCamera::Init();

	m_posV = D3DXVECTOR3(500, TITILE_LENGTH_Y, 000);	//視点
	m_posR = D3DXVECTOR3(500, 10, 300);												//注視点
}

//=============================================================================
// 終了処理
//=============================================================================
void CRankingCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRankingCamera::Update(void)
{
	CCamera::Update();

	//カメラの位置更新
	m_posV.z += m_fMove;
	m_posR.z += m_fMove;

	//カメラに慣性を付ける
	m_fMove += (0.0f - m_fMove) * WINDY;
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CRankingCamera::Set(void)
{
	CCamera::Set();
}