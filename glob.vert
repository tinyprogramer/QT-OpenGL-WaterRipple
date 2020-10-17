//#version 330 core

attribute vec2 vertex;
varying vec2 coord;
void main() {
	coord = vertex * 0.5 + 0.5;
	gl_Position = vec4(vertex, 0.0, 1.0);
}
