#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D prevFrame;
uniform float time;

float hash(vec2 p) {
 return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{
    vec2 size  = textureSize(prevFrame, 0);
    vec2 texel = 1.0 / size;

    vec2 pixel = floor(fragTexCoord * size);

    float rnd = hash(pixel);

    float v0 = mix(120.0, 240.0, rnd);
    float g  = -1000.0;

    float dy = v0 * time - 0.5 * g;

    vec2 srcPixel = vec2(pixel.x, pixel.y + dy);
    vec2 uv = srcPixel * texel;

    finalColor = texture(prevFrame, uv);
    finalColor.a = 1.0;
}
