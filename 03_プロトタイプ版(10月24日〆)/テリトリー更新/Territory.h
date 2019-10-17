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
#include "Player.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;

//=============================================================================
// Xファイルクラス
//=============================================================================
class CTerritory : public CSceneX
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_START,	//始点
		STATE_PASS,		//通過点
	}STATE;

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
	//関数
	void UpdateNormal(CPlayer * pPlayer, int nCntPlayer);
	void UpdateStart(CPlayer * pPlayer, int nCntPlayer);
	void UpdatePass(CPlayer * pPlayer, int nCntPlayer);
	void SetColor(D3DXCOLOR Color);
	void ResetTerritory(CTerritory * pTop);
	bool ChackList(CTerritory * pTop, int * nCountTerritory);
	void ChackIn(D3DXVECTOR3 * pPos, int nMaxPos, int nNumPlayer);
	CTerritory * DestoroyTerritoryStart(void);
	CTerritory * DestoroyTerritory(void);

	//変数
	int  m_nTex;			//テクスチャ番号
	int  m_nModelType;		//モデル番号
	int  m_nPoint;			//ポイント
	int  m_nNumPlayer;		//プレイヤー番号
	int  m_nOldNumPlayer;	//前回のプレイヤー番号
	float m_fLength;		//長さ
	bool m_bGetTerritory;	//テリトリーの取得状態
	D3DXCOLOR m_Color;		//色
	D3DXCOLOR m_OldColor;	//前回の色
	STATE m_State;			//状態
	CTerritory * m_pPrev;	//前のテリトリーへのポインタ
	CTerritory * m_pNext;	//次のテリトリーへのポインタ

private:
	//関数宣言
	void CreateCollider(void);

	//変数宣言
	static LPDIRECT3DTEXTURE9 *  m_ppTexture;			// テクスチャへのポインタ
	static LPD3DXMESH *			 m_pMesh;				//メッシュ情報へのポインタ
	static LPD3DXBUFFER *		 m_pBuffMat;			//マテリアル情報へのポインタ
	static DWORD *				 m_pNumMat;				//マテリアル情報の数
	static D3DXVECTOR3 *		 m_pVtxMin;				//モデル最小値
	static D3DXVECTOR3 *		 m_pVtxMax;				//モデル最大値
	static int m_nMaxTex;								//テクスチャの最大数
	static int m_nMaxModel;								//モデルの最大数
	static int m_nCountObj;								//オブジェクトのカウンター
	static int m_nMaxObj;								//オブジェクトの最大数
	static D3DXCOLOR m_DefaultColor;					//デフォルトの色
	static D3DXCOLOR m_CountryColor[CPlayer::TYPE_MAX];	//国の色
};
#endif