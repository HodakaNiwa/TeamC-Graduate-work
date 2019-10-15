//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					キャラクター管理 [character.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//	---<<#include>>---
#include "main.h"
#include "motion.h"

//	---<<マクロ>>---

//	---<<前方宣言>>---
class CModel;
class CMotion;

//==============================================
//				キャラクター管轄クラス
//==============================================
class CCharacter
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
	HRESULT Init(char FileName[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<モデル情報の読み込み>>---
	void ModelSetLode(void);	
	//	---<<モーション情報の読み込み>>---
	void MotionSetLode(void);

	//	---<<外部データの読み込みに必要な処理>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);

private:
	//	---<<モデル関連変数>>---
	CModel **m_ppModel;
	char* m_TextName[40];						//	読み込むテキストファイル名
	static LPDIRECT3DTEXTURE9 m_pTexture;		
	static char *m_pUVTexName;					//	UVテクスチャ名
	static int m_nPartsNum;						//	パーツ数
	static char **m_aPartsName;					//	各パーツ名
	static int *m_aIndex;						//	インデックス
	static int *m_nParent;						//	親番号
	static MODEL_OFFSET *m_aOffset;				//	各パーツの位置等情報

	//	---<<モーション関連変数>>---
	CMotion *m_pMotion;
	static CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;

	D3DXVECTOR3					m_Pos;							//位置

};

#endif