//=============================================================================
//
// 選択処理のプレイヤーの処理 [charMultRendering.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _CHRMULTRENDERING_H_
#define _CHRMULTRENDERING_H_

//=================================================
//		include / 他のデータ・コードの呼び出し・利用
//=================================================
#include "main.h"
#include "sceneX.h"
#include "game.h"
#include "scene.h"
#include "character.h"

//==============================================
//					マクロ定義
//==============================================
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)

//==============================================
//					前方宣言
//==============================================
class CSound;
class CModel;
class CScene3D;
class CEnemy;
class CSceneX;
class CModel;
class CMotion;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CCharMultRender : public CCharacter
{
public:
	typedef enum
	{//プレイヤーのタイプ
		PLAYERTYPE_SPEED = 0,
		PLAYERTYPE_POWER,
		PLAYERTYPE_TECHNIQUE,
		PLAYERTYPE_MAX
	}PLAYERTYPE;

	typedef enum
	{//国のタイプ
		TYPE_JAPAN = 0,		//日本
		TYPE_RUSSIA,		//ロシア
		TYPE_AMERICA,		//アメリカ
		TYPE_BRAZIL,		//ブラジル
		TYPE_ENGLAND,		//イギリス
		TYPE_ITALY,			//イタリア
		TYPE_NEWZEALAND,	//ニュージーランド
		TYPE_SOUTHAFRICA,	//南アフリカ
		TYPE_MAX,
	}TYPE;

	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CCharMultRender(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CCharMultRender();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCharMultRender *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charactertype, int nNumCharaIdx, int nCuntry);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPosV(D3DXVECTOR3 PosV) { m_PosV = PosV; }
	void SetPosR(D3DXVECTOR3 PosR) { m_PosR = PosR; }
	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTextureMT; }

protected:							//親と子クラスが使用できる
	//	位置・回転情報
	CModel** m_pModel;
	int m_nNumPlayer;
	PLAYERTYPE m_PlayerType;

	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_rot;					//回転量

	PLAYERSTATE m_PlayerState;					//プレイヤーの状態
	CMotion *m_pMotion;							//モーションの情報

private:
	void InitMultTexture(void);

	//マルチレンダリングの設定
	LPDIRECT3DTEXTURE9 m_pTextureMT; // レンダリング用のTexturte
	LPDIRECT3DSURFACE9 m_pRenderMT;	 // Textureにレンダリングするためのパイプ
	LPDIRECT3DSURFACE9 m_pBuffMT;	 // Textureに使用するバッファ
	D3DVIEWPORT9	   m_viewportMT; // Textureに使用するビューポート
	D3DXVECTOR3		   m_PosV;		 // 視点
	D3DXVECTOR3		   m_PosR;		 // 注視点
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

};
#endif