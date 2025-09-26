#include "../include/wired/wSprite.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"

#include <stddef.h>
#include <string.h>

typedef struct _wSpriteFrame
{
	int ticks;
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

	int action;
	int recovery;

	bool canInterrupt;
};

wSpriteAnim * wSpriteAnimAlloc()
{
	wSpriteAnim *ret = wMemAlloc(sizeof(wSpriteAnim));
	ret->cls = &wSpriteAnimClass;
	ret->numFrames = 0;
	ret->frames = NULL;
	ret->action = 0;
	ret->recovery = 0;
	ret->canInterrupt = false;
	return ret;
}

void wSpriteAnimFree(wSpriteAnim *anim)
{
	if (!anim)
		return;

	wMemFree(anim->frames);
	wMemFree(anim);
}

wSpriteAnim * wSpriteAnimCopy(const wSpriteAnim *src)
{
	wSpriteAnim *ret = wSpriteAnimAlloc();
	ret->numFrames = src->numFrames;
	ret->frames = wMemAlloc(sizeof(wSpriteFrame) * src->numFrames);
	memcpy(ret->frames, src->frames, sizeof(wSpriteFrame) * src->numFrames);

	ret->action = src->action;
	ret->recovery = src->recovery;
	ret->canInterrupt = src->canInterrupt;

	return ret;
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

void wSpriteAnimAppendFrame(wSpriteAnim *anim, int ticks, wRect rect)
{
	anim->frames = wMemRealloc(anim->frames, sizeof(wSpriteFrame) * (anim->numFrames + 1));
	anim->frames[anim->numFrames].rect = rect;
	anim->frames[anim->numFrames].ticks = ticks;
	anim->numFrames += 1;
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

typedef struct _wSpriteSheetAnim wSpriteSheetAnim;
struct _wSpriteSheetAnim
{
	wString *name;
	wSpriteAnim *anim;
};

struct _wSpriteSheet
{
	int numAnims;
	wSpriteSheetAnim *anims;
};

wSpriteSheet *wSpriteSheetAlloc()
{
}

void wSpriteSheetFree(wSpriteSheet *sheet)
{
}
