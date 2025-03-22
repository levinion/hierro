#version 330 core
layout(location = 0) in vec2 _position;
layout(location = 1) in vec2 _size;
layout(location = 2) in float _radius;
layout(location = 3) in vec4 _color;

out vec2 position;
out vec2 size;
out float radius;
out vec4 color;

void main()
{
    gl_Position = vec4(_position, 1.0);
    position = _position;
    size = _size;
    radius = _radius;
    color = _color;
}
