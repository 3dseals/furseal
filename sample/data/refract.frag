/*
 refract.frag

  Created on: 2011-5-29
      Author: Lional King

  Copyright (c) 2011 netsurfers
*/


uniform float fs_uni_00;

uniform sampler2D fs_tex_00;
uniform sampler2D fs_tex_01;

varying vec4 vary_color;
varying vec2 vary_texcoord1;
varying vec2 vary_texcoord2;


void main(void)
{
    vec4 color = texture2D(fs_tex_01, vary_texcoord2);

    gl_FragColor.rgb = texture2D(fs_tex_00, vary_texcoord1.st + (color.rg - 0.5) * fs_uni_00).rgb;
    gl_FragColor.a = color.a;
}
