//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_CAMERAPOS	(4)

//=============================================================================
// 前方宣言
//=============================================================================
class CInputKeyboard;
class CPlayer;
class CModel;
class CTerritory;
class CRobot;
class CCharacter;

//=============================================================================
// カメラクラス
//=============================================================================
class CCamera
{
public:
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	void SetViewport(D3DVIEWPORT9 viewport);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetPosV(void) { return m_posV; };		//視点の取得
	D3DXVECTOR3 GetPosR(void) { return m_posR; };		//注視点の取得
	void SetPosV(D3DXVECTOR3 PosV) { m_posV = PosV; };	//視点の取得
	void SetPosR(D3DXVECTOR3 PosR) { m_posR = PosR; };	//視点の取得
	void ReSetCamera(void);								//カメラの設定
	int GetViewAngle(void) { return m_nViewAngle; };	//画角の取得
	bool ReflectObject(D3DXVECTOR3 ObjectPos);			//カメラに写っているかどうかの判定

protected:

	D3DXVECTOR3 m_posV;				//視点
	D3DXVECTOR3 m_posVDest;			//目的の視点
	D3DXVECTOR3 m_posR;				//注視点
	D3DXVECTOR3 m_posRDest;			//目的の注視点
	D3DXVECTOR3 m_vecU;				//上方向ベクトル
	D3DXVECTOR3 m_Rot;				//向き
	D3DXVECTOR3 m_RotDest;			//目的の向き
	D3DXMATRIX  m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX  m_mtxView;			//ビューマトリクス
	float		m_Lngth;			//距離
	float		m_Lngth2;			//変更距離
	float		m_fAngle;			//角度
	int			m_nViewAngle;		//画角
	bool		m_bRot;				//回転しているかどうか
	bool        m_bHight;			//高さが変化したかどうか
	D3DVIEWPORT9 m_Viewport;

private:
	void RemakeAngle(float * pAngle);
	void DebugUpdate(CInputKeyboard * pInputKeyboard);

	D3DXVECTOR3 m_VecA[MAX_CAMERAPOS];
};


//=============================================================================
// 回転カメラクラス
//=============================================================================
class CRotationCamera : public CCamera
{
public:    // 誰でもアクセス可能
	CRotationCamera();
	~CRotationCamera();

	static CRotationCamera *Create(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength);

	HRESULT Init(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength);
	void Uninit(void);
	void Update(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	float GetRotSpeed(void) { return m_fRotSpeed; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fRotSpeed;
};


//=============================================================================
// ゲームカメラクラス
//=============================================================================
class CGameCamera : public CCamera
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_CLEAR,
	}TYPE;

	CGameCamera();
	~CGameCamera();
	void Init(CPlayer *pPlayer = NULL);
	void Uninit(void);
	void Update(void);
	void Set(void);
	void SetType(TYPE type) { m_type = type; };

private:
	void TypeNormal(void);
	void TypeClear(void);
	bool CheckTerritoryTopInScreen(CTerritory *pTerritoryTop, D3DXVECTOR3 &pos);
	void CutAddValue(float fCutHeight, float fCutDepth);
	float	m_fOldHight;	//前回の高さ
	bool	m_bSetRdest;
	TYPE	m_type;
	CPlayer *m_pTargetPlayer;
	float   m_fAddHeight;	// 高さに加える値
	float   m_fAddDepth;	// 奥行に加える値
};

//=============================================================================
// タイトルカメラクラス
//=============================================================================
class CTitleCamera : public CCamera
{
public:
	//カメラのアングルタイプ
	typedef enum
	{
		TYPE_00,
		TYPE_01,
		TYPE_MAX,
	}TYPE;

	CTitleCamera();
	~CTitleCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	int m_nCounterTime;	//時間のカウンター
	TYPE m_type;		//カメラのタイプ
	bool m_bSetCamera;	//カメラの位置を設定したかどうか
};

//=============================================================================
// リザルトカメラクラス
//=============================================================================
class CResultCamera : public CCamera
{
public:
	typedef enum
	{
		STATE_CHAR_IN = 0,
		STATE_CHAR_ZOOM,
		STATE_CHAR_ZOOMOUT,
		STATE_CHAR_STOP,
	}STATE;

	CResultCamera();
	~CResultCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);
	void SetTargetPos(D3DXVECTOR3 Pos) { m_TargetCharPos = Pos; }
	void SetState(STATE state) { m_state = state; }
	STATE GetState(void) { return m_state; }

private:
	void UpdateCharIn(void);
	void UpdateCharZoom(void);
	void UpdateCharZoomOut(void);

	D3DXVECTOR3 m_EndPosV;			//最終始点
	D3DXVECTOR3 m_EndPosR;			//最終注視点
	D3DXVECTOR3 m_ZoomOutPos;		//ズームアウト用の視点
	D3DXVECTOR3 m_TargetCharPos;	//ターゲットキャラの位置
	D3DXVECTOR3 m_MoveV;			//視点の移動量
	D3DXVECTOR3 m_MoveR;			//注視点の移動量
	int m_nCountTime;				//時間カウンター
	STATE m_state;					//更新状態
};

//=============================================================================
// ランキングカメラクラス
//=============================================================================
class CRankingCamera : public CCamera
{
public:
	CRankingCamera();
	~CRankingCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	float m_fSpeed;
	float m_fCountDistance;
	float m_fMove;
};

//=============================================================================
// イベントカメラクラス←追加(よしろう)
//=============================================================================
class CEventCamera : public CCamera
{
public:
	typedef enum
	{
		TYPE_ROBOT = 0,
		TYPE_AREABONUS_START,
		TYPE_AREABONUS_FINISH,
		TYPE_DIVISION_START,
		TYPE_DIVISION_FINISH,
		TYPE_MAX
	}TYPE;

	CEventCamera();
	~CEventCamera();
	void Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CCharacter *pChar = NULL, TYPE type = TYPE_ROBOT);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	void RobotEvent(void);
	void AreaBonusEvent_Start(void);
	void AreaBonusEvent_Finish(void);
	void DivisionEvent_Start(void);
	void DivisionEvent_Finish(void);

	TYPE 		m_Type;
	CCharacter	*m_pTargetRobot;
	int			m_nCounter;
};
#endif