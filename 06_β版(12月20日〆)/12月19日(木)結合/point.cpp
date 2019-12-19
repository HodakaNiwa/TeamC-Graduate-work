//=============================================================================
//
// �|�C���g�̏��� [point.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "point.h"
#include "manager.h"
#include "scene.h"
#include "game.h"
#include "numberBillborad.h"
#include "sceneBillBorad.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HIGHT	(60.0f)
#define SIZE	(D3DXVECTOR3(22.0f, 37.0f, 0.0f))
#define SIZE1	(D3DXVECTOR3(30.0f, 60.0f, 0.0f))
#define SIZE2	(D3DXVECTOR3(40.0f, 70.0f, 0.0f))
#define LIFE	(60)
#define TEX_NAME ("data/TEXTURE/game/plus.png")

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPoint::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CPoint * CPoint::Create(int nPoint, D3DXVECTOR3 pos, D3DXCOLOR Color)
{
	CPoint * pScore = NULL;

	pScore = new CPoint;
	pScore->m_pos = pos;
	pScore->m_Color = Color;
	pScore->m_nPoint = nPoint;
	pScore->Init();
	return pScore;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPoint::CPoint():CScene(10, CScene::OBJTYPE_BILLBORAD){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPoint::~CPoint(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPoint::Init(void)
{
	//�ϐ��̏�����
	m_pPlus = NULL;
	m_nLife = LIFE;
	m_bDeth = false;

	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		m_nNumber[nCnt] = 0;
		m_pNumber[nCnt] = NULL;
	}

	//�i���o�[�̃T�C�Y�ݒ�
	D3DXVECTOR3 Size = INITIALIZE_VECTOR3;
	float fMiddle = 0.0f;
	if (m_nPoint < 4)
	{
		Size = SIZE;
		fMiddle = 32.0f;
	}
	else if (m_nPoint < 8)
	{
		Size = SIZE1;
		fMiddle = 40.0f;
	}
	else if (m_nPoint > 7)
	{
		Size = SIZE2;
		fMiddle = 50.0f;
	}

	//�i���o�[�̐���
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		m_pos.y = HIGHT;
		m_pos.x += nCnt * fMiddle;
		m_pNumber[nCnt] = CNumberBillborad::Create(m_pos, Size);
		m_pNumber[nCnt]->SetColor(m_Color);
	}

	//�|�C���g�̔ɉh
	AddScore(m_nPoint);

	//�v���X�̕\��
	if (m_nNumber[0] == 0)
	{
		m_pos.x -= fMiddle * 2;
		m_pPlus = CSceneBillborad::Create(m_pos);
	}
	else
	{
		m_pos.x -= fMiddle * MAX_NUMBER_POINT + 1;
		m_pPlus = CSceneBillborad::Create(m_pos);
	}

	//�v���X�̐��l�ݒ�
	if (m_pPlus != NULL)
	{
		m_pPlus->BindTexture(m_pTexture);						//�e�N�X�`���̐ݒ�
		m_pPlus->SetSize(D3DXVECTOR3(Size.x, Size.x, 0.0f));	//�T�C�Y�̐ݒ�
		m_pPlus->SetColor(m_Color);								//�F�̐ݒ�
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPoint::Uninit(void)
{
	//�����̔j��
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			m_pNumber[nCnt]->Uninit();
			delete m_pNumber[nCnt];
			m_pNumber[nCnt] = NULL;
		}
	}

	m_pPlus = NULL;

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPoint::Update(void)
{
	//�\�����Ԃ����炷
	m_nLife--;

	//�v���X�̈ʒu�A�F�̐ݒ�
	if (m_pPlus != NULL)
	{	
		//�ʒu�̐ݒ�
		D3DXVECTOR3 pos = m_pPlus->GetPos();
		D3DXCOLOR col = m_pPlus->GetColor();
		bool bDeth = false;
		pos.y++;
		m_pPlus->SetPos(pos);	

		if (m_nLife < 0)
		{
			col.a -= 0.05f;

			//�l�̏C��
			if (col.a < 0.0f) { col.a = 0.0f; }
			m_pPlus->SetColor(col);
		}
	}

	//�ʒu�A�F�̍X�V
	D3DXCOLOR Color = INITIALIZE_VECTOR4;
	D3DXVECTOR3 Pos = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			//�ʒu�X�V
			Pos = m_pNumber[nCnt]->GetPos();
			Pos.y++;
			m_pNumber[nCnt]->SetPos(Pos);

			//�\�����Ԃ��Ȃ��Ȃ�����			
			if (m_nLife > 0) { continue; }
			Color = m_pNumber[nCnt]->GetColor();
			Color.a -= 0.05f;

			if (Color.a < 0.0f)
			{
				Color.a = 0.0f;
				m_bDeth = true;
			}

			//�F�̐ݒ�
			m_pNumber[nCnt]->SetColor(Color);
		}
	}

	//���S�t���O���L����������
	if (m_bDeth)
	{
		if (m_pPlus != NULL)
		{
			m_pPlus->Uninit();
			m_pPlus = NULL;
		}

		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPoint::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (int nCnt = 0; nCnt < MAX_NUMBER_POINT; nCnt++)
	{
		if (nCnt == 0 && m_nNumber[0] == 0) { continue; }
		m_pNumber[nCnt]->Draw();
	}

	//���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void CPoint::AddScore(int nScore)
{
	//�ϐ��錾
	int nCntNum;			//�X�R�A�̌����̃J�E���^�[
	int nNum = 1;
	int nNum2 = 1;

	//�X�R�A�̉��Z
    m_nPoint = nScore;

	//�����̌v�Z
	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_NUMBER_POINT; nCntNum++)
	{
		m_nNumber[nCntNum] = (m_nPoint % nNum) / nNum2;	//�i�[���鐔�����v�Z
		float fUV = m_nNumber[nCntNum] * 0.1f;
		m_pNumber[nCntNum]->SetUV(D3DXVECTOR2(0.0f + fUV, 0.0f), D3DXVECTOR2(0.1f + fUV, 1.0f));

		//�������P�����炷
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CPoint::GetScore(void)
{
	return m_nPoint;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CPoint::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPoint::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}