//=============================================================================
//
// �Z���N�g���� [select.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"
#include "modebase.h"

//=================================
// �Z���N�g�N���X
//=================================
class CSelect : public CModeBase
{
public:
	CSelect();				// �R���X�g���N�^
	~CSelect();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
};
#endif