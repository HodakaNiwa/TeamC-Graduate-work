//*****************************************************************************
//
//     �n���̏���[mine.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MINE_H_
#define _MINE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CLoadEffect;

//*****************************************************************************
//    �n���N���X�̒�`
//*****************************************************************************
class CMine : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//	���
	//--------------------
	typedef enum
	{
		TYPE_NONE = 0,	// �ʏ���
		TYPE_STARTUP,	// �N�����
		TYPE_EXPLOSION,	// �������
		TYPE_MAX
	}TYPE;

	CMine(int nPriority = 3, OBJTYPE objtype = OBJTYPE_MINE);
	~CMine();

	static CMine *Create(D3DXVECTOR3 pos, CScene *pParentChara);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetType(const int nType) { m_nType = nType; }
	void SetScaleUp(const float fScaleUp) { m_fScaleUp = fScaleUp; }
	void SetExplosionTimer(const int nExplosionTimer) { m_nExplosionTimer = nExplosionTimer; }
	void SetParentChara(CScene *pParentChara) { m_pParentChara = pParentChara; }

	int GetType(void) { return m_nType; }
	int GetLife(void) { return m_nLife; }
	float GetScaleUp(void) { return m_fScaleUp; }
	int GetExplosionTimer(void) { return m_nExplosionTimer; }
	CScene *GetParentChara(void) { return m_pParentChara; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void None(void);
	void StartUp(void);
	void Explosion(void);
	void CreateStartUpCollider(void);
	void CreateExplosionEffect(void);
	void CreateExplosionCollider(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	int m_nType;							// ���
	int m_nLife;							// ����
	float m_fScaleUp;						// �ǂꂭ�炢�傫�����邩
	int m_nExplosionTimer;					// �����܂ł̎��Ԃ��v������
	CScene *m_pParentChara;					// ���g�𐶐������L�����ւ̃|�C���^
	CLoadEffect * m_pLoadEffect;			// �G�t�F�N�g�̏��


	//--------------------
	//	�ÓI�����o�ϐ�
	//--------------------
	static LPD3DXMESH m_pMesh;
	static LPD3DXBUFFER m_pBuffMat;
	static DWORD m_nNumMat;
};

#endif