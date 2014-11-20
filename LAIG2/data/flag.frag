uniform sampler2D textureImage;
varying vec4 coords;

void main(){
  gl_FragColor=texture2D(textureImage, gl_TexCoord[0].st)+ coords.y*(0.7, 0.7, 0.7, 1.0);
}
