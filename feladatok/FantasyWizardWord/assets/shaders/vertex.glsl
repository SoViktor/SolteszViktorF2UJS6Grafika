#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;       // Világkoordináta
out vec3 Normal;        // Normálvektor

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // Egyszerű normál – ha nincs normál adatod, akkor model-transzformált vertex irányból becsüljük
    Normal = normalize(mat3(transpose(inverse(model))) * vec3(0.0, 0.0, 1.0)); 

    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPos;
}
