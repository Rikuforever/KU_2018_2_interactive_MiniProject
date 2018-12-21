#version 130

uniform vec3 cameraPos;
uniform samplerCube skybox;

in vec3 fragPos;

void main()
{
    const vec3 lightDir = vec3(0.577, 0.577, 0.577);

    // calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_TexCoord[0].xy*vec2(2.0, 2.0) - vec2(1.0, 1.0);
    float mag = dot(N.xy, N.xy);

    if (mag > 1.0) discard;   // kill pixels outside circle

    N.z = sqrt(1.0-mag);

    // calculate lighting
    // float diffuse = max(0.0, dot(lightDir, N));
	// gl_FragColor = gl_Color * diffuse;

	// Reflect & Refract
	vec3 I = normalize(fragPos - cameraPos);

	vec3 reflectRay = reflect(I, normalize(N));
	vec3 refractRay = refract(I, normalize(N), 0.65);

    gl_FragColor = vec4(textureCube(skybox, refractRay).rgb, 1.0);
}