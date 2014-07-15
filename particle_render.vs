#version 330 compatibility

#include "common.s"

#define WIDTH	1920.0
#define HEIGHT	1080.0
#define ZNEAR	0.01
#define ZFAR	1.0
#define PROJ_W	0.03

const mat4x4 projection =
mat4
(
	-ZNEAR/(PROJ_W), 0.0,				0.0,							0.0,
	0.0,	ZNEAR/(HEIGHT/WIDTH*PROJ_W),0.0,							0.0,
	0.0,	0.0,						(ZFAR+ZNEAR)/(ZFAR-ZNEAR),		1.0,
	0.0,	0.0,						-2.0*ZFAR*ZNEAR/(ZFAR-ZNEAR),	0.0
);

uniform float sample_count;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_vert_pos;
//out vec3 vary_pos;

void main()
{
#if 0
	if(gl_VertexID == 0)
	{
		gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
	}else if(gl_VertexID == 1)
	{
		gl_Position = vec4(0.0, 1.0, 0.0, 1.0);
	}else
	{
		gl_Position = vec4(1.0, 0.0, 0.0, 1.0);
	}
#endif

#if 0
	gl_Position = vec4(cos(gl_VertexID/2.0), sin(gl_VertexID/2.0), 0.0, 1.0);
	gl_Position.xy /= gl_InstanceID;
#endif

	float t = get_second(sample_count);
	vec3 view_pos	= in_position-vec3(0.5, 0.5, 0.0);
#if 1
	float angle = (gbl_2(t)+gbl_3(t))*2.*PI;
	vec2 rotx = vec2(cos(angle), sin(angle));
	vec2 rotz = rotx.yx*vec2(-1., 1.);
	view_pos.xz = view_pos.x * rotx + view_pos.z * rotz - (rotz*.5-vec2(0., .5)) ;
#endif

	view_pos.xy += in_vert_pos*0.004;
	gl_Position = projection*vec4(view_pos, 1.0);

	if(gl_InstanceID == NUM_LINES-2)
	{
		gl_Position.xy = in_vert_pos*2.0-vec2(1.);
		gl_Position.y *= WIDTH/HEIGHT;
		gl_Position.zw = vec2(0.9999, 1.0);
	}
	if(gl_InstanceID == NUM_LINES-1)
	{
		gl_Position.xy = in_vert_pos*2.0-vec2(1.);
		gl_Position.zw = vec2(0.99999, 1.0);
	}
//	vary_pos =
	gl_TexCoord[0].xyz = in_position;

	gl_TexCoord[0].w = gl_InstanceID/FLT(NUM_LINES);

	//UV座標.
	gl_TexCoord[1].xy = in_vert_pos;
	gl_TexCoord[1].z =
		(gl_InstanceID == NUM_LINES-1) ? 1. :
		((gl_InstanceID == NUM_LINES-2) ? .5 : 0.);
}
