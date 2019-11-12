//=============================================================================
//
// 2Dポリゴンの処理 [scene2D.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 2Dポリゴンクラス
//=============================================================================
class CScene2D: public CScene
{
public:
	typedef enum
	{
		TYPE_LOGO = 0,
		TYPE_BOTTUN,
		TYPE_GEAR,
		TYPE_MAX,
	}TYPE;

	CScene2D(int nPriority, CScene::OBJTYPE type);
	CScene2D();
	~CScene2D();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

	//テクスチャの設定
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR2 GetTex(void);
	void SetTex(float uv, int nPattrn);
	void SetTex2(float uv, float uv2, int nPattrn, int Width);
	void SetTexUV(float u, float u2, float v, float v2);
	void SetTexMove(const float uv);
	void SetTexMove2(const float uv);

	//位置情報
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetOtherPos(void);

	//座標の設定
	void SetPos(const D3DXVECTOR3 pos, const D3DXVECTOR3 Otherpos);
	void SetRotPos(const D3DXVECTOR3 pos, const float fAngle, const float fRadius);

	//ポリゴンのサイズ
	D3DXVECTOR3 GetSize(void);
	void SetSize(const D3DXVECTOR3 size);

	//使用しているかどうか
	bool GetUse(void);
	void SetUse(const bool bUse);

	//色の設定
	void SetColoer(const D3DXCOLOR col);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	D3DXCOLOR GetColor(void) { return m_Color; };

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;			// ポリゴンの位置
	D3DXVECTOR3					m_Other;		// ポリゴンの対角線
	D3DXVECTOR3					m_Size;			// ポリゴンのサイズ
	D3DXVECTOR2					m_Tex;			// テクスチャのUV値
	D3DXCOLOR					m_Color;		// 色
	bool						m_bUse;			// 使用しているかどうか
};

#endif