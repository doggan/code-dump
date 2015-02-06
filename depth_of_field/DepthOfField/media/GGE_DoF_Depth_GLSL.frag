// GGE Depth of Field

uniform float nearBlurP0;
uniform float nearBlurP1;
uniform float farBlurP0;
uniform float farBlurP1;

// View-space depth.
varying float depth;

void main()
{
    float f;

    // Scale depth value into 0-1 range, between our camera and farBlurP1.
    // Note that anything beyond farBlurP1 gets clamped to 1.0, meaning full blur.
    f = depth / farBlurP1;
    
    gl_FragColor = vec4(f, f, f, 1.0);
}