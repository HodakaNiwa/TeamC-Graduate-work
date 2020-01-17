//=============================================================================
//
// 波紋処理 [ripple.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _RIPPLES_H_
#define _RIPPLES_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// 爆発跡クラス
//=============================================================================
class CRipples : public CScene3D
{
public:
	CRipples();
	CRipples(int nPriority, CScene::OBJTYPE obj);
	~CRipples();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRipples * Create(const D3DXVECTOR3 pos);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nLife;	//表示時間
	bool m_bDeth;	//死亡フラグ
};
#endif
