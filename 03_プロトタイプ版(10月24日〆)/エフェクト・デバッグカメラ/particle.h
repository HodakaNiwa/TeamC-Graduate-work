//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 佐藤 安純	Sato Asumi
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene.h"
#include "sceneBillBorad.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CParticle : public CSceneBillborad
{
public:
	typedef enum
	{//エフェクトの動きの種類
		TYPE_NORMAL = 0,
		TYPE_SPEW,
		TYPE_GRANDSMOKE,
		TYPE_LIGHT,
		TYPE_CICLE,
		TYPE_STAR,
		TYPE_MAX,
	}TYPE;

	typedef enum
	{//描画状態
		DRAWTYPE_NORMAL = 0,
		DRAWTYPE_ADD,
		DRAWTYPE_MAX,
	}DRAWTYPE;

	CParticle();
	CParticle(int nParticle);
	~CParticle();
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//---------------------
	// Load & Unload
	//---------------------
	static void LoadTexture(void);
	static void UnloadTexture(void);

	//---------------------
	// Set & Get
	//---------------------
	D3DXCOLOR GetColor(void) { return m_col; };			// 色の取得
	void SetColor(D3DXCOLOR col);						// 色の設定
	D3DXVECTOR2 GetSize(void) { return m_size; }		// サイズの取得
	void SetSize(D3DXVECTOR2 size) { m_size = size; }	// サイズの設定
	void RemakeSize(D3DXVECTOR2 size);					// サイズの変更
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置の取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置の設定

	void SetDrawType(DRAWTYPE DrawType) { m_DrawType = DrawType; };		//描画タイプの設定
	void SetColorA(float colA) { m_colA = colA; };						//色の透明度の設定
	void SetZbuffu(bool bZBuffu) { m_bZBuffu = bZBuffu; };				//Zバッファの有効設定
	void SetLife(int nLife) { m_nLife = nLife; };						//ライフの設定
	void SetChangeCol_a(float fCol_a) { m_fChangeCol_a = fCol_a; };		//透明度の変化量の設定
	void SetUseGravity(bool bGravity) { m_bGravity = bGravity; };		//重力設定
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; };					//スピードの設定
	void SetMinHight(int nMinHight) { m_nMinHight = nMinHight; };		//高さの最小値の設定
	void SetMaxHight(int nMaxHight) { m_nMaxHight = nMaxHight; };		//高さの最大値の設定
	void SetGravity(float fGravity) { m_fGravity = fGravity; };			//重力の設定
	void SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV);
	
protected:
	void SetAngle(float * pAngle);
	D3DXVECTOR3		m_move;			//移動量
	float			m_fSpeed;		//移動スピード
	float			m_colA;			//透明度の設定	
	int				m_nLife;		//表示時間
	float			m_fChangeCol_a;	//α値の変化量
	bool			m_bGravity;		//重力のON・OFF
	int				m_nMinHight;	//高さの最小値
	int				m_nMaxHight;	//高さの最大値
	float			m_fGravity;		//重力
	bool			m_bBound;		//バウンド
	float			m_fBoundHight;	//バウンドする高さ
	bool			m_bDeth;		//死亡フラグ

private:
	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 向き
	D3DXCOLOR				m_col;				// 色
	D3DXVECTOR2				m_size;				// 大きさ

	static LPDIRECT3DTEXTURE9 * m_ppTexture;	//テクスチャのポインタ
	static int					m_nMaxTexture;	//テクスチャの最大数
	DRAWTYPE					m_DrawType;		//描画のタイプ
	bool						m_bZBuffu;		//Zバッファを有効にするかどうか
};

//=============================================================================
// 星のパーティクルクラス
//=============================================================================
class CSpewParticle : public CParticle
{
public:
	CSpewParticle(int nPriority);
	~CSpewParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//フレームカウンター
};

//=============================================================================
// 土煙のパーティクルクラス
//=============================================================================
class CGrandSmokeParticle : public CParticle
{
public:
	CGrandSmokeParticle(int nPriority);
	~CGrandSmokeParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//フレームカウンター
};

//=============================================================================
// ライトパーティクル
//=============================================================================
class CLightParticle : public CParticle
{
public:
	CLightParticle(int nPriority);
	~CLightParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	float m_fChangeSize;	//サイズの変化量
};

//=============================================================================
// 円形のパーティクルクラス
//=============================================================================
class CCicleParticle : public CParticle
{
public:
	CCicleParticle(int nPriority);
	~CCicleParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//フレームカウンター
};

//=============================================================================
// 星のパーティクルクラス
//=============================================================================
class CStarParticle : public CParticle
{
public:
	CStarParticle(int nPriority);
	~CStarParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//フレームカウンター
};
#endif