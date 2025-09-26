#include "../include/wired/wSprite.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

#include <stddef.h>

typedef struct _wSprite
{
	wRect rect;
} wSprite;

struct _wSpriteAnim
{
	int numSprites;
	wSprite *sprites;
};

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

int wSpriteAnimGetFrameCount(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

wRect wSpriteAnimGetFrameRect(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	wAssert(frame >= 0 && frame < anim->numSprites);

	return anim->sprites[frame].rect;
}

int wSpriteAnimGetAction(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

void wSpriteAnimSetAction(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	// TODO
}

int wSpriteAnimGetRecovery(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

void wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	// TODO
}

int wSpriteAnimGetCanInterrupt(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

void wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt)
{
	wAssert(anim != NULL);
	// TODO
}
