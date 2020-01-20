//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "modebase.h"
#include "scene.h"

//=================================
// マクロ定義
//=================================
#define TEX_TUTORIAL_BG (10)
#define TEX_TUTORIAL    (3)
#define MAX_OBJTYPE		(13)

//=================================
// 前方宣言
//=================================
class CMoveUI;
class CLoadTextTerritory;
class CCamera;
class CFieldManager;
class CLoadTextEffect;
class CCharacter;
class CTutorialTechniquePlayer;
class CTerritoryRender;
class CRingRender;
class CEffectManager;

//=================================
// チュートリアルクラス
//=================================
class CTutorial : public CModeBase
{
public:
	typedef enum
	{
		STATE_FIRST = 0,		//冒頭
		STATE_WINCONDITIONS,	//勝利条件
		STATE_GETTERRITORY1,	//テリトリーの取得１
		STATE_GETTERRITORY2,	//テリトリーの取得2
		STATE_CUTLINE,			//ラインを切る
		STATE_POINT,			//ポイント
		STATE_CONTROLER,		//コントローラー
		STATE_CHARACTER,		//キャラクター
		STATE_END,
	}STATE;

	CTutorial();			// コンストラクタ
	~CTutorial();			// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void LoadTex(void);		//テクスチャの読み込み
	void UnloadTex(void);	//テクスチャの破棄
	CLoadTextEffect * GetEffectLoad(void) { return m_pLoadEffect; }
	CCharacter * GetTutorialPlayer(void) { return m_pCharTutorial; }
	CEffectManager * GetEffectManager(void) { return m_pEffectManager; }
	CRingRender * GetRingRender(void) { return m_pRingRender; } //安

private:
	void UpdateFirst(void);
	void UpdateWin(void);
	void UpdateGetTerritory(void);
	void UpdateCutLine(void);
	void UpdateGetPoint(void);
	void UpdateControler(void);
	void UpdateCharacter(void);
	void UpdateEnd(void);
	void DeleteTitle(void);
	void MoveTitle(STATE NextState);
	bool MoveSlideTitle(void);
	bool TextFadeInOut(CMoveUI * pText, int nFadeOutTime, int nCountTime);
	void CreateMultiWindow(D3DXVECTOR3 Pos, D3DXVECTOR3 Size);
	void DeleteMultiWindow(void);
	void CreateMultRenderingTex(void);
	void DrawMultRendering(void);
	void SetObjTypeList(void);
	void DeleteCharacter(void);

	LPDIRECT3DTEXTURE9 m_pTextureBG[TEX_TUTORIAL_BG];
	LPDIRECT3DTEXTURE9 m_pTextureText[TEX_TUTORIAL];
	LPDIRECT3DTEXTURE9 m_pTextureLogo;

	STATE m_state;
	CMoveUI * m_pBG;
	CMoveUI * m_pTitle;
	CMoveUI * m_pText00;
	CMoveUI * m_pText01;
	CMoveUI * m_pWindow;
	CScene::OBJTYPE m_ObjTypeList[MAX_OBJTYPE];
	int m_nCountTime;
	bool m_bMoveSlideTitle;
	bool m_bSlideOut;

	//マルチレンダリングの設定
	LPDIRECT3DTEXTURE9 m_pTextureMT;		// レンダリング用のTexturte
	LPDIRECT3DSURFACE9 m_pRenderMT;	// Textureにレンダリングするためのパイプ
	LPDIRECT3DSURFACE9 m_pBuffMT;	// Textureに使用するバッファ
	D3DVIEWPORT9	   m_viewportMT;	// Textureに使用するビューポート
	D3DXVECTOR3		   m_PosV;		// 視点
	D3DXVECTOR3		   m_PosR;		// 注視点

	//読み込むオブジェクト
	CLoadTextTerritory * m_pLoadTerritory;
	CFieldManager * m_pFieldManager;
	CCamera * m_pCamera;
	CLoadTextEffect * m_pLoadEffect;			//	エフェクト管轄クラスへのポインタ
	CCharacter * m_pCharTutorial;				// チュートリアルキャラ
	CCharacter * m_pCharDamege;					// ダメージを受けるキャラクター
	CTerritoryRender * m_pTerritoryRender;		// テリトリーレンダラークラスへのポインター
	CRingRender * m_pRingRender;				// リングレンダラーへのポインター
	CEffectManager * m_pEffectManager;			//エフェクトマネージャー
};
#endif