//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					プレイヤー処理 [player.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
#define MODEL_UV	("DATA/TEXTURE/MODEL/player.png")

//プレイヤーステータス
#define PLAYER_SPEED	(2.2f)
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
#define MAX_TERRITORY	(10)

//==============================================
//					前方宣言
//==============================================
class CSound;
class CModel;
class CScene3D;
class CObject;
class CEnemy;
class CSceneX;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CEnemyAttackSphereCollider;
class CModel;
class CTerritory;
class CSceneOrbit;
class CLine;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CPlayer : public CCharacter
{
public:

	typedef enum
	{//プレイヤーの人数
		NUM_PLAYER_01 = 0,
		NUM_PLAYER_02,
		NUM_PLAYER_03,
		NUM_PLAYER_04,
		NUM_PLAYER_MAX
	}NUMPLAYER;

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

	CPlayer();
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40]);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
	static CPlayer *Create(D3DXVECTOR3 pos,int nNumPlayer, char ModelTxt[40], char MotionTxt[40], TYPE type);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void PlayerMove(void);				//プレイヤーの移動処理
	void Collision(void);
	void AddTerritoryList(CTerritory * pTerritory);
	void CutLine(int nID);	
	NUMPLAYER GetNumPlayer(void){ return m_NumPlayer; }

	//国のタイプ
	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
	static D3DXCOLOR m_CountryColor[CPlayer::TYPE_MAX];	//国の色

protected:
	//	位置・回転情報
	CModel** m_pModel;
	int m_nNumPlayer;
	NUMPLAYER m_NumPlayer;					//プレイヤーの人数

private:
	void CountDownLineTime(void);										//テリトリーの取得更新処理
	void ResetList(void);												//リストの初期化
	bool ChackSamePos(CTerritory * pTerritory, int nNumTerritory);		//同じ位置かどうか
	void ShapeComplete(CTerritory * pTerritory);						//完成したときの処理
	void ChackInShape(void);											//図形に入っているかどうかの処理
	void GetTerritory(CTerritory * pTerritory);							//テリトリーの取得
	void CreateOrbitLine(void);											//奇跡の線の生成
	void UninitOrtbitLine(void);										//ラインの破棄
	void ResetLine(void);												//ラインの生成処理
	void UninitLine(void);												//ラインの破棄
	void UninitCopyLine(void);											//コピーラインの破棄
	void CreateEndLine(void);											//最後の線の生成
	void UpdateShapeComplete(void);										//完成後の更新処理

	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_posOld;				//位置
	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_rot;					//移動量
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_vtxMax;
	D3DXVECTOR3 m_vtxMin;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	static LPD3DXMESH m_pMeshModel;			//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBufferMatModel;	//マテリアル情報へのポインタ
	static DWORD m_nNumMatModel;			//マテリアル情報の数
	bool m_bUse;							//使用しているかどうか
	int m_nLineTime;						//ラインの時間
	int m_nMaxLineTime;						//ラインの時間最大数

	static CSceneX *m_apSceneX;
	D3DXVECTOR3 m_Angle;								//角度

	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//国のタイプ
	TYPE m_type;

	//テリトリー
	CTerritory * m_apTerritory[MAX_TERRITORY];	//テリトリーのリスト
	CTerritory * m_pOldStartTerritory;			//初期位置のテリトリー
	int m_nCountTerritory;						//カウンター
	int m_nCountTime;							//時間カウンター
	bool m_bBlockStartTerritory;				//初期テリトリーをブロック
	bool m_bMakeShape;							//図形が完成フラグ

	//ライン
	CSceneOrbit * m_pOrbitLine;					//ライン
	CLine *		  m_apLine[MAX_TERRITORY];		//ライン
	CLine *		  m_apCopyLine[MAX_TERRITORY];	//コピーライン
	int			  m_nCntTimeCopyLine;			//ラインのタイマー
};

////==============================================
////		キャラクターの派生 → プレイヤー
////==============================================
//class CSpeedPlayer : public CPlayer
//{
//public:
//	CSpeedPlayer();
//	~CSpeedPlayer();
//
//	HRESULT Init(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40],char ModelTxt2[40], char MotionTxt[40], char MotionTxt2[40]);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	static CSpeedPlayer *Create(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40], char ModelTxt2[40], char MotionTxt[40], char MotionTxt2[40]);
//private:
//};

#endif