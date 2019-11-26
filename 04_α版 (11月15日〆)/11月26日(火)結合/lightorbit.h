//*****************************************************************************
//
//     ���̋O�Ղ̏���[lightorbit.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHTORBIT_H_
#define _LIGHTORBIT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    ���̋O�ՃN���X�̒�`
//*****************************************************************************
class CLightOrbit : public CScene
{
public:		// �N�ł��A�N�Z�X�\
	CLightOrbit(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIGHTORBIT);
	~CLightOrbit();

	static CLightOrbit *Create(D3DXVECTOR3 pos, D3DXVECTOR3 *pDestPos, int nDestPlayerIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}

	//------------------------
	//  Set & Get
	//------------------------
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_Move = move; }
	void SetDestPos(D3DXVECTOR3 *pDestPos) { m_pDestPos = pDestPos; }
	void SetCounter(const int nCounter) { m_nCounter = nCounter; }
	void SetDestPlayerIdx(const int nDestPlayerIdx) { m_nDestPlayerIdx = nDestPlayerIdx; }

	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXVECTOR3 *GetDestPos(void) { return m_pDestPos; }
	int GetCounter(void) { return m_nCounter; }
	int GetDestPlayerIdx(void) { return m_nDestPlayerIdx; }


protected:	// ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void ClearVariable(void);

private:	// ���̃N���X�������A�N�Z�X�\
	//------------------------
	//  �֐�
	//------------------------
	bool CalcMove(void);
	void Movement(void);
	void CreateEffect3D(void);

	//------------------------
	//  �����o�ϐ�
	//------------------------
	D3DXVECTOR3 m_Pos;			// ���g�̈ʒu
	D3DXVECTOR3 m_Move;			// �ړ���
	D3DXVECTOR3 *m_pDestPos;	// �ړI�̍��W
	int m_nCounter;				// �G�t�F�N�g�̏o�����Ǘ�����J�E���^�[
	int m_nDestPlayerIdx;		// �ړI�̃v���C���[�̔ԍ�
	float m_fAccel;				// ������
};

#endif