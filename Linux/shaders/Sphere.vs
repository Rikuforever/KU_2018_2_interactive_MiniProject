#version 130

uniform float pointRadius;  // point size in world space
uniform float pointScale;   // scale to calculate size in pixels
uniform float densityScale;
uniform float densityOffset;

out vec3 fragPos;

void main()
{
    fragPos = gl_Vertex.xyz;
	
	// calculate window-space point size
    vec3 posEye = vec3(gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0));
    float dist = length(posEye);
    gl_PointSize = pointRadius * (pointScale / dist);

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0);

    gl_FrontColor = gl_Color;
}