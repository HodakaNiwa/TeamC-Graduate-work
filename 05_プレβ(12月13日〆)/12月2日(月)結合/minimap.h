//*****************************************************************************
//
//     �~�j�}�b�v�̏���[minimap.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MINIMAP_MAX_PLAYERNUM (8)

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CCharacter;

//*****************************************************************************
//    �~�j�}�b�v�N���X�̒�`
//*****************************************************************************
class CMiniMap
{
public:    // �N�ł��A�N�Z�X�\
	CMiniMap();
	~CMiniMap();

	static CMiniMap *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SettingPlayerIconPos(void);
	void SettingBGPos(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetCharacter(CCharacter *pChara, const int nIdx) { m_pCharacter[nIdx] = pChara; }

	CScene2D *GetBG(void) { return m_pBG; }
	CScene2D *GetPlayerIcon(const int nIdx) { return m_pPlayerIcon[nIdx]; }
	CCharacter *GetCharacter(const int nIdx) { return m_pCharacter[nIdx]; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateBG(void);
	void CreatePlayerIcon(void);
	void ReleaseBG(void);
	void ReleasePlayerIcon(void);


	//--------------------
	//	�����o�ϐ�
	//--------------------
	CScene2D *m_pBG;									// �w�i�Ɏg�p����|���S��
	CScene2D *m_pPlayerIcon[MINIMAP_MAX_PLAYERNUM];		// �e�v���C���[�̈ʒu��\������|���S��
	CCharacter *m_pCharacter[MINIMAP_MAX_PLAYERNUM];	// �v���C���[�̏��ւ̃|�C���^
	D3DXVECTOR3 m_BGPos;								// �w�i�̈ʒu
};

#endif