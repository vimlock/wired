#include "../include/wired/wSprite.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wString.h"
#include "../include/wired/wPainter.h"
#include "../include/wired/wCache.h"
#include "../include/wired/wTexture.h"

#include <stddef.h>
#include <string.h>

typedef struct _wSpriteFrame
{
	int time;
	int duration;
	wRect rect;
} wSpriteFrame;

static const wClass wSpriteAnimClass =
{
	.name = "SpriteAnim",
	.base = NULL,
	.version = 1,
};

struct _wSpriteAnim
{
	const wClass *cls;
	int numFrames;
	wSpriteFrame *frames;
	int duration;

	int action;
	int recovery;

	bool canInterrupt;
	bool loop;

	wString *texture;
};

wSpriteAnim * wSpriteAnimAlloc()
{
	wSpriteAnim *ret = wMemAlloc(sizeof(wSpriteAnim));
	ret->cls = &wSpriteAnimClass;
	ret->numFrames = 0;
	ret->frames = NULL;
	ret->action = 0;
	ret->recovery = 0;
	ret->duration = 0;
	ret->canInterrupt = false;
	ret->loop = false;
	ret->texture = wStringFromCString("");
	return ret;
}

void wSpriteAnimFree(wSpriteAnim *anim)
{
	if (!anim)
		return;

	wMemFree(anim->frames);
	wMemFree(anim->texture);
	wMemFree(anim);
}

wSpriteAnim * wSpriteAnimCopy(const wSpriteAnim *src)
{
	wAssert(src != NULL);

	wSpriteAnim *ret = wSpriteAnimAlloc();
	wSpriteAnimAssign(ret, src);

	return ret;
}

void wSpriteAnimAssign(wSpriteAnim *dst, const wSpriteAnim *src)
{
	wAssert(dst != NULL);
	wAssert(src != NULL);

	dst->frames = wMemRealloc(dst->frames, sizeof(wSpriteFrame) * src->numFrames);
	memcpy(dst->frames, src->frames, sizeof(wSpriteFrame) * src->numFrames);
	dst->numFrames = src->numFrames;

	dst->action = src->action;
	dst->recovery = src->recovery;
	dst->canInterrupt = src->canInterrupt;
	dst->duration = src->duration;
	dst->loop = src->loop;

	wStringAssign(dst->texture, src->texture);
}

int wSpriteAnimLoad(wSpriteAnim *anim, const wString *path)
{
	wAssert(anim != NULL);
	return W_NOT_IMPLEMENTED;
}

int wSpriteAnimSave(wSpriteAnim *anim, const wString *path)
{
	wAssert(anim != NULL);
	return W_NOT_IMPLEMENTED;
}

int wSpriteAnimGetFrameCount(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->numFrames;
}

int wSpriteAnimGetDuration(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->duration;
}

int wSpriteAnimGetFrameIndex(const wSpriteAnim *anim, int time)
{
	wAssert(anim != NULL);

	int i = 0;
	while (i + 1 < anim->numFrames) {

		if (anim->frames[i].time >= time)
			break;

		i++;
	}

	return i;
}

void wSpriteAnimAppendFrame(wSpriteAnim *anim, int ticks, wRect rect)
{
	wAssert(anim != NULL);
	wAssert(ticks >= 1);

	anim->frames = wMemRealloc(anim->frames, sizeof(wSpriteFrame) * (anim->numFrames + 1));
	anim->frames[anim->numFrames].rect = rect;
	anim->frames[anim->numFrames].duration = ticks;
	anim->numFrames += 1;

	anim->duration = 0;
	for (int i = 0; i <anim->numFrames; ++i) {
		anim->frames[i].time = anim->duration;
		anim->duration += anim->frames[i].duration;
	}
}

wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	wAssert(frame >= 0 && frame < anim->numFrames);

	return anim->frames[frame].rect;
}

int wSpriteAnimGetAction(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->action;
}

void wSpriteAnimSetAction(wSpriteAnim *anim, int tick)
{
	wAssert(anim != NULL);
	anim->action = tick;
}

int wSpriteAnimGetRecovery(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->recovery;
}

void wSpriteAnimSetRecovery(wSpriteAnim *anim, int tick)
{
	wAssert(anim != NULL);
	anim->recovery = tick;
}

int wSpriteAnimGetCanInterrupt(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->canInterrupt;
}

void wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt)
{
	wAssert(anim != NULL);
	anim->canInterrupt = canInterrupt;
}

int wSpriteAnimGetLooping(const wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->loop;
}

void wSpriteAnimSetLooping(wSpriteAnim *anim, int looping)
{
	wAssert(anim != NULL);
	anim->loop = looping;
}

void wSpriteAnimSetTexture(wSpriteAnim *anim, const wString *id)
{
	wAssert(anim != NULL);
	wStringAssign(anim->texture, id);
}

const wString * wSpriteAnimGetTexture(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	return anim->texture;
}

typedef struct _wSpriteSheetAnim wSpriteSheetAnim;
struct _wSpriteSheetAnim
{
	wString *name;
	wSpriteAnim *anim;
};

static const wClass wSpriteSheetClass = {
	.name = "SpriteSheet",
	.base = NULL,
	.version = 1
};

