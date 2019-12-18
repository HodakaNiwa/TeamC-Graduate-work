//*****************************************************************************
//
//     �X�R�A�ϓ��̏���[scoreChange.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCORECHANGE_H_
#define _SCORECHANGE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CNumber2D;

//*****************************************************************************
//    �X�R�A�ϓ��N���X�̒�`
//*****************************************************************************
class CScoreChange : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	CScoreChange(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScoreChange();

	static CScoreChange *Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size, int nScoreChange);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 SizeAdjust(D3DXVECTOR3 size);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumber2D(CNumber2D *pNumber2D, const int nIdx) { m_pNumber2D[nIdx] = pNumber2D; }
	void SetNumberDigit(const int nNumberDigit) { m_nNumberDigit = nNumberDigit; }
	void SetScoreChange(const int nScoreChange) { m_nScoreChange = nScoreChange; }
	void SetChangeTimer(const int nChangeTimer) { m_nChangeTimer = nChangeTimer; }

	CNumber2D *GetNumber2D(const int nIdx) { return m_pNumber2D[nIdx]; }
	int GetNumberDigit(void) { return m_nNumberDigit; }
	int GetScoreChange(void) { return m_nScoreChange; }
	int GetChangeTimer(void) { return m_nChangeTimer; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CalcScoreChangeDigit(void);
	void CreateNumber(void);
	void ReleaseNumber(void);
	void PosDown(void);
	void PosDown_Number(void);
	void AlphaAdd(void);
	void AlphaAdd_Number(const D3DXCOLOR col);
	void DrawNumber(void);


	//--------------------
	//	�����o�ϐ�
	//--------------------
	CNumber2D **m_pNumber2D;	// �����|���S���N���X�ւ̃|�C���^
	int m_nNumberDigit;			// �����̌���
	int m_nScoreChange;			// �ϓ������X�R�A��
	int m_nChangeTimer;			// ���l�̕ύX���Ǘ�����J�E���^�[
};

#endif