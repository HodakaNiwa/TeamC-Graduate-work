//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					キャラクター管理 [character.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//	---<<#include>>---
#include "main.h"
#include "motion.h"
#include "scene.h"

//	---<<マクロ>>---
#define MAX_TERRITORY	(10)
#define TRIANGLE_VERTEX	(3)
#define FIRST_LINETIME	(60 * 20)	//ラインの時間の初期数
#define MIN_TERRITORY	(3)			//テリトリーの最低限数
#define TERRITORY_TOP	(0)			//テリトリーの先頭情報
#define NOT_GETTIME		(60)		//テリトリーの取得できない時間
#define LINE_TIME		(90)		//ラインを消さない時間

//	---<<前方宣言>>---
class CModel;
class CMotion;
class CTerritory;
class CLine;
class CSceneOrbit;

//==============================================
//				キャラクター管轄クラス
//==============================================
class CCharacter : public CScene
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}MODEL_OFFSET;

	typedef enum
	{//プレイヤーのタイプ
		CHARCTERTYPE_SPEED = 0,
		CHARCTERTYPE_POWER,
		CHARCTERTYPE_TECHNIQUE,
		CHARCTERTYPE_MAX
	}CHARCTERTYPE;

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

	typedef struct
	{
		D3DXVECTOR3 pos[TRIANGLE_VERTEX];
		D3DXVECTOR3 VecA[TRIANGLE_VERTEX];
	}TRAIANGLE;


	//	---<<コンストラクタ・デストラクタ>>---
	CCharacter(int nPriority, OBJTYPE objType);
	~CCharacter();

	static CCharacter* Create();

	//	---<<基盤関数>>---
	HRESULT Init(int nCharaNum, char ModelTxt[40], char MotionTxt[40],int nType, int nCuntry);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Unload(void);


	//	---<<モデル情報の読み込み>>---
	void ModelSetLode(void);
	//	---<<モーション情報の読み込み>>---
	void MotionSetLode(void);
	//	---<<テクスチャの読み込み>>---
	void TextureSetLode(int nStartIdx, int nNumTex);

	//	---<<外部データの読み込みに必要な処理>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);
	static D3DXCOLOR m_CountryColor[CCharacter::TYPE_MAX];	//国の色

															//	---<<Get関数>>---
	CModel** GetModel() { return m_ppModel; }
	CMotion *GetMotion() { return m_pMotion; }

	//	---<<Set関数>>---
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

	int GetAttack(void) { return m_nAttack; }					//攻撃力の取得
	float GetAttackSpeed(void) { return m_fAttackSpeed; }		//攻撃速度の取得
	float GetRange(void) { return m_fRange; }					//射程の取得
	float GetSpeed(void) { return m_fSpeed; }					//移動量の取得
	float GetInertia(void) { return m_fInertia; }				//慣性の取得
	float GetLineTime(void) { return m_fLineTime; }				//線を描く時間の取得

	void AddTerritoryList(CTerritory * pTerritory);
	void CutLine(int nID);

	//国のタイプ Geter & Set
	void SetType(int nType) { m_nType = nType; }
	int GetType(void) { return m_nType; }

	//キャラタイプの取得
	CHARCTERTYPE GetCharcterType(void) { return m_CharcterType; }

	//リザルト用キャラタイプと国の取得
	static CHARCTERTYPE GetCharcterTypeResult(int nType) { return m_CharcterTypeResult[nType]; }
	int GetCuntryResult(int nNumCuntry) { return m_nCuntry[nNumCuntry]; }

