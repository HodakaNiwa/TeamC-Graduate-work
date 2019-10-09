//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_POLYGON (5)

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//=============================================================================
// �|�[�Y�N���X
//=============================================================================
class CPause
{
public:
	typedef enum
	{
		STATE_CONTINUE = 0,
		STATE_RETRY,
		STATE_QUIT,
		STATE_MAX,
	}STATE;

	typedef enum
	{
		STATEBOTTUN_NORMAL = 0,
		STATEBOTTUN_SELECT,
	}STATE_BOTTUN;

	CPause();
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause * Create(void);

private:
	void BottunUpdate(void);
	CScene2D * m_pScene2D[MAX_POLYGON];	//2D�|���S���̃_�u���|�C���^
	STATE m_state;						//���
	STATE_BOTTUN m_StateBottun;			//�{�^���̏�Ԑݒ�
	bool m_bDeth;						//���S���
};
#endif
