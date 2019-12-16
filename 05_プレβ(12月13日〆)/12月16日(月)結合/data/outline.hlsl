//*****************************************************************************
//
//     �A�E�g���C���`��V�F�[�_�[[outline.hlsl]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define OUTLINE_WIDTH (0.6f)
#define OUTLINE_COLOR (float4(0.0f,0.0f,0.0f,1.0f))

//*****************************************************************************
//     �\���̂̒�`
//*****************************************************************************
struct OutputVertex
{
	float4 pos    : POSITION0;		// ���_���W
	float4 normal : NORMAL0;		// �@���x�N�g��
	float4 color  : COLOR0;			// ���_�J���[
	float2 uv     : TEXCOORD0;		// �e�N�X�`��UV���W
};

//*****************************************************************************
//     �O���[�o���ϐ��錾
//*****************************************************************************
float4x4 g_MtxView;		// �r���[�}�g���b�N�X
float4x4 g_MtxProj;		// �v���W�F�N�V�����}�g���b�N�X
float4x4 g_MtxWorld;	// ���[���h�}�g���b�N�X

//=============================================================================
//     ���_�V�F�[�_�[
//=============================================================================
OutputVertex Vertex(float3 inLocalPos : POSITION0,
	float3 inLocalNormal : NORMAL0,
	float2 uv : TEXCOORD0)
{
	OutputVertex output = (OutputVertex)0;

	// ���[�J�����_���W�����[���h���W�ɕϊ�
	float4 LocalPos = float4(inLocalPos, 1.0f);
	output.pos = mul(LocalPos, g_MtxWorld);

	// �@���x�N�g�������[���h��Ԃɕϊ�(�A�E�g���C���Ɏg�p)
	output.normal = mul(inLocalNormal, g_MtxWorld);
	output.normal = normalize(output.normal);

	// ���[���h���_���W��@�������Ɋg��
	output.pos.xyz += output.normal.xyz * OUTLINE_WIDTH;

	// ���[���h���_���W���X�N���[�����W�ɕϊ�
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// �F�̐ݒ�(�A�E�g���C���̐F�����)
	output.color = OUTLINE_COLOR;

	// �e�N�X�`�����W�̐ݒ�
	output.uv = uv;

	return output;
}

//=============================================================================
//     �s�N�Z���V�F�[�_�[
//=============================================================================
float4 Pixel(float4 color : COLOR0) : COLOR0
{
	return color;	// ���_�̐F�����̂܂܏o��
}

//=============================================================================
//     �e�N�j�b�N�錾
//=============================================================================
technique Render
{
	pass Pass0
	{
		VertexShader = compile vs_3_0 Vertex();
		PixelShader = compile ps_3_0 Pixel();
	}
};