precision highp float;

uniform sampler2D texture;
uniform vec2 delta;

varying vec2 coord;

void main() {
vec4 info = texture2D(texture, coord);

//vec2 dx = vec2(delta.x, 0.0);
//vec2 dy = vec2(0.0, delta.y);
vec2 dx = vec2(0.0025, 0.0);
vec2 dy = vec2(0.0, 0.0025);

float average = (
	texture2D(texture, coord - dx).r +
	texture2D(texture, coord - dy).r +
	texture2D(texture, coord + dx).r +
	texture2D(texture, coord + dy).r
) * 0.25;

info.g += (average - info.r) * 2.0;
info.g *= 0.995;
info.r += info.g;

gl_FragColor = info;
}
