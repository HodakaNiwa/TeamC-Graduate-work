//=============================================================================
//
// タイトル処理 [title.h]
// Author : 佐藤安純
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"
#include "modebase.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLoadTextObject;
class CLoadTextTerritory;
class CRotationCamera;
class CFieldManager;

//=====================
// 基本クラス
//=====================
class CTitle : public CModeBase
{
public:
	typedef enum
	{
		SOUNDSTATE_NONE = 0,
		SOUNDSTATE_01,
		SOUNDSTATE_02,
		SOUNDSTATE_MAX
	}SOUNDSTATE;
	CTitle();								// コンストラクタ
	~CTitle();								// デストラクタ
	void	Init(void);						// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	void	Draw(void);						// 描画処理

private:
	//--------------------
	//	関数
	//--------------------
	void CreateTitleGear(void);
	void CreateCamera(void);

	//--------------------
	//	メンバ変数
	//--------------------
	CLoadTextObject *m_pLoadObject;			// オブジェクトの読み込み
	CLoadTextTerritory *m_pLoadTerritory;	// テリトリーの読み込み
	CRotationCamera *m_pRotationCamera;		// 回転カメラ
	CFieldManager *m_pFieldManager;			// 地面管轄クラス
	SOUNDSTATE m_SoundState;
};


//================================
// タイトルロゴ用の歯車クラス
//================================
class CTitleGear2D : public CScene2D
{
public:     // 誰でもアクセス可能
	CTitleGear2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CTitleGear2D();

	static CTitleGear2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float fRotSpeed, float fTexHeight, int nPriority = 6, CScene2D::TYPE type = CScene2D::TYPE_GEAR);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRot(const float fRot);
	void SetRotSpeed(const float fRotSpeed);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);

	float GetRot(void);
	float GetRotSpeed(void);
	float GetAngle(void);
	float GetLength(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CalcPolygonScale(void);
	void RotationPolygon(void);

	float m_fRot;        // 現在の向き
	float m_fRotSpeed;   // 回転スピード
	float m_fAngle;      // ポリゴンの向き
	float m_fLength;     // ポリゴンの長さ
};

#endif