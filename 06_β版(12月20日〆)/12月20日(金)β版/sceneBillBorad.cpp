//=============================================================================
//
// �r���{�[�h���� [scenebillbord.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "sceneBillborad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// ��������
//=============================================================================
CSceneBillborad * CSceneBillborad::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CSceneBillborad * pSceneBillboard;
	pSceneBillboard = new CSceneBillborad;

	//����������
	pSceneBillboard->Init();

	//�ݒ菈��
	pSceneBillboard->Set(pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f));

	return pSceneBillboard;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneBillborad::CSceneBillborad() : CScene(3, OBJTYPE_BILLBORAD)
{
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneBillborad::CSceneBillborad(int nPriority, OBJTYPE ObjType) : CScene(nPriority, ObjType)
{
	//�ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_size = INITIALIZE_VECTOR3;
	m_Color = INITIALIZE_VECTOR4;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneBillborad::~CSceneBillborad()
{
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CSceneBillborad::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneBillborad::Init(void)
{
	//�ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_size = INITIALIZE_VECTOR3;
	m_Color = INITIALIZE_VECTOR4;
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV�l�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneBillborad::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneBillborad::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneBillborad::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX		  mtxView;					//�r���[�}�g���N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���C�e�B���O�𖳌��ɂ��� ��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���@��
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pVtxBuff != NULL)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}

	//���C�e�B���O��L���ɂ���@��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���@��
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CSceneBillborad::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CSceneBillborad::SetSize(D3DXVECTOR3 size)
{
	//�T�C�Y�̕ۑ�
	m_size = size;

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{
		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// ���W�̐ݒ�
//=============================================================================
void CSceneBillborad::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CSceneBillborad::SetColor(D3DXCOLOR color)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_Color = color;

	//���_���W�̐ݒ�
	pVtx[0].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[1].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[2].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[3].col = D3DXCOLOR(color.r, color.g, color.b, color.a);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// a�l�̍X�V
//=============================================================================
void CSceneBillborad::SetColorABillborad(float colA)
{
	//a�l�̍X�V
	m_Color.a = colA;

	//�F�̐ݒ�
	SetColor(m_Color);
}

//=============================================================================
// UV�̐ݒ�
//=============================================================================
void CSceneBillborad::SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(uvMIn.x, uvMax.y);
	pVtx[1].tex = D3DXVECTOR2(uvMax.x, uvMax.y);
	pVtx[2].tex = D3DXVECTOR2(uvMIn.x, uvMIn.y);
	pVtx[3].tex = D3DXVECTOR2(uvMax.x, uvMIn.y);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}