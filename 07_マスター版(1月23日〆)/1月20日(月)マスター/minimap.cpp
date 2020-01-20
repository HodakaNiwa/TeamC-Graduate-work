//*****************************************************************************
//
//     �~�j�}�b�v�̏���[minimap.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "minimap.h"
#include "manager.h"
#include "modebase.h"
#include "player.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// �x�N�g���̏�����
#define MINAMAP_PRIORITY (7)							// �~�j�}�b�v�̏����D�揇��

// �w�i�p�}�N��
#define MINIMAP_BG_POS      (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define MINIMAP_BG_POS_ONCE (D3DXVECTOR3(1220.0f, 60.0f, 0.0f))
#define MINIMAP_BG_SIZE     (D3DXVECTOR3(50.0f,50.0f,0.0f))
#define MINIMAP_BG_COL      (D3DXCOLOR(0.4f,0.4f,0.4f,1.0f))

// �v���C���[�A�C�R���p�}�N��
#define MINIMAP_PLAYERICON_TEXIDX     (0)
#define MINIMAP_PLAYERICON_SIZE       (D3DXVECTOR3(5.0f,5.0f,0.0f))
#define MINIMAP_PLAYERICON_LIMIT_X    (30.0f)
#define MINIMAP_PLAYERICON_LIMIT_Y    (30.0f)
#define MINIMAP_PLAYERICON_RIVISION_X (2.0f)
#define MINIMAP_PLAYERICON_RIVISION_Y (5.0f)

//=============================================================================
//    ��������
//=============================================================================
CMiniMap * CMiniMap::Create(void)
{
	CMiniMap *pMiniMap = NULL;
	if (pMiniMap != NULL) { return NULL; }

	// ���������m��
	pMiniMap = new CMiniMap;
	if (pMiniMap == NULL) { return NULL; }

	// ����������
	if (FAILED(pMiniMap->Init()))
	{
		pMiniMap->Uninit();
		delete pMiniMap;
		return NULL;
	}

	return pMiniMap;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMiniMap::CMiniMap()
{
	m_pBG = NULL;
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		m_pPlayerIcon[nCnt] = NULL;
		m_pCharacter[nCnt] = NULL;
	}

	m_BGPos = INITIALIZE_VECTOR3;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMiniMap::~CMiniMap() {}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMiniMap::Init(void)
{
	// �w�i�̈ʒu��������
	m_BGPos = MINIMAP_BG_POS;

	// �w�i�̍쐬
	CreateBG();

	// �v���C���[�A�C�R���̍쐬
	CreatePlayerIcon();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMiniMap::Uninit()
{
	// �w�i�̊J��
	ReleaseBG();

	// �v���C���[�A�C�R���̊J��
	ReleasePlayerIcon();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMiniMap::Update()
{
	// �v���C���[�A�C�R���̈ʒu���ړ�������
	SettingPlayerIconPos();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMiniMap::Draw()
{

}

//=============================================================================
//    �v���C���[�A�C�R���̈ʒu��ݒ肷�鏈��
//=============================================================================
void CMiniMap::SettingPlayerIconPos(void)
{
	D3DXVECTOR3 IconPos;
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		// �A�C�R������������Ă��Ȃ���Ώ����������̃��[�v��
		if (m_pPlayerIcon[nCnt] == NULL) { continue; }

		// �|�C���^���ݒ肳��Ă��Ȃ���Ώ����������̃��[�v��
		if (m_pCharacter[nCnt] == NULL) { continue; }

		// �ʒu���~�j�}�b�v�̃T�C�Y�ɍ��킹��
		IconPos.x = m_pCharacter[nCnt]->GetPos().x / MINIMAP_PLAYERICON_LIMIT_X + m_BGPos.x + MINIMAP_PLAYERICON_RIVISION_X;
		IconPos.y = -m_pCharacter[nCnt]->GetPos().z / MINIMAP_PLAYERICON_LIMIT_Y + m_BGPos.y + MINIMAP_PLAYERICON_RIVISION_Y;
		IconPos.z = 0.0f;

		// �ʒu�̐ݒ�
		m_pPlayerIcon[nCnt]->SetPos(IconPos, MINIMAP_PLAYERICON_SIZE);
	}
}

//=============================================================================
//    �w�i�̈ʒu��ݒ肷�鏈��
//=============================================================================
void CMiniMap::SettingBGPos(void)
{
	m_BGPos = MINIMAP_BG_POS_ONCE;
	if (m_pBG == NULL) { return; }
	m_pBG->SetPos(m_BGPos, MINIMAP_BG_SIZE);
}

//=============================================================================
//    �w�i���쐬���鏈��
//=============================================================================
void CMiniMap::CreateBG(void)
{
	if (m_pBG != NULL) { return; }
	m_pBG = CScene2D::Create(m_BGPos, MINIMAP_BG_SIZE, CScene2D::TYPE_LOGO);

	if (m_pBG != NULL)
	{
		m_pBG->BindTexture(NULL);
		m_pBG->SetColoer(MINIMAP_BG_COL);
	}
}

//=============================================================================
//    �v���C���[�A�C�R�����쐬���鏈��
//=============================================================================
void CMiniMap::CreatePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		// �A�C�R������������Ă��Ȃ���Ώ����������̃��[�v��
		if (m_pPlayerIcon[nCnt] != NULL) { continue; }

		// �A�C�R���̍쐬
		m_pPlayerIcon[nCnt] = CScene2D::Create(VECTOR_ZERO, MINIMAP_PLAYERICON_SIZE, CScene2D::TYPE_LOGO);
		if (m_pPlayerIcon[nCnt] == NULL) { continue; }

		// �l�̐ݒ�
		m_pPlayerIcon[nCnt]->BindTexture(CManager::GetModeBase()->GetTexture()[MINIMAP_PLAYERICON_TEXIDX]);
		m_pPlayerIcon[nCnt]->SetColoer(CPlayer::m_CountryColor[nCnt]);
	}
}

//=============================================================================
//    �w�i���폜���鏈��
//=============================================================================
void CMiniMap::ReleaseBG(void)
{
	if (m_pBG != NULL)
	{
		m_pBG->Uninit();
		m_pBG = NULL;
	}
}

//=============================================================================
//    �v���C���[�A�C�R�����폜���鏈��
//=============================================================================
void CMiniMap::ReleasePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		if (m_pPlayerIcon[nCnt] != NULL)
		{
			m_pPlayerIcon[nCnt]->Uninit();
			m_pPlayerIcon[nCnt] = NULL;
		}
	}
}