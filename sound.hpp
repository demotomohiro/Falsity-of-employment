#ifndef SOUND_H
#define SOUND_H

#include <Mmreg.h>

#include "sound_conf.h"

#define SOUND_NUM_CHANNELS		2
#define SOUND_NUM_SAMPLES		(SOUND_SAMPLE_RATE*SOUND_LENGTH_IN_SECOND*SOUND_NUM_CHANNELS)

#ifdef USE_SMALL_SHADER
#	include "sound.vs.hpp"
#endif

typedef float sample_type;

sample_type	samples[SOUND_NUM_SAMPLES];

namespace sound
{
WAVEFORMATEX wave_format =
{
	WAVE_FORMAT_IEEE_FLOAT,
	SOUND_NUM_CHANNELS,							//nChannels
	SOUND_SAMPLE_RATE,							//nSamplesPerSec,
	SOUND_SAMPLE_RATE*sizeof(sample_type)*
	SOUND_NUM_CHANNELS,							//nAvgBytesPerSec
	sizeof(sample_type)*SOUND_NUM_CHANNELS,		//nBlockAlign
	sizeof(sample_type)*8,						//wBitsPerSample
	0
};

WAVEHDR wave_hdr =
{
	(LPSTR)samples,
	sizeof(samples),
	0,
	0,
	0,
	0,
	0,
	0
};

HWAVEOUT h_wave_out;

#ifdef DEMO_MODE
#	define CALL_WAVE_OUT(func)	func
#else
#	define CALL_WAVE_OUT(func)											\
	do																	\
	{																	\
		const MMRESULT r = (func);										\
		if(r == MMSYSERR_NOERROR)										\
			break;														\
		check_error(r, WIDE_CHAR(__FILE__), __LINE__, WIDE_CHAR(#func));\
	}while(0)

__forceinline void check_error(const MMRESULT result, const wchar_t* filename, const int line, const wchar_t* func)
{
	if(result == MMSYSERR_NOERROR)
		return;

	std::wcout	<< filename << L"(" << line << L"): "
				<< L"Failed to call " << func << std::endl;

	wchar_t text[MAXERRORLENGTH];
	MMRESULT r = waveOutGetErrorTextW(result, text, MAXERRORLENGTH);
	if(r == MMSYSERR_NOERROR)
	{
		std::wcout << L"Error:" << text << std::endl;
	}else
	{
		std::wcout << L"Failed to call waveOutGetErrorText" << std::endl;
	}
	std::wcout	<< L"I'm sorry.\n";
	std::wcout	<< L"If your PC satisfies requirement and you think this is a bug, Please copy this log now and send it and your PC's spec to gpuppur at gmail dot com." << std::endl;
	std::wcout	<< L"Thank you in advance." << std::endl;
	system("pause");
	exit(0);
}
#endif

void make_sound(HWND hWnd)
{
	GLuint	vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		sizeof(samples),
		0,
		GL_DYNAMIC_COPY
	);

	GLuint sound_prog_obj =
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"sound.vs", GL_VERTEX_SHADER
		);
#else
		tofu::glsl::load_program_wo_link
		(
			sound_vs, GL_VERTEX_SHADER
		);
#endif

#	ifndef VAR_OUT_SAMPLE
#		define VAR_OUT_SAMPLE 	"out_sample"
#	endif
	const char* varyings[] = {VAR_OUT_SAMPLE};
	glTransformFeedbackVaryings
	(
		sound_prog_obj, 1, varyings, GL_INTERLEAVED_ATTRIBS
	);
	glLinkProgram(sound_prog_obj);
	tofu::glsl::check_link_error(sound_prog_obj);

	glUseProgram(sound_prog_obj);

	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, SOUND_SAMPLE_RATE*SOUND_LENGTH_IN_SECOND);
	glEndTransformFeedback();

	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(samples), samples);
	glDisable(GL_RASTERIZER_DISCARD);

	CALL_WAVE_OUT(waveOutOpen(&h_wave_out, WAVE_MAPPER, &wave_format, (DWORD_PTR)hWnd, 0, CALLBACK_WINDOW));
	CALL_WAVE_OUT(waveOutPrepareHeader(h_wave_out, &wave_hdr, sizeof(wave_hdr)));
	CALL_WAVE_OUT(waveOutWrite(h_wave_out, &wave_hdr, sizeof(wave_hdr)));
}

__forceinline float get_position()
{
	MMRESULT r;

	MMTIME mmtime =
	{
		TIME_SAMPLES,
		0
	};

	r = waveOutGetPosition(h_wave_out, &mmtime, sizeof(mmtime));
	assert(r==MMSYSERR_NOERROR);
	assert(mmtime.wType==TIME_SAMPLES);

	return static_cast<float>(mmtime.u.sample);
}
#undef CALL_WAVE_OUT
}
#endif

