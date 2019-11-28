//=============================================================================
//
// �p�[�e�B�N���r���{�[�h���� [particlebillboad.cpp]
// Author : ���� ����
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "loadText.h"
#include "tutorial.h"
#include "sceneBillBorad.h"
#include "camera.h"

//==================================
// �}�N����`
//==================================
#define MAX_LIFE_GRAND		(30)
#define CHANGE_SIZE			(0.1f)
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�x�N�g���̏�����
#define FLAM				(8)
#define CHANGE_A			(0.06f)
#define CHANGE_A_STAR		(0.14f)
#define RAND_ANGLE			(314)
#define CICLE_MIN			(-200)
#define CICLE_MAX			(400)
#define LIGHT_MIN			(18.0f)
#define LIGHT_MAX			(32.0f)
#define SIZE_UP				(0.15f)
#define SPEW_MIN			(-100)
#define SPEW_MAX			(200)

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 * CParticle::m_ppTexture = NULL;
int CParticle::m_nMaxTexture = 0;

//=============================================================================
// ��������
//=============================================================================
CParticle * CParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority)
{
	//�C���X�^���X�̐���
	CParticle * pParticle = NULL;

	if (type == TYPE_NORMAL)
	{//�ʏ�
		pParticle = new CParticle(nPriority);
	}
	else if (type == TYPE_SPEW)
	{//�����^
		pParticle = new CSpewParticle(nPriority);
	}
	else if (type == TYPE_GRANDSMOKE)
	{//���̏�Ŋg��
		pParticle = new CGrandSmokeParticle(nPriority);
	}
	else if (type == TYPE_LIGHT)
	{//�g��k��
		pParticle = new CLightParticle(nPriority);
	}
	else if (type == TYPE_CICLE)
	{//�~�`�^
		pParticle = new CCicleParticle(nPriority);
	}
	else if (type == TYPE_STAR)
	{//�����^
		pParticle = new CStarParticle(nPriority);
	}

	if (pParticle != NULL)
	{
		pParticle->SetSpeed(fSpeed);						//�X�s�[�h�̐ݒ�
		pParticle->SetMinHight(nMinHight);					//�����̍ŏ��l
		pParticle->SetMaxHight(nMaxHight);					//�����̍ő�l

		pParticle->Init();					//����������

		//�ݒ菈��
		pParticle->SetSize(D3DXVECTOR2(size.x, size.y));	//�T�C�Y�̐ݒ�
		pParticle->SetPos(pos);				//�ʒu�̐ݒ�
		pParticle->SetDrawType(DrawType);	//�`��^�C�v�̐ݒ�
		pParticle->SetColor(col);			//�F�̐ݒ�
		pParticle->SetColorA(col.a);		//�F��a�l�̐ݒ�
		pParticle->SetZbuffu(bZBuffu);		//Z�o�b�t�@�̐ݒ�
		pParticle->SetLife(nLife);			//���C�t�̐ݒ�

		if (m_ppTexture[nNumTex] != NULL)
		{
			pParticle->BindTexture(m_ppTexture[nNumTex]);		//�e�N�X�`���̊�����
		}
	}
	else
	{
		MessageBox(NULL, TEXT("pParticle��NULL�ł�"), TEXT("CEffectTool::Create()"), MB_OK);
	}

	return pParticle;
}

