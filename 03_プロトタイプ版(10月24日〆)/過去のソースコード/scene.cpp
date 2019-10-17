//=============================================================================
//
// �I�u�W�F�N�g�̏��� [renderer.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "scene.h"
#include "input.h"
#include "manager.h"
#include "collision.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CScene * CScene::m_pTop[MAX_LAYER] = {};
CScene * CScene::m_pCur[MAX_LAYER] = {};
int CScene::m_nNumAll = 0;
bool CScene::m_bPause = false;
int CScene::m_nCountDraw = 0;

//�f�o�b�N�p�ϐ�
int CScene::m_anNumOBJ[MAX_LAYER] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE ObjType)
{
	//�����蔻��|�C���^
	m_pColliderManger = NULL;

	//���S�t���O
	m_bDeth = false;

	//�`�悷�邩�ǂ���
	m_bDraw = false;

	//�����ʒu�̕ۑ�
	if (m_pTop[nPriority] == NULL)
	{
		m_pTop[nPriority] = this;		//���߂̈ʒu��ۑ�����
	}

	//�O�̃I�u�W�F�N�g�i�ʒu�j�̃|�C���^��ۑ�����
	if (m_pCur[nPriority] != NULL)
	{
		m_pPrev = m_pCur[nPriority];
	}
	else
	{
		m_pPrev = NULL;
	}

	//�Ō���̃|�C���^��ۑ�����
	m_pCur[nPriority] = this;

	//�Ō���̎��̃I�u�W�F�N�g��NULL��Ԃɂ���
	m_pNext = NULL;			

	//�O��̃I�u�W�F�N�g��m_pNext�Ɍ��݈ʒu�i�Ō���j�̃|�C���^��n��
	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pCur[nPriority];
	}

	m_nID = m_nNumAll;			//�ԍ��̕ۑ�
	m_nNumAll++;				//�����̉��Z
	m_anNumOBJ[nPriority]++;	//�e���C���[�̃I�u�W�F�N�g���̉��Z
	m_ObjType = ObjType;		//�I�u�W�F�N�g�^�C�v�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
//	�S�������̊J��
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				pScene->Uninit();
				pScene = pScene->m_pNext;	//�X�^�[�g�ʒu�����̃I�u�W�F�N�g�ɐi�߂�
			}

			//�g�b�v�̈ʒu��ۑ�����
			pScene = m_pTop[nCntLayer];

			//���S�t���O�̂ݔj������
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->m_bDeth == true)
				{
					pScene->Deth(nCntLayer);	//����
				}
				pScene = pSceneNext;			//�|�C���^��i�߂�
			}

			m_pTop[nCntLayer] = NULL;	//�n�܂�̈ʒu������������
			m_pCur[nCntLayer] = NULL;	//�Ō��������������
		}
	}

	m_nNumAll = 0;	//�l�̏�����

	//�R���C�_�[�̑S�j��
	CCollider::ReleaseAll();
}

//=============================================================================
//	�t�F�[�h�ȊO�̃��������J������
//=============================================================================
void CScene::ReleaseFade(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
				if (pScene->m_ObjType != OBJTYPE_FADE)
				{
					pScene->Uninit();
				}
				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
		}
	}

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = m_pTop[nCntLayer];

			//���S�t���O�̂ݔj������
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->m_bDeth == true)
				{
					if (pScene->m_ObjType != OBJTYPE_FADE)
					{
						pScene->Deth(nCntLayer);	//����
					}
					else
					{
						pScene->m_bDeth = false;	//���ȂȂ���Ԃɂ���
					}
				}
				pScene = pSceneNext;				//�|�C���^��i�߂�
			}
		}
	}
}

//=============================================================================
//	�������̊J��
//=============================================================================
void CScene::Release(void)
{	
	m_bDeth = true;
}

//=============================================================================
//	�S�Ă̍X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	//�f�o�b�N�\���̎擾
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	//�ϐ��錾
	CScene * pScene = NULL;

	if (m_bPause == false)
	{
		for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
		{
			if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
			{
				//�g�b�v�̈ʒu��ۑ�����
				CScene * pScene = m_pTop[nCntLayer];

				while (pScene != NULL)
				{
					CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
					pScene->Update();
					pScene = pSceneNext;					//�|�C���^��i�߂�
				}
			}
		}
	}

	//�R���C�_�[�̍X�V����
	CCollider::UpdateAll();

	//�Փ˃`�F�b�N
	CheckCollisionAll();

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			pScene = m_pTop[nCntLayer];

			//���S�t���O�̂ݔj������
			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->m_bDeth == true)
				{
					pScene->Deth(nCntLayer);	//����
				}
				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
		}
	}

	m_nCountDraw = 0;
}

//=============================================================================
//	�S�Ă̕`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = m_pTop[nCntLayer];

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->m_bDraw == false)
				{
					pScene->Draw();
					m_nCountDraw++;
				}

				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
		}
	}

	//�R���C�_�[�̕`�揈��
	CCollider::DrawAll();
}

//=============================================================================
//	���S�t���O�̐ݒ�
//=============================================================================
void CScene::Deth(int nCntLayer)
{
	//�R���C�_�[�̂����蔻��Ǌ��N���X�̔j��
	if (m_pColliderManger != NULL)
	{
		m_pColliderManger->Uninit();
		m_pColliderManger = NULL;
	}

	if (m_pPrev != NULL)
	{
		m_pPrev->m_pNext = m_pNext;	//�O�̃I�u�W�F�N�g�Ɍ��݈ʒu��m_pNext��������
	}
	else
	{
		m_pTop[nCntLayer] = m_pNext;			//�X�^�[�g�ʒu�����̃I�u�W�F�N�g�Ɉړ�����
	}

	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		m_pCur[nCntLayer] = m_pPrev;			//�X�^�[�g�ʒu�����̃I�u�W�F�N�g�Ɉړ�����
	}

	m_anNumOBJ[nCntLayer]--;	//�e���C���[�̃I�u�W�F�N�g�̑��������炷
	m_nNumAll--;				//�I�u�W�F�N�g�̑��������炷
	delete this;	
}

//=============================================================================
// �R���C�_�[�̏Փ˔���`�F�b�N
//=============================================================================
void CScene::CheckCollisionAll(void)
{
	//�f�o�b�N�\���̎擾
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	//�ϐ��錾
	CScene * pScene = NULL;

	if (m_bPause == false)
	{
		for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
		{
			if (m_pTop[nCntLayer] != NULL && m_pCur[nCntLayer] != NULL)
			{
				//�g�b�v�̈ʒu��ۑ�����
				CScene * pScene = m_pTop[nCntLayer];

				while (pScene != NULL)
				{
					CScene * pSceneNext = pScene->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
					pScene->Collision();
					pScene = pSceneNext;					//�|�C���^��i�߂�
				}
			}
		}
	}

	CCollider::DeathCheck();
}

//=============================================================================
// �R���W����
//=============================================================================
void CScene::Collision(void){}