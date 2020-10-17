precision highp float;

const float PI = 3.141592653589793;
uniform sampler2D texture;
uniform vec2 center;
uniform float radius;
uniform float strength;

varying vec2 coord;

void main() {
	vec4 info = texture2D(texture, coord);

	float drop = max(0.0, 1.0 - length(center * 0.5 + 0.5 - coord) / radius);
	//float drop = max(0.0, 1.0 - length(center  - coord) / radius);
	drop = 0.5 - cos(drop * PI) * 0.5;
	//'if(drop>0.0)', 
	//	'drop=0.5;',
		
	info.r += drop * strength;

	//gl_FragColor=texture2D(texture,coord);
	gl_FragColor = info;
}