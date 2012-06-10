--
attribute vec2 aPosition;

uniform vec2 uScale;
uniform vec2 uTranslation;

void main() {
	gl_Position = vec4(aPosition * uScale + uTranslation, 0, 1);
}
--
void main() {
	gl_FragColor = vec4(1, 1, 1, 1);
}
