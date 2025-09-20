#include "../include/wired/wScript.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wLog.h"
#include "../include/wired/wError.h"
#include "../include/wired/wArgs.h"

enum
{
	W_EXIT_SUCCESS =  0,
	W_EXIT_FATAL   = 127
};

int main(int argc, const char *argv[])
{
	int err;

	wString script;
	wStringInit(&script);

	wString function;

	wStringInit(&function);
	wStringFromCString(&function, "update");

	wLogOpenStderr();

	err = wArgsParse(argc, argv);
	if (err) {
		wLogFatal("Could not parse arguments");
		return W_EXIT_FATAL;
	}

	wLogDebug("Initializing platform");
	err = wPlatformInit();
	if (err) {
		wLogFatal("Could not initialize platform: %s", wErrorStr(err));
		return W_EXIT_FATAL;
	}

	wLogDebug("Initializing script VM");
	err = wScriptVmInit();
	if (err) {
		wLogFatal("Could not initialize script engine: %s", wErrorStr(err));
		return W_EXIT_FATAL;
	}

	err = wWindowOpen(800, 600);
	if (err) {
		wLogFatal("Could not open window: %s", wErrorStr(err));
		return W_EXIT_FATAL;
	}


	if (wArgsGetNumPositional() > 0) {
		wStringFromCString(&script, wArgsGetPositional(0));

		err = wScriptLoad(&script);
		if (err) {
			wLogFatal("Failed to execute script: %s", wErrorStr(err));
			wApplicationQuit();
		}
	}

	if (!wApplicationShouldQuit()) {
		wLogDebug("Entering main loop");
	}

	while (!wApplicationShouldQuit()) {

		wWindowPollEvents();

		wScriptCall(&script, &function);

		err = wWindowSwapBuffers();
		if (err) {
			wLogFatal("Could not swap buffers: %s", wErrorStr(err));
			wApplicationQuit();
		}
	}

	wLogDebug("Shutting down");
	wScriptVmFree();
	wPlatformFree();

	wStringFree(&script);

	return W_EXIT_SUCCESS;
}
