uniform sampler2D textureImage;
varying vec4 coords;

void main(){
  gl_FragColor=texture2D(textureImage, coords);
}
