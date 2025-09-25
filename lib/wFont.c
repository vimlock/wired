#include "../include/wired/wFont.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wError.h"
#include "../include/wired/wPlatform.h"
#include "../include/wired/wTexture.h"
#include "../include/wired/wImage.h"
#include "../include/wired/wLog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string.h>

#define ATLAS_W 1024
#define ATLAS_H 1024
#define GLYPH_W 48
#define GLYPH_H 48

#define GLYPH_MARGIN 1
#define FONT_SIZE 32

#define MAX_GLYPHS_PER_DRAW 512

typedef struct _wFontGlyph wFontGlyph;
struct _wFontGlyph
{
	unsigned long code;
	float u0;
	float v0;
	float u1;
	float v1;
	float width;
	float height;
	float bearingX;
	float bearingY;
	float advance;
};

struct _wFont
{
	FT_Library ft;
	FT_Face face;
	wPlatformOps *platform;
	wNativeHandle atlas;

	wFontGlyph *glyphs;
	int numGlyphs;

	wNativeHandle ibo;
	wNativeHandle vbo;

	wVertex *vertexBuf;
	wIndex *indexBuf;
};

static void wFontLoadGlyph(wFont *font, unsigned long code)
{
	int err;

	err = FT_Load_Char(font->face, code, FT_LOAD_RENDER);
	if (err) {
		return;
	}

	FT_Face face = font->face;
	FT_GlyphSlot glyph = face->glyph;

	int idx = font->numGlyphs++;

	int aw = ATLAS_W / GLYPH_W;
	int ah = ATLAS_H / GLYPH_H;

	int tx = idx % aw;
	int ty = idx / aw;
	int tw = glyph->bitmap.width;
	int th = glyph->bitmap.rows;

	wFontGlyph *dst = &font->glyphs[idx];

	if (glyph->bitmap.width > GLYPH_W || glyph->bitmap.rows > GLYPH_H) {
		wLogWarn("glyph %lu (%d, %d) exceeds atlas slot size", code,
			(int)glyph->bitmap.width,
			(int)glyph->bitmap.rows
		);
	}

	int txp = tx * GLYPH_W;
	int typ = ty * GLYPH_H;

	font->platform->textureData(font->atlas, txp, typ, tw, th, W_IMAGE_GRAYSCALE8, glyph->bitmap.buffer);

	dst->code = code;
	dst->width  = glyph->bitmap.width;
	dst->height = glyph->bitmap.rows;
	dst->bearingX = glyph->bitmap_left;
	dst->bearingY = glyph->bitmap_top;
	dst->advance = glyph->advance.x >> 6;
	dst->u0 = txp / (float)ATLAS_W;
	dst->u1 = (txp + tw) / (float)ATLAS_W;
	dst->v0 = typ / (float)ATLAS_W;
	dst->v1 = (typ + th) / (float)ATLAS_W;
}

static wFontGlyph *wFontFindGlyph(wFont *font, unsigned long code)
{
	for (int i = 0; i < font->numGlyphs; ++i) {
		if (font->glyphs[i].code == code)
			return &font->glyphs[i];
	}

	return NULL;
}

wFont *wFontAlloc()
{
	wFont *ret = wMemAlloc(sizeof(wFont));
	ret->platform = wPlatform;
	ret->glyphs = wMemAlloc((ATLAS_W / GLYPH_W) * (ATLAS_H / GLYPH_H) *sizeof(wFontGlyph));
	ret->numGlyphs = 0;

	ret->vertexBuf = wMemAlloc(sizeof(wVertex) * MAX_GLYPHS_PER_DRAW * 4);
	ret->indexBuf = wMemAlloc(sizeof(wIndex) * MAX_GLYPHS_PER_DRAW * 6);

	return ret;
}

static const char *wFontError(int err)
{
	if (err == 0)
		return "no error";

	switch (err) {
		case FT_Err_Cannot_Open_Resource:
			return "cannot open resource";
		case FT_Err_Unknown_File_Format:
			return "unknown file format";
		case FT_Err_Invalid_Argument:
			return "invalid argument";
		case FT_Err_Cannot_Render_Glyph:
			return "cannot render glyph";
	}

	return "Unknown error";
}

