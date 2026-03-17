struct PS_INPUT
{
  float4 Position : SV_Position;
  float2 Texcoord : TEXCOORD0;
};

PS_INPUT main(uint vertexID : SV_VertexID)
{
  PS_INPUT output = (PS_INPUT) 0;
  
  float2 position[3] =
  {
    float2(-1.0f, -1.0f),
    float2(3.0f, -1.0f),
    float2(-1.0f, 3.0f)
  };
  
  output.Position = float4(position[vertexID], 0.0f, 1.0f);
  output.Texcoord = (position[vertexID] + float2(1.0f, 1.0f)) * 0.5f;
  output.Texcoord.y = 1.0f - output.Texcoord.y;
    
  return output;
}
