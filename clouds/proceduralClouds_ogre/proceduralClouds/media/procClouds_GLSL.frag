// GGE Procedural Clouds Module - fragment shader

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
uniform int cloudMode;

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
    
    // White clouds, no lighting.
    if (cloudMode == 0)
    {
        tex += texture2D(tex_skyColor, gl_TexCoord[0]);
        
        gl_FragColor = vec4(tex, 1.0);
    }
    // Basic 'lighting'. Needs to be improved.
    else if (cloudMode == 1)
    {
        float density = 0.0;
        
        for (int i = 0; i < 64; i++)
            density += .1 * tex.r;
        
        float light = 1.0 / exp(density * .4);
        vec4 cloudColor = vec4(light, light, light, tex.r * 5.0);
        vec4 skyColor = texture2D(tex_skyColor, gl_TexCoord[0]);
        
        gl_FragColor = vec4(cloudColor.a * cloudColor.r + skyColor.r * skyColor.r,
                            cloudColor.a * cloudColor.g + skyColor.g * skyColor.g,
                            cloudColor.a * cloudColor.b + skyColor.b * skyColor.b,
                            1.0);
    }
    // Creepy black sky w/ red clouds.
    else if (cloudMode == 2)
    {
        gl_FragColor = vec4(1.0 * tex.r, 0.0, 0.0, 1.0);
    }
    // Render only clouds (no sky color). Useful for parallax effectss.
    else if (cloudMode == 3)
    {
        gl_FragColor = vec4(tex, tex.r * 2.0);
    }
    // Reddish clouds with no sky.
    else if (cloudMode == 4)
    {
        gl_FragColor = vec4(0.0, 1.0 * tex.r, 0.0, tex.r * 2.0);
    }
}