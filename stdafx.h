#ifndef STDAFX_H
#define STDAFX_H

#pragma warning(push, 1)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <Mmsystem.h>
#pragma warning(pop)

//#	define GLEW4K_INVERSE

#ifndef DEMO_MODE
#	define GLEW_STATIC
#	include <GL/glew.h>
#	include <GL/gl.h>
#	include <GL/wglew.h>
#	include <GL/wglext.h>
#	include <iostream>
#	include <fstream>
#	include <tofu/opengl/utility.hpp>
#	define GLEW4KB_METHOD_ID 4
#	define GLEW4KB_LIST
#	define GLEW4KB_CUSTOM_BEFORE_FUNC 1
#	include <tofu/opengl/glew4kb/glew4kb.hpp>
#else

#	include <GL/gl.h>
#	include <GL/glext.h>
#	include <GL/wglext.h>

#if 0
//#include "setup_gl_ext.hpp"
#define GLEW4K_INLINE __forceinline
#ifdef GLEW4K_INVERSE
#	include "glew4k_exts_directs.h"
#else
#	include "glew4k_exts_indirects.h"
#endif
#endif

#define GLEW4KB_INLINE __forceinline
#include "glew4kb_exts.h"

#endif

#ifdef NDEBUG
#	define _TOFU_OPENGL_GLSL_ENABLE_CHECK_ERROR 0
#	define _TOFU_OPENGL_GLSL_ENABLE_SOURCE_FILE 0
#else
#endif

#include <cassert>
#define TOFU_OPENGL_GLSL_PREPROCESSOR_ENABLE_EMIT_LINE_DIRECTIVES 0
#include <tofu/opengl/glsl.hpp>

#endif
