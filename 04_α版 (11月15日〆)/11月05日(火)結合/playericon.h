//=============================================================================
//
// �v���C���[�̃A�C�R���̏��� [Logo.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _PLAYERICON_H_
#define _PLAYERICON_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

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

private:
	void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer;}
	void UpdateInputSelect(void);
	void UpdatePos(void);
	int m_nNumCuntry;			//���ԍ�
	int m_nOldNumCuntry;		//�O��̍��ԍ�
	int m_nNumPlayer;			//�v���C���[�̔ԍ�
	int m_nCountInput;			//�C���v�b�g�̐�
	CMoveUI * m_pNationalFlag;	//����	
};

#endif