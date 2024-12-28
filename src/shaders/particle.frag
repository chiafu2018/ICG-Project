#version 330 core

uniform float tint;
uniform vec3 my_color;

in vec2 fragmentTexCoord;
out vec4 FragColor;

void main()
{
    // Slightly warm grey smoke color (RGB(0.6, 0.6, 0.55) with alpha set to `tint`)

    // Create a simple circular particle
    //vec2 center = vec2(0.5, 0.5);
    // float dist = length(fragmentTexCoord - center);
    //float alpha = smoothstep(0.5, 0.4, dist);
    
    // FragColor = fColor;
    // FragColor.a *= alpha;

    FragColor = vec4(my_color, tint);
}
