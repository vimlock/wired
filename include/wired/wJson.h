#pragma once

#include <stdbool.h>

#include "wString.h"

enum wJsonType
{
	W_JSON_NULL,
	W_JSON_BOOL,
	W_JSON_NUMBER,
	W_JSON_STRING,
	W_JSON_ARRAY,
	W_JSON_OBJECT,
};

typedef struct _wJson wJson;

wJson *wJsonParse(const wString *jsonStr);
void wJsonFree(wJson *json);

int wJsonGetType(const wJson *json);

bool wJsonIsNull(const wJson *json);
bool wJsonIsBool(const wJson *json);
bool wJsonIsNumber(const wJson *json);
bool wJsonIsString(const wJson *json);
bool wJsonIsArray(const wJson *json);
bool wJsonIsObject(const wJson *json);

bool wJsonGetBool(const wJson *json);
int wJsonGetInt(const wJson *json);
int wJsonGetString(const wJson *json, wString *ret);

int wJsonGetSize(const wJson *json);

const wString *wJsonGetKey(const wJson *json, int index);
const wJson *wJsonGetValue(const wJson *json, int index);
const wJson *wJsonFindValue(const wJson *json, const wString *key);
