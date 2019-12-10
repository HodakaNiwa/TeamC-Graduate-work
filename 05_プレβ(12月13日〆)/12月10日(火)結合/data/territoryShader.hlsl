//*****************************************************************************
//
//     �e���g���[�`��V�F�[�_�[[territoryShader.hlsl]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
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
texture g_Texture;		// �e�N�X�`��
float3 g_LightDir0;		// ���C�g�̌���(1�{��)
float3 g_LightDir1;		// ���C�g�̌���(2�{��)
float3 g_LightDir2;		// ���C�g�̌���(3�{��)
float4 g_LightDiffuse0;	// ���C�g�̊g�U��(1�{��)
float4 g_LightDiffuse1;	// ���C�g�̊g�U��(2�{��)
float4 g_LightDiffuse2;	// ���C�g�̊g�U��(3�{��)

sampler g_TexSampler = sampler_state
{
	texture = (g_Texture);
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//=============================================================================
//     ���_�V�F�[�_�[
//=============================================================================
OutputVertex Vertex(
	float3 inLocalPos : POSITION0,
	float3 inLocalNormal : NORMAL0,
	float4 color : COLOR0,
	float2 uv : TEXCOORD0,
	float4 World1 : TEXCOORD1,
	float4 World2 : TEXCOORD2,
	float4 World3 : TEXCOORD3,
	float4 World4 : TEXCOORD4)
{
	OutputVertex output = (OutputVertex)0;

	// ���[���h�}�g���b�N�X������
	float4x4 mtxWorld;
	mtxWorld._11 = World1.x; mtxWorld._12 = World1.y; mtxWorld._13 = World1.z; mtxWorld._14 = World1.w;// 1�s��
	mtxWorld._21 = World2.x; mtxWorld._22 = World2.y; mtxWorld._23 = World2.z; mtxWorld._24 = World2.w;// 2�s��
	mtxWorld._31 = World3.x; mtxWorld._32 = World3.y; mtxWorld._33 = World3.z; mtxWorld._34 = World3.w;// 3�s��
	mtxWorld._41 = World4.x; mtxWorld._42 = World4.y; mtxWorld._43 = World4.z; mtxWorld._44 = World4.w;// 4�s��

	// ���[�J�����_���W���X�N���[�����W�ɕϊ�
	float4 LocalPos = float4(inLocalPos, 1.0f);
	output.pos = mul(LocalPos, mtxWorld);
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// �@���x�N�g�������[���h��Ԃɕϊ�(���C�e�B���O�̌v�Z�Ɏg�p)
	output.normal = mul(inLocalNormal, mtxWorld);
	output.normal = normalize(output.normal);

	// �F�̐ݒ�(�g�U���̌v�Z����K�p)
	float Power0 = saturate(dot(output.normal, g_LightDir0));
	Power0 = clamp(Power0, 0.0, 1.0f);
	float Power1 = saturate(dot(output.normal, g_LightDir1));
	Power1 = clamp(Power1, 0.0, 1.0f);
	float Power2 = saturate(dot(output.normal, g_LightDir2));
	Power2 = clamp(Power2, 0.0, 1.0f);
	output.color = ((g_LightDiffuse0 * color) * Power0) + ((g_LightDiffuse1 * color) * Power1) + ((g_LightDiffuse2 * color) * Power2);
	output.color.a = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	output.uv = uv;

	return output;
}

//=============================================================================
//     �s�N�Z���V�F�[�_�[(�e�N�X�`���L��)
//=============================================================================
float4 TexPixel(float4 color : COLOR0, float2 uv : TEXCOORD0) : COLOR0
{
	return tex2D(g_TexSampler, uv) * color;	// �e�N�X�`���̐F���o��
}

//=============================================================================
//     �s�N�Z���V�F�[�_�[(�e�N�X�`������)
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
		PixelShader = compile ps_3_0 TexPixel();
	}
	pass Pass1
	{
		VertexShader = compile vs_3_0 Vertex();
		PixelShader = compile ps_3_0 Pixel();
	}
};