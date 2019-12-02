//*****************************************************************************
//
//     リングの処理[ring.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RING_H_
#define _RING_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "sceneX.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************


//*****************************************************************************
//    リングクラスの定義
//*****************************************************************************
class CRing : public CSceneX
{
public:    // 誰でもアクセス可能
	CRing();
	~CRing();

	static CRing *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------


protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------

	//--------------------
	//	メンバ変数
	//--------------------
};

#endif