#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 
in vec4 outColor;

uniform sampler2D ourTexture;
uniform bool useNeonEffect;

void main() {
    vec4 texColor; 

    // Use texture color or outColor based on the condition
    if (outColor == vec4(-1.0, -1.0, -1.0, -1.0)) {
        texColor = texture(ourTexture, TexCoord);
    } else {
        texColor = outColor;
    }

    if(useNeonEffect){
        // Stronger neon intensity
        float neonIntensity = 4.0; 

        vec3 glow = texColor.rgb * neonIntensity;
        glow = clamp(glow, 0.0, 1.0);

        // Mix the original color with the amplified glow
        vec3 neonColor = mix(texColor.rgb, glow, 0.7); 
        FragColor = vec4(neonColor, texColor.a);

    }else{
        FragColor = texColor; 
    }

}
