#ifndef OPPI_SCENE_HPP
#define OPPI_SCENE_HPP


//#include "scene_list.hpp"
#include "particles.hpp"
//#include "AA.hpp"


namespace scene
{

namespace detail
{

}

//OpenGLを初期化する前にsceneの準備.
__forceinline void pre_GL_init(HWND hWnd)
{
//	AA::aa(hWnd);
}

void init()
{
	using namespace detail;


#if 0
	gl_AA::init();
#else
//	gl_AA::init_texture();
#endif
	particles::init();

//	scene_list::init();
}

void render()
{
	using namespace detail;
	using namespace tofu;

#if 0
	gl_AA::bind_texture();
	cloth::set_camera
	(
#ifndef DEMO_MODE
		(
	 	proj_mat
		*mvmat
		).data()
#else
		projection
#endif
	);
	cloth::render();
#endif
	particles::render();
}
}

#endif
