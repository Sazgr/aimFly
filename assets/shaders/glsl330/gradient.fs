#version 330

out vec4 finalColor;

uniform vec4 startColor;
uniform vec4 endColor;
uniform int vertical;
uniform vec4 rectBounds; // x, y, width, height

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec2 relativePos = gl_FragCoord.xy - rectBounds.xy;
    float t = vertical == 1 ? relativePos.y / rectBounds.w : relativePos.x / rectBounds.z;
    
    vec4 col = mix(startColor, endColor, t);

    float noise = rand(gl_FragCoord.xy) * (1.0/255.0);
    finalColor = vec4(col.rgb + noise, col.a);
}