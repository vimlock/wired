#include "../include/wired/wPainter.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wFont.h"
#include "../include/wired/wShader.h"
#include "../include/wired/wImage.h"
#include "../include/wired/wTexture.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"
#include "../include/wired/wError.h"

#include <string.h>
#include <math.h>

#define W_PAINTER_MAX_STACK 32

enum wPainterDirtyFlags
{
	W_PAINTER_VIEWPORT = 1 << 0,
	W_PAINTER_SCISSOR  = 1 << 1,
	W_PAINTER_COLOR    = 1 << 2,
	W_PAINTER_VIEW_MAT = 1 << 3,
	W_PAINTER_PROJ_MAT = 1 << 4,
};

typedef struct _wPainterState
{
	wRectI viewport;
	wRectI scissor;

	wMat4 projMat;
	wMat4 viewMat;

	wMat4 mvpMat;

	wColor color;
	float borderWidth;
	float borderRadius;

	unsigned dirty;
} wPainterState;

static wClass wPainterClass =
{
	.name = "Painter",
	.base = NULL,
	.version = 1,
};

struct _wPainter
{
	const wClass *class;
	wPainterState *state;
	wPainterState *stack;
	wPlatformOps *platform;

	wNativeHandle vbo;
	wNativeHandle ibo;

	int numIndices;

	wNativeHandle emptyTex;

	wNativeHandle roundedTex;
	wNativeHandle borderTex;

	wShader *shader;
	wFont *font;

	int uniformColor;
	int uniformMvp;
};

static void setRectMesh(wPainter *painter, wRect rect, wRect uv)
{
	float vertices[4*5] = {
		rect.x,          rect.y,          0.0f,  uv.x,        uv.y,
		rect.x + rect.w, rect.y,          0.0f,  uv.x + uv.w, uv.y,
		rect.x + rect.w, rect.y + rect.h, 0.0f,  uv.x + uv.w, uv.y + uv.h,
		rect.x,          rect.y + rect.h, 0.0f,  uv.x,        uv.y + uv.h
	};

	uint16_t indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	wPlatformOps *platform = painter->platform;

	platform->bufferData(painter->vbo, sizeof(vertices), vertices);
	platform->bufferData(painter->ibo, sizeof(indices),  indices);

	painter->numIndices = 6;
}

static void setSlicedRectMesh(wPainter *painter, wRect rect, wRect uvRect)
{
	float le = 16.0f;
	float re = 16.0f;
	float te = 16.0f;
	float be = 16.0f;

	float bl = 0.5f;
	float br = 0.5f;
	float bt = 0.5f;
	float bb = 0.5f;

	float x0 = rect.x;
	float x1 = rect.x + le;
	float x2 = rect.x + rect.w - re;
	float x3 = rect.x + rect.w;

	float y0 = rect.y;
	float y1 = rect.y + te;
	float y2 = rect.y + rect.h - be;
	float y3 = rect.y + rect.h;

	float u0 = 0.0f;
	float u1 = 16;
	float u2 = 16;
	float u3 = 32;

	float v0 = 0.0f;
	float v1 = 16;
	float v2 = 16;
	float v3 = 32;

	float xcoords[4] = { x0, x1, x2, x3 };
	float ycoords[4] = { y0, y1, y2, y3 };
	float ucoords[4] = { u0, u1, u2, u3 };
	float vcoords[4] = { v0, v1, v2, v3 };

	wVertex vertices[16];

	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k)  {
			int idx = (i * 4 + k);
			vertices[idx].posX = xcoords[k];
			vertices[idx].posY = ycoords[i];
			vertices[idx].posZ = 0.0f;
			vertices[idx].texX = ucoords[k];
			vertices[idx].texY = vcoords[i];
		}
	}

	wIndex indices[] = {
		0,1,4,4,1,5,
		1,2,5,5,2,6,
		2,3,6,6,3,7,

		4,5,8,8,9,5,
		5,6,9,9,6,10,
		6,7,10,10,7,11,

		8,9,12,12,9,13,
		9,10,13,13,10,14,
		10,11,14,14,11,15
	};

	wPlatformOps *platform = painter->platform;

	platform->bufferData(painter->vbo, sizeof(vertices), vertices);
	platform->bufferData(painter->ibo, sizeof(indices),  indices);

	painter->numIndices = 54;
}

