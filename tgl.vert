#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aOffset;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform mat4 tranNormal;
//uniform vec3 spColor;

void main()
{
     gl_Position = vec4(aPos, 1.0);
    //ourColor = aColor;
	//ourColor= spColor;
    TexCoord =  aTexCoord;
	//Normal = mat3(transpose(inverse(model))) * aNormal;
	//Normal=vec3(model*vec4(aNormal,0.0));
	//Normal=aNormal;
	//Normal=mat3(tranNormal)*aNormal;
	//FragPos=vec3(model*vec4(aPos,1.0));
}