int wFontLoad(wFont *font, const wString *path)
{
	int err;

	err = FT_Init_FreeType(&font->ft);
	if (err) {
		wLogError("Could not initialize freetype: %s", wFontError(err));
		return W_INVALID_OPERATION;
	}

	err = FT_New_Face(font->ft, wStringData(path), 0, &font->face);
	if (err)  {
		wLogError("Could not load font: %s", wFontError(err));
		return W_NO_SUCH_FILE;
	}

	FT_Set_Pixel_Sizes(font->face, 0, FONT_SIZE);

	wPlatformOps *platform = font->platform;

	font->atlas = font->platform->textureCreate(1024, 1024, W_IMAGE_GRAYSCALE8);
	platform->textureSetWrap(font->atlas, W_TEXTURE_CLAMP);
	platform->textureSetFilter(font->atlas, W_TEXTURE_BILINEAR);

	for (char ch = 'a'; ch <= 'z'; ch++)
		wFontLoadGlyph(font, ch);
	for (char ch = 'A'; ch <= 'Z'; ch++)
		wFontLoadGlyph(font, ch);
	for (char ch = '0'; ch <= '9'; ch++)
		wFontLoadGlyph(font, ch);

	wFontLoadGlyph(font, '.');
	wFontLoadGlyph(font, ',');
	wFontLoadGlyph(font, ':');
	wFontLoadGlyph(font, ';');
	wFontLoadGlyph(font, ' ');
	wFontLoadGlyph(font, '-');
	wFontLoadGlyph(font, '_');
	wFontLoadGlyph(font, '(');
	wFontLoadGlyph(font, ')');
	wFontLoadGlyph(font, '+');
	wFontLoadGlyph(font, '*');
	wFontLoadGlyph(font, '%');
	wFontLoadGlyph(font, '{');
	wFontLoadGlyph(font, '}');
	wFontLoadGlyph(font, '?');
	wFontLoadGlyph(font, '!');

	font->vbo = platform->bufferCreate(sizeof(wVertex) * MAX_GLYPHS_PER_DRAW * 4, NULL);
	font->ibo = platform->bufferCreate(sizeof(wIndex) * MAX_GLYPHS_PER_DRAW * 6, NULL);

	wLogDebug("Loaded font '%s'", wStringData(path));

	return W_SUCCESS;
}

void wFontFree(wFont *font)
{
	if (!font)
		return;

	font->platform->textureDestroy(font->atlas);
	font->atlas = 0;

	font->platform->bufferDestroy(font->vbo);
	font->vbo = 0;

	font->platform->bufferDestroy(font->ibo);
	font->ibo = 0;

	wMemFree(font->vertexBuf);
	wMemFree(font->indexBuf);

	FT_Done_Face(font->face);
	FT_Done_FreeType(font->ft);
	
	wMemFree(font);
}

void wFontRender(wFont *font, const wString *text, wRect r)
{
	r.w = 256;
	r.h = 256;

	wVertex vertices[4] = {
		{ r.x,       r.y,       0.0f, 0.0f, 0.0f },
		{ r.x + r.w, r.y,       0.0f, 0.75f, 0.0f },
		{ r.x + r.w, r.y + r.h, 0.0f, 0.75f, 0.75f },
		{ r.x,       r.y + r.h, 0.0f, 0.0f, 0.75f }
	};

	uint16_t indices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	wPlatformOps *platform = font->platform;

	platform->textureBind(font->atlas, 0);
	
	int numGlyphs = 0;

	float x = r.x;
	float y = r.y + FONT_SIZE;

	for (unsigned i = 0; i < wStringSize(text); ++i) {
		char ch = *(wStringData(text) + i);
		wFontGlyph *glyph = wFontFindGlyph(font, ch);
		if (!glyph) {
			wLogWarn("Glyph not found for '%c'", ch);
			continue;
		}

		float xpos = x + glyph->bearingX;
		float ypos = y - glyph->bearingY;
		float w = glyph->width;
		float h = glyph->height;

		wVertex *vertices = font->vertexBuf + numGlyphs * 4;
		wIndex *indices = font->indexBuf + numGlyphs * 6;

		float u0 = glyph->u0;
		float v0 = glyph->v0;
		float u1 = glyph->u1;
		float v1 = glyph->v1;

		vertices[0] = (wVertex){ xpos,     ypos,     0, u0, v0 };
		vertices[1] = (wVertex){ xpos + w, ypos,     0, u1, v0 };
		vertices[2] = (wVertex){ xpos + w, ypos + h, 0, u1, v1 };
		vertices[3] = (wVertex){ xpos,     ypos + h, 0, u0, v1 };

		wIndex baseIndex = numGlyphs * 4;

		indices[0] = baseIndex + 0;
		indices[1] = baseIndex + 1;
		indices[2] = baseIndex + 2;

		indices[3] = baseIndex + 2;
		indices[4] = baseIndex + 3;
		indices[5] = baseIndex + 0;

		x += glyph->advance;

		numGlyphs++;
	}

	platform->bufferData(font->vbo, sizeof(wVertex) * numGlyphs * 4, font->vertexBuf);
	platform->bufferData(font->ibo, sizeof(wIndex) * numGlyphs * 6, font->indexBuf);

	platform->draw(numGlyphs * 6, font->vbo, font->ibo);
}

wRect wFontGetRect(wFont *font, const wString *text)
{
	wRect ret = {0, 0, 0, FONT_SIZE};
	wFontGlyph *last = NULL;

	for (unsigned i = 0; i < wStringSize(text); ++i) {
		char ch = *(wStringData(text) + i);
		wFontGlyph *glyph = wFontFindGlyph(font, ch);
		if (!glyph) {
			wLogWarn("Glyph not found for '%c'", ch);
			continue;
		}

		ret.w += glyph->advance;
		last = glyph;
	}

	if (last) {
		ret.w -= last->advance;
		ret.w += last->bearingX + last->width;
	}

	return ret;
}
