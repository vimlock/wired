#include "../include/wired/wEffect.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wAssert.h"

#include <stddef.h>

static const wClass wSpriteEffectClass = {
	.name = "SpriteEffect",
	.base = NULL,
	.version = 1,
};

static const wClass wSpriteParticlesClass = {
	.name = "SpriteParticles",
	.base = NULL,
	.version = 1,
};

struct _wSpriteEffect
{
	wStatus *source;
};

struct _wSpriteParticles
{
	wStatus *source;
};

void wSpriteEffectRegister(const wString *name)
{
}

void wSpriteParticlesRegister(const wString *name)
{
}
