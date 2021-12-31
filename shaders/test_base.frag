#version $version core

in vec4 out_color;
in vec2 out_tex_coord;
in float out_texture_id;
in float out_is_clicked;
in float out_type_id;

out vec4 frag_color;


// one texture only as uniform
uniform sampler2D textures[$slots];

uniform vec4 light_color;
uniform vec3 light_pos;


vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = light_pos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * light_color;
}

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * light_color;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(light_pos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * light_color;
}























void main()
{
  // if type is maps, use texture
  int texture_index = int(out_texture_id);

  // entity or tile:
  if(out_type_id == 1.0 || out_type_id == 0.0)
  {
    frag_color = texture(textures[texture_index], out_tex_coord);
    frag_color.r = (1-out_is_clicked)*frag_color.r;
  } 
  // text:
  else if (out_type_id == 3.0)
  {
    frag_color = vec4(out_color.rgb, texture(textures[texture_index], out_tex_coord).r);
  }
  // debug lines or menu:
    else if (out_type_id == 4.0 || out_type_id == 2.0)
  {
    frag_color = out_color.rgba;
  }
  // buttons
   else if (out_type_id == 6.0)
  {
    frag_color = out_color.rgba;
    frag_color.r = (1-out_is_clicked)*frag_color.r;
  }
  // models and light
  else if (out_type_id == 8.0)
  {
    frag_color = direcLight();
  }



  //frag_color = texture(textures[texture_index], out_tex_coord);
  // frag_color.r = (1-out_is_clicked)*frag_color.r; // for clicking 
  // frag_color.g = 0.2f;
  // 1,1,1,1 for white
  // 0,0,0,1 for black
  // frag_color = vec4((texture_index/4.0f), (texture_index/4.0f), (texture_index/4.0f), 1.0f);
}