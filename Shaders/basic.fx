cbuffer Data : register(b0)
{
    row_major matrix World;
    row_major matrix VP;
}

struct VSInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput VS(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position, 0.0F, 1.0F);
    output.position = mul(output.position, World);
    output.position = mul(output.position, VP);

    output.uv = input.uv;

    return output;
}

float4 PS(VSOutput input) : SV_TARGET
{
    float borderThickness = 0.45F;

    float2 distToCenter = abs(input.uv - 0.5F);
    float maxDist = 0.5F;

    float alpha = step(maxDist - borderThickness, maxDist - distToCenter.x) * step(maxDist - borderThickness, maxDist - distToCenter.y);
    float4 borderColor = float4(0, 0, 0, 1);
    float4 fillColor = float4(1, 1, 0, 1);

    return lerp(borderColor, fillColor, alpha);
}