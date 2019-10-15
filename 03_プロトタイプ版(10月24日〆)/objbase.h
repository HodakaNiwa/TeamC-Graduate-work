//=============================================================================
//
// オブジェクトベース処理 [objbase.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _OBJ_BASE_H_
#define _OBJ_BASE_H_

#include "main.h"
#include "sceneX.h"
#include "loadText.h"

//=============================================================================
// Xファイルクラス
//=============================================================================
class CObjBase : public CSceneX
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_MAX,
	}TYPE;

	CObjBase();
	CObjBase(int nPriority, OBJTYPE objtype);
	~CObjBase();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjBase * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, TYPE type);
	
	static void LoadTex(CLoadTextObject::TEX_INFO TexInfo);
	static void UnloadTex(void);
	static void LoadModel(CLoadTextObject::OBJ_INFO ObjInfo);
	static void UnloadModel(void);
	static int GetCountObj(void) { return m_nCountObj; }
	static int GetMaxObj(void) { return m_nMaxObj; }
	static int GetMaxTex(void) { return m_nMaxTex; }
	static int GetMaxModel(void) { return m_nMaxModel; }
	static void Save(HWND hWnd);
	static void SetCountObj(int nCntObj) { m_nCountObj = nCntObj; }
	static void SetAddMaxObj(void) { m_nMaxObj++; }
	static void SetDownMaxObj(void) { m_nMaxObj--; }
	static CObjBase * RequestPointer(int nCountObj);
	void SetTexNum(int nTex) { m_nTex = nTex; }
	void SetModelType(int ModelType) { m_nModelType = ModelType; }
	void SetType(TYPE type) { m_Type = type; }
	int GetTex(void) { return m_nTex; }
	int GetModelType(void) { return m_nModelType; }
	TYPE GetType(void) { return m_Type; }

protected:
	int  m_nTex;
	int  m_nModelType;
	TYPE m_Type;

private:
	//変数宣言
	static LPDIRECT3DTEXTURE9 *  m_ppTexture;		// テクスチャへのポインタ
	static LPD3DXMESH *			 m_pMesh;			//メッシュ情報へのポインタ
	static LPD3DXBUFFER *		 m_pBuffMat;		//マテリアル情報へのポインタ
	static DWORD *				 m_pNumMat;			//マテリアル情報の数
	static D3DXVECTOR3 *		 m_pVtxMin;			//モデル最小値
	static D3DXVECTOR3 *		 m_pVtxMax;			//モデル最大値
	static int m_nMaxTex;	//テクスチャの最大数
	static int m_nMaxModel;	//モデルの最大数
	static int m_nCountObj;	//オブジェクトのカウンター
	static int m_nMaxObj;	//オブジェクトの最大数
};
#endif