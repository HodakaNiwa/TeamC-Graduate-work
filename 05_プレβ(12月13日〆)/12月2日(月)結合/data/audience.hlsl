//*****************************************************************************
//
//     �ϋq�`��V�F�[�_�[[instancing.hlsl]
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
float4x4 g_MtxLocal;	// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X
texture g_Texture;		// �e�N�X�`��
float3 g_LightDir;		// ���C�g�̌���
float4 g_LightDiffuse;	// ���C�g�̊g�U��
float4 g_LightAmbient;	// ���C�g�̊���

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
	output.pos = mul(LocalPos, g_MtxLocal);
	output.pos = mul(output.pos, mtxWorld);
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// �@���x�N�g�������[���h��Ԃɕϊ�(���C�e�B���O�̌v�Z�Ɏg�p)
	output.normal = mul(inLocalNormal, mtxWorld);
	output.normal = normalize(output.normal);

	// �F�̐ݒ�(�g�U���̌v�Z����K�p)
	float Power = saturate(dot(output.normal, g_LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	output.color = ((g_LightDiffuse * color) * Power) + g_LightAmbient;
	output.color.a = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	output.uv = uv;

	return output;
}

//=============================================================================
//     ���_�V�F�[�_�[(���Ǝ�p)
//=============================================================================
OutputVertex Vertex2(
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
	output.pos = mul(LocalPos, g_MtxLocal);
	output.pos = mul(output.pos, mtxWorld);
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// �@���x�N�g�������[���h��Ԃɕϊ�(���C�e�B���O�̌v�Z�Ɏg�p)
	output.normal = mul(inLocalNormal, mtxWorld);
	output.normal = normalize(output.normal);

	// �F�̐ݒ�(�g�U���̌v�Z����K�p)
	float Power = saturate(dot(output.normal, g_LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	float4 color1 = ((g_LightDiffuse * color) * Power) + g_LightAmbient;

	Power = saturate(dot(output.normal, -g_LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	float4 color2 = ((g_LightDiffuse * color) * Power) + g_LightAmbient;

	float3 LightDir = g_LightDir;
	LightDir.x *= -1.0f;
	Power = saturate(dot(output.normal, LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	float4 color3 = ((g_LightDiffuse * color) * Power) + g_LightAmbient;

	LightDir = g_LightDir;
	LightDir.z *= -1.0f;
	Power = saturate(dot(output.normal, LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	float4 color4 = ((g_LightDiffuse * color) * Power) + g_LightAmbient;

	output.color = (color1 + color2 + color3 + color4) / 2.0f;
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
	pass Pass2
	{
		VertexShader = compile vs_3_0 Vertex2();
		PixelShader = compile ps_3_0 Pixel();
	}
};