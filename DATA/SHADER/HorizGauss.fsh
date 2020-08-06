#version 330 core

uniform	sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

const float coef[] = float[] (
0.106595,	0.140367,	0.165569,	0.174938,	0.165569,	0.140367,	0.106595
);

void main (void)
{
	outputColor = vec4(0.0, 0.0, 0.0, 0.0);

	outputColor += textureOffset(texture_0, texCoord, ivec2(-3, 0)) * coef[0];
	outputColor += textureOffset(texture_0, texCoord, ivec2(-2, 0)) * coef[1];
	outputColor += textureOffset(texture_0, texCoord, ivec2(-1, 0)) * coef[2];
	outputColor += textureOffset(texture_0, texCoord, ivec2(+0, 0)) * coef[3];
	outputColor += textureOffset(texture_0, texCoord, ivec2(+1, 0)) * coef[4];
	outputColor += textureOffset(texture_0, texCoord, ivec2(+2, 0)) * coef[5];
	outputColor += textureOffset(texture_0, texCoord, ivec2(+3, 0)) * coef[6];
}