void wPainterBindRectMesh(wPainter *painter, wRect rect, wRect uv)
{
	setRectMesh(painter, rect, uv);
}

void wPainterBindShader(wPainter *painter)
{
	wNativeHandle shader = wShaderGetNativeHandle(painter->shader);

	wPlatformOps *platform = painter->platform;

	platform->shaderBind(shader);

	wMat4Multiply(&painter->state->projMat, &painter->state->viewMat, &painter->state->mvpMat);

	platform->shaderSetValue(shader, painter->uniformMvp, W_SHADER_MAT4, &painter->state->mvpMat);
	platform->shaderSetValue(shader, painter->uniformColor, W_SHADER_VEC4, &painter->state->color);
}

static void setTexture(wPainter *painter, wNativeHandle tex, int index)
{
	painter->platform->textureBind(tex, index);
}

void wPainterBindTexture(wPainter *painter, wTexture *tex, int index)
{
	wNativeHandle handle;
	if (tex)
		handle = wTextureGetNativeHandle(tex);
	else
		handle = painter->emptyTex;

	setTexture(painter, handle, index);
}

void wPainterDrawCall(wPainter *painter)
{
	wPlatformOps *platform = painter->platform;

	platform->draw(painter->numIndices, painter->vbo, painter->ibo);
}

static void drawRect(wPainter *painter, wRect rect)
{
	if (!painter->shader) {
		wLogWarn("No shader set for painter");
		return;
	}

	wRect uvs = {0, 0, 1, 1 };

	setRectMesh(painter, rect, uvs);
	wPainterBindShader(painter);
	wPainterDrawCall(painter);
}

static int wPainterCheckSupported(wPlatformOps *p)
{
	if (!p->bufferCreate)  return W_NOT_SUPPORTED;
	if (!p->bufferDestroy) return W_NOT_SUPPORTED;
	if (!p->bufferData)    return W_NOT_SUPPORTED;

	if (!p->textureCreate) return W_NOT_SUPPORTED;
	if (!p->textureDestroy) return W_NOT_SUPPORTED;
	if (!p->textureData)    return W_NOT_SUPPORTED;

	if (!p->shaderBind)     return W_NOT_SUPPORTED;
	if (!p->shaderSetValue) return W_NOT_SUPPORTED;

	if (!p->clear) return W_NOT_SUPPORTED;
	if (!p->draw) return W_NOT_SUPPORTED;

	if (!p->setViewport) return W_NOT_SUPPORTED;
	if (!p->setScissor) return W_NOT_SUPPORTED;

	return W_SUCCESS;
}

wPainter *wPainterAlloc()
{
	wPainter *ret;

	ret = wMemAlloc(sizeof(wPainter));
	ret->class = &wPainterClass;
	return ret;
}

