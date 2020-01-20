//=============================================================================
//
// ひび割れ処理 [crack.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _CRACK_H_
#define _CRACK_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// ひび割れクラス
//=============================================================================
class CCrack : public CScene3D
{
public:
	CCrack();
	CCrack(int nPriority, CScene::OBJTYPE obj);
	~CCrack();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCrack * Create(const D3DXVECTOR3 pos);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nLife;		//表示時間
	bool m_bDeth;		//死亡フラグ
	int m_nCountTime;	//カウンター
	int m_nPattern;		//パターン
};
#endif
