uniform sampler2D tex_skyColor;
uniform sampler2D tex_octave1;
uniform sampler2D tex_octave2;
uniform sampler2D tex_octave3;
uniform sampler2D tex_octave4;
uniform float theTime;
uniform float cloudCover;
uniform float cloudSharpness;
uniform float scrollSpeedU;
uniform float scrollSpeedV;

varying float horizonInterp;

// Remove cloud cover (creates 'no-clouds' areas).
void removeCloudCover(inout vec3 tex)
{
    tex = max(tex - (1.0 - cloudCover), 0.0);
}

// Exponentiate to get more realism.
void makeRealistic(inout vec3 tex)
{
    tex = 1.0 - pow((1.0 - cloudSharpness), tex * 255.0);
}

void main()
{   
    // The higher octave textures should be repeated more often, and
    //  be given less overall weight. They just add fine details.
    float uCoordOffset = gl_TexCoord[0].s + scrollSpeedU * theTime;
    float vCoordOffset = gl_TexCoord[0].t + scrollSpeedV * theTime;
    
    vec3 tex = vec3(0.0, 0.0, 0.0);
    
    // This is a basic averaging filter to smooth out the texture.
    vec3 tex1 = texture2D(tex_octave1, vec2(uCoordOffset + 0.1, vCoordOffset)) / 2.0;
    tex1 += texture2D(tex_octave1, vec2(uCoordOffset + 0.1, vCoordOffset + 0.1)) / 2.0;
    tex1 += texture2D(tex_octave1, vec2(uCoordOffset, vCoordOffset + 0.1)) / 2.0;
    tex1 *= 0.25;
        
    vec3 tex2 = texture2D(tex_octave2, vec2(uCoordOffset + 0.1, vCoordOffset) * 2.0) / 4.0;
    tex2 += texture2D(tex_octave2, vec2(uCoordOffset + 0.1, vCoordOffset + 0.1) * 2.0) / 4.0;
    tex2 += texture2D(tex_octave2, vec2(uCoordOffset, vCoordOffset + 0.1) * 2.0) / 4.0;
    tex2 *= 0.25;
        
    vec3 tex3 = texture2D(tex_octave3, vec2(uCoordOffset + 0.1, vCoordOffset) * 4.0) / 8.0;
    tex3 += texture2D(tex_octave3, vec2(uCoordOffset + 0.1, vCoordOffset + 0.1) * 4.0) / 8.0;
    tex3 += texture2D(tex_octave3, vec2(uCoordOffset, vCoordOffset + 0.1) * 4.0) / 8.0;
    tex3 *= 0.25;
        
    vec3 tex4 = texture2D(tex_octave4, vec2(uCoordOffset + 0.1, vCoordOffset) * 8.0) / 16.0;
    tex4 += texture2D(tex_octave4, vec2(uCoordOffset + 0.1, vCoordOffset + 0.1) * 8.0) / 16.0;
    tex4 += texture2D(tex_octave4, vec2(uCoordOffset, vCoordOffset + 0.1) * 8.0) / 16.0;
    tex4 *= 0.25;
        
    tex = tex1 + tex2 + tex3 + tex4;
    
    removeCloudCover(tex);
    makeRealistic(tex);
    
    // TODO: Additive. Should we use something else? Modular? etc?
    tex += texture2D(tex_skyColor, gl_TexCoord[0]);
    
    vec3 horizonColor = vec3(1.0, 0.0, 0.0);
    if (horizonInterp >= 0.0 && horizonInterp < 1.0)
    {
        tex += horizonColor * (1.0 - horizonInterp);
    }
    
    gl_FragColor = vec4(tex, 1.0);
}