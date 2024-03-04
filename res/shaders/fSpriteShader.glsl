#version 460
in vec2 texPositions;
out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    frag_color = texture(tex, texPositions);
    if (frag_color.rgb == vec3(0.0)){
        discard;
    }
}
