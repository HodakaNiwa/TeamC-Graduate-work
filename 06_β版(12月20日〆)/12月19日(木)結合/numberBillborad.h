//=============================================================================
//
// ナンバービルボード処理 [scenebillboard.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _NUMBERBILLBORD_H_
#define _NUMBERBILLBORD_H_

#include "main.h"

//=============================================================================
// ビルボードクラス
//=============================================================================
class CNumberBillborad
{
public:
	CNumberBillborad();
	~CNumberBillborad();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumberBillborad * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };		//ワールドマトリックスの取得
	void SetSize(D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetSize(void) { return m_size; }				//サイズの設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXCOLOR GetColor(void) { return m_Color; }
	void SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax);
	void SetColor(D3DXCOLOR color);
	static void LoadTex(void);									//テクスチャの読み込み
	static void UnloadTex(void);								//テクスチャの破棄

private:
	//メンバ関数
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetColorABillborad(float colA);

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;							// テクスチャへのポインタ

	//メンバ変数
	D3DXVECTOR3				m_pos;								//中心座標
	D3DXVECTOR3				m_size;								//サイズ
	D3DXVECTOR3				m_rot;								//向きの設定
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;							//ワールドマトリックス
	D3DXCOLOR				m_Color;							//色
};
#endif