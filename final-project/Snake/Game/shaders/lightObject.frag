#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D image;
uniform vec3 sprite_color;
uniform int type;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 color = texture(image, TexCoord).rgb;

	vec3 ambient = 0.5 * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = 0.8 * diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor;
	
	//combine
	vec3 result = (ambient + diffuse + specular) * sprite_color;

	if (type == 1) {
		FragColor = vec4(result, 1.0);
	} else {
		result = (ambient + diffuse + specular) * color;
		FragColor = vec4(result, 1.0);
	}	

	//vec3 result = (ambient + diffuse + specular) * color;
	//FragColor = vec4(result, 1.0);
}