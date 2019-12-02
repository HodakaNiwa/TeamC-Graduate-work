//=============================================================================
//
// �O�Տ��� [sceneOrbit.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sceneOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "LoadText.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 * CSceneOrbit::m_pTexture = NULL;
CSceneOrbit::ORBIT_INFO * CSceneOrbit::m_pOrbitInfo = NULL;
int CSceneOrbit::m_nMaxTexture = 0;
CLoadTextOrbit * CSceneOrbit::m_pLoadText = NULL;

//=============================================================================
// ��������
//=============================================================================
CSceneOrbit * CSceneOrbit::Create(TYPE type, D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CSceneOrbit * pSceneOrbit;
	pSceneOrbit = new CSceneOrbit;

	//����������
	pSceneOrbit->Init();		//����������
	pSceneOrbit->SetType(type);	//�^�C�v�̐ݒ�

	//�ݒ菈��
	pSceneOrbit->Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pSceneOrbit;
}

//=============================================================================
// �e�N�X�`���ݒ�
//=============================================================================
void CSceneOrbit::Load(void)
{
	if (m_pOrbitInfo == NULL)
	{
		m_pLoadText = CLoadTextOrbit::Create();
		m_pOrbitInfo = m_pLoadText->GetOrbit();
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	CLoadTextOrbit::TEX_INFO TexInfo = m_pLoadText->GetTexInfo();		//��Ղ̃e�N�X�`�����̎擾
	m_nMaxTexture = TexInfo.nMaxTex;									//�e�N�X�`���̍ő吔���擾

	if (m_pTexture == NULL) { m_pTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture]; }

	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_pTexture[nCntTex]);
	}
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CSceneOrbit::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	//�|�C���^�̏�����
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
 		m_pTexture = NULL;
	}

	//�O�Ղ̃|�C���^���̔j��
	if (m_pOrbitInfo != NULL)
	{
		m_pOrbitInfo = NULL;
	}

	//�e�L�X�g�̔j��
	if (m_pLoadText != NULL)
	{
		m_pLoadText->Uninit();
		delete m_pLoadText;
		m_pLoadText = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneOrbit::CSceneOrbit() : CScene(4, OBJTYPE_ORBIT)
{
	m_pVtxBuff = NULL;				//���_�o�b�t�@
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneOrbit::~CSceneOrbit(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneOrbit::Init(void)
{
	//�ϐ��̏�����
	m_pVtxBuff = NULL;			//���_�o�b�t�@
	m_pMtxParent = NULL;		//�}�g���b�N�X�̎n�_�̃|�C���^
	m_pMtxParentEnd = NULL;		//�}�g���b�N�X�̏I�_�|�C���^
	m_aPosVertex = NULL;		//�ۑ��p���_���
	m_type = TYPE_MAX;			//�^�C�v���
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneOrbit::Uninit(void)
{
	//�ۑ��p���_�̔j��
	if (m_aPosVertex != NULL)
	{
		delete[] m_aPosVertex;
		m_aPosVertex = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneOrbit::Update(void)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv; nCntPrimitiv++)
		{
			pVtx[0].tex.x += m_pOrbitInfo[m_type].fAnim;

			//�Y���̈ʒu�܂Ői�߂�
			pVtx++;
		}

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneOrbit::Draw(void)
{
	//�ϐ��錾
	D3DXVECTOR3 pos;
	D3DXVECTOR3 pos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtxOfset, mtxOfset1;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtx = *m_pMtxParent;

	D3DXVec3TransformCoord(&pos1, &m_pOrbitInfo[m_type].pos[0], m_pMtxParent);
	D3DXVec3TransformCoord(&pos2, &m_pOrbitInfo[m_type].pos[1], m_pMtxParentEnd);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�����炷
	for (int nCntPos = m_pOrbitInfo[m_type].nPrimitiv / 2 - 2; nCntPos >= 0; nCntPos--)
	{
		m_aPosVertex[nCntPos * 2 + 2] = m_aPosVertex[nCntPos * 2];
		m_aPosVertex[nCntPos * 2 + 3] = m_aPosVertex[nCntPos * 2 + 1];
	}

	//���_���擾����
	m_aPosVertex[0] = pos1;
	m_aPosVertex[1] = pos2;

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv; nCntPrimitiv++)
	{
		pVtx[0].pos = m_aPosVertex[nCntPrimitiv];

		//�Y���̈ʒu�܂Ői�߂�
		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (m_type == TYPE_STAR)
	{
		// Z�e�X�g�����𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//���ʂ��J�����O����

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[m_pOrbitInfo[m_type].nTex]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
						   0,
						   m_pOrbitInfo[m_type].nPorigon);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//���ʂ��J�����O����

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_type == TYPE_STAR)
	{
		// Z�e�X�g�������f�t�H���g�ɂ���
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CSceneOrbit::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	float fWidthSize = 0;		//�����̃T�C�Y
	float fHightSize = 0;		//�c���̃T�C�Y
	D3DXVECTOR3 Vec0[2] = {};	//�x�N�g��0
	D3DXVECTOR3 Vec1[2] = {};	//�x�N�g��1
	int nCntPos = 0;				//�ǂݍ��񂾍��W�z���i�߂�
	m_pos = pos;

	//�ۑ��p���_�̐���
	if (m_aPosVertex == NULL)
	{
		m_aPosVertex = new D3DXVECTOR3[m_pOrbitInfo[m_type].nPrimitiv];
	}

	//�|���S���p�̖@��
	D3DXVECTOR3 nor[8000] = {};	//�|���S���̖@���x�N�g��
	int nCntNor = 0;			//�@���̔z���i�߂�

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_pOrbitInfo[m_type].nPrimitiv,	//�m�ۂ��钸�_�o�b�t�@�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv / 2; nCntPrimitiv++)
	{
	    //���_���W�̐ݒ�
		pVtx[0].pos = m_pOrbitInfo[m_type].pos[0] + m_pos;
		pVtx[1].pos = m_pOrbitInfo[m_type].pos[1] + m_pos;

		//���W��ۑ�����
		m_aPosVertex[nCntPos] = pVtx[0].pos;
		m_aPosVertex[nCntPos + 1] = pVtx[1].pos;

		//�@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//�F�̐ݒ�
		pVtx[0].col = m_pOrbitInfo[m_type].color;
		pVtx[1].col = m_pOrbitInfo[m_type].color;

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntPrimitiv * m_pOrbitInfo[m_type].fUV), 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + (nCntPrimitiv * m_pOrbitInfo[m_type].fUV), 0.0f);

		//�Y���̈ʒu�܂Ői�߂�
		pVtx += 2;
		nCntPos += 2;		//�z��̃J�E���^�[��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CSceneOrbit::SetColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv / 2; nCntPrimitiv++)
	{
		//�F�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;

		//�Y���̈ʒu�܂Ői�߂�
		pVtx += 2;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}