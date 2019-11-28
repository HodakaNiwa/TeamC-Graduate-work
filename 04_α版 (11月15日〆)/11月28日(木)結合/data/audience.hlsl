//*****************************************************************************
//
//     観客描画シェーダー[instancing.hlsl]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
//*****************************************************************************
//     構造体の定義
//*****************************************************************************
struct OutputVertex
{
	float4 pos    : POSITION0;		// 頂点座標
	float4 normal : NORMAL0;		// 法線ベクトル
	float4 color  : COLOR0;			// 頂点カラー
	float2 uv     : TEXCOORD0;		// テクスチャUV座標
};

//*****************************************************************************
//     グローバル変数宣言
//*****************************************************************************
float4x4 g_MtxView;		// ビューマトリックス
float4x4 g_MtxProj;		// プロジェクションマトリックス
float4x4 g_MtxLocal;	// ローカル座標におけるワールドマトリックス
texture g_Texture;		// テクスチャ
float3 g_LightDir;		// ライトの向き
float4 g_LightDiffuse;	// ライトの拡散光
float4 g_LightAmbient;	// ライトの環境光

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
//     頂点シェーダー
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

	// ワールドマトリックスを結合
	float4x4 mtxWorld;
	mtxWorld._11 = World1.x; mtxWorld._12 = World1.y; mtxWorld._13 = World1.z; mtxWorld._14 = World1.w;// 1行目
	mtxWorld._21 = World2.x; mtxWorld._22 = World2.y; mtxWorld._23 = World2.z; mtxWorld._24 = World2.w;// 2行目
	mtxWorld._31 = World3.x; mtxWorld._32 = World3.y; mtxWorld._33 = World3.z; mtxWorld._34 = World3.w;// 3行目
	mtxWorld._41 = World4.x; mtxWorld._42 = World4.y; mtxWorld._43 = World4.z; mtxWorld._44 = World4.w;// 4行目

	// ローカル頂点座標をスクリーン座標に変換
	float4 LocalPos = float4(inLocalPos, 1.0f);
	output.pos = mul(LocalPos, g_MtxLocal);
	output.pos = mul(output.pos, mtxWorld);
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// 法線ベクトルをワールド空間に変換(ライティングの計算に使用)
	output.normal = mul(inLocalNormal, mtxWorld);
	output.normal = normalize(output.normal);

	// 色の設定(拡散光の計算式を適用)
	float Power = saturate(dot(output.normal, g_LightDir));
	Power = clamp(Power, 0.0, 1.0f);
	output.color = ((g_LightDiffuse * color) * Power) + g_LightAmbient;
	output.color.a = 1.0f;

	// テクスチャ座標の設定
	output.uv = uv;

	return output;
}

//=============================================================================
//     頂点シェーダー(頭と手用)
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

	// ワールドマトリックスを結合
	float4x4 mtxWorld;
	mtxWorld._11 = World1.x; mtxWorld._12 = World1.y; mtxWorld._13 = World1.z; mtxWorld._14 = World1.w;// 1行目
	mtxWorld._21 = World2.x; mtxWorld._22 = World2.y; mtxWorld._23 = World2.z; mtxWorld._24 = World2.w;// 2行目
	mtxWorld._31 = World3.x; mtxWorld._32 = World3.y; mtxWorld._33 = World3.z; mtxWorld._34 = World3.w;// 3行目
	mtxWorld._41 = World4.x; mtxWorld._42 = World4.y; mtxWorld._43 = World4.z; mtxWorld._44 = World4.w;// 4行目

	// ローカル頂点座標をスクリーン座標に変換
	float4 LocalPos = float4(inLocalPos, 1.0f);
	output.pos = mul(LocalPos, g_MtxLocal);
	output.pos = mul(output.pos, mtxWorld);
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// 法線ベクトルをワールド空間に変換(ライティングの計算に使用)
	output.normal = mul(inLocalNormal, mtxWorld);
	output.normal = normalize(output.normal);

	// 色の設定(拡散光の計算式を適用)
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

	// テクスチャ座標の設定
	output.uv = uv;

	return output;
}

//=============================================================================
//     ピクセルシェーダー(テクスチャ有り)
//=============================================================================
float4 TexPixel(float4 color : COLOR0, float2 uv : TEXCOORD0) : COLOR0
{
	return tex2D(g_TexSampler, uv) * color;	// テクスチャの色を出力
}

//=============================================================================
//     ピクセルシェーダー(テクスチャ無し)
//=============================================================================
float4 Pixel(float4 color : COLOR0) : COLOR0
{
	return color;	// 頂点の色をそのまま出力
}

//=============================================================================
//     テクニック宣言
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