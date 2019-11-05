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
// �}�N����`
//=================================
#define MAX_COUNTRY	(8)
#define NUMPLAYER	(4)
#define SELECT_TEX	(2)

//=================================
// �O���錾
//=================================
class CCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CMoveUI;
class CPlayerIcon;

//=================================
// �Z���N�g�N���X
//=================================
class CSelect : public CModeBase
{
public:
	typedef enum
	{
		STATE_CUNTRY_SELECT = 0,	//���I��
		STATE_CHARACTER_SELECT,		//�L�����I��
		STATE_DICISION,				//����
		STATE_CHANGE,				//�ύX���
	}STATE;

	CSelect();				// �R���X�g���N�^
	~CSelect();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	static CMoveUI * GetNatonalFlag(int nCuntry) { return m_pNationalFlag[nCuntry]; }	//�����̃|�C���^���擾
	static int GetEntryPlayer(void) { return m_nEntryPlayer; }
	LPDIRECT3DTEXTURE9 GetCuntryTex(int nTexNum) { return m_apTextureCountry[nTexNum]; }
	void SetState(STATE state) { m_state = state; }

private:
	void LoadTexture(void);
	void UnloadTexture(void);
	void InitCuntrySelect(void);
	void UpdateCuntrySelect(void);
	void InitCharacterSelect(void);
	void UpdateCharacterSelect(void);
	void UninitCharacterSelect(void);
	void UpdateDicision(void);
	void UpdateChange(void);

	static CMoveUI * m_pNationalFlag[MAX_COUNTRY];		//���̐�
	static int m_nEntryPlayer;							//�v���C���[�̃G���g���[��

	LPDIRECT3DTEXTURE9 m_apTextureCountry[MAX_COUNTRY];	//�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_apTexPlayer[NUMPLAYER];		//�v���C���[
	LPDIRECT3DTEXTURE9 m_apTexCuntoryName[MAX_COUNTRY];	//���̖��O
	LPDIRECT3DTEXTURE9 m_pTexNoEntry;					//�G���g���[�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pTex[SELECT_TEX];				//���̑��̃e�N�X�`��
	CCamera * m_pCamera;								//�J����
	CFieldManager * m_pFieldManager;					//�t�B�[���h�}�l�[�W���[
	CLoadTextObject * m_pLoadTextObj;					//�I�u�W�F�N�g�ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;				// �e���g���[�̓ǂݍ���
	CPlayerIcon * m_pNumPlayer[NUMPLAYER];				//�v���C���[�̑I���摜
	CMoveUI * m_pCuntoryName[NUMPLAYER];				//���̖��O�\��
	CMoveUI * m_pNoEntry[NUMPLAYER];					//�v���C���[�̃G���g���[���Ă��邩�ǂ���
	CMoveUI * m_pLogo;									//���SUI
	CMoveUI * m_pFram[NUMPLAYER];						//�t���[��
	STATE m_state;										//��Ԑݒ�
};
#endif