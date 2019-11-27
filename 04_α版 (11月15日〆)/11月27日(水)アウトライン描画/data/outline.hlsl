//*****************************************************************************
//
//     アウトライン描画シェーダー[outline.hlsl]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define OUTLINE_WIDTH (0.6f)
#define OUTLINE_COLOR (float4(0.0f,0.0f,0.0f,1.0f))

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
float4x4 g_MtxWorld;	// ワールドマトリックス

//=============================================================================
//     頂点シェーダー
//=============================================================================
OutputVertex Vertex(float3 inLocalPos : POSITION0,
	float3 inLocalNormal : NORMAL0,
	float2 uv : TEXCOORD0)
{
	OutputVertex output = (OutputVertex)0;

	// ローカル頂点座標をワールド座標に変換
	float4 LocalPos = float4(inLocalPos, 1.0f);
	output.pos = mul(LocalPos, g_MtxWorld);

	// 法線ベクトルをワールド空間に変換(アウトラインに使用)
	output.normal = mul(inLocalNormal, g_MtxWorld);
	output.normal = normalize(output.normal);

	// ワールド頂点座標を法線方向に拡大
	output.pos.xyz += output.normal.xyz * OUTLINE_WIDTH;

	// ワールド頂点座標をスクリーン座標に変換
	output.pos = mul(output.pos, g_MtxView);
	output.pos = mul(output.pos, g_MtxProj);

	// 色の設定(アウトラインの色を入力)
	output.color = OUTLINE_COLOR;

	// テクスチャ座標の設定
	output.uv = uv;

	return output;
}

//=============================================================================
//     ピクセルシェーダー
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
		PixelShader = compile ps_3_0 Pixel();
	}
};