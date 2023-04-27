#version 400

in vec2 uv;
in vec4 color;

uniform sampler2D Image;
out vec4 FragColor;

void main()
{
  vec4 c = texture(Image, uv);
  FragColor = c;
}
