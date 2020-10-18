precision highp float;

uniform sampler2D samplerBackground;
uniform sampler2D samplerRipples;
uniform vec2 delta;

uniform float perturbance;
varying vec2 ripplesCoord;
varying vec2 backgroundCoord;

void main() {
	float height = texture2D(samplerRipples, ripplesCoord).r;
	float heightX = texture2D(samplerRipples, vec2(ripplesCoord.x + delta.x, ripplesCoord.y)).r;
	float heightY = texture2D(samplerRipples, vec2(ripplesCoord.x, ripplesCoord.y + delta.y)).r;
	vec3 dx = vec3(delta.x, heightX - height, 0.0);
	vec3 dy = vec3(0.0, heightY - height, delta.y);
	vec2 offset = -normalize(cross(dy, dx)).xz;
	//offset=vec2(0.1,0.1);
	float specular = pow(max(0.0, dot(offset, normalize(vec2(-0.6, 1.0)))), 4.0);
	gl_FragColor = texture2D(samplerBackground, backgroundCoord + offset * perturbance) + specular;
	//gl_FragColor = texture2D(samplerBackground, backgroundCoord+delta) + specular;
	//gl_FragColor=texture2D(samplerRipples,ripplesCoord);
	//gl_FragColor=mix(texture(samplerBackground, backgroundCoord), texture(samplerRipples, ripplesCoord), 0.5);
}