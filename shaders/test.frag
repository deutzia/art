// Simple box blur
uniform sampler2D texture;
uniform float texture_x;
uniform float texture_y;
uniform int kernel_size;

vec4 px(vec2 pos)
{
    return texture2D(texture, pos);
}

void main()
{
    vec2 offset = vec2(1.0 / texture_x, 1.0 / texture_y);
    // lookup the pixel in the texture
    vec2 pos = gl_TexCoord[0].xy;

    // multiply it by the color;

    int count = 0;

    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int x = -kernel_size; x <= kernel_size; ++x)
    {
        for (int y = -kernel_size; y <= kernel_size; ++y)
        {
            vec2 pos2 = pos + vec2(float(x) * offset.x, float(y) * offset.y);
            if (0.0 <= pos2.x && pos2.x <= 1.0 &&
                0.0 <= pos2.y && pos2.y <= 1.0)
            {
                gl_FragColor += px(pos2);
                ++count;
            }
        }
    }
    
    gl_FragColor *= gl_Color / float(count);

    //gl_FragColor.r *= pos.x;
}