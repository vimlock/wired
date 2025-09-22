#include "../include/wired/wPlatform.h"
#include "../include/wired/wError.h"
#include "../include/wired/wLog.h"
#include "../include/wired/wAssert.h"

#include <GL/gl.h>
#include <GL/glext.h>

#define CHECK_ERROR() \
do { \
	int err; \
	while ((err = glGetError())) \
		wLog(W_LOG_ERROR, __FILE__, __LINE__, "OpenGL error: %s", errorStr(err));\
} while(0)

static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLDELETESHADERPROC glDeleteShader;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLDETACHSHADERPROC glDetachShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
static PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
static PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
static PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
static PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
static PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;

static PFNGLCREATETEXTURESPROC glCreateTextures;
// static PFNGLDELETETEXTURESPROC glDeleteTextures;

static PFNGLCREATEBUFFERSPROC glCreateBuffers;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
static PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
static PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;
static PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
static PFNGLGETNAMEDBUFFERPARAMETERIVPROC glGetNamedBufferParameteriv;

static PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
static PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
static PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib;
static PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
static PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
static PFNGLBINDBUFFERPROC glBindBuffer;
static PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
static PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;

static GLuint vao;

static int loadProcs(wPlatformOps *p)
{
	if (!p->getProcAddress) {
		wLogWarn("getProcAddress not provided");
		return W_NOT_SUPPORTED;
	}

#define PROC(name) if (!(name = p->getProcAddress(#name))) return W_NOT_SUPPORTED

	PROC(glDebugMessageCallback);

	PROC(glCreateProgram);
	PROC(glDeleteProgram);
	PROC(glLinkProgram);
	PROC(glAttachShader);
	PROC(glDetachShader);
	PROC(glGetUniformLocation);
	PROC(glGetProgramiv);
	PROC(glGetProgramInfoLog);
	PROC(glUseProgram);
	PROC(glProgramUniform1fv);
	PROC(glProgramUniform2fv);
	PROC(glProgramUniform3fv);
	PROC(glProgramUniform4fv);
	PROC(glProgramUniformMatrix4fv);

	PROC(glCreateShader);
	PROC(glDeleteShader);
	PROC(glShaderSource);
	PROC(glCompileShader);
	PROC(glGetShaderiv);
	PROC(glGetShaderInfoLog);
	PROC(glGetNamedBufferParameteriv);


	PROC(glCreateTextures);

	PROC(glCreateBuffers);
	PROC(glDeleteBuffers);
	PROC(glNamedBufferData);
	PROC(glBindBuffer);
	PROC(glNamedBufferStorage);
	PROC(glNamedBufferSubData);

	PROC(glCreateVertexArrays);
	PROC(glBindVertexArray);
	PROC(glEnableVertexArrayAttrib);
	PROC(glDisableVertexArrayAttrib);
	PROC(glVertexArrayAttribFormat);
	PROC(glVertexArrayAttribBinding);
	PROC(glVertexArrayVertexBuffer);
	PROC(glVertexArrayElementBuffer);

	return W_SUCCESS;
}

const char *errorStr(int err)
{
	switch (err) {
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	}

	return "<unknown>";
}

static wNativeHandle shaderCreate()
{
	wNativeHandle ret = glCreateProgram();
	CHECK_ERROR();
	return ret;
}

static void shaderDestroy(wNativeHandle shader)
{
	if (!shader)
		return;

	glDeleteProgram(shader);
	CHECK_ERROR();
}

static int shaderCompile(wNativeHandle shader, const wString *vert, const wString *frag)
{
	CHECK_ERROR();

	int res = W_SUCCESS;

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *vss[] = {
		wStringData(vert),
		NULL
	};

	const GLchar *fss[] = {
		wStringData(frag),
		NULL
	};

	glShaderSource(vertShader, 1, vss, NULL);
	glShaderSource(fragShader, 1, fss, NULL);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint vstatus;
	GLint fstatus;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vstatus);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fstatus);

	char log[512];

	if (!vstatus) {
		glGetShaderInfoLog(vertShader, 512, NULL, log);
		wLogError("Vertex shader compilation failed:");
		wLogError("%s", log);
		res = W_INVALID_OPERATION;
	}

	if (!fstatus) {
		glGetShaderInfoLog(fragShader, 512, NULL, log);
		wLogError("Fragment shader compilation failed:");
		wLogError("%s", log);
		res = W_INVALID_OPERATION;
	}

	if (fstatus && vstatus) {
		glAttachShader(shader, vertShader);
		glAttachShader(shader, fragShader);

		glLinkProgram(shader);

		GLint lstatus;
		glGetProgramiv(shader, GL_LINK_STATUS, &lstatus);
		if (!lstatus) {
			glGetProgramInfoLog(shader, 512, NULL, log);
			wLogError("Shader linking failed:");
			wLogError("%s", log);
			res = W_INVALID_OPERATION;
		}

		//glDetachShader(shader, vertShader);
		//glDetachShader(shader, fragShader);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	wLogInfo("Shader compiled");

	return res;
}

