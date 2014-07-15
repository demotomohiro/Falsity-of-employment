//#pragma comment(linker,"/subsystem:windows")
//#pragma comment(linker,"/NODEFAULTLIB")
#include "stdafx.h"

//#ifdef DEMO_MODE
#ifdef NDEBUG
#define USE_SMALL_SHADER
#endif

#ifndef DEMO_MODE
#	define WIDE_CHAR2(n) L##n
#	define WIDE_CHAR(n) WIDE_CHAR2(n)
#endif

#include "sound.hpp"
#include "scene.hpp"

#if 0
// :%s/"/\\"/g
// :%s/\(^.*$\)/"\1\\r\\n"/g
const static char AA[] = 
#include "aa.h"
;
#endif

#ifndef WIDTH
#define WIDTH 640
#define HEIGHT 480
#endif

#ifdef __cplusplus
extern "C" { 
#endif
#ifdef DEMO_MODE
	int _fltused=1; 
#endif
	void _cdecl _check_commonlanguageruntime_version(){}
#ifdef __cplusplus
}
#endif

static DEVMODE devmode =
{
	{0}, 0, 0, sizeof(DEVMODE), 0,
	DM_PELSWIDTH | DM_PELSHEIGHT,		//画面の何を変更するか.
	{0}, 0, 0, 0, 0, 0, {0}, 0,
	0,									//Bits Per Pixel
	WIDTH,								//画面の幅(pixel)
	HEIGHT,								//画面の高さ(pixel)
	{0},
	0									//画面の周波数(Hz)
};

static PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),		//この構造体のサイズ
	1,									//OpenGLバージョン
	PFD_DRAW_TO_WINDOW |				//Windowスタイル
	PFD_SUPPORT_OPENGL |				//OpenGL
	PFD_DOUBLEBUFFER,					//ダブルバッファ使用可能
	PFD_TYPE_RGBA,						//RGBAカラー
	24,									//色数
	0, 0,								//RGBAのビットとシフト設定        
	0, 0,								//G
	0, 0,								//B
	0, 0,								//A
	0,									//アキュムレーションバッファ
	0, 0, 0, 0,							//RGBAアキュムレーションバッファ
	24,									//Zバッファ    
	0,									//ステンシルバッファ
	0,									//使用しない
	PFD_MAIN_PLANE,						//レイヤータイプ
	0,									//予約
	0, 0, 0								//レイヤーマスクの設定・未使用
};

_forceinline void setupGLState()
{
	reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"))(1);

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_MULTISAMPLE);
//
}

#ifndef DEMO_MODE
void print_last_error()
{
	const DWORD err = GetLastError();
	LPWSTR pmsg;
	const DWORD	size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		0,
		err,
		0,
		reinterpret_cast<LPWSTR>(&pmsg),
		0,
		0);
	if(size == 0)
	{
		std::wcerr << L"Failed to call FormatMessageW." << std::endl;
		return;
	}
	std::wcerr << pmsg << std::endl;
	LocalFree(pmsg);
}

struct gl_call_info
{
	const	char*	funcname;
			int		line;
	const	char*	filename;

	gl_call_info():
		funcname(0), line(0), filename(0)
	{
	}

	void set(const char* funcname, int line, const char* filename)
	{
		this->funcname	= funcname;
		this->line		= line;
		this->filename	= filename;
	}
};

gl_call_info& get_gl_call_info()
{
	static gl_call_info gci;

	return gci;
}

void APIENTRY gl_debug_callback(
	GLenum source, GLenum type, GLuint id,
	GLenum severity,
	GLsizei length, const char *message,
	GLvoid * /*userParam*/)
{
	using namespace std;

	(source);
	(type);
	(id);
	(severity);
	(length);

	assert(get_gl_call_info().funcname);

	cerr << "Message from glDebugMessageCallback:\n";
	cerr <<
		"In " << get_gl_call_info().funcname << ", " <<
		get_gl_call_info().filename << ":" <<
		get_gl_call_info().line <<
		endl;

	cerr << message << endl;
}

namespace glew4kb
{
void custom_before_func(const char* name, int line, const char* file)
{
	get_gl_call_info().set(name, line, file);
}
}

