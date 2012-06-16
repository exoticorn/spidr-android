varying mediump float vOffset;
--
attribute vec2 aPosition;
attribute vec3 aNormalOffset;

uniform vec2 uScale;
uniform vec2 uTranslation;
uniform vec2 uPixelScale;

void main() {
	vec2 basePos = aPosition * uScale + uTranslation;
	vec2 offset = aNormalOffset.xy * aNormalOffset.z * uPixelScale;
	gl_Position = vec4(basePos + offset, 0, 1);
	vOffset = aNormalOffset.z;
}
--
void main() {
	lowp float alpha = 1.0 - smoothstep(0.5, 1.5, abs(vOffset));
	gl_FragColor = vec4(0.8, 1.0, 0.8, 0.6) * sqrt(alpha);
}
