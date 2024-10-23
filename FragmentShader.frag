#version 460
 
in vec4 gl_FragCoord;

uniform int x_pxCoords;
uniform int y_pxCoords;
uniform vec4 inputColor;

out vec4 myOutputColor;

void main()
{
    if(gl_FragCoord.x == x_pxCoords && gl_FragCoord.y == y_pxCoords)
    {
        myOutputColor = inputColor;
    }
} 