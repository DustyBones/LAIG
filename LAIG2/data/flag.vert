uniform int time;
uniform int wind;
varying vec4 coords;
const float PI=3.1415;


void main(){
	vec4 temp = vec4(gl_Vertex.x, gl_Vertex.y + sin(gl_Vertex.x*wind*PI + time*0.001*wind)*0.05, gl_Vertex.z, 1.0);
	gl_Position = gl_ModelViewProjectionMatrix * temp;
	gl_FrontColor = vec4(0, 0, 0 ,0.0);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	coords=temp;
}
