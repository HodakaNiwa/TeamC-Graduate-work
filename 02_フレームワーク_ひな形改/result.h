//=============================================================================
//
// ���U���g���� [result.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�
#define MAX_RESULTLOGO			(2)		// result���S�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************

//=====================
// ��{�N���X
//=====================
class CResult : public CModeBase
{
public:
	typedef enum
	{
		TYPE_OVER = 0,
		TYPE_CLEAR,
		TYPE_MAX,
	}TYPE;

	CResult();				// �R���X�g���N�^
	~CResult();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	static void SetType(TYPE type) { m_type = type; }

private:
	static TYPE m_type;					//���ʏ��
};
#endif