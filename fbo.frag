#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture1;
uniform sampler2D texture23;

void main()
{ 
    //gl_FragColor = texture(screenTexture, TexCoords);
        gl_FragColor = vec4(vec3(1.0 - texture(screenTexture1, TexCoords)), 1.0);
        //FragColor=vec4(1.0,1.0,1.0,1.0);
		
		//FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //FragColor = vec4(average, average, average, 1.0);
}

//const float offset = 1.0 / 30.0;  

//void main()
//{
//    vec2 offsets[9] = vec2[](
//        vec2(-offset,  offset), // 左上
//        vec2( 0.0f,    offset), // 正上
//        vec2( offset,  offset), // 右上
//        vec2(-offset,  0.0f),   // 左
//        vec2( 0.0f,    0.0f),   // 中
//        vec2( offset,  0.0f),   // 右
//        vec2(-offset, -offset), // 左下
//        vec2( 0.0f,   -offset), // 正下
//        vec2( offset, -offset)  // 右下
//    );
//
//    float kernel[9] = float[](
//    1.0 / 16, 2.0 / 16, 1.0 / 16,
//    2.0 / 16, 4.0 / 16, 2.0 / 16,
//    1.0 / 16, 2.0 / 16, 1.0 / 16  
//	);
//
//    vec3 sampleTex[9];
//    for(int i = 0; i < 9; i++)
//    {
//        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
//    }
//    vec3 col = vec3(0.0);
//    for(int i = 0; i < 9; i++)
//        col += sampleTex[i] * kernel[i];
//
//    FragColor = vec4(col, 1.0);
//}
