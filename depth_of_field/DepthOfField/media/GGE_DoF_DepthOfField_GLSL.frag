// GGE Depth of Field

uniform sampler2D scene;				// full resolution image
uniform sampler2D depth;				// full resolution image with depth values
uniform sampler2D blur;					// downsampled and blurred image

uniform float nearBlurP0;
uniform float nearBlurP1;
uniform float farBlurP0;
uniform float farBlurP1;

uniform int useNearBlur;    // Treat as bool.
uniform int useFarBlur;     // Treat as bool.

void main()
{
    // "De-code" our depth value into world units.
    float depthVal = texture2D(depth, gl_TexCoord[0].st).r * farBlurP1;
    
    // Determine the 'region' this depth value falls into:
    // 0 = nearBlurP0 <> nearBlurP1 = blur
    // 1 = farBlurP0 <> farBlurP1 = blur
    // 2 = all the other depths = no blur applied
    int region;
    
    if (depthVal >= nearBlurP0 && depthVal <= nearBlurP1)
        region = 0;
    else if (depthVal >= farBlurP0 && depthVal <= farBlurP1)
        region = 1;
    else
        region = 2;

    vec4 sceneVal = texture2D(scene, gl_TexCoord[0].st);
    vec4 blurVal = texture2D(blur, gl_TexCoord[0].st);
    
    // Between the near blur planes, apply LERPed blur.
    if (region == 0)
    {
        float blurAmnt;
        
        if (useNearBlur)
            blurAmnt = (nearBlurP1 - depthVal) / (nearBlurP1 - nearBlurP0);
        else
            blurAmnt = 0.0;
            
        float normAmnt = 1.0 - blurAmnt;
        gl_FragColor = (blurAmnt * blurVal + normAmnt * sceneVal);
    }
    // Between the far blur planes (or beyond the farBlurP1), applyed LERPed blur.
    else if (region == 1)
    {
        float blurAmnt;
        
        if (useFarBlur)
            blurAmnt = (depthVal - farBlurP0) / (farBlurP1 - farBlurP0);
        else
            blurAmnt = 0.0;
            
        float normAmnt = 1.0 - blurAmnt;
        gl_FragColor = (blurAmnt * blurVal + normAmnt * sceneVal);
    }
    // All other areas, no blur.
    else
    {
        gl_FragColor = sceneVal;
    }
}