#version 330

#include "common.s"

out vec2 out_sample;

float softnoise1D(float t, float freq)
{
	float d = rand3(freq/1000.).x;
	float n = floor(t*freq)+d*0.1;
	float a = t*freq - n;
	float c0 = rand3(n/freq/FLT(SOUND_LENGTH_IN_SECOND)).x;
	float c1 = rand3((n+1.0)/freq/FLT(SOUND_LENGTH_IN_SECOND)).x;
	return mix(c0, c1, smoothstep(0., 1., a));
}

float fbm1D(float t, float freq)
{
	float r = 0;
	r += softnoise1D(t, freq)*0.5;
	r += softnoise1D(t, freq*2.0)*0.25;
	r += softnoise1D(t, freq*4.0)*0.125;
	r += softnoise1D(t, freq*8.0)*0.0625;
	return r;
}

float kukei(float t)
{
	return sign(fract(t)-0.5);
}

float triangle(float t)
{
	float f = fract(t)*2.0;

	return f < 1.0 ? f : 2 - f;
}

float enve(float t)
{
//	return -4.*t*t+4.*t;
	return -4.*t+4.*sqrt(t);
}

void main()
{
	float t = get_second(gl_VertexID);
	const float p2 = 2.0*3.1415926;
	float s = t*p2;

	float n = floor(t*8.);
	float cfreq = exp(floor(rand3(n/8./FLT(SOUND_LENGTH_IN_SECOND)).x*8.)*0.25)+160.;

	out_sample = vec2(0.);
//	out_sample = vec2(sin(s*400.0+sin(s*10)), sin(s*400.0+sin(s*10)))*0.5;
//	out_sample = vec2(fract(t*400))*0.25;

	//Low freq
#if 1
	const float bass_amp = 0.15;
	if(sin(2.*PI*t/FLT(SOUND_LENGTH_IN_SECOND)*8.) > 0.)
	{
		out_sample += vec2(kukei(s*8.)+fract(s*7.95)+rand3(s)*0.5)/2.5*bass_amp*pulse_fract(t);
		out_sample += vec2(kukei(s*32.)+fract(s*16.95)+rand3(s)*0.5)/2.5*bass_amp*(pulse_fract(t-0.5)+pulse_fract(t-0.75));
	}else
	{
		out_sample += vec2(kukei(s*32.)+fract(s*7.95)+rand3(s)*0.5)/2.5*bass_amp*pulse_fract(t*2.);
		out_sample += vec2(kukei(s*32.)+fract(s*7.95)+rand3(s)*0.5)/2.5*bass_amp*pulse_fract(t*3.);
		out_sample += vec2(kukei(s*16.)+fract(s*16.95)+rand3(s)*0.5)/2.5*bass_amp*(pulse_fract(t-0.25)+pulse_fract(t-0.75));
	}
#endif

	//High freq
	const float row_sec = 0.5;
	const float row2_sec = row_sec*2.;
	float nrm_t = t/FLT(SOUND_LENGTH_IN_SECOND);
	float melody_type = floorN(t/8., 16.)*0.5; 
	float row_step = floorN(nrm_t, FLT(SOUND_LENGTH_IN_SECOND)/row_sec);
	float row2_step = floorN(nrm_t, FLT(SOUND_LENGTH_IN_SECOND)/row2_sec);
	float frac;
	float step = floorFract(snd_sin(t/row2_sec), 4., frac);
//	float step = (rand3(row2_step).x > .2) ? row_step : row2_step;
	float freq	= exp(floor(rand3(step+melody_type).x*5.)*0.5)+70.;
	float freq2	= exp(floor(rand3(row2_step).x*3.)*0.5)+40.;
	out_sample += vec2(snd_sin(s*freq)*0.25*triangle(frac));
	out_sample += vec2(triangle(s*freq2)*0.24*enve(frac) + rand3(s*freq).x*0.01);

	//1
	float freq_base = lcl_1(t);
	float amp1 = 1.-pulse_fract(t);
	out_sample += fbm1D(t, 1000.+exp(freq_base*8.*0.25)*2222.)*0.25*amp1*amp1*gbl_1(t);

	//2
	out_sample += snd_sin(t*(gbl_2(t)*222.+222.))*0.2*pulse_double(t)*gbl_2(t);

	//3
	float phase3 = sin(s*(t+1.0))*2;
	out_sample += vec2(sign(snd_sin(t*cfreq+phase3)-0.5))*0.25*narrow_wave2(t)*gbl_3(t);

	//4
	out_sample += vec2(snd_sin(t*cfreq))*0.25*gbl_4(t);

	out_sample *= 1.0 - gbl_end(t);
}
