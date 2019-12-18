//=============================================================================
//
// �I�������̃v���C���[�̏��� [charMultRendering.cpp]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#include "charMultRendering.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define MINE_MODEL_NAME	("data/MODEL/mine000.x")
#define RIGHT_HAND		(5)

//�r���[�|�[�g�̈ʒu�ݒ�
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCharMultRender::CCharMultRender(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pModelMine = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;
	m_nNumPlayer = 0;
	m_bStopMotion = false;
	m_PlayerType = PLAYERTYPE_MAX;
	m_move = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_PosV = INITIALIZE_VECTOR3;
	m_PosR = INITIALIZE_VECTOR3;

	// �r���[�|�[�g�̏�����
	m_viewportMT.X = 0;						// X�̐ݒ�
	m_viewportMT.Y = 0;						// Y�̐ݒ�
	m_viewportMT.Width = SCREEN_WIDTH;		// ���̐ݒ�
	m_viewportMT.Height = SCREEN_HEIGHT;	// �����̐ݒ�
	m_viewportMT.MinZ = 0.0f;				// ��O
	m_viewportMT.MaxZ = 1.0f;				// ��
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCharMultRender::~CCharMultRender(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharMultRender::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry)
{
	//�v���C���[�̃i���o�[���擾
	m_nNumPlayer = nNumPlayer;
	m_MotionType = MOTION_TYPE_NONE;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pModelMine = NULL;
	m_bStopMotion = false;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType, nCuntry);
	CCharacter::SetPos(pos);

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();

	//�}���`�����_�����O�p�e�N�X�`���̐���
	InitMultTexture();
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharMultRender::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CCharMultRender::Uninit(void)
{
	CCharacter::Uninit();

	//�e�N�X�`���̔j��
	if (m_pTextureMT != NULL)
	{
		m_pTextureMT->Release();
		m_pTextureMT = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	//�o�b�t�@�̔j��
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
	}

	if (m_pMotion != NULL)
	{
		m_pMotion = NULL;
	}

	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}

	if (m_pModelMine != NULL)
	{
		m_pModelMine->Uninit();
		delete m_pModelMine;
		m_pModelMine = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  CCharMultRender::Update(void)
{
	if (m_pMotion == NULL) { return; }

	if (m_MotionType == MOTION_TYPE_SELECT)
	{
		//���[�V�������̎擾
		int nKey = m_pMotion->GetKey();
		int nFram = m_pMotion->GetnCountMotion();

		switch (m_CharcterType)
		{
		case CCharacter::CHARCTERTYPE_POWER:
			if ((nKey == 1) && (nFram == 16)) { m_bStopMotion = true; }
			break;

		case CCharacter::CHARCTERTYPE_SPEED:
			if ((nKey == 1) && (nFram == 12)) { m_bStopMotion = true; }
			break;

		case CCharacter::CHARCTERTYPE_TECHNIQUE:
			//�n���̐���
			if (m_pModelMine == NULL)
			{
				m_pModelMine = CModel::Create(D3DXVECTOR3(0.0f, -9.0f, 0.0f), MINE_MODEL_NAME, D3DXVECTOR3(2.5f, 2.5f, 2.5f));
				m_pModelMine->SetRot(D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f));
				m_pModelMine->SetParent(m_pModel[RIGHT_HAND]);
			}

			if ((nKey == 2) && (nFram == 13)) { m_bStopMotion = true; }
			break;
		}
	}
	else
	{
		m_bStopMotion = false;

		//�n���̔j��
		if (m_pModelMine != NULL)
		{
			m_pModelMine->Uninit();
			delete m_pModelMine;
			m_pModelMine = NULL;
		}
	}

	if (m_bStopMotion) { return; }
	CCharacter::Update();

	if (m_pMotion->GetNumMotion() == m_MotionType) { return; }

	//���[�V�����̐ݒ�
	switch (m_MotionType)
	{
	case MOTION_TYPE_LOSE:
		m_pMotion->SetNumMotion(MOTION_TYPE_LOSE);
		break;

	case MOTION_TYPE_SELECT:
		m_pMotion->SetNumMotion(MOTION_TYPE_SELECT);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CCharMultRender::Draw(void)
{
	//�ϐ��錾
	PDIRECT3DSURFACE9 pRenderOrg;	//�����_���[�̕ۑ��p�ϐ�
	LPDIRECT3DSURFACE9 pBuffOrg;	//�o�b�t�@�̕ۑ��p�ϐ�
	D3DVIEWPORT9 viewportOrg;
	D3DXMATRIX  mtxProjectionOrg;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  mtxViewOrg;			//�r���[�}�g���N�X
	D3DXMATRIX  mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  mtxView;			//�r���[�}�g���N�X

									//�f�o�C�X�̎擾
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���݂̏�Ԃ�ۑ�����
	pDevice->GetViewport(&viewportOrg);							//�r���[�|�[�g�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);				//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//�v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->GetRenderTarget(0, &pRenderOrg);
	pDevice->GetDepthStencilSurface(&pBuffOrg);

	//�ݒ菈��
	pDevice->SetRenderTarget(0, m_pRenderMT);
	pDevice->SetDepthStencilSurface(m_pBuffMT);

	//�^�[�Q�b�g�EZ�o�b�t�@�̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	pDevice->SetViewport(&m_viewportMT);	//�r���[�|�[�g�̐ݒ�

	D3DXMatrixIdentity(&mtxView);			//�r���[�}�g���b�N�X�̏�����

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView, &m_PosV, &m_PosR, &POS_U);

	// �r���[�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2500.0f);										//�J�����̕`��͈�

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	CCharacter::Draw();

	//�n���̕`��
	if (m_pModelMine != NULL) { m_pModelMine->Draw(1.0f); }

	//�f�t�H���g�ɖ߂�
	pDevice->SetViewport(&viewportOrg);							//�r���[�|�[�g�̎擾
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);				//�r���[�}�g���b�N�X�̎擾
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//�v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->SetRenderTarget(0, pRenderOrg);
	pDevice->SetDepthStencilSurface(pBuffOrg);
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CCharMultRender *CCharMultRender::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],CHARCTERTYPE charctertype, int nNumCharaIdx, int nCuntry)
{
	CCharMultRender *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CCharMultRender;
		if (pPlayer != NULL)
		{//NULL�`�F�b�N
			pPlayer->m_CharcterType = charctertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumCharaIdx, nCuntry);
		}
		else
		{
			MessageBox(0, "�v���C���[��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "�v���C���[��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CCharMultRender::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// �e�N�X�`���̏���������
//=============================================================================
void CCharMultRender::InitMultTexture(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg = NULL;
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;

	// Texture�̍쐬
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// �����_���[�̎擾
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// �o�b�t�@�̐���
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// ���݂̃����_���[�^�[�Q�b�g��ۑ�
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// ���݂̃o�b�t�@��ۑ�
	pDevice->SetRenderTarget(0, m_pRenderMT);		// �����_���[�̐ݒ�
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// �o�b�t�@�̐ݒ�
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ���ɖ߂�
	pDevice->SetRenderTarget(0, pRenderOrg);		// �����_���[
	pDevice->SetDepthStencilSurface(pBuffOrg);		// �o�b�t�@

	// �r���[�|�[�g�̏�����
	m_viewportMT.X = 0;						// X�̐ݒ�
	m_viewportMT.Y = 0;						// Y�̐ݒ�
	m_viewportMT.Width = SCREEN_WIDTH;		// ���̐ݒ�
	m_viewportMT.Height = SCREEN_HEIGHT;	// �����̐ݒ�
	m_viewportMT.MinZ = 0.0f;				// ��O
	m_viewportMT.MaxZ = 1.0f;				// ��

	//���_�A�����_�̐ݒ�
	D3DXVECTOR3 Pos = CCharacter::GetPos();
	m_PosV = Pos + POS_V;
	m_PosR = Pos + POS_R;
}