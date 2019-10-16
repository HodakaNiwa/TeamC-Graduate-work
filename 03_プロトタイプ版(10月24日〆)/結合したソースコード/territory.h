//=============================================================================
//
// テリトリー処理 [Territory.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include "main.h"
#include "sceneX.h"
#include "loadText.h"

//=============================================================================
// Xファイルクラス
//=============================================================================
class CTerritory : public CSceneX
{
public:
	CTerritory();
	CTerritory(int nPriority, OBJTYPE objtype);
	~CTerritory();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTerritory * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, int nPoint);
	
	static void LoadTex(CLoadTextTerritory::TEX_INFO TexInfo);
	static void UnloadTex(void);
	static void LoadModel(CLoadTextTerritory::OBJ_INFO ObjInfo);
	static void UnloadModel(void);
	static int GetCountObj(void) { return m_nCountObj; }
	static int GetMaxObj(void) { return m_nMaxObj; }
	static int GetMaxTex(void) { return m_nMaxTex; }
	static int GetMaxModel(void) { return m_nMaxModel; }
	static void Save(HWND hWnd);
	static void SetCountObj(int nCntObj) { m_nCountObj = nCntObj; }
	static void SetAddMaxObj(void) { m_nMaxObj++; }
	static void SetDownMaxObj(void) { m_nMaxObj--; }
	static CTerritory * RequestPointer(int nCountObj);
	void SetTexNum(int nTex) { m_nTex = nTex; }
	void SetModelType(int ModelType) { m_nModelType = ModelType; }
	int GetTex(void) { return m_nTex; }
	int GetModelType(void) { return m_nModelType; }
	int GetPoint(void) { return m_nPoint; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }

protected:
	int  m_nTex;
	int  m_nModelType;
	int  m_nPoint;

private:
	//変数宣言
	static LPDIRECT3DTEXTURE9 *  m_ppTexture;		// テクスチャへのポインタ
	static LPD3DXMESH *			 m_pMesh;			//メッシュ情報へのポインタ
	static LPD3DXBUFFER *		 m_pBuffMat;		//マテリアル情報へのポインタ
	static DWORD *				 m_pNumMat;			//マテリアル情報の数
	static D3DXVECTOR3 *		 m_pVtxMin;			//モデル最小値
	static D3DXVECTOR3 *		 m_pVtxMax;			//モデル最大値
	static int m_nMaxTex;							//テクスチャの最大数
	static int m_nMaxModel;							//モデルの最大数
	static int m_nCountObj;							//オブジェクトのカウンター
	static int m_nMaxObj;							//オブジェクトの最大数
};
#endif