protected:
	void CountDownLineTime(void);										//テリトリーの取得更新処理
	void ResetList(void);												//リストの初期化
	bool ChackSamePos(CTerritory * pTerritory, int nNumTerritory);		//同じ位置かどうか
	void ShapeComplete(CTerritory * pTerritory);						//完成したときの処理
	void ChackInShape(void);											//図形に入っているかどうかの処理
	void GetTerritory(CTerritory * pTerritory);							//テリトリーの取得
	void ResetLine(void);												//ラインの生成処理
	void UninitLine(void);												//ラインの破棄
	void UninitCopyLine(void);											//コピーラインの破棄
	void CreateEndLine(void);											//最後の線の生成
	void UpdateShapeComplete(void);										//完成後の更新処理
	void InitTraiangle(TRAIANGLE * pTraiangle, int nMaxTraiangle);		//三角形の構造体の初期化
	void MakeTraiangle(int nCnt, int * nCountTraiangle, int * pnFarNumTerritory, D3DXVECTOR3 * pListPos, TRAIANGLE * pTraiangle);				//三角形を作る処理
	void MakePolygon(TRAIANGLE pTraiangle);								//ポリゴンの生成
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture) { m_ppTexture = pTexture; }

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
	int m_nLineTime;							//ラインの時間
	int m_nMaxLineTime;							//ラインの時間最大数
	int m_nNumPlayerNo;							//プレイヤーナンバー(エネミーも含む)

	//国のタイプ
	int m_nType;

	//リキャスト用変数
	int m_nButtonCounter;						//ボタンカウンター
	int m_nRecastCounter;						//リキャスト用カウンター
	int m_nRecastTimer;							//リキャスト時間
	bool m_bRecast;								//リキャストしてるか

	//キャラクター用変数
	CHARCTERTYPE m_CharcterType;						//ゲーム用のタイプ
	static CHARCTERTYPE m_CharcterTypeResult[8];		//リザルト用のキャラタイプ
	static  int m_nCuntry[8];							//リザルト用の国タイプ
	bool m_bCharaMotionState;							//モーション状態の管理
private:
	D3DXVECTOR3					m_Pos;							//位置
	D3DXVECTOR3					m_Scale;						//移動量
	D3DXVECTOR3					m_Rot;							//向き
	D3DXMATRIX					m_mtxWorld;						//ワールドマトリックス

	//	---<<モーション関連関数>>---
	void ClearMotionInfo(void);
	void ClearMotionInfo_KeyInfo(int nMotionIdx);
	void ClearMotionInfo_KeyInfo_Key(int nMotionIdx, int nKeyInfoIdx);

	//	---<<軌跡の関数>>---
	virtual void CreateOrbitLine(void) = 0;								//奇跡の線の生成
	virtual void UninitOrtbitLine(void) = 0;							//ラインの破棄

	//	---<<コリジョン関連関数>>---
	void CreateCylinderCollider(void);

	//	---<<モデル関連変数>>---
	CModel **m_ppModel;
	char* m_cModelTxt[40];					//	読み込むテキストファイル名
	LPDIRECT3DTEXTURE9 *m_ppTexture;		//	テクスチャへのポインタ(ダブルポインタ)
	char **m_ppUVTexName;					//	UVテクスチャ名
	int m_nTexNum;							//	使用するテクスチャの数
	int m_nLoadTexNum;						//	読み込んだテクスチャの数
	int m_nPartsNum;						//	パーツ数
	char **m_aPartsName;					//	各パーツ名
	int *m_aIndex;							//	インデックス
	int *m_nParent;							//	親番号
	int *m_nTexIdx;							//	使用するテクスチャの番号
	MODEL_OFFSET *m_aOffset;				//	各パーツの位置等情報

											//	---<<モーション関連変数>>---
	CMotion *m_pMotion;
	char* m_cMotionTxt[40];						//	読み込むテキストファイル名
	CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;

	//	---<<キャラクターステータス関連変数>>---
	int m_nAttack;							//攻撃力
	float m_fAttackSpeed;					//攻撃速度
	float m_fRange;							//射程
	float m_fSpeed;							//移動量
	float m_fInertia;						//慣性
	float m_fLineTime;						//線を描ける時間
	void CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer);	// テリトリースコアの削除
	void AddTerritoryPoint(CTerritory * pTerritory);					// テリトリースコアの追加
};

#endif