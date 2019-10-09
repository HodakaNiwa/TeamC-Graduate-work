//=============================================================================
//
// �{�^���̏��� [Bottun.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _BOTTUN_H_
#define _BOTTUN_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 2D�|���S���N���X
//=============================================================================
class CBottun: public CScene2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_ENTER,
		STATE_MAX,
	}STATE;

	CBottun(int nPriority, CScene::OBJTYPE type);
	~CBottun();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	STATE m_State;		//��Ԑݒ�
	float m_fCol_a;		//�F�̃��l
	float m_fChangeCol;	//�F�̕ω���
};

#endif