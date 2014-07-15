#ifndef JUPITER_PARTICLES_HPP
#define JUPITER_PARTICLES_HPP

typedef unsigned int index_type;

#include "particle_conf.h"

namespace particles
{

float	vertices[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};	//xyxyxyxy

GLuint particle_prog_obj;
GLuint dynamics_prog_obj;

GLuint	quad_vbo;
GLuint	vbo[3];
int		vbo_switch;
GLint	particle_prog_sample_count_uniform_loc;
GLint	dynamics_prog_sample_count_uniform_loc;

namespace
{
#ifdef USE_SMALL_SHADER
#	include "particle_dynamics.vs.hpp"
#endif
__forceinline void init_dynamics_prog_obj()
{
	dynamics_prog_obj = 
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"particle_dynamics.vs", GL_VERTEX_SHADER
		);
#else
		tofu::glsl::load_program_wo_link
		(
			particle_dynamics_vs, GL_VERTEX_SHADER
		);
#endif
#	ifndef VAR_OUT_POSITIONS
#		define VAR_OUT_POSITIONS	"out_positions"
#	endif
#	ifndef VAR_SAMPLE_COUNT
#		define VAR_SAMPLE_COUNT		"sample_count"
#	endif

	const char* varyings[] = {VAR_OUT_POSITIONS};
	glTransformFeedbackVaryings
	(
		dynamics_prog_obj, 1, varyings, GL_INTERLEAVED_ATTRIBS
	);
	glLinkProgram(dynamics_prog_obj);
	tofu::glsl::check_link_error(dynamics_prog_obj);

	glUseProgram(dynamics_prog_obj);
	GLint loc = glGetUniformLocation(dynamics_prog_obj, VAR_SAMPLE_COUNT);
	assert(loc!=-1);
	dynamics_prog_sample_count_uniform_loc = loc;

}

#ifdef USE_SMALL_SHADER
#	include "particle_render.vs.hpp"
#	include "particle_render.fs.hpp"
#endif

__forceinline void init_render_prog_obj()
{
	particle_prog_obj =
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"particle_render.vs", "particle_render.fs"
		);
#	define VAR_SAMPLE_COUNT "sample_count"
#else
		tofu::glsl::load_program_wo_link
		(
			particle_render_vs, particle_render_fs
		);
#endif
	glLinkProgram(particle_prog_obj);
	tofu::glsl::check_link_error(particle_prog_obj);

	glUseProgram(particle_prog_obj);
	GLint loc = glGetUniformLocation(particle_prog_obj, VAR_SAMPLE_COUNT);
	assert(loc!=-1);
	particle_prog_sample_count_uniform_loc = loc;
}

}

__forceinline void init()
{
	vbo_switch = 0;

	glGenBuffers(3, vbo);
	const static GLsizeiptr vbo_size = sizeof(float)*3*NUM_LINES;
//	float zeros[vbo_size/sizeof(float)];	//初期値が必要ならば!
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	init_dynamics_prog_obj();
	init_render_prog_obj();
}

__forceinline void render()
{
	const float pos = sound::get_position();

	glUseProgram(dynamics_prog_obj);
	glUniform1f(dynamics_prog_sample_count_uniform_loc, pos);

	const size_t crnt_pos_vbo_id	= (vbo_switch+0)%3;
	const size_t new_pos_vbo_id		= (vbo_switch+1)%3;
	const size_t prev_pos_vbo_id	= (vbo_switch+2)%3;

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[prev_pos_vbo_id]);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (float*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[crnt_pos_vbo_id]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (float*)(0));

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[new_pos_vbo_id]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, NUM_LINES);
	glEndTransformFeedback();

#if 0
	//for debugging.
	float result[NUM_VERTICES_PER_LINE*3];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(result), &result);
#endif

	glUseProgram(particle_prog_obj);
	glUniform1f(particle_prog_sample_count_uniform_loc, pos);
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[new_pos_vbo_id]);
	glVertexAttribDivisor(0, 1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (float*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

//	glDrawArrays(GL_TRIANGLES, 0, num_particles*3);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP/*GL_LINE_STRIP*/, 0, 4, NUM_LINES);

	++vbo_switch;
}
}

#endif
