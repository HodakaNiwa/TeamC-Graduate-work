//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "modebase.h"
#include <dshow.h>

//=================================
// �O���錾
//=================================
class CMovie;

//=================================
// �`���[�g���A���N���X
//=================================
class CTutorial : public CModeBase
{
public:
	CTutorial();				// �R���X�g���N�^
	~CTutorial();				// �f�X�g���N�^
	void	Init(void);			// ����������
	void	Uninit(void);		// �I������
	void	Update(void);		// �X�V����
	void	Draw(void);			// �`�揈��

private:
	CMovie * m_pMovie;
};
#endif