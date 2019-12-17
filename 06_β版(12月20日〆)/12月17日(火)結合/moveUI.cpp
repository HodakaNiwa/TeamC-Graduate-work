//=============================================================================
//
// �ړ��^�C�v��UI�̏��� [MoveUI.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "moveUI.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "scene2D.h"
#include "library.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����

//=============================================================================
// ��������
//=============================================================================
CMoveUI * CMoveUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture)
{
	CMoveUI * pMoveUI = new CMoveUI;

	pMoveUI->Init();
	pMoveUI->Set(pos, size);
	pMoveUI->BindTexture(pTexture);

	return pMoveUI;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMoveUI::CMoveUI() : CScene2D(10, CScene::OBJTYPE_UI){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMoveUI::~CMoveUI(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMoveUI::Init(void)
{
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMoveUI::Uninit()
{
	CScene2D::Uninit();

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMoveUI::Update(){}

//=============================================================================
// �`�揈��
//=============================================================================
void CMoveUI::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// ���Ɉړ�����
//=============================================================================
void CMoveUI::UpdateMoveLeft(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.x -= fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// �E�Ɉړ�����
//=============================================================================
void CMoveUI::UpdateMoveRight(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.x += fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// ��Ɉړ�����
//=============================================================================
void CMoveUI::UpdateMoveUp(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.y -= fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// ���Ɉړ�����
//=============================================================================
void CMoveUI::UpdateMoveDown(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.y += fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// �g�又��
//=============================================================================
void CMoveUI::UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	//�T�C�Y�̎擾
	D3DXVECTOR3 Size = CScene2D::GetSize();

	//X�̃T�C�Y�ݒ�
	if (Size.x < AimSize.x)
	{
		Size.x += Move.x;
	}
	else
	{
		Size.x = AimSize.x;
	}

	//Y�̃T�C�Y�ݒ�
	if (Size.y < AimSize.y)
	{
		Size.y += Move.y;
	}
	else
	{
		Size.y = AimSize.y;
	}

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(Size);
}

//=============================================================================
//	�k������
//=============================================================================
void CMoveUI::UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	//�T�C�Y�̎擾
	D3DXVECTOR3 Size = CScene2D::GetSize();

	//X�̃T�C�Y�ݒ�
	if (Size.x > AimSize.x)
	{
		Size.x -= Move.x;
	}
	else
	{
		Size.x = AimSize.x;
	}

	//Y�̃T�C�Y�ݒ�
	if (Size.y > AimSize.y)
	{
		Size.y -= Move.y;
	}
	else
	{
		Size.y = AimSize.y;
	}

	//�T�C�Y�̐ݒ�
	CScene2D::SetSize(Size);
}

//=============================================================================
//	�ړI�̈ʒu�܂ňړ�����
//=============================================================================
void CMoveUI::UpdateMoveDest(D3DXVECTOR3 AimPos, float fSpeed)
{
	//�ϐ��錾
	bool bBigerX = false;
	bool bBigerY = false;

	//�ړI�n�������������傫����
	D3DXVECTOR3 Pos = CScene2D::GetPos();
	if (Pos.x > AimPos.x) { bBigerX = true; }
	if (Pos.y > AimPos.y) { bBigerY = true; }

	//�ړI�̒����_�̊p�x�����߂�
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.y, AimPos.x, AimPos.y);

	//�����_�̍X�V
	Pos.x += sinf(fAngle) * fSpeed;
	Pos.y += cosf(fAngle) * fSpeed;

	//�l�̏C��
	if (bBigerX)
	{//X�l���ړI�n���傫��
		if (Pos.x < AimPos.x) { Pos.x = AimPos.x; }
	}
	else
	{//X�l���ړI�n��菬����
		if (Pos.x > AimPos.x) { Pos.x = AimPos.x; }
	}

	if (bBigerY)
	{//Y�l���ړI�n���傫��
		if (Pos.y < AimPos.y) { Pos.y = AimPos.y; }
	}
	else
	{//Y�l���ړI�n��菬����
		if (Pos.y > AimPos.y) { Pos.y = AimPos.y; }
	}

	//�ʒu��ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
//	���_���Ƃ̐ݒ�ŖړI�n�܂ł̈ړ�
//=============================================================================
void CMoveUI::UpdateMoveDestVertex(D3DXVECTOR3 AimPos, float fSpeed)
{
	//�ϐ��錾
	bool bBigerX = false;
	bool bBigerY = false;

	//�ړI�n�������������傫����
	D3DXVECTOR3 Pos = CScene2D::GetPos();
	if (Pos.x > AimPos.x) { bBigerX = true; }
	if (Pos.y > AimPos.y) { bBigerY = true; }

	//�ړI�̒����_�̊p�x�����߂�
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.y, AimPos.x, AimPos.y);

	//�����_�̍X�V
	Pos.x += sinf(fAngle) * fSpeed;
	Pos.y += cosf(fAngle) * fSpeed;

	//�l�̏C��
	if (bBigerX)
	{//X�l���ړI�n���傫��
		if (Pos.x < AimPos.x) { Pos.x = AimPos.x; }
	}
	else
	{//X�l���ړI�n��菬����
		if (Pos.x > AimPos.x) { Pos.x = AimPos.x; }
	}

	if (bBigerY)
	{//Y�l���ړI�n���傫��
		if (Pos.y < AimPos.y) { Pos.y = AimPos.y; }
	}
	else
	{//Y�l���ړI�n��菬����
		if (Pos.y > AimPos.y) { Pos.y = AimPos.y; }
	}

	//�ʒu��ݒ�
	CScene2D::SetPosVertex(Pos);
}