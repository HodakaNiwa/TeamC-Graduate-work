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
// 前方宣言
//=============================================================================
class CInputKeyboard;
class CPlayer;
class CModel;
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
	void ReSetCamera(void);								//カメラの設定
	int GetViewAngle(void) { return m_nViewAngle; };	//画角の取得
	bool ReflectObject(D3DXVECTOR3 ObjectPos);				//カメラに写っているかどうかの判定

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
	float	m_fOldHight;	//前回の高さ
	bool	m_bSetRdest;
	TYPE	m_type;
	CPlayer *m_pTargetPlayer;
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
#endif