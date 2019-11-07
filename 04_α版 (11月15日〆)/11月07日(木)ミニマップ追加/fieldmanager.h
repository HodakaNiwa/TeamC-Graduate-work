//=============================================================================
//
// �n�ʃ}�l�[�W���[���� [fieldmanager.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _FIELDMANAGER_H_
#define _FIELDMANAGER_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_FIELD	(32 * 32)
#define LINE		(32)
#define BLOCK_SIZE	(D3DXVECTOR3(62.5f, 0.0f, 62.5f))
#define FIELD_TEX	(4)

//=============================================================================
// �O���錾
//=============================================================================
class CField;

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CFieldManager
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_XTURN,
		STATE_RANDOM,
		STATE_WAVE,
		STATE_COUNTDOWN,
		STATE_TIME_2,
		STATE_TIME_1,
		STATE_TIME_30,
		STATE_MAX,
	}STATE;

	CFieldManager();
	~CFieldManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetState(STATE state);
	
private:
	void UpdateXTurn(void);
	void UpdateRamdom(void);
	void ChangeField(bool bChangeMain);
	void UpdateWave(void);
	void UpdateTime(void);
	void UpdateCountDown(void);

	CField *			m_pField[MAX_FIELD];		//�n�ʃ|�C���^
	CField *			m_pFieldGraund[MAX_FIELD];	//�n�ʃ|�C���^
	CField *			m_pCountDownField;			//�J�E���g�_�E���p�ϐ�
	CField *			m_pMainField;				//���C���̒n��
	LPDIRECT3DTEXTURE9	m_pTexture[FIELD_TEX];		// �e�N�X�`���ւ̃|�C���^
	STATE				m_state;					// ���
	int					m_nCountTime;				// ���Ԃ̉��Z
	int					m_nCountY;					// Y���̃J�E���^�[
	int					m_nCountX;					// X���̃J�E���^�[
	int					m_nCountY2;					// Y���̃J�E���^�[
	int					m_nCountX2;					// X���̃J�E���^�[
	int					m_nCountY3;					// Y���̃J�E���^�[
	int					m_nCountX3;					// X���̃J�E���^�[
	int					m_nCountY4;					// Y���̃J�E���^�[
	int					m_nCountX4;					// X���̃J�E���^�[
	float				m_fCol_a;					// �F�̃��l
};
#endif
