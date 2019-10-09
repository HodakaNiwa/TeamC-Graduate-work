//=============================================================================
//
// ���U���g���� [result.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

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
class CResult
{
public:
	typedef enum
	{
		TYPE_OVER = 0,
		TYPE_CLEAR,
		TYPE_MAX,
	}TYPE;

	CResult();								// �R���X�g���N�^
	~CResult();								// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��
	
	static void SetType(TYPE type) { m_type = type; }

private:
	LPDIRECT3DTEXTURE9 * m_ppTexture;	// �e�N�X�`���̃|�C���^�ւ̃|�C���^
	int					m_nMaxTexture;	// �ǂݍ��ރe�N�X�`���̍ő吔
	int m_nCountTime;					//���Ԃ̃J�E���^�[
	static TYPE m_type;					//���ʏ��
};
#endif