int wPainterInit(wPainter *painter)
{
	wAssert(painter != NULL);

	int err;

	err = wPainterCheckSupported(wPlatform);
	if (err)
		return err;

	painter->platform = wPlatform;

	wPlatformOps *platform = painter->platform;

	painter->stack = wMemAlloc(sizeof(wPainterState) * W_PAINTER_MAX_STACK);
	memset(painter->stack, 0x0, sizeof(wPainterState)  * W_PAINTER_MAX_STACK);

	painter->state = painter->stack;

	wMat4Identity(&painter->state->projMat);
	wMat4Identity(&painter->state->viewMat);
	wMat4Identity(&painter->state->mvpMat);

	painter->vbo = platform->bufferCreate(sizeof(float) * 16 * 5, NULL);
	painter->ibo = platform->bufferCreate(sizeof(uint16_t) * 54, NULL);

	painter->emptyTex = platform->textureCreate(1, 1, W_IMAGE_RGBA8);

	uint32_t emptyData[] = {
		0xFFFFFFFF,
	};

	platform->textureData(painter->emptyTex, 0, 0, 1, 1, W_IMAGE_RGBA8, emptyData);

	int sz = 32;
	uint8_t *tex = wMemAlloc(sz * sz);
	memset(tex, 0x0, sz*sz);


	float cx = (sz - 1) / 2.0f;
	float cy = (sz - 1) / 2.0f;

	float r = (sz / 2.0f) - 1.0f;

	float width = 4.0f;
	float ir = r - width;

	for (int y = 0; y < sz; ++y) {
		for (int x = 0; x < sz; ++x) {
			float dx = x - cx;
			float dy = y - cy;

			if (dx*dx + dy*dy <= r*r)
				tex[y * sz + x] = 255;
		}
	}

	painter->roundedTex = platform->textureCreate(sz, sz, W_IMAGE_GRAYSCALE8);
	platform->textureData(painter->roundedTex, 0, 0, sz, sz, W_IMAGE_GRAYSCALE8, tex);

	memset(tex, 0x0, sz*sz);
	for (int y = 0; y < sz; ++y) {
		for (int x = 0; x < sz; ++x) {
			float dx = x - cx;
			float dy = y - cy;

			float sqr = dx*dx + dy*dy;
			float d = sqrtf(dx*dx + dy*dy);

			if (d <= r && d > ir) {
				float douter = fabs(d - r);
				float dinner = fabs(d - ir);
				float delta = fminf(douter, dinner);

				if (delta <= 1.0f)
					tex[y*sz+x] = delta * 255; 
				else 
					tex[y*sz+x] = 255;
			}
		}
	}

	painter->borderTex = platform->textureCreate(sz, sz, W_IMAGE_GRAYSCALE8);
	platform->textureData(painter->borderTex, 0, 0, sz, sz, W_IMAGE_GRAYSCALE8, tex);

	wMemFree(tex);

	return W_SUCCESS;
}

void wPainterFree(wPainter *painter)
{
	if (!painter)
		return;

	if (painter->vbo)
		painter->platform->bufferDestroy(painter->vbo);

	if (painter->ibo)
		painter->platform->bufferDestroy(painter->ibo);

	if (painter->emptyTex)
		painter->platform->textureDestroy(painter->emptyTex);

	wMemFree(painter->stack);
	painter->stack = NULL;
}

void wPainterSetFont(wPainter *painter, wFont *font)
{
	wAssert(painter != NULL);
	painter->font = font;
}

wFont *wPainterGetFont(wPainter *painter)
{
	wAssert(painter != NULL);
	return painter->font;
}

void wPainterSetShader(wPainter *painter, wShader *shader)
{
	wAssert(painter != NULL);

	painter->shader = shader;
	if (painter != NULL) {
		painter->uniformColor = wShaderGetUniformLocation(shader, "uColor");

		painter->uniformMvp = wShaderGetUniformLocation(shader, "uMvp");
		if (painter->uniformMvp < 0) {
			wLogWarn("uMvp not used? how does shader know where to draw");
		}
	}
}

void wPainterDrawRect(wPainter *painter, wRect rect)
{
	wAssert(painter != NULL);

	setTexture(painter, painter->emptyTex, 0);
	drawRect(painter, rect);
}

void wPainterDrawSlicedRect(wPainter *painter, wRect rect)
{
	wAssert(painter != NULL);

	wPlatformOps *platform = painter->platform;

	setSlicedRectMesh(painter, rect, (wRect){ 16, 16, 16, 16});
	wPainterBindShader(painter);
	setTexture(painter, painter->roundedTex, 0);
	platform->draw(54, painter->vbo, painter->ibo);
}

void wPainterDrawBorderRect(wPainter *painter, wRect rect)
{
	wAssert(painter != NULL);

	wPlatformOps *platform = painter->platform;

	setSlicedRectMesh(painter, rect, (wRect){ 0.5, 0.5, 0.5, 0.5});
	wPainterBindShader(painter);
	setTexture(painter, painter->borderTex, 0);
	platform->draw(54, painter->vbo, painter->ibo);
}

void wPainterDrawText(wPainter *painter, wRect rect, const wString *str)
{
	wAssert(painter != NULL);
	wAssert(str != NULL);

	if (!painter->font) {
		wLogWarn("No font set");
		return;
	}
	
	wPainterBindShader(painter);
	setTexture(painter, painter->emptyTex, 0);
	wFontRender(painter->font, str, rect);
}

