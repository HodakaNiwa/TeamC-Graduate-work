//*****************************************************************************
//
//     �L�����A�C�R���̏���[charaicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "charaicon.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCharaicon::CCharaicon()
{
	m_pParentChara = NULL;
	m_pDestChara = NULL;
	m_bDisp = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCharaicon::~CCharaicon()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCharaicon *CCharaicon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	CCharaicon *pCharaicon = NULL;
	if (pCharaicon != NULL) { return NULL; }

	// ���������m��
	pCharaicon = new CCharaicon;
	if (pCharaicon == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pCharaicon->SetPos(pos);              // �|���S���̍��W
	pCharaicon->SetRot(fRot);             // �|���S���̌���
	pCharaicon->SetCol(col);              // �|���S���̐F
	pCharaicon->SetWidth(fWidth);         // �|���S���̕�
	pCharaicon->SetHeight(fHeight);       // �|���S���̍���
	pCharaicon->SetTexU(fTexU);           // �|���S���̍���e�N�X�`��U���W
	pCharaicon->SetTexV(fTexV);           // �|���S���̍���e�N�X�`��V���W
	pCharaicon->SetTexWidth(fTexWidth);   // �|���S���̃e�N�X�`�����W�̉���
	pCharaicon->SetTexHeight(fTexHeight); // �|���S���̃e�N�X�`�����W�̏c��

	// ����������
	if (FAILED(pCharaicon->Init()))
	{
		pCharaicon->Uninit();
		delete pCharaicon;
		return NULL;
	}

	return pCharaicon;	// �C���X�^���X�̃A�h���X��Ԃ�
}


//=============================================================================
//    ����������
//=============================================================================
HRESULT CCharaicon::Init(void)
{
	if (FAILED(CIcon2D::Init()))
	{// �A�C�R���̏������Ɏ��s
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCharaicon::Uninit(void)
{
	CIcon2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CCharaicon::Update(void)
{

}

//=============================================================================
//    �ړI�̃L�������`�悳��Ă��邩���肷�鏈��
//=============================================================================
void CCharaicon::CheckDraw(void)
{
	if (m_pDestChara == NULL || m_pDestChara == m_pParentChara) { m_bDisp = false; return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �ړI�̃L�����̈ʒu���擾
	D3DXVECTOR3 pos = m_pDestChara->GetPos();

	// �f�o�C�X����r���[�}�g���b�N�X�ƃv���W�F�N�V�����}�g���b�N�X���擾
	D3DXMATRIX mtxWorld, mtxView, mtxProj, mtxViewport;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�|�[�g�s����쐬
	D3DVIEWPORT9 Viewport;
	D3DXMatrixIdentity(&mtxViewport);
	pDevice->GetViewport(&Viewport);
	float fValue1 = (float)Viewport.Width / 2;
	float fValue2 = (float)Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._33 = Viewport.MaxZ - Viewport.MinZ;
	mtxViewport._41 = (float)(Viewport.X + fValue1);
	mtxViewport._42 = (float)(Viewport.Y + fValue2);
	mtxViewport._43 = Viewport.MinZ;
	mtxWorld = mtxView * mtxProj * mtxViewport;

	// �ʒu���X�N���[���ɓ��e
	D3DXVec3TransformCoord(&pos, &pos, &mtxWorld);

	// �X�N���[�����Ɏ��܂��Ă��邩����
	if (pos.x <= Viewport.X || pos.x >= Viewport.X + Viewport.Width
		|| pos.y <= Viewport.Y || pos.y >= Viewport.Y + Viewport.Height)
	{// ���܂��Ă��Ȃ��Ȃ�A�C�R����`�悷��
		m_bDisp = true;
	}
	else
	{// ���܂��Ă���Ȃ�A�C�R���͏���
		m_bDisp = false;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CCharaicon::Draw(void)
{
	// �`�攻��
	CheckDraw();

	if (m_bDisp == false) { return; }

	// �X�N���[���Ƃ̌�_���Z�o
	CalcCrossPoint_ScreenLine();

	// �A�C�R���̕`��
	CIcon2D::Draw();
}

//=============================================================================
//    �X�N���[���[�Ƃ̌�_���v�Z���鏈��
//=============================================================================
void CCharaicon::CalcCrossPoint_ScreenLine(void)
{
	if (m_pDestChara == NULL || m_pParentChara == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �ړI�̃L�����ʒu�Ɛe���̃L�����N�^�[�̈ʒu���擾
	D3DXVECTOR3 DestPos = m_pDestChara->GetPos();
	D3DXVECTOR3 ParentPos = m_pParentChara->GetPos();

	// �f�o�C�X����r���[�}�g���b�N�X�ƃv���W�F�N�V�����}�g���b�N�X���擾
	D3DXMATRIX mtxWorld, mtxView, mtxProj, mtxViewport;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	mtxWorld = mtxView * mtxProj;

	// �r���[�|�[�g�s����쐬
	D3DVIEWPORT9 Viewport;
	D3DXMatrixIdentity(&mtxViewport);
	pDevice->GetViewport(&Viewport);
	float fValue1 = (float)Viewport.Width / 2;
	float fValue2 = (float)Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._33 = Viewport.MaxZ - Viewport.MinZ;
	mtxViewport._41 = (float)(Viewport.X + fValue1);
	mtxViewport._42 = (float)(Viewport.Y + fValue2);
	mtxViewport._43 = Viewport.MinZ;
	mtxWorld *= mtxViewport;

	// �X�N���[�����W�ɕϊ�
	D3DXVec3TransformCoord(&DestPos, &DestPos, &mtxWorld);
	D3DXVec3TransformCoord(&ParentPos, &ParentPos, &mtxWorld);

	// 4�{�̃x�N�g���̌�_���Z�o
	float X = (float)Viewport.X;
	float Y = (float)Viewport.Y;
	float Width = (float)Viewport.Width;
	float Height = (float)Viewport.Height;
	D3DXVECTOR3 ScreenPos[4]
	{
		D3DXVECTOR3(X, Y, 0.0f),
		D3DXVECTOR3(X + Width, Y, 0.0f),
		D3DXVECTOR3(X + Width, Y + Height, 0.0f),
		D3DXVECTOR3(X, Y + Height, 0.0f),
	};
	D3DXVECTOR3 InterPos = INITIALIZE_VECTOR3;
	int nIdxInter = -1;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// �������쐬
		D3DXVECTOR3 Vec1 = D3DXVECTOR3(DestPos.x - ScreenPos[nCnt].x, DestPos.y - ScreenPos[nCnt].y, 0.0f);
		D3DXVECTOR3 Vec2 = D3DXVECTOR3(ParentPos.x - DestPos.x, ParentPos.y - DestPos.y, 0.0f);
		D3DXVECTOR3 Vec3 = D3DXVECTOR3(ScreenPos[(nCnt + 1) % 4].x - ScreenPos[nCnt].x, ScreenPos[(nCnt + 1) % 4].y - ScreenPos[nCnt].y, 0.0f);

		// �O�ς��o��(0�Ȃ畽�s�Ȃ̂ŏ����������̃��[�v��)
		float fDeno = Vec2.x * Vec3.y - Vec2.y * Vec3.x;
		if (fDeno == 0) { continue; }

		// �����̔䗦���Z�o
		float fLengthStart = (Vec1.y * Vec3.x - Vec1.x * Vec3.y) / fDeno;
		float fLengthEnd = (Vec1.y * Vec2.x - Vec1.x * Vec2.y) / fDeno;
		if (fLengthStart < 0 || 1 < fLengthStart || fLengthEnd < 0 || 1 < fLengthEnd) { continue; }

		// ��_�Z�o
		InterPos.x = Vec2.x * fLengthStart + DestPos.x;
		InterPos.y = Vec2.y * fLengthStart + DestPos.y;
		nIdxInter = nCnt;
	}
	float fLength = GetLength();
	switch (nIdxInter)
	{// �ǂ̉�ʒ[�ƌ�_�����΂�Ă��邩�ŏ����킯
	case 0:
		InterPos.y += fLength;
		break;
	case 1:
		InterPos.x -= fLength;
		break;
	case 2:
		InterPos.y -= fLength;
		break;
	case 3:
		InterPos.x += fLength;
		break;
	}

	// �ʒu�̐ݒ�
	SetPos(InterPos);
	SetVtxBuffPos();
}