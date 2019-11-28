//=============================================================================
//
// �n�ʃ}�l�[�W���[���� [fieldmanager.h]
// Author : �������� Sato_Asumi & Hodaka Niwa
//
//=============================================================================
#ifndef _FIELDMANAGER_H_
#define _FIELDMANAGER_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_FIELD		(32 * 32)
#define LINE			(32)
#define BLOCK_SIZE		(D3DXVECTOR3(62.5f, 0.0f, 62.5f))
#define FIELD_TEX		(4)

//=============================================================================
// �O���錾
//=============================================================================
class CField;
class CFieldShader;
class CFieldRender;

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CFieldManager
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_XTURN,
		STATE_RANDOM,
		STATE_WAVE,
		STATE_COUNTDOWN,
		STATE_TIME_2,
		STATE_TIME_1,
		STATE_TIME_30,
		STATE_MAX,
	}STATE;

	// �n�ʕ`��p�񋓌^
	typedef enum
	{
		FIELD_TYPE_NONE = 0,
		FIELD_TYPE_GROUND,
		FIELD_TYPE_MAIN,
		FIELD_TYPE_COUNTDOWN,
		FIELD_TYPE_MAX
	}FIELD_TYPE;

	CFieldManager();
	~CFieldManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetState(STATE state);
	CFieldRender *GetFieldRender(const int nIdx) { return m_pFieldRender[nIdx]; }

private:
	void UpdateXTurn(void);
	void UpdateRamdom(void);
	void ChangeField(bool bChangeMain);
	void UpdateWave(void);
	void UpdateTime(void);
	void UpdateCountDown(void);

	CField *			m_pField[MAX_FIELD];		//�n�ʃ|�C���^
	CField *			m_pFieldGraund[MAX_FIELD];	//�n�ʃ|�C���^
	CField *			m_pCountDownField;			//�J�E���g�_�E���p�ϐ�
	CField *			m_pMainField;				//���C���̒n��
	LPDIRECT3DTEXTURE9	m_pTexture[FIELD_TEX];		// �e�N�X�`���ւ̃|�C���^
	STATE				m_state;					// ���
	int					m_nCountTime;				// ���Ԃ̉��Z
	int					m_nCountY;					// Y���̃J�E���^�[
	int					m_nCountX;					// X���̃J�E���^�[
	int					m_nCountY2;					// Y���̃J�E���^�[
	int					m_nCountX2;					// X���̃J�E���^�[
	int					m_nCountY3;					// Y���̃J�E���^�[
	int					m_nCountX3;					// X���̃J�E���^�[
	int					m_nCountY4;					// Y���̃J�E���^�[
	int					m_nCountX4;					// X���̃J�E���^�[
	float				m_fCol_a;					// �F�̃��l

	// �n�ʕ`��p�ϐ�
	CFieldShader *m_pFieldShader;					// �n�ʕ`��p�̃V�F�[�_�[
	CFieldRender *m_pFieldRender[FIELD_TYPE_MAX];	// �n�ʕ`��N���X�ւ̃|�C���^

	// �n�ʕ`��p�֐�
	void LoadShader(void);
	void MakeFieldRender(void);
	void ReleaseFieldShader(void);
	void ReleaseFieldRender(void);
	void SetFieldData_None(void);
	void SetFieldData_Ground(void);
	void SetFieldData_Main(void);
	void SetFieldData_CountDown(void);

	void SetFieldVertex_None(void);
	void SetFieldMtxWorld_None(void);
	void SetFieldColor_None(void);

	void SetFieldVertex_Ground(void);
	void SetFieldMtxWorld_Ground(void);
	void SetFieldColor_Ground(void);

	void SetFieldVertex_Main(void);
	void SetFieldMtxWorld_Main(void);
	void SetFieldColor_Main(void);

	void SetFieldVertex_CountDown(void);
	void SetFieldMtxWorld_CountDown(void);
	void SetFieldColor_CountDown(void);
};
#endif
