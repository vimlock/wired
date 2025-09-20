#include "../include/wired/wGraphics.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"

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

int wSpriteAnimSetAction(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	// TODO
	return W_SUCCESS;
}

int wSpriteAnimGetRecovery(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

int wSpriteAnimSetRecovery(wSpriteAnim *anim, int frame)
{
	wAssert(anim != NULL);
	// TODO
	return W_SUCCESS;
}

int wSpriteAnimGetCanInterrupt(wSpriteAnim *anim)
{
	wAssert(anim != NULL);
	// TODO
	return 0;
}

int wSpriteAnimSetCanInterrupt(wSpriteAnim *anim, bool canInterrupt)
{
	wAssert(anim != NULL);
	// TODO
	return W_SUCCESS;
}
