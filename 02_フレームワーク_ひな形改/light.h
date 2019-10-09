//=============================================================================
//
// ���C�g���� [light.h]
// Author : �������� Sato_Asumi 
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAXLIGHT	(3)		//���C�g�̍ő吔
#define MAX_COL		(3)		//RGB

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;

//*****************************************************************************
// ���C�g�N���X
//*****************************************************************************
class CLight
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_RED,
		TYPE_GREEN,
		TYPE_BLUE,
	}TYPE;

	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);
	D3DLIGHT9 m_light[MAXLIGHT];	//���C�g�̐ݒ�
	D3DXVECTOR3 GetvecDir(int nNumLight) { return m_vecDir[nNumLight]; };	//���C�g�̃x�N�g�����擾

private:
	void UpdateLight(void);
	void ChangeNumber(int * nNum, CInputKeyboard * pInputKeyboard);
	bool SetValue(float * fValue_X, float * fValue_Y, float * fValue_Z, CInputKeyboard * pInputKeyboard);
	void SetLight(void);
	void SetColor(void);
	D3DXVECTOR3 m_vecDir[MAXLIGHT];
	TYPE m_type;						//��Ԑݒ�
	bool m_bSwich;						//���C�g�̐؂�ւ�
	bool m_bSetLight;					//���C�g�̐ݒ肵�����ǂ���
	int m_nNumLight;					//���C�g�̔ԍ�
	int m_nNumDir;						//�����̔ԍ�
	int m_nCntTime;						//���Ԃ̃J�E���^�[
	D3DXCOLOR m_Col;					//���݂̐F
	D3DXCOLOR m_AimCol;					//�ړI�̐F
};
#endif
