//*****************************************************************************
//
//     �L�����A�C�R���̏���[charaicon.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARAICON_H_
#define _CHARAICON_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "skilicon.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CCharacter;

//*****************************************************************************
//    �L�����A�C�R���N���X�̒�`
//*****************************************************************************
class CCharaicon : public CIcon2D
{
public:    // �N�ł��A�N�Z�X�\
	CCharaicon();
	~CCharaicon();

	static CCharaicon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetParentChara(CCharacter *pParentChara) { m_pParentChara = pParentChara; }
	void SetDestChara(CCharacter *pDestChara) { m_pDestChara = pDestChara; }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }

	CCharacter *GetParentChara(void) { return m_pParentChara; }
	CCharacter *GetDestChara(void) { return m_pDestChara; }
	bool GetDisp(void) { return m_bDisp; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CheckDraw(void);
	void CalcCrossPoint_ScreenLine(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	CCharacter *m_pParentChara;		// �e�ƂȂ�v���C���[
	CCharacter *m_pDestChara;		// �ʒu���w���ړI�̃v���C���[
	bool m_bDisp;					// �`�悷�邩���Ȃ���
};

#endif