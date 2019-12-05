//=============================================================================
//
// �v���C���[�̃A�C�R���̏��� [playericon.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _PLAYERICON_H_
#define _PLAYERICON_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "select.h"

//=============================================================================
// �O���錾
//=============================================================================
class CMoveUI;

//=============================================================================
// 2D�|���S���N���X
//=============================================================================
class CPlayerIcon: public CScene2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE,
		STATE_MAX,
	}STATE;

	CPlayerIcon();
	CPlayerIcon(int nPriority, CScene::OBJTYPE type);
	~CPlayerIcon();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayerIcon * Create(D3DXVECTOR3 Pos, int nNumPlayer);
	int GetNumCuntry(void) { return m_nNumCuntry; }
	void SetNumCuntry(int nNumCuntry) { m_nNumCuntry = nNumCuntry; }
	void UpdateMoveLeft(float fMove);
	void UpdateMoveRight(float fMove);
	void SetNumInput(int nNumInput) { m_nNumInput = nNumInput; }					//�R���g���[���[�̔ԍ���ۑ�
	void SetInputType(CSelect::INPUTTYPE InputType) { m_InputType = InputType; }	//���̓f�o�C�X�̃^�C�v��ݒ�

private:
	void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer;}
	void UpdateInputXpad(void);
	void UpdateInputKeyboard(void);
	void UpdateInputMouse(void);
	void UpdatePos(void);
	void UpdateMove(void);

	static STATE m_state;				//���
	int m_nNumCuntry;					//���ԍ�
	int m_nOldNumCuntry;				//�O��̍��ԍ�
	int m_nNumPlayer;					//�v���C���[�̔ԍ�
	int m_nNumInput;					//�R���g���[���[�̃i���o�[
	CMoveUI * m_pNationalFlag;			//����	
	CSelect::INPUTTYPE m_InputType;	//���̓f�o�C�X�̎��
};

#endif