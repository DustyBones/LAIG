uniform int time;
varying vec4 coords;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+vec4(0.0, sin(gl_MultiTexCoord0.s+time), 0.0, 1.0));
	coords=gl_Position;
}
