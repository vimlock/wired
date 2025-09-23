#pragma once

typedef struct _wClass wClass;

struct _wClass
{
	const char *name;
	const wClass *base;
	int version;
};
