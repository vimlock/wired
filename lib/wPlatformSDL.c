#include "../include/wired/wPlatform.h"
#include "../include/wired/wEvent.h"
#include "../include/wired/wError.h"
#include "../include/wired/wLog.h"

#include <SDL2/SDL.h>

static SDL_Window *window = NULL;
static SDL_GLContext context = 0;

int wGlInit(wPlatformOps *p);

static int windowCreate(int w, int h)
{
	int err;

	window = SDL_CreateWindow("",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if (!window) {
		wLogError("Could not create window: %s", SDL_GetError());
		return W_INVALID_OPERATION;
	}

	context = SDL_GL_CreateContext(window);
	if (!context) {
		wLogError("Could not create GL context: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		window = NULL;
		return W_INVALID_OPERATION;
	}

	err = SDL_GL_MakeCurrent(window, context);
	if (err < 0)  {
		wLogError("Could not make context current: %s", SDL_GetError());
		return W_INVALID_OPERATION;
	}

	wLogDebug("Initializing OpenGL functions");

	err = wGlInit(wPlatform);
	if (err)
		return err;

	SDL_GL_SetSwapInterval(1);

	return W_SUCCESS;
}

static void windowDestroy()
{
	SDL_DestroyWindow(window);
	window = NULL;
}

static int windowSetIcon(const wImage *img)
{
	return W_NOT_IMPLEMENTED;
}

static int windowSetTitle(const wString *title)
{
	return W_NOT_IMPLEMENTED;
}

static int windowSetFullscreen()
{
	return W_NOT_IMPLEMENTED;
}

static int windowMaximize()
{
	return W_NOT_IMPLEMENTED;
}

static int windowMinimize()
{
	return W_NOT_IMPLEMENTED;
}

static int windowGetGeometry(wRectI *ret)
{
	SDL_GetWindowSize(window, &ret->w, &ret->h);
	SDL_GetWindowPosition(window, &ret->x, &ret->y);
	return W_SUCCESS;
}

static int screenGetCount()
{
	return 0;
}

static int windowSwapBuffers()
{
	SDL_GL_SwapWindow(window);
	return W_SUCCESS;
}

static void windowOnEvent(SDL_Event *e)
{
	wEvent evt;

	switch (e->type) {
	case SDL_QUIT:
		evt.type = W_EVENT_QUIT;
		break;
	case SDL_KEYDOWN:
		evt.type = W_EVENT_KEY_PRESS;
		break;
	case SDL_KEYUP:
		evt.type = W_EVENT_KEY_RELEASE;
		break;
	case SDL_MOUSEMOTION:
		evt.type = W_EVENT_MOUSE_MOVE;
		evt.mouse.x = e->motion.x;
		evt.mouse.y = e->motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		evt.type = W_EVENT_MOUSE_PRESS;
		evt.mouse.x = e->button.x;
		evt.mouse.y = e->button.y;
		break;
	case SDL_MOUSEBUTTONUP:
		evt.type = W_EVENT_MOUSE_RELEASE;
		evt.mouse.x = e->button.x;
		evt.mouse.y = e->button.y;
		break;
	case SDL_MOUSEWHEEL:
		evt.type = W_EVENT_MOUSE_WHEEL;
		evt.wheel.y = e->wheel.y;
		break;
	default:
		return;
	}

	wEventPush(&evt);
}

static int windowPollEvents()
{
	SDL_Event e;
	while (wEventCanPush() && SDL_PollEvent(&e)) {
		windowOnEvent(&e);
	}

	return W_SUCCESS;
}

void * getProcAddress(const char *name)
{
	void *ret = SDL_GL_GetProcAddress(name);
	if (!ret)
		wLogWarn("Function '%s' not found", name);

	return ret;
}

int wSdlInit(wPlatformOps *p)
{
	int err;


	SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

	err = SDL_Init(SDL_INIT_VIDEO);
	if (err < 0) {
		wLogError("Failed to initialize SDL: %s", SDL_GetError());
		return W_INVALID_OPERATION;
	}

	wLogDebug("Loading OpenGL library");
	err = SDL_GL_LoadLibrary(NULL);
	if (err < 0) {
		wLogError("Could not load GL library: %s", SDL_GetError());
		return W_NOT_SUPPORTED;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	p->windowCreate = windowCreate;
	p->windowDestroy = windowDestroy;
	p->windowSetIcon = windowSetIcon;
	p->windowSetFullscreen = windowSetFullscreen;
	p->windowSetTitle = windowSetTitle;
	p->windowMaximize = windowMaximize;
	p->windowMinimize = windowMinimize;
	p->windowGetGeometry = windowGetGeometry;
	p->windowSwapBuffers = windowSwapBuffers;
	p->windowPollEvents = windowPollEvents;
	p->screenGetCount = screenGetCount;
	p->getProcAddress = getProcAddress;

	return W_SUCCESS;
}
