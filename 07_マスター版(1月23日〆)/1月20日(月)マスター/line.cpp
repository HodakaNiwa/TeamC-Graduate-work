//=============================================================================
//
// �r���{�[�h���� [scenebillbord.cpp]
// Author : �������� Sato_Asumi
//
//===========================================================================
#include "line.h"
#include "manager.h"
#include "renderer.h"
#include "Player.h"
#include "library.h"
#include "collision.h"
#include "tutorial.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LINE_LIGHTING
#define LINE_HIGHT			(15.0f)
#define WIDTH				(15.0f)
#define COLLISION_WIDTH		(30.0f)
#define UPDATE_LENGTH		(1300.0f)

//=============================================================================
// ��������
//=============================================================================
CLine * CLine::Create(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, int nNumPlayer, int nId)
{
	//�C���X�^���X�̐���
	CLine * pLine;
	pLine = new CLine;

	//����������
	pLine->SetNumPlayer(nNumPlayer);
	pLine->SetTerritoryPos(StartPos, EndPos);
	pLine->SetID(nId);
	pLine->Init();

	return pLine;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLine::CLine() : CScene3D(3, OBJTYPE_LINE)
{
	m_nNumPlayer = 0;
	m_nID = 0;
	m_StartPos = INITIALIZE_VECTOR3;
	m_EndPos = INITIALIZE_VECTOR3;
	m_col = INITIALIZE_VECTOR4;
	m_bCompleteShape = false;

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		m_VecA[nCnt] = INITIALIZE_VECTOR3;
		m_WorldPos[nCnt] = INITIALIZE_VECTOR3;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLine::~CLine(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLine::Init(void)
{
	CScene3D::Init();

	//�ϐ��̏�����
	m_bCompleteShape = false;

	int nNumPlayer = m_nNumPlayer;
	m_col = CPlayer::m_CountryColor[nNumPlayer];

	//�F�̐ݒ�
	CScene3D::SetColor(m_col);

	//���S�_�����߂�
	D3DXVECTOR3 Pos = (m_StartPos + m_EndPos) / 2.0f;
	Pos.y = LINE_HIGHT;

	//�T�C�Y�����߂�
	D3DXVECTOR3 Size = INITIALIZE_VECTOR3;
	Size.z = (CFunctionLib::Vec2Length(m_StartPos.x, m_StartPos.z, m_EndPos.x, m_EndPos.z)) / 2.0f;
	Size.x = WIDTH;

	//�ʒu�A�T�C�Y��ݒ肷��
	CScene3D::Set(Pos, Size);

	//�����̐ݒ�
	float fRot = CFunctionLib::CalcAngleToDest(m_StartPos.x, m_StartPos.z, m_EndPos.x, m_EndPos.z);
	CScene3D::SetRot(D3DXVECTOR3(0.0f, fRot, 0.0f));

	//VecA�����߂�
	RequestVecA();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLine::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLine::Update(void)
{
	if (m_bCompleteShape) { return; }	//�}�`���������Ă��邩�ǂ���

	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();

		int nNumPlayer = pGame->GetNumPlay();
		nNumPlayer += pGame->GetEnemyNum();

		for (int nCntPlayer = 0; nCntPlayer < nNumPlayer; nCntPlayer++)
		{
			if (nCntPlayer == m_nNumPlayer) { continue; }
			D3DXVECTOR3 Pos = CScene3D::GetPos();
			CCharacter * pOtherPlayer = CManager::GetGame()->GetChara(nCntPlayer);
			D3DXVECTOR3 PlayerPos = pOtherPlayer->GetPos();

			float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//���������߂�

			if (fLength > UPDATE_LENGTH) { continue; }
			bool bCutLine = CollsionLine(PlayerPos);	//�����蔻�������

			if (!bCutLine) { continue; }
			CManager::GetGame()->GetChara(m_nNumPlayer)->CutLine(m_nID);	//���C���ؒf����	
		}
	}
	else if(mode == CManager::MODE_TUTORIAL)
	{
		if (0 == m_nNumPlayer) { return; }
		//���C���̐ؒf���� 
		D3DXVECTOR3 Pos = CScene3D::GetPos();
		D3DXVECTOR3 PlayerPos = CManager::GetTutorial()->GetTutorialPlayer()->GetPos();

		float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//���������߂�

		if (fLength < UPDATE_LENGTH)
		{
			bool bCutLine = CollsionLine(PlayerPos);	//�����蔻�������
			
			//���C���̔j��
			if (bCutLine) { Uninit(); }
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLine::Draw(void)
{
	
#ifdef LINE_LIGHTING
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O�����Ȃ��ݒ��
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�ɏ������݂��Ȃ��ݒ��
	DWORD Zwrite;
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &Zwrite);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���ʂ̕`�揈��
	CScene3D::Draw();

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	// Z�o�b�t�@�ɏ������ސݒ��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, Zwrite);
#else
	// ���ʂ̕`�揈��
	CScene3D::Draw();
#endif
}

//=============================================================================
// ���Ƃ̓����蔻��
//=============================================================================
bool CLine::CollsionLine(D3DXVECTOR3 PlayerPos)
{
	//�ϐ��錾
	bool bIn = false;
	int nCountEnter = 0;

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		D3DXVECTOR3 VecB = PlayerPos - m_WorldPos[nCnt];

		//�O�ς����߂�
		float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(m_VecA[nCnt].x, m_VecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

		//�x�N�g�����傫�����ǂ���
		if (fCross >= 0) 
		{ 
			nCountEnter++; 
		}
		else
		{
			break;
		}
	}

	//�����Ă��邩���肷��
	if (nCountEnter >= MAX_POS)
	{ 
		bIn = true; 
	}

	return bIn;
}

//=============================================================================
// �F�̕ύX����
//=============================================================================
bool CLine::ColChange(void)
{
	//�ϐ��錾
	bool bDeth = false;	//���S�t���O

	m_col.a -= 0.05f;

	if (m_col.a < 0.0f)
	{
		m_col.a = 0.0f;
		bDeth = true;	//���S�t���O��L���ɂ���
	}

	//�F��ݒ�
	SetColor(m_col);
	return bDeth;
}


//=============================================================================
// VecA�����߂�
//=============================================================================
void CLine::RequestVecA(void)
{
	//�ϐ��錾
	D3DXVECTOR3 PrimitivPos[MAX_POS] = {};

	//----------------------------
	//	���_�̈ʒu���擾
	//----------------------------
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	PrimitivPos[0] = pVtx[0].pos;
	PrimitivPos[1] = pVtx[1].pos;
	PrimitivPos[2] = pVtx[2].pos;
	PrimitivPos[3] = pVtx[3].pos;

	//���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	//----------------------------
	//	���[���h���W�����߂�
	//----------------------------
	D3DXMATRIX	mtxLineWorld, mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 Rot = CScene3D::GetRot();			//�����̎擾
	D3DXVECTOR3 Pos = CScene3D::GetPos();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxLineWorld);
	RequestWorldMatrix(Pos, Rot, &mtxLineWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxLineWorld);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxPriWorld, mtxPriRot, mtxPriTrans;			

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPriWorld);
		RequestWorldMatrix(PrimitivPos[nCnt], INITIALIZE_VECTOR3, &mtxPriWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxLineWorld);

		//���[���h���W�̕ۑ�
		m_WorldPos[nCnt] = D3DXVECTOR3(mtxPriWorld._41, mtxPriWorld._42, mtxPriWorld._43);
	}

	//----------------------------
	//	���������ւ���
	//----------------------------
	D3DXVECTOR3 WorldPos = m_WorldPos[3];
	m_WorldPos[3] = m_WorldPos[1];
	m_WorldPos[1] = m_WorldPos[2];
	m_WorldPos[2] = Pos;

	//----------------------------
	//	VecA�����߂�
	//----------------------------
	for (int nCntVec = 0; nCntVec < MAX_POS; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % MAX_POS) == 0) { nCnt = 0; }

		m_VecA[nCntVec] = m_WorldPos[nCnt] - m_WorldPos[nCntVec];
	}
}

//=============================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//=============================================================================
void CLine::RequestWorldMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXMATRIX * mtxWorld)
{
	D3DXMATRIX mtxRot, mtxParent; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(mtxWorld);

	// ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// ��]�𔽉f����
	mtxWorld->_11 = mtxRot._11;
	mtxWorld->_12 = mtxRot._12;
	mtxWorld->_13 = mtxRot._13;
	mtxWorld->_21 = mtxRot._21;
	mtxWorld->_22 = mtxRot._22;
	mtxWorld->_23 = mtxRot._23;
	mtxWorld->_31 = mtxRot._31;
	mtxWorld->_32 = mtxRot._32;
	mtxWorld->_33 = mtxRot._33;

	// �I�t�Z�b�g�ʒu�𔽉f
	mtxWorld->_41 = pos.x;
	mtxWorld->_42 = pos.y;
	mtxWorld->_43 = pos.z;
}