struct _wSpriteSheet
{
	const wClass *cls;
	int numAnims;
	wSpriteSheetAnim *anims;
};

wSpriteSheet *wSpriteSheetAlloc()
{
	wSpriteSheet *ret = wMemAlloc(sizeof(wSpriteSheet));
	ret->cls = &wSpriteSheetClass;
	ret->numAnims = 0;

	return ret;
}

void wSpriteSheetFree(wSpriteSheet *sheet)
{
	if (!sheet)
		return;

	for (int i = 0; i < sheet->numAnims; ++i) {
		wSpriteAnimFree(sheet->anims[i].anim);
		wStringFree(sheet->anims[i].name);
	}

	wMemFree(sheet->anims);
	wMemFree(sheet);
}

void wSpriteSheetSetAnim(wSpriteSheet *sheet, const wString *state, const wSpriteAnim *anim)
{
	for (int i = 0; i < sheet->numAnims; ++i) {
		if (wStringEquals(sheet->anims[i].name, state)) {
			wSpriteAnimAssign(sheet->anims[i].anim, anim);
			return;
		}
	}

	sheet->anims = wMemRealloc(sheet->anims, sizeof(wSpriteSheetAnim) * (sheet->numAnims + 1));
	sheet->anims[sheet->numAnims].name = wStringCopy(state);
	sheet->anims[sheet->numAnims].anim = wSpriteAnimCopy(anim);
	sheet->numAnims++;
}

wSpriteAnim *wSpriteSheetFindAnim(wSpriteSheet *sheet, const wString *state)
{
	wAssert(sheet != NULL);
	wAssert(state != NULL);

	for (int i = 0; i < sheet->numAnims; ++i) {
		if (wStringEquals(sheet->anims[i].name, state))
			return sheet->anims[i].anim;
	}

	return NULL;
}

int wSpriteSheetGetAnimCount(const wSpriteSheet *sheet)
{
	wAssert(sheet != NULL);
	return sheet->numAnims;
}

wSpriteAnim *wSpriteSheetGetAnim(const wSpriteSheet *sheet, int index)
{
	wAssert(sheet != NULL);
	wAssert(index >= 0 && index < sheet->numAnims);
	return sheet->anims[index].anim;
}

wString *wSpriteSheetGetAnimName(const wSpriteSheet *sheet, int index)
{
	wAssert(sheet != NULL);
	wAssert(index >= 0 && index < sheet->numAnims);
	return sheet->anims[index].name;
}

static const wClass wSpriteRendererClass =
{
	.name = "SpriteRenderer",
	.base = NULL,
	.version = 1,
};
struct _wSpriteRenderer
{
	const wClass *cls;
	wSpriteSheet *sheet;
	wSpriteAnim *anim;
	wString *animName;
	float speed;
	float time;
};

wSpriteRenderer *wSpriteRendererAlloc()
{
	wSpriteRenderer *ret = wMemAlloc(sizeof(wSpriteRenderer));
	ret->cls = &wSpriteRendererClass;
	ret->speed = 1.0f;
	ret->animName = wStringFromCString("");

	return ret;
}

void wSpriteRendererFree(wSpriteRenderer *self)
{
	if (!self)
		return;

	wStringFree(self->animName);
	wMemFree(self);
}

void wSpriteRendererSetSheet(wSpriteRenderer *self, wSpriteSheet *sheet)
{
	wAssert(self != NULL);

	self->sheet = sheet;
	self->time = 0.0f;

	if (self->sheet) {
		self->anim = wSpriteSheetFindAnim(self->sheet, self->animName);
	}
	else {
		self->anim = NULL;
	}
}

void wSpriteRendererSetAnim(wSpriteRenderer *self, const wString *name)
{
	wAssert(self != NULL);

	wStringAssign(self->animName, name);

	if (!self->sheet)
		return;

	self->time = 0.0f;

	if (self->sheet) {
		self->anim = wSpriteSheetFindAnim(self->sheet, self->animName);
	}
	else {
		self->anim = NULL;
	}
}

void wSpriteRendererUpdate(wSpriteRenderer *self)
{
	wAssert(self != NULL);

	if (!self->anim)
		return;

	if (self->time >= wSpriteAnimGetDuration(self->anim)) {

		if (wSpriteAnimGetLooping(self->anim)) {
			self->time -= wSpriteAnimGetDuration(self->anim);
		}
		else {
			return;
		}
	}
	else {
		self->time += 1.0f * self->speed;
	}
}

void wSpriteRendererDraw(wSpriteRenderer *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);

	if (!self->anim)
		return;

	int index = wSpriteAnimGetFrameIndex(self->anim, self->time);

	wRect uvs = wSpriteAnimGetFrameRect(self->anim, index);
	wRect rect = { 0, 0, uvs.w, uvs.h };

	wTexture *tex = wGetTexture(wSpriteAnimGetTexture(self->anim));

	wPainterBindRectMesh(painter, rect, uvs);
	wPainterBindTexture(painter, tex, 0);
	wPainterBindShader(painter);
	wPainterDrawCall(painter);
}

void wSpriteRendererSetSpeed(wSpriteRenderer *self, float speed)
{
	wAssert(self != NULL);
	self->speed = speed;
}