bool init_debug_context(HDC hdc)
{
	if(!SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd))
	{
		std::wcerr << L"Failed to call SetPixelFormat" << std::endl;
		return false;
	}

	HGLRC tempContext = wglCreateContext(hdc);
	if(tempContext == NULL)
	{
		std::wcerr << L"Failed to call wglCreateContext" << std::endl;
		return false;
	}

	if(!wglMakeCurrent(hdc, tempContext))
	{
		std::wcerr << L"Failed to call wglMakeCurrent(hdc, tempContext)" << std::endl;
		return false;
	}

	std::cout <<
		"\nGL_VERSION:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VERSION)) <<
		"\nGL_RENDERER:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_RENDERER)) <<
		"\nGL_VENDOR:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << std::endl;
	std::wcerr << L"If GL_VERSION is lower than 3.2, this demo will not work." << std::endl;
	std::wcerr << L"If GL_VERSION is 3.1 or 3.0, updating your GPU's driver might bring OpenGL 3.2 or 3.3." << std::endl;
	std::wcerr << L"If GL_VERSION is lower than 3.0, go to electronics store and ask them give me a 3D accelerator which supports OpenGL version 3.2 or higher." << std::endl;

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
		WGL_CONTEXT_MINOR_VERSION_ARB,	2,
		WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	if(!wglGetProcAddress("wglCreateContextAttribsARB"))
	{
		std::wcerr << L"Failed to get wglCreateContextAttribsARB." << std::endl;
		return false;
	}

	const HGLRC hrc =
		reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>
		(
			wglGetProcAddress("wglCreateContextAttribsARB")
		)(hdc, 0, attribs);
	if(hrc == NULL)
	{
		std::wcerr << L"Failed to call wglCreateContextAttribsARB," << std::endl;
		return false;
	}

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(tempContext);
	if(wglMakeCurrent(hdc, hrc) == FALSE)
	{
		std::wcerr << L"Failed to call wglMakeCurrent(hdc, hrc)." << std::endl;
		return false;
	}

	using namespace std;

	GLenum glewError;

	glewExperimental = GL_TRUE;
	if((glewError = glewInit()) != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

	if(!GLEW_VERSION_3_2)
	{
		std::wcerr << L"Error: This demo needs OpenGL 3.2 or higher, but your computer doesn't support it." << std::endl;
		return false;
	}

	tofu::opengl::print_context_info();

	GLint cntxt_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &cntxt_flags);
	if(cntxt_flags & GL_CONTEXT_FLAG_DEBUG_BIT && glIsEnabled(GL_DEBUG_OUTPUT)==GL_TRUE)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		GLuint ids = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &ids, GL_TRUE);
		glDebugMessageCallback(&gl_debug_callback, NULL);
	}

	return true;
}
#endif

#ifdef DEMO_MODE
void WinMainCRTStartup()
{
#else
int main()
//int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	//標準出力に日本語を表示するためにロケールを設定.
	std::locale::global(std::locale(""));
#endif

#ifdef DEMO_MODE
	if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		return;
#endif

	HWND hWnd;
	hWnd =	CreateWindowW(L"STATIC", 0,
		WS_POPUP|WS_VISIBLE, 0, 0,
		WIDTH, HEIGHT, NULL, NULL, NULL,NULL);

#ifdef DEMO_MODE
	if (!hWnd) return;
#else
	if (!hWnd)
	{
		std::wcerr << L"Failed to call CreateWindowW." << std::endl;
		print_last_error();
		system("pause");
		return 0;
	}
#endif

//	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
	scene::pre_GL_init(hWnd);

	HDC hdc = GetDC(hWnd);

#ifdef DEMO_MODE
	if(!SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd))
		return;

	wglMakeCurrent(hdc, wglCreateContext(hdc));
	ShowCursor(false);
#else
	if(!init_debug_context(hdc))
	{
		print_last_error();
		system("pause");
		return 0;
	}
#endif

#ifdef DEMO_MODE
	glew4kb_setup_GL_ext();
#endif
	setupGLState();

	scene::init();
	sound::make_sound(hWnd);

/*
	static float ProjectionMatrix[] = {
			2.0f/2.0f,	0.0f,				0.0f,			0.0f,
			0.0f,			2.0f/(tp*2.0f),		0.0f,			0.0f,
			0.0f,			0.0f,				-(f+n)/(f-n),	-1.0f,
			0.0f,			0.0f,				-2.0f*f*n/(f-n),0.0f};

	glUniformMatrix4fv(
		body::getProjectionMatLoc(), 1, GL_FALSE, 
		&ProjectionMatrix[0]);
*/

#if 0
	MSG	msg;
	while(TRUE)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if((msg.message==WM_CHAR)&&(msg.wParam==0x1B))
				break;//ESC
			DispatchMessage(&msg);
		}else{
			OnIdle();
			SwapBuffers(hdc);
		}
	}

	ExitProcess(0);
	return	;
#else
	MSG msg;
loop:
	//実行中にキーボードやマウスからの入力を受け取るならPeekMessageを呼ばないと
	//OSからハングしたプロセスとみなされる.
	//DisableProcessWindowsGhostingを呼ぶという方法もあるらしい.
	PeekMessage(&msg, 0, 0, 0, PM_REMOVE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene::render();
	SwapBuffers(hdc);
	if(GetAsyncKeyState(VK_ESCAPE) || msg.message == MM_WOM_DONE)
#ifdef DEMO_MODE
		ExitProcess(0);
#else
	{
	//	return 0;
		glew4kb::output();
		ExitProcess(0);
	}
#endif
	goto loop;
#endif
}
