float4 main(float2 inPos : POSITION) : VS_POSITION
{
    return float4(inPos, 0, 1);
}