static int shaderGetUniformLocation(wNativeHandle shader, const char *name)
{
	CHECK_ERROR();
	return glGetUniformLocation(shader, name);
}

static int shaderSetValue(wNativeHandle shader, int location, int type, const void *data)
{
	CHECK_ERROR();

	if (location < 0)
		return W_INVALID_ARGUMENT;

	switch (type) {
		case W_SHADER_MAT4:
			glProgramUniformMatrix4fv(shader, location, 1, GL_FALSE, data);
			break;
		case W_SHADER_VEC4:
			glProgramUniform4fv(shader, location, 1, data);
			break;
	}

	return W_SUCCESS;
}

static int shaderBind(wNativeHandle shader)
{
	glUseProgram(shader);
	CHECK_ERROR();
	return W_SUCCESS;
}

static wNativeHandle textureCreate()
{
	GLuint id = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	CHECK_ERROR();
	return id;
}

static void textureDestroy(wNativeHandle tex)
{
	GLuint id = tex;
	glDeleteTextures(1, &id);
	CHECK_ERROR();
}

static int textureGenMipMaps(wNativeHandle tex)
{
	return W_NOT_IMPLEMENTED;
}

static int textureSetFilter(wNativeHandle tex, int mode)
{
	if (mode != W_TEXTURE_NEAREST && mode != W_TEXTURE_BILINEAR)
		return W_NOT_SUPPORTED;

	return W_NOT_IMPLEMENTED;
}

static int textureSetWrap(wNativeHandle tex, int mode)
{
	if (mode != W_TEXTURE_REPEAT && mode != W_TEXTURE_CLAMP)
		return W_NOT_SUPPORTED;

	return W_NOT_IMPLEMENTED;
}

static int textureBind(wNativeHandle tex, int index)
{
	return W_NOT_IMPLEMENTED;
}

static int textureData(wNativeHandle tex, int w, int h, const void *data)
{
	return W_NOT_IMPLEMENTED;
}

static wNativeHandle bufferCreate(size_t size, const void *data)
{
	GLuint id = 0;
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, size, data, GL_DYNAMIC_STORAGE_BIT);
	CHECK_ERROR();
	return id;
}

static void bufferDestroy(wNativeHandle buf)
{
	GLuint id = buf;
	glDeleteBuffers(1, &id);
	CHECK_ERROR();
}

static int bufferData(wNativeHandle buf, size_t size, const void *data)
{
	CHECK_ERROR();

	GLint bufSize = 0;
	glGetNamedBufferParameteriv(buf, GL_BUFFER_SIZE, &bufSize);

	wAssert((GLint)size <= bufSize);

	glNamedBufferSubData(buf, 0, size, data);
	return W_SUCCESS;
}

int clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_ERROR();
	return W_SUCCESS;
}

static int setScissor(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	CHECK_ERROR();
	return W_SUCCESS;
}

static int setViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	CHECK_ERROR();
	return W_SUCCESS;
}

static int draw(int numElements, wNativeHandle vbo, wNativeHandle ibo)
{
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 5);
	glVertexArrayElementBuffer(vao, ibo);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);

	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	glDisableVertexArrayAttrib(vao, 0);
	glDisableVertexArrayAttrib(vao, 1);

	return W_SUCCESS;
}

static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const* userdata)
{
	switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		case GL_DEBUG_SEVERITY_LOW:
			wLogDebug("%s", message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			wLogWarn("%s", message);
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			wLogError("%s", message);
			break;
		default:
			wLogError("%s", message);
	}
}

int wGlInit(wPlatformOps *p)
{
	int err = loadProcs(p);
	if (err)
		return err;

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, NULL);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	p->shaderCreate = shaderCreate;
	p->shaderDestroy = shaderDestroy;
	p->shaderCompile = shaderCompile;
	p->shaderSetValue = shaderSetValue;
	p->shaderBind = shaderBind;
	p->shaderGetUniformLocation = shaderGetUniformLocation;

	p->textureCreate = textureCreate;
	p->textureDestroy = textureDestroy;
	p->textureGenMipMaps = textureGenMipMaps;
	p->textureSetFilter = textureSetFilter;
	p->textureSetWrap = textureSetWrap;
	p->textureData = textureData;
	p->textureBind = textureBind;

	p->bufferCreate = bufferCreate;
	p->bufferDestroy = bufferDestroy;
	p->bufferData  = bufferData;

	p->setViewport = setViewport;
	p->setScissor = setScissor;
	p->clear = clear;
	p->draw = draw;

	return W_SUCCESS;
}
