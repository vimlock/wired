#include "../include/wired/wCache.h"
#include "../include/wired/wImage.h"
#include "../include/wired/wTexture.h"
#include "../include/wired/wString.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"

typedef struct _wCacheItem wCacheItem;
struct _wCacheItem
{
	wString *id;
	wString *path;
	void *data;
};

typedef struct _wCache wCache;
struct _wCache
{
	int numItems;
	wCacheItem *items;
};

static wCache wDefaultCache;

void *wCacheFind(wCache *c, const wString *id)
{
	for (int i = 0; i < c->numItems; ++i) {
		if (wStringEquals(id, c->items[i].id))
			return c->items[i].data;
	}

	wLogWarn("%s not loaded", wStringData(id));

	return NULL;
}

void wCacheSet(wCache *c, const wString *id, const wString *path, void *data)
{
	if (!data) {
		wLogWarn("%s not loaded (%s)",
				wStringData(id),
				wStringData(path)
		);
	}

	for (int i = 0; i < c->numItems; ++i) {
		if (wStringEquals(id, c->items[i].id)) {
			wStringAssign(c->items[i].path, path);
			c->items[i].data = data;
		}
	}

	c->items = wMemRealloc(c->items, sizeof(wCacheItem) * (c->numItems + 1));
	c->items[c->numItems].id = wStringCopy(id);
	c->items[c->numItems].path = wStringCopy(path);
	c->items[c->numItems].data = data;
	c->numItems++;
}

void wLoadTexture(const wString *id, const wString *path)
{
	wImage *img;
	wTexture *tex;
	int err;

	img = wImageAlloc();
	err = wImageLoad(img, path);
	if (err) {
		wLogWarn("Loading image failed");
		wImageFree(img);
		wCacheSet(&wDefaultCache, id, path, NULL);
		return;
	}

	tex = wTextureAlloc();
	err = wTextureLoadFromImage(tex, img);
	if (err) {
		wLogWarn("Loading texture from image failed");
		wTextureFree(tex);
		wImageFree(img);
		wCacheSet(&wDefaultCache, id, path, NULL);
		return;
	}

	wImageFree(img);
	wCacheSet(&wDefaultCache, id, path, tex);
}

wTexture *wGetTexture(const wString *id)
{
	return wCacheFind(&wDefaultCache, id);
}
