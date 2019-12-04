//*****************************************************************************
//
//     �X�L���A�C�R���̏���[skilicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "skilicon.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "input.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SKILICON_TEX_FILENAME_0 "data/TEXTURE/game/icon_speed_color.png"	// �X�s�[�h�^�̃X�L���A�C�R��
#define SKILICON_TEX_FILENAME_1 "data/TEXTURE/game/icon_power_color.png"	// �p���[�^�̃X�L���A�C�R��
#define SKILICON_TEX_FILENAME_2 "data/TEXTURE/game/icon_mine_color.png"		// �n���^�̃X�L���A�C�R��
#define SKILICON_TEX_FILENAME_3 "data/TEXTURE/game/icon_speed_bg.png"		// �X�s�[�h�^�̃X�L���A�C�R��(�D�F)
#define SKILICON_TEX_FILENAME_4 "data/TEXTURE/game/icon_power_bg.png"		// �p���[�^�̃X�L���A�C�R��(�D�F)
#define SKILICON_TEX_FILENAME_5 "data/TEXTURE/game/icon_mine_bg.png"		// �n���^�̃X�L���A�C�R��(�D�F)

// �|���S���̒l�ݒ�p
#define SKILICON_POLYGON_POS_0  (D3DXVECTOR3(75.0f,75.0f,0.0f))
#define SKILICON_POLYGON_POS_1  (D3DXVECTOR3(1205.0f,75.0f,0.0f))
#define SKILICON_POLYGON_POS_2  (D3DXVECTOR3(75.0f,645.0f,0.0f))
#define SKILICON_POLYGON_POS_3  (D3DXVECTOR3(1205.0f,645.0f,0.0f))

#define SKILICON_POLYGON_WIDTH  (120.0f)
#define SKILICON_POLYGON_HEIGHT (120.0f)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSkilicon::m_apTexture[SKILICON_TEXTURE_MAX] = {};

