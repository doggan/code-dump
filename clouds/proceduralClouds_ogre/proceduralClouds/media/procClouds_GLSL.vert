// GGE Procedural Clouds Module - vertex shader

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}