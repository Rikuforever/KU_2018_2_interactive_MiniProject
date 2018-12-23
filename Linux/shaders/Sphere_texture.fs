#version 130

uniform sampler2D tex; //set 2D Texture@Fragment Shader

void main()
{
    
	vec2 texCoord = gl_TexCoord[0].st;
	vec3 color = texture2D(tex,texCoord).rgb;
	if(length(color)<0.01f) discard;
	gl_FragColor = vec4(color,1.0f);

}