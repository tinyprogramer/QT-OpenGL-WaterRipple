

precision highp float;

attribute vec2 vertex;
//uniform vec2 topLeft;
//uniform vec2 bottomRight;
//uniform vec2 containerRatio;
varying vec2 ripplesCoord;
varying vec2 backgroundCoord;
void main() {
	//backgroundCoord = mix(topLeft, bottomRight, vertex * 0.5 + 0.5);
	//backgroundCoord.y = 1.0 - backgroundCoord.y;
	//ripplesCoord = vec2(vertex.x, -vertex.y) * containerRatio * 0.5 + 0.5;
	backgroundCoord=vertex*0.5+0.5;
	ripplesCoord=backgroundCoord;
	//gl_Position = vec4(vertex.x, -vertex.y, 0.0, 1.0);
	gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);
}