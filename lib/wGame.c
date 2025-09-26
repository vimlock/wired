#include "../include/wired/wGame.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wClass.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wTileMap.h"

#include <stddef.h>

static wClass wPlayerClass =
{
	.name = "Player",
	.base = NULL,
	.version = 1,
};

typedef struct _wPlayer wPlayer;
struct _wPlayer
{
	const wClass *cls;

	wEntityId character;
};

static wClass wGameClass =
{
	.name = "Game",
	.base = NULL,
	.version = 1,
};

struct _wGame
{
	const wClass *cls;
	wTileLayer *background;
	wPlayer *player;
};

wGame *wGameAlloc()
{
	wGame *ret = wMemAlloc(sizeof(wGame));
	ret->cls = &wGameClass;

	return ret;
}

void wGameFree(wGame *game)
{
	if (!game)
		return;

	wTileLayerFree(game->background);

	wMemFree(game);
}

void wGameDraw(wGame *game, wPainter *painter)
{
	wAssert(game != NULL);
	wAssert(painter != NULL);

	wTileLayerDraw(game->background, painter);
}

wEntity *wGameGetEntity(wGame *game, wEntityId id)
{
	wAssert(game != NULL);

	return NULL;
}

void wGameRegisterStatus(wGame *game, const wString *name, const wString *script)
{
}

void wGameRegisterAbility(wGame *game, const wString *name, const wString *script)
{
}
