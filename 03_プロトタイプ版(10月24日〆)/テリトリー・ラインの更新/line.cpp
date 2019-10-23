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
#include "library.h"

//=============================================================================
// �}�N����`
//=============================================================================
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
CLine::CLine() : CScene3D(3, OBJTYPE_LINE){}

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

	//�F�̐ݒ�
	CPlayer::TYPE PlayerType = CManager::GetGame()->GetPlayer(m_nNumPlayer)->GetType();
	CScene3D::SetColor(CPlayer::m_CountryColor[(int)PlayerType]);

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
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		if (nCntPlayer == m_nNumPlayer) { continue; }
		D3DXVECTOR3 Pos = CScene3D::GetPos();
		CPlayer * pOtherPlayer = CManager::GetGame()->GetPlayer(nCntPlayer);
		D3DXVECTOR3 PlayerPos = pOtherPlayer->GetPos();

		float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//���������߂�

		if (fLength > UPDATE_LENGTH) { continue; }
		bool bCutLine = CollsionLine(PlayerPos);	//�����蔻�������

		if (!bCutLine){ continue;}
		CManager::GetGame()->GetPlayer(m_nNumPlayer)->CutLine(m_nID);	//���C���ؒf����					
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLine::Draw(void)
{
	CScene3D::Draw();
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

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&mtxLineWorld, &mtxLineWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&mtxLineWorld, &mtxLineWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxLineWorld);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxPriWorld, mtxPriRot, mtxPriTrans;			

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPriWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxPriRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxPriRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxPriTrans, PrimitivPos[nCnt].x, PrimitivPos[nCnt].y, PrimitivPos[nCnt].z);
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxPriTrans);

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