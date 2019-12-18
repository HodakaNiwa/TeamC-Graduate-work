//=============================================================================
//
// ナンバーの処理 [number.h]
// Author : Yamashita
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene2D.h"

//ナンバークラス
class CNumber
{
public:
	CNumber();											//コンストラクタ
	~CNumber();											//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fWeight, float fHeight);					//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

	//静的メンバ関数
	static CNumber *Create(D3DXVECTOR3 pos,float fWeight,float fHeight);
	static HRESULT Load(void);						//テクスチャ読み込み
	static void Unload(void);
	
	void SetNumber(int nNumber);
	void SetColNumber(D3DXCOLOR col);
private:
	static LPDIRECT3DTEXTURE9     m_pTexture;
	LPDIRECT3DVERTEXBUFFER9		  m_pVtxBuff;
	D3DXVECTOR3					  m_pos;					// ポリゴンの位置

	D3DXCOLOR					  m_col;					// ポリゴンの位置
};
#endif

