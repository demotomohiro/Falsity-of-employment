#version 330

#include "common.s"

uniform float sample_count;

layout(location = 0) in vec3 in_prev_pos;
layout(location = 1) in vec3 in_crnt_pos;

out vec3 out_positions;

void main()
{
	float t = get_second(sample_count);

	float nrm_insid = float(gl_VertexID)/FLT(NUM_LINES);
	const float num_ring = 8.0;
	const float ring_rad = 0.13;
	float angle = nrm_insid*PI*2.0 * num_ring;
	vec2 sub_ring = vec2(cos(angle*8.0+nrm_insid), sin(angle*8.0))*ring_rad*0.3;
	vec2 ring = vec2(cos(angle), sin(angle))*0.13 + vec2(0.5) + sub_ring;
	vec3 rand_pos = vec3(ring, nrm_insid);

//	out_positions = vec3(0.5, 0.5, 0.01) + t*0.00001;
//	return;

	if(gbl_begin(t) > 0.0)
	{
		out_positions = rand_pos;
		return;
	}

	vec3 torus = vec3(step(1.0, in_crnt_pos)-step(0.0, -in_crnt_pos));
	vec3 torus_prev = vec3(step(1.0, in_prev_pos)-step(0.0, -in_prev_pos));
	vec3 crnt_pos = in_crnt_pos - torus;
	vec3 prev_pos = in_prev_pos - torus - torus_prev;

	out_positions = crnt_pos;

	vec3 dlt = crnt_pos - prev_pos;
	const float max_dlt = 0.005;
	if(dot(dlt, dlt) > max_dlt*max_dlt)
		dlt = normalize(dlt)*max_dlt;
	out_positions += dlt*0.98;

	//1
#if 1
	vec3 delta = vec3(0.);
//	out_positions.z -= 0.0002;
//	out_positions.y += pulse(t)*0.0002;
	float gbl1v = gbl_1(t);//max(gbl_1a(t)*4.-3., 0.);
	float angle1 = gbl_1a(t)*2.+gbl1v*2.;
	delta.x = sin(angle1)*0.02;
	delta.z = cos(angle1);
	delta.y = (lcl_1(t)-0.5)*2.*(1.-gbl1v);
	out_positions += delta*0.0132*gbl_1(t);
#endif

	//2
#if 1
	out_positions
	+=
	-getNormal(crnt_pos)
	*
	0.004
	*
	DF(crnt_pos)
	*
	gbl_2(t);
#endif

	//3
#if 1
	float gbl3 = gbl_3a(t);
	if(crnt_pos.y < gbl3 && gbl3>0.02)
	{
		out_positions.y -= 0.0001;
	}
#endif

	//4
#if 1
	vec3 delta4 = rand_pos - crnt_pos;
	out_positions += delta4 * 0.002 * gbl_4(t);
	if(gbl_4a(t) < 0.5 && gbl_4a(t) > 0.)
	{
		vec3 dist = crnt_pos - vec3(0.5);
		float d = dot(dist, dist);
		vec3 dir  = normalize(dist);

	//	out_positions += dir/d * (gbl_4a(t)-0.25)*0.0002;
		out_positions += dir/d * (gbl_4a(t)-0.2)*0.0002;
	}
#endif
}
