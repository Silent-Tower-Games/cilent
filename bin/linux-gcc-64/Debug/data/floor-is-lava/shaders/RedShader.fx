Texture2D SpriteTexture;
float magnitude;

sampler2D SpriteTextureSampler = sampler_state
{
	Texture = <SpriteTexture>;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 TextureCoordinates : TEXCOORD0;
};

float4 MainPS(VertexShaderOutput input) : COLOR
{
	float4 color = tex2D(SpriteTextureSampler, input.TextureCoordinates) * input.Color;
	
	color.g *= magnitude;
	color.b *= magnitude;
	
	return color;
}

technique SpriteDrawing
{
	pass P0
	{
		PixelShader = compile ps_3_0 MainPS();
	}
};
