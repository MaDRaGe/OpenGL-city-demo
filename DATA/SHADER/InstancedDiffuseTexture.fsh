#version 330 core

uniform sampler2D texture_0;

uniform vec4 mAmbient;
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform float mShininess;

uniform vec4 lAmbient;
uniform vec4 lDiffuse;
uniform vec4 lSpecular;
uniform vec4 lPosition;

uniform vec3 fogColor;
uniform float fogNear;
uniform float fogFar;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 outputColor;

float getFogFactor(float _dist, float _fogNear, float _fogFar)
{
	if (_dist > _fogFar)
	{
		return 1;
	}
	if (_dist < _fogNear)
	{
		return 0;
	}
	return (1 - (_fogFar - _dist) / (_fogFar - _fogNear));
}


void main (void)
{
	vec3 n_Normal = normalize(normal);
	vec3 n_ToEye = normalize (vec3(0,0,0) - position);
	vec3 n_ToLight = normalize(vec3(lPosition));
	vec3 n_Reflect = normalize(reflect(-n_ToLight,n_Normal));
	
	vec4 ambient = mAmbient * lAmbient;  
	vec4 diffuse = mDiffuse * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);
	vec4 specular = mSpecular * lSpecular * pow(max(dot(n_ToEye, n_Reflect), 0.0f), mShininess);

	vec3 color = vec3(ambient + diffuse + specular); 
	float alpha = mDiffuse.a;

	vec4 texColor = texture(texture_0, texCoord);
	color = color * vec3(texColor);	
	
	
	float fogDistance = gl_FragCoord.z / gl_FragCoord.w;
	
	color = mix(color, fogColor, getFogFactor(fogDistance, fogNear, fogFar));
	
	outputColor = vec4(color, alpha);
}
