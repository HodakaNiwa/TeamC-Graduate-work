//*****************************************************************************
//
//     �V�F�[�_�[�̏���[shader.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADER_H_
#define _SHADER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "light.h"

//*****************************************************************************
//    �V�F�[�_�[�N���X�̒�`
//*****************************************************************************
class CShader
{
public:    // �N�ł��A�N�Z�X�\
	CShader(char *pFileName);
	virtual ~CShader();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void SetParamToEffect(void) = 0;

	HRESULT Begin(void);
	HRESULT End(void);

	HRESULT BeginPass(int nIdx = 0);
	HRESULT EndPass(void);

	HRESULT CommitChanges(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void SetFileName(char *pFileName);

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_hTechnique;
	char m_pFileName[256];

private:   // ���̃N���X�������A�N�Z�X�\
};


//*****************************************************************************
//    �C���X�^���V���O�V�F�[�_�[�N���X�̒�`
//*****************************************************************************
class CInstancingShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	CInstancingShader(char *pFileName);
	~CInstancingShader();

	static CInstancingShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void SetMtxLocal(const D3DXMATRIX mtxLocal) { m_MtxLocal = mtxLocal; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir) { m_LightDir = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse) { m_LightDiffuse = LightDiffuse; }
	void SetLightAmbient(const D3DXCOLOR LightAmbient) { m_LightAmbient = LightAmbient; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_MtxLocal;				// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_LightDir;				// ���C�g�̕���
	D3DXCOLOR m_LightDiffuse;			// ���C�g�̊g�U��
	D3DXCOLOR m_LightAmbient;			// ���C�g�̊���

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hMtxLocal;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir;
	D3DXHANDLE m_hLightDiffuse;
	D3DXHANDLE m_hLightAmbient;
};

//*****************************************************************************
//    �n�ʃV�F�[�_�[�N���X�̒�`
//*****************************************************************************
class CFieldShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	CFieldShader(char *pFileName);
	~CFieldShader();

	static CFieldShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ���C�g�̕���
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ���C�g�̊g�U��

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    �A�E�g���C���V�F�[�_�[�N���X�̒�`
//*****************************************************************************
class COutlineShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	COutlineShader(char *pFileName);
	~COutlineShader();

	static COutlineShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_MtxWorld;				// ���[���h�}�g���b�N�X

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hMtxWorld;
};

//*****************************************************************************
//    �����O�V�F�[�_�[�N���X�̒�`
//*****************************************************************************
class CRingShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	CRingShader(char *pFileName);
	~CRingShader();

	static CRingShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ���C�g�̕���
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ���C�g�̊g�U��

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    �e���g���[�V�F�[�_�[�N���X�̒�`
//*****************************************************************************
class CTerritoryShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	CTerritoryShader(char *pFileName);
	~CTerritoryShader();

	static CTerritoryShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ���C�g�̕���
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ���C�g�̊g�U��

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    �e�V�F�[�_�[�N���X�̒�`(�ۉe)
//*****************************************************************************
class CShadowShader : public CShader
{
public:    // �N�ł��A�N�Z�X�\
	CShadowShader(char *pFileName);
	~CShadowShader();

	static CShadowShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXMATRIX m_MtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX m_MtxProjection;				// �v���W�F�N�V�����}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ���C�g�̕���
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ���C�g�̊g�U��

	// �e�p�����[�^�̃n���h��
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

#endif