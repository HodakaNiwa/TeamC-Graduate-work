//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�v���C���[�Ǘ� [player.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _PLAYER_H_
#define _PLAYER_H_

//	---<<#include>>---
#include "main.h"
#include "character.h"

//	---<<�}�N��>>---

//	---<<�O���錾>>---
class CModel;
//==============================================
//				�L�����N�^�[�Ǌ��N���X
//==============================================
class CPlayer : public CCharacter
{
public:
	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CPlayer();
	~CPlayer();

	static CPlayer* Create(char FileName[40]);

	//	---<<��Պ֐�>>---
	HRESULT Init(char FileName[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	
};

#endif