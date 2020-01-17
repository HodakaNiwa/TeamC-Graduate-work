//*****************************************************************************
//
//     ��̏���[sky.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sky.h"
#include "manager.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SKY_BG_TEXFILENAME      "data/TEXTURE/Bg/bg000.jpg"			// �w�i�e�N�X�`���̃t�@�C���p�X��
#define SKY_MESHSPHER_POS       (D3DXVECTOR3(0.0f,-9000.0f,0.0f))	// ���b�V���X�t�B�A�̈ʒu
#define SKY_MESHSPHER_COL       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))	// ���b�V���X�t�B�A�̐F
#define SKY_MESHSPHERE_XBLOCK   (30)								// ���b�V���X�t�B�A�̉��̕�����
#define SKY_MESHSPHERE_YBLOCK   (30)								// ���b�V���X�t�B�A�̏c�̕�����
#define SKY_MESHSPHERE_RADIUS   (15000.0f)							// ���b�V���X�t�B�A�̔��a
#define SKY_MESHSPHERE_PRIORITY (2)									// ���b�V���X�t�B�A�̏����D�揇��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSky::m_pTexture = NULL;

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSky::CSky(int nPriority, OBJTYPE objType) : CMeshSphere(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSky::~CSky()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSky *CSky::Create(void)
{
	// �������m��
	CSky *pSky = NULL;
	pSky = new CSky(SKY_MESHSPHERE_PRIORITY);
	if (pSky == NULL)
	{
		return NULL;
	}

	// �e��l�̑��
	pSky->SetPos(SKY_MESHSPHER_POS);			// ���b�V���X�t�B�A�̍��W
	pSky->SetRot(INITIALIZE_VECTOR3);			// ���b�V���X�t�B�A�̌���
	pSky->SetCol(SKY_MESHSPHER_COL);			// ���b�V���X�t�B�A�̐F
	pSky->SetRadius(SKY_MESHSPHERE_RADIUS);		// ���b�V���X�t�B�A�̔��a
	pSky->SetXBlock(SKY_MESHSPHERE_XBLOCK);		// ���̕�����
	pSky->SetYBlock(SKY_MESHSPHERE_YBLOCK);		// �c�̕�����

	// ����������
	if (FAILED(pSky->Init()))
	{
		pSky->Uninit();
		return NULL;
	}

	return pSky;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CSky::Load(void)
{
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), SKY_BG_TEXFILENAME, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̊J������
//=============================================================================
void CSky::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture);
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSky::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CMeshSphere::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSky::Uninit(void)
{
	CMeshSphere::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSky::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSky::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O�����Ȃ��ݒ��
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���ʂ̕`�揈��
	CMeshSphere::Draw();

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}