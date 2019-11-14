//=============================================================================
//
// ���U���g���� [result.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�
#define MAX_RESULTLOGO			(2)		// result���S�̍ő吔
#define CUNTRY					(8)		// ���̐�
#define RESULT_TEX				(1)		// ���U���g�̃e�N�X�`��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CResultCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CCharResult;
class CMoveUI;
class CScene2D;

//=====================
// ��{�N���X
//=====================
class CResult : public CModeBase
{
public:
	typedef enum
	{
		TYPE_OVER = 0,
		TYPE_CLEAR,
		TYPE_MAX,
	}TYPE;

	typedef enum
	{
		STATE_WHITE = 0,
		STATE_ZOOM,
		STATE_RANKING,
	}STATE;

	CResult();				// �R���X�g���N�^
	~CResult();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void LoadTex(void);
	void UnloadTex(void);
	static void SetType(TYPE type) { m_type = type; }

private:
	void UpdateWhite(void);
	void UpdateZoom(void);
	void UpdateRanking(void);
	static TYPE m_type;					//���ʏ��

	STATE m_state;
	LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_TEX];
	CResultCamera * m_pCamera;				//�J����
	CFieldManager * m_pFieldManager;		//�t�B�[���h�}�l�[�W���[
	CLoadTextObject * m_pLoadTextObj;		//�I�u�W�F�N�g�ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;	// �e���g���[�̓ǂݍ���
	CCharResult * m_pWinChar;				//�����L����
	CScene2D * m_pWhiteBack;				//���w�i
	CMoveUI * m_pNatinalFlag;				//����
	CMoveUI * m_pWinerName;					//���Җ�

	int m_nCounter;
	int m_nTime;
	bool m_bWhiteIn;						//�t�F�[�h�C���o���邩�ǂ���
};
#endif