//*****************************************************************************
//     CSkilicon�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSkilicon::CSkilicon()
{
	m_pIconMask = NULL;
	m_pIcon = NULL;
	m_pIconBG = NULL;
	m_pParentChara = NULL;
	m_nIdx = 0;
	m_nRecastTime = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSkilicon::~CSkilicon()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSkilicon *CSkilicon::Create(CCharacter *pChara, int nIdx, int nRecastTime)
{
	// ���������m��
	CSkilicon *pSkilicon = NULL;
	pSkilicon = new CSkilicon;
	if (pSkilicon == NULL) { return NULL; }

	// ����������
	if (FAILED(pSkilicon->Init(pChara, nIdx, nRecastTime)))
	{
		pSkilicon->Uninit();
		delete pSkilicon;
		return NULL;
	}

	return pSkilicon;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CSkilicon::Load(void)
{
	char aFileName[SKILICON_TEXTURE_MAX][256]
	{
		SKILICON_TEX_FILENAME_0,
		SKILICON_TEX_FILENAME_1,
		SKILICON_TEX_FILENAME_2,
		SKILICON_TEX_FILENAME_3,
		SKILICON_TEX_FILENAME_4,
		SKILICON_TEX_FILENAME_5,
	};

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < SKILICON_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̊J������
//=============================================================================
void CSkilicon::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < SKILICON_TEXTURE_MAX; nCntTex++)
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
HRESULT CSkilicon::Init(CCharacter *pChara, int nIdx, int nRecastTime)
{
	// �e���̃L�����N�^�[�ւ̃|�C���^��ۑ�����
	m_pParentChara = pChara;

	// ���g�̔ԍ���ۑ�
	m_nIdx = nIdx;

	// ���L���X�g�ɂ����鎞�Ԃ�ۑ�
	m_nRecastTime = nRecastTime;

	// �w�i���쐬
	CreateIconBG();

	// �}�X�N�p�̃|���S�����쐬
	CreateIconMask();

	// �X�L���A�C�R�����쐬
	CreateIcon();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSkilicon::Uninit(void)
{
	// �w�i���J��
	ReleaseIconBG();

	// �}�X�N�p�̃|���S�����J��
	ReleaseIconMask();

	// �X�L���A�C�R�����J��
	ReleaseIcon();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSkilicon::Update(void)
{
	// �A�C�R���̃��L���X�g����
	RecastIcon();
}

//=============================================================================
//    �A�C�R���p�̃}�X�N�����ɖ߂�����
//=============================================================================
void CSkilicon::RevivalIconMask(void)
{
	if (m_pIcon == NULL || m_pIconMask == NULL) { return; }
	m_pIconMask->SetPos(m_pIcon->GetPos());
	m_pIconMask->SetScale(SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    �A�C�R���p�̃}�X�N�����ɖ߂�����(�n���^�p)
//=============================================================================
void CSkilicon::RevivalIconMask_teq(void)
{
	if (m_pIcon == NULL || m_pIconMask == NULL) { return; }
	D3DXVECTOR3 IconPos = m_pIcon->GetPos();
	float fHeight = m_pIconMask->GetHeight();
	float fCutValue = SKILICON_POLYGON_HEIGHT * 0.3f;
	fHeight += fCutValue;
	fCutValue /= 2.0f;
	if (fHeight <= 0.0f)
	{
		fHeight = 0.0f;
		fCutValue = 0.0f;
	}
	m_pIconMask->SetPos(IconPos + D3DXVECTOR3(0.0f, -(SKILICON_POLYGON_HEIGHT / 2.0f) + fCutValue, 0.0f));
	m_pIconMask->SetScale(SKILICON_POLYGON_WIDTH, fHeight);
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSkilicon::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// �X�L���A�C�R���̔w�i��`��
	DrawIconBG();

	// �X�e���V���o�b�t�@���N���A����
	ClearStencil(pDevice);

	// �X�L���A�C�R���}�X�N�p�̃|���S�����X�e���V���o�b�t�@�ɕ`��
	DrawIconMask(pDevice);

	// �X�L���A�C�R����`��(���̎��}�X�N��������)
	DrawIcon(pDevice);

	// �X�e���V���o�b�t�@���N���A����
	ClearStencil(pDevice);
}

//=============================================================================
//    �X�L���A�C�R���w�i���J�����鏈��
//=============================================================================
void CSkilicon::CreateIconBG(void)
{
	if (m_pIconBG != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIconBG = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) ,
		SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	if (m_pIconBG == NULL) { return; }
	m_pIconBG->BindTexture(m_apTexture[m_pParentChara->GetCharcterType() + (SKILICON_TEXTURE_MAX / 2)]);
}

//=============================================================================
//    �}�X�N�p�̃|���S�����J�����鏈��
//=============================================================================
void CSkilicon::CreateIconMask(void)
{
	if (m_pIconMask != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIconMask = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f, 0.0f);
	if (m_pIconMask == NULL) { return; }
	//m_pIconMask->BindTexture(m_apTexture[2 + (SKILICON_TEXTURE_MAX / 2)]);
}

//=============================================================================
//    �X�L���A�C�R�����J�����鏈��
//=============================================================================
void CSkilicon::CreateIcon(void)
{
	if (m_pIcon != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIcon = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	if (m_pIcon == NULL) { return; }
	m_pIcon->BindTexture(m_apTexture[m_pParentChara->GetCharcterType()]);
}

//=============================================================================
//    �X�L���A�C�R���w�i���J�����鏈��
//=============================================================================
void CSkilicon::ReleaseIconBG(void)
{
	if (m_pIconBG != NULL)
	{
		m_pIconBG->Uninit();
		delete m_pIconBG;
		m_pIconBG = NULL;
	}
}

//=============================================================================
//    �}�X�N�p�̃|���S�����J�����鏈��
//=============================================================================
void CSkilicon::ReleaseIconMask(void)
{
	if (m_pIconMask != NULL)
	{
		m_pIconMask->Uninit();
		delete m_pIconMask;
		m_pIconMask = NULL;
	}
}

//=============================================================================
//    �X�L���A�C�R�����J�����鏈��
//=============================================================================
void CSkilicon::ReleaseIcon(void)
{
	if (m_pIcon != NULL)
	{
		m_pIcon->Uninit();
		delete m_pIcon;
		m_pIcon = NULL;
	}
}

//=============================================================================
//    �A�C�R�������L���X�g���鏈��
//=============================================================================
void CSkilicon::RecastIcon(void)
{
	if (m_pIconMask == NULL) { return; }
	float fHeight = m_pIconMask->GetHeight();
	float fCutValue = SKILICON_POLYGON_HEIGHT / m_nRecastTime;
	fHeight -= fCutValue;
	fCutValue /= 2.0f;
	if (fHeight <= 0.0f)
	{
		fHeight = 0.0f;
		fCutValue = 0.0f;
	}
	m_pIconMask->SetScale(m_pIconMask->GetWidth(), fHeight);
	m_pIconMask->SetPos(m_pIconMask->GetPos() + D3DXVECTOR3(0.0f, -fCutValue, 0.0f));
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    �X�L���A�C�R���p�w�i��`�悷�鏈��
//=============================================================================
void CSkilicon::DrawIconBG(void)
{
	if (m_pIconBG != NULL)
	{
		m_pIconBG->Draw();
	}
}

//=============================================================================
//    �}�X�N�p�̃|���S�����X�e���V���o�b�t�@�ɕ`�悷�鏈��
//=============================================================================
void CSkilicon::CSkilicon::DrawIconMask(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIconMask == NULL) { return; }

	// �X�e���V���o�b�t�@���g�p����ݒ��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �F��`�悵�Ȃ��ݒ��
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// �X�e���V���e�X�g�̕��@��ݒ�(�K�����i)
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x00);

	// �X�e���V���}�X�N�l�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �X�e���V���e�X�g�ɂ��l�̕ϓ���ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	// �}�X�N�|���S���̕`��
	m_pIconMask->Draw();

	// �X�e���V���o�b�t�@�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// �`�悷��F�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

//=============================================================================
//    �X�L���A�C�R����`�悷�鏈��
//=============================================================================
void CSkilicon::DrawIcon(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIcon == NULL) { return; }

	// �X�e���V���o�b�t�@���g�p����ݒ��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���e�X�g�̕��@��ݒ�(�Q�ƒl���傫�����̂����i)
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x00);

	// �X�e���V���}�X�N�l�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �e�X�g�ɂ��l�̕ϓ���ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// �A�C�R����`�悷��
	m_pIcon->Draw();

	// �X�e���V���o�b�t�@�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
//    �X�e���V���o�b�t�@���N���A���鏈��
//=============================================================================
void CSkilicon::ClearStencil(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
}


//*****************************************************************************
//     CIcon2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CIcon2D::CIcon2D()
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
CIcon2D::~CIcon2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CIcon2D *CIcon2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	// ���������m��
	CIcon2D *pIcon2D = NULL;
	pIcon2D = new CIcon2D;
	if (pIcon2D == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pIcon2D->SetPos(pos);              // �|���S���̍��W
	pIcon2D->SetRot(fRot);             // �|���S���̌���
	pIcon2D->SetCol(col);              // �|���S���̐F
	pIcon2D->SetWidth(fWidth);         // �|���S���̕�
	pIcon2D->SetHeight(fHeight);       // �|���S���̍���
	pIcon2D->SetTexU(fTexU);           // �|���S���̍���e�N�X�`��U���W
	pIcon2D->SetTexV(fTexV);           // �|���S���̍���e�N�X�`��V���W
	pIcon2D->SetTexWidth(fTexWidth);   // �|���S���̃e�N�X�`�����W�̉���
	pIcon2D->SetTexHeight(fTexHeight); // �|���S���̃e�N�X�`�����W�̏c��

	// ����������
	if (FAILED(pIcon2D->Init()))
	{
		pIcon2D->Uninit();
		delete pIcon2D;
		return NULL;
	}

	return pIcon2D;		// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CIcon2D::Init(void)
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
void CIcon2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CIcon2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CIcon2D::Draw(void)
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
void CIcon2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
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
void CIcon2D::SetScale(float fWidth, float fHeight)
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
void CIcon2D::SetVtxBuffValue(void)
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
void CIcon2D::SetVtxBuffPos(void)
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
void CIcon2D::SetVtxBuffCol(void)
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
void CIcon2D::SetVtxBuffTex(void)
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