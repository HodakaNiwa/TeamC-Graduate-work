//=============================================================================
//
// オブジェクトの処理 [scene.h]
// Author : 佐藤 安純　[ SATO ASUMI ] : 丹羽保貴 : 小林将兵 : 山下敦史
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "renderer.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CColliderManager;

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LAYER	(12)

//=============================================================================
// オブジェクトクラス
//=============================================================================
class CScene
{
public:
	//オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_SHADOW,
		OBJTYPE_FLOR,
		OBJTYPE_SKY,
		OBJTYPE_OBJE,
		OBJTYPE_BILLBORAD,
		OBJTYPE_BULLET,
		OBJTYPE_ORBIT,
		OBJTYPE_PARTICLE,
		OBJTYPE_EXPROTION,
		OBJTYPE_FADE,
		OBJTYPE_UI,
		OBJTYPE_BOTTUN,
		OBJTYPE_SCORE,
		OBJTYPE_DAMEGE,
		OBJTYPE_COMBO,
		OBJTYPE_GAUGE,
		OBJTYPE_NUMBER2D,
		OBJTYPE_2DPOLYGON,
		OBJTYPE_MESHSPHERE,
		OBJTYPE_COLDISPSPHERE,
		OBJTYPE_OBJ,
		OBJTYPE_TERRITORY,
		OBJTYPE_COLORPOINT,
		OBJTYPE_LINE,
		OBJTYPE_MINE,
		OBJTYPE_LIGHTORBIT,
		OBJTYPE_EFFECT3D,
		OBJTYPE_SCREENEFFECT,
		OBJTYPE_FIELDRENDER,
		OBJTYPE_MESHRING,
		OBJTYPE_RINGEFFECT,
		OBJTYPE_EMITTER,
		OBJTYPE_RINGEMITTER,
		OBJTYPE_RING,
		OBJTYPE_TERRITORYRENDER,
		OBJTYPE_RINGRENDER,
		OBJTYPE_ROBOT,
		OBJTYPE_TRIANGLE,
		OBJTYPE_DIVISIONWALL,
		OBJTYPE_ROBOTBULLET,
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE ObjType = OBJTYPE_NONE);
	~CScene();

	//メンバ関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) = 0;
	virtual void Collision(void);

	static void ReleaseAll(void);									//全メモリの開放
	static void ReleaseFade(void);									//フェード以外を開放
	static void UpdateAll(void);									//全て更新する
	static void DrawAll();											//全て表示する
	static void DeathCheck(void);									//開放チェック
	static CScene * GetTop(int nLayer) { return m_pTop[nLayer]; };	//先頭のオブジェクトポインタの取得
	static CScene * GetCur(int nLayer) { return m_pCur[nLayer]; };	//現在（最後尾）のオブジェクトポインタの取得
	static void CheckCollisionAll(void);
	static int GetNumDrawViewPort(void) { return m_nNumDrawViewPort; }	//何番目のビューポートを描画しているか
	void SetCOlliderManager(CColliderManager * pColliderManger) { m_pColliderManger = pColliderManger; }
	CColliderManager * GetColloderManger(void) { return m_pColliderManger; }
	OBJTYPE GetObjType(void) { return m_ObjType; };					//オブジェクトタイプの取得
	CScene * GetpNext(void) { return m_pNext; };					//次のオブジェクトポインタの取得
	static void SetPause(bool bPause) { m_bPause = bPause; };		//ポーズの設定
	bool GetDraw(void) { return m_bDraw; }							//描画状態の取得
	bool GetDeth(void) { return m_bDeth; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; };					//描画するかどうか

protected:
	void Release(void);
	void Deth(int nCntLayer);


private:
	static CScene * m_pTop[MAX_LAYER];		//先頭オブジェクトへのポインタ
	static CScene * m_pCur[MAX_LAYER];		//現在のオブジェクトへのポインタ（最後尾）
	CScene * m_pPrev;						//前のオブジェクトへのポインタ
	CScene * m_pNext;						//次のオブジェクトへのポインタ

	//静的メンバ変数
	static int		m_nNumAll;				//配列のカウンタ
	static int		m_nNumDrawViewPort;		//何番目のビューポートを描画しているのか

	//デバック用変数
	static int		m_anNumOBJ[MAX_LAYER];	//各レイヤーのオブジェクト数

	//メンバ変数
	bool m_bDeth;			//死亡フラグ
	bool m_bDraw;			//描画するかどうか
	static bool m_bPause;	//ポーズ状態にする
	static int m_nCountDraw;
	int m_nID;				//使用ID
	CColliderManager * m_pColliderManger;

	D3DXVECTOR3	m_pos;		// ポリゴンの位置
	D3DXVECTOR3	m_Other;	// ポリゴンの対角線
	OBJTYPE m_ObjType;		//オブジェクトのタイプ
};
#endif