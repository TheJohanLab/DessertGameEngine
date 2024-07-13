// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_Scale;

uniform mat4 u_VPMatrix;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_Scale;

void main()
{
	v_TexCoord = a_TextureCoord;
	v_Color = a_Color;
	v_TexIndex = a_TextureIndex;
	v_Scale = a_Scale;
	gl_Position = u_VPMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
//in vec2 v_ScreenPos;
in float v_TexIndex;
in float v_Scale;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[32];

void main()
{
	//float dist = 1.0f - distance(v_ScreenPos * 0.8f, vec2(0.0f));
	//dist = clamp(dist, 0.0f, 1.0f);
	//dist = sqrt(dist);
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_Scale) * v_Color;
	//color = v_Color;
		
}