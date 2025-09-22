#include "../include/wired/wShader.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wMemory.h"

#include <string.h>

struct _wShader
{
	wNativeHandle handle;
	wPlatformOps *platform;

	wString vertSource;
	wString fragSource;

	bool compiled;
};

wShader *wShaderAlloc()
{
	wShader *ret;

	ret = wMemAlloc(sizeof(wShader));
	memset(ret, 0x0, sizeof(wShader));

	return ret;
}

int wShaderInit(wShader *shader)
{
	shader->platform = wPlatform;
	shader->handle = shader->platform->shaderCreate();
	if (!shader->handle) {
		return W_NOT_SUPPORTED;
	}

	return W_SUCCESS;
}

int wShaderLoad(wShader *shader, const wString *vert, const wString *frag)
{
	wAssert(shader != NULL);
	wAssert(vert != NULL);
	wAssert(frag != NULL);

	int err;
	wNativeHandle handle;

	handle = wPlatform->shaderCreate();
	err = wPlatform->shaderCompile(handle, vert, frag);
	if (err)
		goto fail;

	return W_SUCCESS;
fail:
	wPlatform->shaderDestroy(handle);

	return err;
}

void wShaderFree(wShader *shader)
{
	if (!shader)
		return;

	wPlatform->shaderDestroy(shader->handle);
	shader->handle = 0;

	wMemFree(shader);
}

int wShaderSetVertex(wShader *shader, const wString *source)
{
	wAssert(shader != NULL);
	return wStringCopy(source, &shader->vertSource);
}

int wShaderSetFragment(wShader *shader, const wString *source)
{
	wAssert(shader != NULL);
	return wStringCopy(source, &shader->fragSource);
}

int wShaderCompile(wShader *shader)
{
	wAssert(shader != NULL);

	int err;

	err = shader->platform->shaderCompile(shader->handle, &shader->vertSource, &shader->fragSource);
	if (err)
		return err;

	shader->compiled = true;
	return W_SUCCESS;
}

int wShaderSetValue(wShader *shader, int location, int type, const void *value)
{
	wAssert(shader != NULL);

	if (!shader)
		return W_INVALID_ARGUMENT;

	return wPlatform->shaderSetValue(shader->handle, location, type, value);
}

wNativeHandle wShaderGetNativeHandle(wShader *shader)
{
	wAssert(shader != NULL);

	return shader->handle;
}

int wShaderGetUniformLocation(wShader *shader, const char *name)
{
	wAssert(shader != NULL);
	wAssert(name != NULL);

	if (!shader->compiled)
		return -1;

	return shader->platform->shaderGetUniformLocation(shader->handle, name);
}

