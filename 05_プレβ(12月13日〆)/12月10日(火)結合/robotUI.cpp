//====================================================================================
//
// ��9���UI����	[robotUI.cpp]
// Author;�r�J�R�N
//
//====================================================================================
#include "robotUI.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "debuglog.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define WARNING_TEX_FILENAME_0 "data/TEXTURE/game/Warning000.jpg"	// �댯�x��

// �댯�x��
#define ROBOTUI_WARNING_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,70.0f,1.0f))
#define ROBOTUI_WARNING_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define ROBOTUI_WARNING_WIDTH	(SCREEN_WIDTH)
#define ROBOTUI_WARNING_HEIGHT	(70.0f)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRobotUI::m_apTexture[ROBOTUI_TEXTURE_MAX] = {};

//*****************************************************************************
//     CRobotUI�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRobotUI::CRobotUI()
{
	m_pRobotWarning = NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRobotUI::~CRobotUI()
{
}

//=============================================================================
//    ��������
//=============================================================================
CRobotUI *CRobotUI::Create(int nNum)
{
	CRobotUI *pRobotUI = NULL;
	if (pRobotUI != NULL) { return NULL; }

	// ���������m��
	pRobotUI = new CRobotUI;
	if (pRobotUI == NULL) { return NULL; }

	// ����������
	if (FAILED(pRobotUI->Init(nNum)))
	{
		//pRobotUI->Uninit();
		//delete pRobotUI;
		//return NULL;
	}

	if (pRobotUI != NULL)
	{
		pRobotUI->m_nNum = nNum;
	}

	return pRobotUI;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CRobotUI::Load(void)
{
	char aFileName[ROBOTUI_TEXTURE_MAX][256]
	{
		WARNING_TEX_FILENAME_0,
	};

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̊J������
//=============================================================================
void CRobotUI::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRobotUI::Init(int nNum)
{
	// �댯�x���𐶐�
	CreateWarning(nNum);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRobotUI::Uninit(void)
{
	// �댯�x�����J��
	ReleaseWarning();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRobotUI::Update(void)
{
	switch (m_nNum)
	{
	case 0:			// �댯�x��
		UpdateWarning();
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRobotUI::Draw(void)
{
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::CreateWarning(int nNum)
{
	m_pRobotWarning = CRobotUI2D::Create(ROBOTUI_WARNING_POS, ROBOTUI_WARNING_COL,
		(float)ROBOTUI_WARNING_WIDTH, ROBOTUI_WARNING_HEIGHT, (float)nNum);
	if (m_pRobotWarning == NULL) { return; }
	m_pRobotWarning->BindTexture(m_apTexture[0]);
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::ReleaseWarning(void)
{
	if (m_pRobotWarning != NULL)
	{
		m_pRobotWarning->Uninit();
		m_pRobotWarning = NULL;
	}
}

//=============================================================================
//    �댯�x���̓���
//=============================================================================
void CRobotUI::UpdateWarning(void)
{
	if (m_pRobotWarning != NULL)
	{// �댯�x����NULL�`�F�b�N
		CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

		bool bEveCamFlag = pGame->GetEVeCamFlag();	// �C�x���g�J�����̎��S�t���O���擾
		if (bEveCamFlag == true)
		{// �C�x���g�J�����������Ă��������
			Uninit();
		}
	}
}

//*****************************************************************************
//     CRobotUI2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRobotUI2D::CRobotUI2D(): CScene(4, CScene::OBJTYPE_ROBOT)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;                         // ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �|���S���̐F
	m_fRot = 0.0f;                             // �|���S���̌���
	m_fWidth = 0.0f;                           // �|���S���̕�
	m_fHeight = 0.0f;                          // �|���S������
	m_fAngle = 0.0f;                           // �|���S�����o���p�x
	m_fLength = 0.0f;                          // �|���S���̒���
	m_fTexU = 0.0f;                            // ����e�N�X�`��U���W
	m_fTexV = 0.0f;                            // ����e�N�X�`��V���W
	m_fTexWidth = 0.0f;                        // �e�N�X�`�����W�̉���
	m_fTexHeight = 0.0f;                       // �e�N�X�`�����W�̏c��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRobotUI2D::~CRobotUI2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRobotUI2D *CRobotUI2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot, int nNum)
{
	// ���������m��
	CRobotUI2D *pRobotUI2D = NULL;
	pRobotUI2D = new CRobotUI2D;
	if (pRobotUI2D == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pRobotUI2D->SetPos(pos);              // �|���S���̍��W
	pRobotUI2D->SetRot(fRot);             // �|���S���̌���
	pRobotUI2D->SetCol(col);              // �|���S���̐F
	pRobotUI2D->SetWidth(fWidth);         // �|���S���̕�
	pRobotUI2D->SetHeight(fHeight);       // �|���S���̍���
	pRobotUI2D->SetTexU(fTexU);           // �|���S���̍���e�N�X�`��U���W
	pRobotUI2D->SetTexV(fTexV);           // �|���S���̍���e�N�X�`��V���W
	pRobotUI2D->SetTexWidth(fTexWidth);   // �|���S���̃e�N�X�`�����W�̉���
	pRobotUI2D->SetTexHeight(fTexHeight); // �|���S���̃e�N�X�`�����W�̏c��

	// ����������
	if (FAILED(pRobotUI2D->Init()))
	{
		pRobotUI2D->Uninit();
		delete pRobotUI2D;
		return NULL;
	}

	return pRobotUI2D;		// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRobotUI2D::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRobotUI2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRobotUI2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRobotUI2D::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CRobotUI2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�֒l��ݒ肷�鏈��
	SetVtxBuffValue();
}

//=============================================================================
//    �|���S���̑傫����ݒ肷�鏈��
//=============================================================================
void CRobotUI2D::SetScale(float fWidth, float fHeight)
{
	// �I�t�Z�b�g�̒����Ɗp�x���v�Z
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fLength /= 2.0f;
	m_fAngle = atan2f(m_fWidth, -m_fHeight);
}

//=============================================================================
//    ���_�o�b�t�@�֒l��ݒ肷�鏈��
//=============================================================================
void CRobotUI2D::SetVtxBuffValue(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫�����v�Z
	SetScale(m_fWidth, m_fHeight);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		m_Pos.z = 0.0f;
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
	}

	// ���_�e�N�X�`��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_���W��ݒ肷�鏈��
//=============================================================================
void CRobotUI2D::SetVtxBuffPos(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_�J���[��ݒ肷�鏈��
//=============================================================================
void CRobotUI2D::SetVtxBuffCol(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�փe�N�X�`�����W��ݒ肷�鏈��
//=============================================================================
void CRobotUI2D::SetVtxBuffTex(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}