//=============================================================================
//
// 数字の処理 [number2D.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _NUMBER2D_H_
#define _NUMBER2D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// ナンバークラス
//=============================================================================
class CNumber2D
{
public:
	//メンバ関数
	CNumber2D();
	~CNumber2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumber2D * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	void SetNumber(const int nNumber);
	void SetbUse(bool bUse);
	void SetPos(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR3 size);					//サイズの設定
	void ResetPos(void);
	void SetColor(D3DXCOLOR col);
	D3DXVECTOR3 GetPos(void) { return m_pos; };		//位置の取得
	D3DXVECTOR3 GetSize(void) { return m_Size; };	//サイズの取得

	//テクスチャの設定
	static HRESULT Load(void);
	static void Unload(void);

private:
	//関数宣言

	//静的メンバ変数宣言
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ

	//変数宣言
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_Other;				// ポリゴンの対角線
	D3DXVECTOR3				m_Size;					// ポリゴンのサイズ
	bool					m_bUse;					// 使用しているかどうか
	D3DXVECTOR3				m_HoldPos;				// 座標の保存
};
#endif