//====================================================================================
//
// 第9戦力UI処理	[robotUI.h]
// Author;荒谷由朗
//
//====================================================================================
#ifndef _ROBOTUI_H_
#define _ROBOTUI_H_

//=============================================================================
//    インクルードファイル
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
//    マクロ定義
//=============================================================================
#define ROBOTUI_TEXTURE_MAX (2)		// テクスチャの最大数

//=============================================================================
//    前方宣言
//=============================================================================
class CScene2D;
class CCharacter;
class CGame;

//=============================================================================
//    UIクラスの定義
//=============================================================================
class CRobotUI2D : public CScene2D
{
public:    // 誰でもアクセス可能
	CRobotUI2D();
	~CRobotUI2D();

	static CRobotUI2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, LPDIRECT3DTEXTURE9 pTexture, float fWidth, float fHeight, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	//--------------------
	//	Set & Get
	//--------------------
	void BindTexture	(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetPos			(const D3DXVECTOR3 pos)				{ m_Pos = pos; }
	void SetCol			(const D3DXCOLOR col)				{ m_Col = col; }
	void SetRot			(const float fRot)					{ m_fRot = fRot; }
	void SetWidth		(const float fWidth)				{ m_fWidth = fWidth; }
	void SetHeight		(const float fHeight)				{ m_fHeight = fHeight; }
	void SetLength		(const float fLength)				{ m_fLength = fLength; }
	void SetTexU		(const float fTexU)					{ m_fTexU = fTexU; }
	void SetTexV		(const float fTexV)					{ m_fTexV = fTexV; }
	void SetTexWidth	(const float fTexWidth)				{ m_fTexWidth = fTexWidth; }
	void SetTexHeight	(const float fTexHeight)			{ m_fTexHeight = fTexHeight; }

	D3DXVECTOR3 GetPos			(void) { return m_Pos; }
	D3DXCOLOR	GetCol			(void) { return m_Col; }
	float		GetRot			(void) { return m_fRot; }
	float		GetWidth		(void) { return m_fWidth; }
	float		GetHeight		(void) { return m_fHeight; }
	float		GetLength		(void) { return m_fLength; }
	float		GetTexU			(void) { return m_fTexU; }
	float		GetTexV			(void) { return m_fTexV; }
	float		GetTexWidth		(void) { return m_fTexWidth; }
	float		GetTexHeight	(void) { return m_fTexHeight; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3				m_Pos;	        // ポリゴンの位置
	D3DXCOLOR               m_Col;          // ポリゴンの色
	float                   m_fRot;         // ポリゴンの向き
	float                   m_fWidth;       // ポリゴンの幅
	float                   m_fHeight;      // ポリゴンの高さ
	float                   m_fLength;      // ポリゴンの長さ
	float                   m_fTexU;        // ポリゴンの左上テクスチャU座標
	float                   m_fTexV;        // ポリゴンの左上テクスチャV座標
	float                   m_fTexWidth;    // ポリゴンのテクスチャ座標の横幅
	float                   m_fTexHeight;   // ポリゴンのテクスチャ座標の縦幅
};

//=============================================================================
// ロボット出現UIクラスの定義
//=============================================================================
class CRobotUI
{
public:    // 誰でもアクセス可能
	CRobotUI();
	~CRobotUI();

	static CRobotUI *Create	(void);
	static HRESULT	Load	(void);
	static void		UnLoad	(void);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	//--------------------
	//	Set & Get
	//--------------------
	void		SetWarning	(CRobotUI2D *pWarning)	{ m_pRobotWarning = pWarning; }
	CRobotUI2D	*GetWarning	(void)					{ return m_pRobotWarning; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateWarning	(void);
	void ReleaseWarning	(void);
	void UpdateWarning	(void);

	void CreateTelop	(void);
	void ReleaseTelop	(void);
	void UpdateTelop	(void);

	//--------------------
	//	静的メンバ変数
	//--------------------
	static LPDIRECT3DTEXTURE9 m_apTexture[ROBOTUI_TEXTURE_MAX];

	//--------------------
	//	メンバ変数
	//--------------------
	CRobotUI2D	*m_pRobotWarning;	// 危険警告
	CRobotUI2D	*m_pRobotTelop;		// 出現テロップ
};
#endif