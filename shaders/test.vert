//uniform float myvar;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position *= gl_VertexID;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
    //gl_FrontColor = vec4(0., 0., 0., 1.);

	//if(gl_Position.x == -1.0)
		//gl_FrontColor.r = 1.0;
    //gl_FrontColor.r = 1.0 - (1.0 + gl_Position.x) / 2.0;
    //gl_FrontColor.g = 1.0 - abs(gl_Position.x);
    //gl_FrontColor.b = (1.0 + gl_Position.x) / 2.0;
    
}