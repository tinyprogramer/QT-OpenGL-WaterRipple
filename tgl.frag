#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;


uniform sampler2D screenTexture;
uniform sampler2D texture2;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;

void main()
{
         gl_FragColor =  mix(texture(screenTexture, TexCoord),texture(texture2,TexCoord),0.5);
	//FragColor =  texture(screenTexture, TexCoord);
	//gl_FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoord)), 1.0);
}
