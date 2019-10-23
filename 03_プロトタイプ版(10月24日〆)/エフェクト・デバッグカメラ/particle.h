//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���� ����	Sato Asumi
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene.h"
#include "sceneBillBorad.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CParticle : public CSceneBillborad
{
public:
	typedef enum
	{//�G�t�F�N�g�̓����̎��
		TYPE_NORMAL = 0,
		TYPE_SPEW,
		TYPE_GRANDSMOKE,
		TYPE_LIGHT,
		TYPE_CICLE,
		TYPE_STAR,
		TYPE_MAX,
	}TYPE;

	typedef enum
	{//�`����
		DRAWTYPE_NORMAL = 0,
		DRAWTYPE_ADD,
		DRAWTYPE_MAX,
	}DRAWTYPE;

	CParticle();
	CParticle(int nParticle);
	~CParticle();
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//---------------------
	// Load & Unload
	//---------------------
	static void LoadTexture(void);
	static void UnloadTexture(void);

	//---------------------
	// Set & Get
	//---------------------
	D3DXCOLOR GetColor(void) { return m_col; };			// �F�̎擾
	void SetColor(D3DXCOLOR col);						// �F�̐ݒ�
	D3DXVECTOR2 GetSize(void) { return m_size; }		// �T�C�Y�̎擾
	void SetSize(D3DXVECTOR2 size) { m_size = size; }	// �T�C�Y�̐ݒ�
	void RemakeSize(D3DXVECTOR2 size);					// �T�C�Y�̕ύX
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�̐ݒ�

	void SetDrawType(DRAWTYPE DrawType) { m_DrawType = DrawType; };		//�`��^�C�v�̐ݒ�
	void SetColorA(float colA) { m_colA = colA; };						//�F�̓����x�̐ݒ�
	void SetZbuffu(bool bZBuffu) { m_bZBuffu = bZBuffu; };				//Z�o�b�t�@�̗L���ݒ�
	void SetLife(int nLife) { m_nLife = nLife; };						//���C�t�̐ݒ�
	void SetChangeCol_a(float fCol_a) { m_fChangeCol_a = fCol_a; };		//�����x�̕ω��ʂ̐ݒ�
	void SetUseGravity(bool bGravity) { m_bGravity = bGravity; };		//�d�͐ݒ�
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; };					//�X�s�[�h�̐ݒ�
	void SetMinHight(int nMinHight) { m_nMinHight = nMinHight; };		//�����̍ŏ��l�̐ݒ�
	void SetMaxHight(int nMaxHight) { m_nMaxHight = nMaxHight; };		//�����̍ő�l�̐ݒ�
	void SetGravity(float fGravity) { m_fGravity = fGravity; };			//�d�͂̐ݒ�
	void SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV);
	
protected:
	void SetAngle(float * pAngle);
	D3DXVECTOR3		m_move;			//�ړ���
	float			m_fSpeed;		//�ړ��X�s�[�h
	float			m_colA;			//�����x�̐ݒ�	
	int				m_nLife;		//�\������
	float			m_fChangeCol_a;	//���l�̕ω���
	bool			m_bGravity;		//�d�͂�ON�EOFF
	int				m_nMinHight;	//�����̍ŏ��l
	int				m_nMaxHight;	//�����̍ő�l
	float			m_fGravity;		//�d��
	bool			m_bBound;		//�o�E���h
	float			m_fBoundHight;	//�o�E���h���鍂��
	bool			m_bDeth;		//���S�t���O

private:
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ����
	D3DXCOLOR				m_col;				// �F
	D3DXVECTOR2				m_size;				// �傫��

	static LPDIRECT3DTEXTURE9 * m_ppTexture;	//�e�N�X�`���̃|�C���^
	static int					m_nMaxTexture;	//�e�N�X�`���̍ő吔
	DRAWTYPE					m_DrawType;		//�`��̃^�C�v
	bool						m_bZBuffu;		//Z�o�b�t�@��L���ɂ��邩�ǂ���
};

//=============================================================================
// ���̃p�[�e�B�N���N���X
//=============================================================================
class CSpewParticle : public CParticle
{
public:
	CSpewParticle(int nPriority);
	~CSpewParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//�t���[���J�E���^�[
};

//=============================================================================
// �y���̃p�[�e�B�N���N���X
//=============================================================================
class CGrandSmokeParticle : public CParticle
{
public:
	CGrandSmokeParticle(int nPriority);
	~CGrandSmokeParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//�t���[���J�E���^�[
};

//=============================================================================
// ���C�g�p�[�e�B�N��
//=============================================================================
class CLightParticle : public CParticle
{
public:
	CLightParticle(int nPriority);
	~CLightParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	float m_fChangeSize;	//�T�C�Y�̕ω���
};

//=============================================================================
// �~�`�̃p�[�e�B�N���N���X
//=============================================================================
class CCicleParticle : public CParticle
{
public:
	CCicleParticle(int nPriority);
	~CCicleParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//�t���[���J�E���^�[
};

//=============================================================================
// ���̃p�[�e�B�N���N���X
//=============================================================================
class CStarParticle : public CParticle
{
public:
	CStarParticle(int nPriority);
	~CStarParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nCountFlam;		//�t���[���J�E���^�[
};
#endif