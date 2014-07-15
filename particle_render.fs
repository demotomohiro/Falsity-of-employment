#version 330 compatibility

#include "common.s"

uniform float sample_count;

//in  vec3 vary_pos;
out vec4 color;

vec3 Hue( float hue )
{
	vec3 rgb = fract(hue + vec3(0.0,2.0/3.0,1.0/3.0));

	rgb = abs(rgb*2.0-1.0);
		
	return clamp(rgb*3.0-1.0,0.0,1.0);
}

vec3 HSVtoRGB( vec3 hsv )
{
	return ((Hue(hsv.x)-1.0)*hsv.y+1.0) * hsv.z;
}

vec3 HSVtoRGB(float h, float s, float v)
{
	return HSVtoRGB(vec3(h, s, v));
}

void main()
{
	vec3 vary_pos = gl_TexCoord[0].xyz;
	float nrm_insid = gl_TexCoord[0].w;
	float t = get_second(sample_count);
	float lit = step(0.85, softnoise(vec3(vary_pos.xz*0.2+vec2(t*0.02, 0.), 0.), 200.));
#if 0
	lit = step(0.75,sin(vary_pos.x*42.0)*sin(vary_pos.z*40.0));
	vec2 p = vary_pos.xz - vec2(0.5);
	vec2 lp = vec2(1.0)-p*p*64.0;
	lit = max(lp.x, 0.)*pow(0.12, 1.0-vary_pos.y);
#endif

	//	color = vec4(nrm_insid);
	//	float vis = floor(nrm_insid*8.)/8. < gbl_1(t) ? 1.0 : 0.0;
	if(nrm_insid < gbl_begin(t))
		discard;

	vec2 uv = gl_TexCoord[1].xy;
	uv = uv*2.0 - vec2(1., 1.);

	color = vec4(1.0);
	if(gl_TexCoord[1].z > 0.5)
	{
		//Background
		float h = (uv.y*0.5+0.5) + abs(sin(t)*2.0)-0.6 + abs(uv.x)*sin(t*1.2+0.5)*0.5;
		color = vec4(0.1, 0.1, 0.5, 1.0) + vec4(exp(-h));
		color *= 0.9;
	}else if(gl_TexCoord[1].z > 0.)
	{
		//Nippon!
		vec2 uv0 = uv + vec2(1., 0.5);
		float theta = atan(uv0.y, uv0.x) + t;
		const float tfreq = 0.003;
		float noise = softnoise(vec3(uv, snd_sin(t*tfreq)), 111.)*0.5 + softnoise(vec3(uv, snd_sin(t*tfreq+.1)), 222.)*0.25 + softnoise(vec3(uv, snd_sin(t*tfreq+.03)), 444.)*0.125;
		if
		(
			length(uv0) + noise*.03
			> (pulse_fract_long(t)*.1 + .3)*min((t-1.)/8., 1.)
			&&
			sin(theta*16.0) < 1.-min((t-20.)/8., 1.)
		)
			discard;
		color = vec4(HSVtoRGB(.0+noise*.2/(1.+length(uv0)*12.), 0.8, 1.), 1.);
	}else
	{

	//	color = vec4(0.3)+lit*0.8+narrow_wave(t)*0.01;
		color = vec4(vec3(1.0-vary_pos.z+0.6), 1.0);
	//	color = vec4(1.0);

#if 0
		color.xyz *=
			mix
			(
				1.,
				abs
				(
					dot(getNormal(vary_pos), vec3(0.58))//,
				//	0.
				),
			1.//	gbl_2(t)
			);
#endif

		float theta = atan(uv.y, uv.x) + t*rand(vec3(nrm_insid))*4.;
		float radius = sin(theta*5.0)*0.4 + 0.6;
	//	color = vec4(uv, 0.0, 1.0);
		if(length(uv)>radius)
		{
			discard;
		}
		color *= vec4(HSVtoRGB(rand(vec3(nrm_insid))*0.4, 0.7, 1.)*0.5, 1.0);

		float dist =vary_pos.z-pulse_fract_long(t);
		dist = (1.0-dist*dist);
		dist = pow(dist, 222.);
		color += vec4(0.9)*dist;
	}

	color *= (1.0 - gbl_end(t) - pow(gbl_begin(t), 2.));

#if 0
	color = vec4(0.);
//	color.r = gbl_begin(t);
//	color.r = gbl_1(t);
	color.r = gbl_end(t);
	color.g = gbl_2(t);
	color.b = gbl_3(t);
#endif
}