void wPainterDrawImage(wPainter *painter, wRect rect, wImage *img)
{
	wAssert(painter != NULL);
	wAssert(img != NULL);

	wTexture *tex;

	tex = wImageGetTexture(img);
	if (!tex)
		return;

	wPainterDrawTexture(painter, rect, tex);
}

void wPainterDrawTexture(wPainter *painter, wRect rect, wTexture *tex)
{
	wAssert(painter != NULL);
	wAssert(tex != NULL);

	setTexture(painter, wTextureGetNativeHandle(tex), 0);
	drawRect(painter, rect);
}

void wPainterClear(wPainter *painter, wColor col)
{
	wAssert(painter != NULL);

	painter->platform->clear(col.r, col.g, col.b, col.a);
}

void wPainterSetColor(wPainter *painter, wColor col)
{
	wAssert(painter != NULL);

	painter->state->dirty |= W_PAINTER_COLOR;
	painter->state->color = col;
}

wColor wPainterGetColor(wPainter *painter)
{
	wAssert(painter != NULL);

	return painter->state->color;
}

void wPainterSetBorderRadius(wPainter *painter, float radius)
{
	wAssert(painter != NULL);

	if (radius < 0.0f)
		radius = 0.0f;

	painter->state->borderRadius = radius;
}

void wPainterSetBorderWidth(wPainter *painter, float width)
{
	wAssert(painter != NULL);

	if (width < 0.0f)
		width = 0.0f;

	painter->state->borderWidth = width;
}

wRectI wPainterGetScissor(wPainter *painter)
{
	wAssert(painter != NULL);

	return painter->state->scissor;
}

void wPainterSetScissor(wPainter *painter, wRectI rect)
{
	wAssert(painter != NULL);

	painter->state->dirty |= W_PAINTER_SCISSOR;
	painter->state->scissor = rect;
	painter->platform->setScissor(rect.x, rect.y,  rect.w, rect.h);
}

void wPainterSetViewport(wPainter *painter, wRectI rect)
{
	wAssert(painter != NULL);

	painter->state->dirty |= W_PAINTER_VIEWPORT;
	painter->state->viewport = rect;
	painter->platform->setViewport(rect.x, rect.y,  rect.w,  rect.h);

	//  wMat4Ortho(&painter->state->projMat, rect.x, rect.x + rect.w, rect.y, rect.y + rect.h, -1, 1);
	wMat4Ortho(&painter->state->projMat,
		(float)rect.x,
		(float)rect.x + (rect.w),
		(float)rect.y,
		(float)rect.y + (rect.h),
		-1, 1);
	// wMat4Transpose(&painter->state->mvpMat);
	painter->state->dirty |= W_PAINTER_PROJ_MAT;
}

wRectI wPainterGetViewport(wPainter *painter)
{
	wAssert(painter != NULL);
	return painter->state->viewport;
}

void wPainterTranslate(wPainter *painter, float x, float y, float z)
{
	wAssert(painter != NULL);

	wMat4Translate(&painter->state->viewMat, x, y, z);
	painter->state->dirty |= W_PAINTER_VIEW_MAT;
}

void wPainterPushState(wPainter *painter)
{
	wAssert(painter != NULL);

	painter->state++;
	*painter->state = *(painter->state - 1);
	painter->state->dirty = 0;
}

void wPainterPopState(wPainter *painter)
{
	wAssert(painter != NULL);
	wAssert(painter->state != painter->stack);

	wPainterState *cur = painter->state;
	wPainterState *old = painter->state - 1;

	if (cur->dirty & W_PAINTER_VIEWPORT) {
		painter->platform->setViewport(old->viewport.x, old->viewport.y, old->viewport.w,  old->viewport.h);
		wLogDebug("Set viewport %d %d %d %d", old->viewport.x, old->viewport.y, old->viewport.w, old->viewport.h);
	}

	if (cur->dirty & W_PAINTER_SCISSOR)
		painter->platform->setScissor(old->scissor.x, old->scissor.y, old->scissor.w, old->scissor.h);

	painter->state--;
}
