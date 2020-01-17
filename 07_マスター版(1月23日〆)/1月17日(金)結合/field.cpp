//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ADD_ROT	(0.05f)
#define MOVE_Z_TIME	(12)
//#define FIELDDRAW
//=============================================================================
// ��������
//=============================================================================
CField * CField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, TYPE type)
{
	//�C���X�^���X�̐���
	CField * pField;
	pField = new CField;

	//����������
	pField->Init();

	//�ݒ菈��
	pField->Set(pos, size);
	pField->SetType(type);

	return pField;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CField::CField() : CScene3D(3, OBJTYPE_FLOR){}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CField::CField(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CField::~CField(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CField::Init(void)
{
	CScene3D::Init();

	//�ϐ��̏�����
	m_state = STATE_NOT_UPDATE;
	m_bLight = false;
	m_nCountTime = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CField::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CField::Update(void)
{
	if (m_state == STATE_NOT_UPDATE) { return; }

	CScene3D::Update();

	//�J�����̕`�攻��
	if (m_type == TYPE_BLOCK) { ChackDraw(); }

	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_X_TURN: UpdateXturn(); break;
	case STATE_X_TURNBACK: UpdateXturnBack(); break;
	case STATE_MOVE_Z: UpdateMoveZ(); break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CField::Draw(void)
{

}

//=============================================================================
// X���̉�]����
//=============================================================================
void CField::UpdateXturn(void)
{
	D3DXVECTOR3 Rot = CScene3D::GetRot();

	//X���ɉ�]�l�̉��Z
	Rot.x += ADD_ROT;

	if (Rot.x > D3DX_PI)
	{
		Rot.x = 0.0f;
		m_state = STATE_NORMAL;
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	//��]�l�̐ݒ�
	CScene3D::SetRot(Rot);
}

//=============================================================================
// X���̉�]�����@����
//=============================================================================
void CField::UpdateXturnBack(void)
{
	D3DXVECTOR3 Rot = CScene3D::GetRot();

	//X���ɉ�]�l�̉��Z
	Rot.x += ADD_ROT;

	if (Rot.x > D3DX_PI * 2.0f)
	{
		Rot.x = 0.0f;
		m_state = STATE_NORMAL;
	}

	//��]�l�̐ݒ�
	CScene3D::SetRot(Rot);
}

//=============================================================================
// Z���̈ړ�����
//=============================================================================
void CField::UpdateMoveZ(void)
{
	//���Ԃ̉��Z
	m_nCountTime++;
	if ((m_nCountTime % MOVE_Z_TIME) != 0) { return; }

	//�ʒu���擾
	D3DXVECTOR3 Pos = CScene3D::GetPos();

	//�ړ�����
	Pos.z += (BLOCK_SIZE.z * 2.0f);

	//�ʒu��ݒ�
	CScene3D::SetPos(Pos);
}

//=============================================================================
// �J�����̕`�攻�菈��
//=============================================================================
void CField::ChackDraw(void)
{
#ifdef FIELDDRAW
	//�ϐ��錾
	CGame * pGame = CManager::GetGame();
	D3DXVECTOR3 Pos = CScene3D::GetPos();
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
#endif
}