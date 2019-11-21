//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "sound.h"
#include "modebase.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数
#define MAX_RANKING_NUMBER		(8)		// ランキングの数字
#define RANKING_BG_MAX			(9)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CNumber;

//================================
// ランキングロゴクラス
//================================
class CRankingBG : public CScene2D
{
public:     // 誰でもアクセス可能
	CRankingBG(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CRankingBG();

	static CRankingBG *Create(D3DXVECTOR3 pos, D3DXVECTOR3 destPos, D3DXVECTOR3 Size, float fMove, float fRotSpeed, float fTexHeight, int nPriority = 6, CScene2D::TYPE type = CScene2D::TYPE_GEAR);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 fPos) { m_pos = fPos; }
	void SetDestPos(const D3DXVECTOR3 destPos) { m_DestPos = destPos; }
	void SetMove(const float fMove) { m_fMove = fMove; }
	void SetRot(const float fRot) { m_fRot = fRot; }
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetDestPos(void) { return m_DestPos; }
	float GetMove(void) { return m_fMove; }
	float GetRot(void) { return m_fRot; }
	float GetRotSpeed(void) { return m_fRotSpeed; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CalcPolygonScale(void);
	void RotationPolygon(void);

	D3DXVECTOR3 m_pos;      // 現在の場所
	D3DXVECTOR3 m_DestPos;  // 目的の場所
	float m_fMove;			// 移動スピード
	float m_fRot;			// 現在の向き
	float m_fRotSpeed;		// 回転スピード
	float m_fAngle;			// ポリゴンの向き
	float m_fLength;		// ポリゴンの長さ
};


//=====================
// 基本クラス
//=====================
class CRanking : public CModeBase
{
public:
	CRanking();			// コンストラクタ
	~CRanking();		// デストラクタ
	void Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

protected: // このクラスと派生クラスだけがアクセス可能

private:				// このクラスだけがアクセス可能
	void LoadTexture(void);
	void UnloadTexture(void);
	void CreateRankingBG(const int nIdx);

	LPDIRECT3DTEXTURE9 m_apTextureRanking[9];				//テクスチャ
	CRankingBG *m_apRankingBG[RANKING_BG_MAX];			//ランキングの背景

	CCamera *m_pCamera;										//カメラ
	CFieldManager *m_pFieldManager;							//フィールドマネージャー
	CLoadTextObject *m_pLoadTextObj;						//オブジェクト読み込み
	CLoadTextTerritory *m_pLoadTerritory;					// テリトリーの読み込み

	int m_nNumRanking;
	int m_nCounterRanking;
	
};


#endif