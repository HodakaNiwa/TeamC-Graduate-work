//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					プレイヤー管理 [player.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _PLAYER_H_
#define _PLAYER_H_

//	---<<#include>>---
#include "main.h"
#include "character.h"

//	---<<マクロ>>---

//	---<<前方宣言>>---
class CModel;
//==============================================
//				キャラクター管轄クラス
//==============================================
class CPlayer : public CCharacter
{
public:
	//	---<<コンストラクタ・デストラクタ>>---
	CPlayer();
	~CPlayer();

	static CPlayer* Create(char ModelTxt[40], char MotionTxt[40]);

	//	---<<基盤関数>>---
	HRESULT Init(char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//	位置・回転情報
	CModel** m_pModel;
	
};

#endif