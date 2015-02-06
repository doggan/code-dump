// GGE Depth of Field

// View-space depth.
varying float depth;

void main()
{
	vec4 viewPos = gl_ModelViewMatrix * gl_Vertex;
	depth = -viewPos.z;

	gl_Position = ftransform(); 
}
