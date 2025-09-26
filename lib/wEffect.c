#include "../include/wired/wEffect.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wMemory.h"

#include <stddef.h>

#define MAX_PARTICLES 100

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

typedef struct _wParticle wParticle;
struct _wParticle
{
	float posX;
	float posY;
	float posZ;

	float velX;
	float velY;

	int life;
};

struct _wSpriteEffect
{
	const wClass *cls;
	wStatus *source;

	wString *shaderUniform;
};

struct _wSpriteParticles
{
	const wClass *cls;
	wStatus *source;

	int numParticles;
	wParticle *particles;

	float gravityX;
	float gravityY;
};

wSpriteEffect *wSpriteEffectAlloc()
{
	wSpriteEffect *ret = wMemAlloc(sizeof(wSpriteEffect));
	ret->cls = &wSpriteEffectClass;
	return ret;
}

void wSpriteEffectFree(wSpriteEffect *e)
{
	if (!e)
		return;

	wMemFree(e);
}

wSpriteParticles * wSpriteParticlesAlloc()
{
	wSpriteParticles *ret = wMemAlloc(sizeof(wSpriteParticles));
	ret->cls = &wSpriteParticlesClass;
	ret->particles = wMemAlloc(sizeof(wParticle) * MAX_PARTICLES);
	ret->numParticles = 0;
	return ret;
}

void wSpriteParticlesFree(wSpriteParticles *p)
{
	if (!p)
		return;

	wMemFree(p->particles);
	wMemFree(p);
}

void wSpriteParticlesSpawn(wSpriteParticles *self)
{
	wAssert(self != NULL);
}

void wSpriteParticlesUpdate(wSpriteParticles *self)
{
	wAssert(self != NULL);

	for (int i = 0; i < self->numParticles; ++i) {
		wParticle *p = &self->particles[i];

		p->posX += p->velX;
		p->posY += p->velY;

		if (p->life < 0) {
			self->particles[i] = self->particles[--self->numParticles];
			i--;
		}

		p->velX += self->gravityX;
		p->velY += self->gravityY;

		p->life--;
	}
}

void wSpriteParticlesDraw(wSpriteParticles *self, wPainter *painter)
{
	wAssert(self != NULL);
	wAssert(painter != NULL);
}

void wSpriteParticlesSetGravity(wSpriteParticles *self, float x, float y)
{
	wAssert(self != NULL);
	self->gravityX = x;
	self->gravityY = y;
}
