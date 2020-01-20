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
// マクロ定義
//=============================================================================
#define TERRITORY_PRIORITY (3)
#define HIGHT_DIVISION	(2)		//縦の分割数
#define WIDTH_DIVISION	(2)		//横の分割数
#define MAX_EREAPOS		((HIGHT_DIVISION + 1) * (WIDTH_DIVISION + 1))	//エリア座標数

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;
class CLoadEffect;
class CRing;

//=============================================================================
// Xファイルクラス
//=============================================================================
class CTerritory : public CSceneX
{
public:
	typedef enum
	{
		STATE_GET,
		STATE_NONE,
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
	static LPD3DXMESH *GetMesh(void) { return m_pMesh; }
	static LPDIRECT3DTEXTURE9 *GetTexture(void) { return m_ppTexture; }
	static int GetCountObj(void) { return m_nCountObj; }
	static int GetMaxObj(void) { return m_nMaxObj; }
	static int GetMaxTex(void) { return m_nMaxTex; }
	static int GetMaxModel(void) { return m_nMaxModel; }
	static void Save(HWND hWnd);
	static void SetCountObj(int nCntObj) { m_nCountObj = nCntObj; }
	static void SetAddMaxObj(void) { m_nMaxObj++; }
	static void SetDownMaxObj(void) { m_nMaxObj--; }
	static void SetErea(void);
	static CTerritory * RequestPointer(int nCountObj);
	static int RequestErea(D3DXVECTOR3 Pos);
	static void ResetColorTerritory(void);	
	void SetTexNum(int nTex) { m_nTex = nTex; }
	void SetModelType(int ModelType) { m_nModelType = ModelType; }
	int GetTex(void) { return m_nTex; }
	int GetModelType(void) { return m_nModelType; }
	int GetPoint(void) { return m_nPoint; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }
	void SetColor(D3DXCOLOR col) { m_Color = col; }
	void SetPlayerNumber(int nNumPlayer);
	int GetPlayerNumber(void) { return m_nNumPlayer; }
	void CreateGetEffect(void);
	int GetErea(void) { return m_nErea; }
	D3DXCOLOR GetColor(void) { return m_Color; }
	void SetState(STATE state) { m_state = state; }	

protected:
	void SetDiffuseColor(D3DXCOLOR Color);
	//変数
	int  m_nTex;			//テクスチャ番号
	int  m_nModelType;		//モデル番号
	int  m_nPoint;			//ポイント
	int  m_nNumPlayer;		//プレイヤー番号
	int  m_nOldNumPlayer;	//前回のプレイヤー番号
	int  m_nErea;			//エリアの番号
	float m_fLength;		//長さ
	bool m_bGetTerritory;	//テリトリーの取得状態
	D3DXCOLOR m_Color;		//色
	D3DXCOLOR m_OldColor;	//前回の色
	CRing **m_ppRing;		//周りのリング

private:
	//関数宣言
	void CreateCollider(void);
	void CreateRing(void);
	void ReleaseRing(void);
	void UpdateGetRing(void);	
	void CreateShadow(void);

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
	static D3DXVECTOR2 m_pEreaPos[MAX_EREAPOS];			//エリアの座標数
	CLoadEffect * m_pLoadEffect;
	STATE m_state;										//リングの状態設定
	int m_nCountTime;									//時間の加算　　　　
	int m_nRingColIndx;									//リングの色のインデックス　
	bool * m_pbChangeColRing;							//リングの色変更
};
#endif