//=========================================
// �f�t�H���g�R���X�g���N�^
//=========================================
CParticle::CParticle() : CSceneBillborad(3, CScene::OBJTYPE_PARTICLE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// �R���X�g���N�^
//=========================================
CParticle::CParticle(int nPriority) : CSceneBillborad(nPriority, CScene::OBJTYPE_PARTICLE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticle::~CParticle(){}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CParticle::LoadTexture(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`�����̎擾
	CLoadTextEffect * pLoadTextTexture = {};
	pLoadTextTexture = CGame::GetEffectLoad();	//�p�[�e�B�N���e�N�X�`���̃|�C���^���擾
	CLoadText::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//�e�N�X�`�����̎擾
	m_nMaxTexture = TexInfo.nMaxTex;								//�e�N�X�`���̍ő吔���擾
	

	//�e�N�X�`�����T�C�Y���q�[�v�̈悩�烁�����𓮓I�m�ۂ���
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// �e�N�X�`���̐���
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CParticle::UnloadTexture(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//�e�N�X�`���|�C���^�̔j��
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=========================================
// ����������
//=========================================
HRESULT CParticle::Init(void)
{
	CSceneBillborad::Init();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDeth = false;
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CParticle::Uninit(void)
{
	CSceneBillborad::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CParticle::Update(void)
{
	//�ϐ��錾
	CGame * pGame = CManager::GetGame();
	D3DXVECTOR3 Pos = CSceneBillborad::GetPos();
	bool bDraw[MAX_NUMPLAYER] = {};
	bool bDrawChack = true;

	//�J�������Ƃ̕`�攻��
	if (pGame == NULL) { return; }
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		//NULL�`�F�b�N
		CGameCamera * pGameCamera = pGame->GetGameCamera(nCnt);
		if (pGameCamera == NULL) { continue; }

		bDraw[nCnt] = pGame->GetGameCamera(nCnt)->ReflectObject(Pos);

		if (bDraw[nCnt])
		{
			bDrawChack = false;	//�`��t���O
			SetDraw(true);
			break;
		}
	}

	if (!bDrawChack) { return; }
	SetDraw(false);		//�`�悵�Ȃ���Ԃɂ���
}

//=========================================
// �`�揈��
//=========================================
void CParticle::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�����Ȃ��ݒ��
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bZBuffu)
	{
		//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	//�r���{�[�h�̕`��
	CSceneBillborad::Draw();

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bZBuffu)
	{
		//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=========================================
// �F�̐ݒ�
//=========================================
void CParticle::SetColor(D3DXCOLOR col)
{
	m_col = col;

	CSceneBillborad::SetColor(m_col);
}

//=========================================
// �T�C�Y�̍Đݒ�
//=========================================
void CParticle::RemakeSize(D3DXVECTOR2 size)
{
	m_size = size;

	CSceneBillborad::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
}

//=============================================================================
// �ʒu�ݒ�
//=============================================================================
void CParticle::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	CSceneBillborad::SetPos(pos);
}

//=============================================================================
// �T�C�Y�ݒ�
//=============================================================================
void CParticle::SetSize(D3DXVECTOR2 size)
{
	m_size = size;

	CSceneBillborad::SetSize(D3DXVECTOR3(size.x, size.y, 0.0f));
}

//=============================================================================
// ��]�̏C������
//=============================================================================
void CParticle::SetAngle(float * pAngle)
{
	//�p�x�̏C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=========================================
// UV���W�̐ݒ�
//=========================================
void CParticle::SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV)
{
	CSceneBillborad::SetUV(MinUV, MaxUV);
}

//*****************************************************************************
//
// ���^�p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSpewParticle::CSpewParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSpewParticle::~CSpewParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpewParticle::Init(void)
{
	CParticle::Init();

	//�ϐ��̏�����
	m_nCountFlam = 0;		//�t���[���J�E���^�[
	m_nLife = 30;			//�\������
	m_fChangeCol_a = 0.0f;	//�����x�̕ω���;
	m_bGravity = true;		//�d�͂�ON�EOFF
	m_fGravity = 0.0f;		//�d��

	float fPosX = (float)(SPEW_MIN + rand() % SPEW_MAX);
	float fPosZ = (float)(SPEW_MIN + rand() % SPEW_MAX);

	//�ړI�̊p�x�����߂�
	float fAngle = atan2f(fPosX, fPosZ);

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;

	//�����ݒ�
	float fHight = (float)(m_nMinHight + rand() % m_nMaxHight);
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSpewParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSpewParticle::Update(void)
{
	//�d�͂𑫂�
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	//���Ԃ̉��Z
	m_nCountFlam++;

	if (m_nCountFlam > FLAM)	//���̃t���[�������o������
	{
		m_nLife--;						//�\�����Ԃ̌��Z

		if (m_nLife < 0)
		{
			m_colA -= m_fChangeCol_a;	//�����x��������

			if (m_colA < 0.0f)
			{
				m_bDeth = true;
			}
			else
			{
				D3DXCOLOR col = CParticle::GetColor();
				CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
			}
		}
	}

	D3DXVECTOR3 pos = CParticle::GetPos();
	pos += m_move;
	CParticle::SetPos(pos);

	CParticle::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpewParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// �y���p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGrandSmokeParticle::CGrandSmokeParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGrandSmokeParticle::~CGrandSmokeParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGrandSmokeParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;		//�t���[���J�E���^�[
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGrandSmokeParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGrandSmokeParticle::Update(void)
{
	//���Ԃ̉��Z
	m_nCountFlam++;

	m_nLife--;				//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	//�T�C�Y�̍X�V
	D3DXVECTOR2 size = CParticle::GetSize();
	size.x += SIZE_UP;
	size.y += SIZE_UP;

	CParticle::SetSize(size);
	CParticle::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGrandSmokeParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// ���C�g�p�[�e�B�N��
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLightParticle::CLightParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLightParticle::~CLightParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLightParticle::Init(void)
{
	CParticle::Init();
	m_fChangeSize = CHANGE_SIZE;
	m_nLife = MAX_LIFE_GRAND;	//�\������
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLightParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLightParticle::Update(void)
{
	//�T�C�Y�̕ۑ�
	D3DXVECTOR2 size = CParticle::GetSize();

	//�p�[�e�B�N���̃T�C�Y��ύX����
	if (size.x > LIGHT_MAX || size.x < LIGHT_MIN)
	{
		m_fChangeSize *= -1;	//�����̕ύX
	}

	//�T�C�Y�̕ύX
	size.x += m_fChangeSize;
	size.y += m_fChangeSize;

	//�T�C�Y�̐ݒ�
	SetSize(size);

	CParticle::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLightParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// �~�`�p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCicleParticle::CCicleParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCicleParticle::~CCicleParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCicleParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;						//�t���[���J�E���^�[
	m_nLife = 30;							//�\������
	m_fChangeCol_a = 0.0f;					//�����x�̕ω���;
	m_bGravity = true;						//�d�͂�ON�EOFF
	m_fGravity = 0.0f;						//�d��

	float fPosX = (float)(CICLE_MIN + rand() % CICLE_MAX);
	float fPosY = (float)(CICLE_MIN + rand() % CICLE_MAX);

	//�ړI�̊p�x�����߂�
	float fAngle = atan2f(fPosX, fPosY);

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;
	m_move.y = cosf(fAngle) * m_fSpeed;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCicleParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCicleParticle::Update(void)
{
	//�d�͂𑫂�
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	m_nLife--;						//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= m_fChangeCol_a;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}
	D3DXVECTOR3 pos = CParticle::GetPos();
	pos += m_move;
	CParticle::SetPos(pos);

	CParticle::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCicleParticle::Draw(void)
{
	CParticle::Draw();
}


//*****************************************************************************
//
// ���p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStarParticle::CStarParticle(int nPriority) : CParticle(nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStarParticle::~CStarParticle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStarParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;			//�t���[���J�E���^�[
								//�����ݒ�
	//m_rot.x = (float)(rand() % RAND_ANGLE);
	//m_rot.z = (float)(rand() % RAND_ANGLE);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStarParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStarParticle::Update(void)
{
	//���Ԃ̉��Z
	m_nCountFlam++;

	m_nLife--;				//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A_STAR;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticle::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStarParticle::Draw(void)
{
	CParticle::Draw();
}