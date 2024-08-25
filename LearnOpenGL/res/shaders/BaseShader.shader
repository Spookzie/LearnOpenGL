#shader vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texCoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texCoord;

void main()
{
   vs_position = v_position;
   vs_color = v_color;
   v_texCoord = vec2(v_texCoord.x, v_texCoord.y * -1.f);

   gl_Position = vec4(v_position, 1.f);
}
 

#shader fragment
#version 330 core

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texCoord;

out vec4 fs_color;

void main()
{
	fs_color = vec4(vs_color, 1.f);
}