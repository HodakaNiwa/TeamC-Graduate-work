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

//	---<<前方宣言>>---
class CModel;
class CMotion;
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


	//	---<<コンストラクタ・デストラクタ>>---
	CCharacter();
	~CCharacter();

	static CCharacter* Create();

	//	---<<基盤関数>>---
	HRESULT Init(char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Unload(void);
	

	//	---<<モデル情報の読み込み>>---
	void ModelSetLode(void);	
	//	---<<モーション情報の読み込み>>---
	void MotionSetLode(void);

	//	---<<外部データの読み込みに必要な処理>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);

	//	---<<Get関数>>---
	CModel** GetModel() { return m_ppModel; }
	//	---<<Set関数>>---

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

private:
	D3DXVECTOR3					m_Pos;							//位置
	D3DXVECTOR3					m_Scale;							//移動量
	D3DXVECTOR3					m_Rot;							//向き
	D3DXMATRIX					m_mtxWorld;						//ワールドマトリックス

	//	---<<モデル関連変数>>---
	CModel **m_ppModel;
	char* m_cModelTxt[40];						//	読み込むテキストファイル名
	static LPDIRECT3DTEXTURE9 m_pTexture;		
	 char *m_pUVTexName;					//	UVテクスチャ名
	 int m_nPartsNum;						//	パーツ数
	 char **m_aPartsName;					//	各パーツ名
	 int *m_aIndex;						//	インデックス
	 int *m_nParent;						//	親番号
	 MODEL_OFFSET *m_aOffset;				//	各パーツの位置等情報

	//	---<<モーション関連変数>>---
	CMotion *m_pMotion;
	char* m_cMotionTxt[40];						//	読み込むテキストファイル名
	CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;
};

#endif