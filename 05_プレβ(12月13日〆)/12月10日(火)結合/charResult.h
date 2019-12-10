//=============================================================================
//
// リザルト用プレイヤーの処理 [charResult.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _CHRRESULT_H_
#define _CHRRESULT_H_

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
class CCharResult : public CCharacter
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
		MOTION_TYPE_NONE = 0,
		MOTION_TYPE_WALK,		//移動状態
		MOTION_TYPE_ACTION,		//
		MOTION_TYPE_BLOWAWAY,
		MOTION_TYPE_DOWN,
		MOTION_TYPE_WAKEUP,
		MOTION_TYPE_WIN,
		MOTION_TYPE_LOSE,
		MOTION_TYPE_SELECT,
		MOTION_TYPE_MAX
	}MOTION_TYPE;

	CCharResult(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CCharResult();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCharResult *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charactertype,int nCharaNum, int nCuntry);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetMotionType(MOTION_TYPE MotionType) { m_MotionType = MotionType; }
	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }

protected:								//親と子クラスが使用できる
	//	位置・回転情報
	CModel** m_pModel;
	PLAYERTYPE m_PlayerType;

	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_rot;					//回転量

	MOTION_TYPE m_MotionType;			//モーションの種類
	CMotion *m_pMotion;					//モーションの情報
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

private:
	void InitMove(void);
	void UpdateMove(void);
	bool m_bStopMotion;

};
#endif