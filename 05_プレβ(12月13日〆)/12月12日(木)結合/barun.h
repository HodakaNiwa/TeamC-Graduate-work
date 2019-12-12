//=============================================================================
//
// 風船処理 [barun.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _BARUN_H_
#define _BARUN_H_

#include "main.h"
#include "sceneBillBorad.h"

//=============================================================================
// ビルボードクラス
//=============================================================================
class CBarun : public CSceneBillborad
{
public:
	CBarun();
	CBarun(int nPriority, OBJTYPE ObjType);
	~CBarun();
	HRESULT Init(int nType, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBarun * Create(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move);
	static void LoadTex(void);
	static void UnloadTex(void);

private:

	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	D3DXVECTOR3 m_move;
};
#endif