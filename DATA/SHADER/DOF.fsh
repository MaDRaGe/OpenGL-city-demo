#version 330 core

uniform	sampler2D texture_0;
uniform	sampler2D texture_1;
uniform	sampler2D texture_2;
uniform float zNear;
uniform float zFar;

in vec2 texCoord;

out vec4 outputColor;

float linearizeDepth()
{
  // camera z near
  float n = zNear;
  // camera z far
  float f = 500;
  
  float z = texture(texture_2, texCoord).x;
  return (2.0 * n) / (f + n - z * (f - n));	
}
float getFactor(float _dist, float _near, float _far)
{
	if (_dist > _far)
	{
		return 1;
	}
	if (_dist < _near)
	{
		return 0;
	}
	return (1 - (_far - _dist) / (_far - _near));
}

void main (void)
{
	vec4 originalImage = texture (texture_0, texCoord);
	
	vec4 bluredImage = texture (texture_1, texCoord);

	float d = linearizeDepth();
	vec4 depthImage = vec4(d, d, d, 1.0);
	
	outputColor = bluredImage;
	//outputColor = mix(originalImage, bluredImage, getFactor(d, 1, 500));
}



