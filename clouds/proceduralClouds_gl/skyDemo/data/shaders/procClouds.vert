varying float horizonInterp;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
    
    float startY = -500.0;
    float stopY = 800.0;
    horizonInterp = (gl_Vertex.z + startY) / (stopY + startY);
} 