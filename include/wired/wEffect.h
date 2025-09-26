#pragma once

typedef struct _wString wString;
typedef struct _wStatus wStatus;
typedef struct _wPainter wPainter;

typedef struct _wSpriteEffect wSpriteEffect;
typedef struct _wSpriteParticles wSpriteParticles;

wSpriteEffect *wSpriteEffectAlloc();
void wSpriteEffectFree(wSpriteEffect *e);

wSpriteParticles * wSpriteParticlesAlloc();
void wSpriteParticlesFree(wSpriteParticles *p);
void wSpriteParticlesSpawn(wSpriteParticles *p);
void wSpriteParticlesUpdate(wSpriteParticles *p);
void wSpriteParticlesDraw(wSpriteParticles *p, wPainter *painter);
void wSpriteParticlesSetGravity(wSpriteParticles *p, float x, float y);
