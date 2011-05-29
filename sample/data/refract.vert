/*
 refract.vert

  Created on: 2011-5-29
      Author: Lional King

  Copyright (c) 2011 netsurfers
*/


uniform mat4 fs_local_to_screen;

attribute vec4 fs_vertex;
attribute vec4 fs_color;
attribute vec2 fs_texcoord;

attribute float fs_att_00, fs_att_01; // texcoords of texture2

varying vec4 vary_color;
varying vec2 vary_texcoord1;
varying vec2 vary_texcoord2;


void main(void)
{
    gl_Position = fs_local_to_screen * fs_vertex;

    vary_color = fs_color;

    vary_texcoord1.s = gl_Position.x / 2.0 + 0.5;
    vary_texcoord1.t = gl_Position.y / -2.0 + 0.5;

    vary_texcoord2.s = fs_att_00;
    vary_texcoord2.t = fs_att_01;

    // Because fs_texcoord isn't used in this shader,
    // you can also use it instead of fs_att_00 and fs_att_01.
}
