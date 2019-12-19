//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "modebase.h"
#include "scene.h"

//=================================
// �}�N����`
//=================================
#define TEX_TUTORIAL_BG (10)
#define TEX_TUTORIAL    (3)
#define MAX_OBJTYPE		(13)

//=================================
// �O���錾
//=================================
class CMoveUI;
class CLoadTextTerritory;
class CCamera;
class CFieldManager;
class CLoadTextEffect;
class CCharacter;
class CTutorialTechniquePlayer;
class CTerritoryRender;
class CRingRender;
class CEffectManager;

//=================================
// �`���[�g���A���N���X
//=================================
class CTutorial : public CModeBase
{
public:
	typedef enum
	{
		STATE_FIRST = 0,		//�`��
		STATE_WINCONDITIONS,	//��������
		STATE_GETTERRITORY1,	//�e���g���[�̎擾�P
		STATE_GETTERRITORY2,	//�e���g���[�̎擾2
		STATE_CUTLINE,			//���C����؂�
		STATE_POINT,			//�|�C���g
		STATE_CONTROLER,		//�R���g���[���[
		STATE_CHARACTER,		//�L�����N�^�[
		STATE_END,
	}STATE;

	CTutorial();			// �R���X�g���N�^
	~CTutorial();			// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	void LoadTex(void);		//�e�N�X�`���̓ǂݍ���
	void UnloadTex(void);	//�e�N�X�`���̔j��
	CLoadTextEffect * GetEffectLoad(void) { return m_pLoadEffect; }
	CCharacter * GetTutorialPlayer(void) { return m_pCharTutorial; }
	CEffectManager * GetEffectManager(void) { return m_pEffectManager; }
	CRingRender * GetRingRender(void) { return m_pRingRender; } //��

private:
	void UpdateFirst(void);
	void UpdateWin(void);
	void UpdateGetTerritory(void);
	void UpdateCutLine(void);
	void UpdateGetPoint(void);
	void UpdateControler(void);
	void UpdateCharacter(void);
	void UpdateEnd(void);
	void DeleteTitle(void);
	void MoveTitle(STATE NextState);
	bool MoveSlideTitle(void);
	bool TextFadeInOut(CMoveUI * pText, int nFadeOutTime, int nCountTime);
	void CreateMultiWindow(D3DXVECTOR3 Pos, D3DXVECTOR3 Size);
	void DeleteMultiWindow(void);
	void CreateMultRenderingTex(void);
	void DrawMultRendering(void);
	void SetObjTypeList(void);
	void DeleteCharacter(void);

	LPDIRECT3DTEXTURE9 m_pTextureBG[TEX_TUTORIAL_BG];
	LPDIRECT3DTEXTURE9 m_pTextureText[TEX_TUTORIAL];
	LPDIRECT3DTEXTURE9 m_pTextureLogo;

	STATE m_state;
	CMoveUI * m_pBG;
	CMoveUI * m_pTitle;
	CMoveUI * m_pText00;
	CMoveUI * m_pText01;
	CMoveUI * m_pWindow;
	CScene::OBJTYPE m_ObjTypeList[MAX_OBJTYPE];
	int m_nCountTime;
	bool m_bMoveSlideTitle;
	bool m_bSlideOut;

	//�}���`�����_�����O�̐ݒ�
	LPDIRECT3DTEXTURE9 m_pTextureMT;		// �����_�����O�p��Texturte
	LPDIRECT3DSURFACE9 m_pRenderMT;	// Texture�Ƀ����_�����O���邽�߂̃p�C�v
	LPDIRECT3DSURFACE9 m_pBuffMT;	// Texture�Ɏg�p����o�b�t�@
	D3DVIEWPORT9	   m_viewportMT;	// Texture�Ɏg�p����r���[�|�[�g
	D3DXVECTOR3		   m_PosV;		// ���_
	D3DXVECTOR3		   m_PosR;		// �����_

	//�ǂݍ��ރI�u�W�F�N�g
	CLoadTextTerritory * m_pLoadTerritory;
	CFieldManager * m_pFieldManager;
	CCamera * m_pCamera;
	CLoadTextEffect * m_pLoadEffect;			//	�G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CCharacter * m_pCharTutorial;				// �`���[�g���A���L����
	CCharacter * m_pCharDamege;					// �_���[�W���󂯂�L�����N�^�[
	CTerritoryRender * m_pTerritoryRender;		// �e���g���[�����_���[�N���X�ւ̃|�C���^�[
	CRingRender * m_pRingRender;				// �����O�����_���[�ւ̃|�C���^�[
	CEffectManager * m_pEffectManager;			//�G�t�F�N�g�}�l�[�W���[
};
#endif