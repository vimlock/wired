#include "../include/wired/wJson.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wLog.h"

#include <ctype.h>
#include <string.h>

struct _wJson
{
	int type;

	wString key;

	union
	{
		wString str;
		bool b;
		double n;
	};

	int numChildren;
	struct _wJson *children;
};

enum wJsonTokenType
{
	W_JSON_TOK_NUMBER,
	W_JSON_TOK_STRING,
	W_JSON_TOK_COMMA,
	W_JSON_TOK_COLON,
	W_JSON_TOK_LCURLY,
	W_JSON_TOK_RCURLY,
	W_JSON_TOK_LBRACKET,
	W_JSON_TOK_RBRACKET,
	W_JSON_TOK_NULL,
	W_JSON_TOK_TRUE,
	W_JSON_TOK_FALSE,
	W_JSON_TOK_EOF,
	W_JSON_TOK_ERROR,
};

typedef struct _wJsonToken
{
	int type;
	int line;
	int col;

	int offset;
	int size;
} wJsonToken;

static void wJsonLexString(wJsonToken *tok)
{
	tok->type = W_JSON_TOK_STRING;
}

static void wJsonLexNumber(wJsonToken *tok)
{
	tok->type = W_JSON_TOK_NUMBER;
}

static void wJsonLexIdent(wJsonToken *tok)
{
	const char *start;

	if (strncmp(start, "true", 4) == 0) {
		tok->type = W_JSON_TOK_TRUE;
		return;
	}
}

static void wJsonLex(wJsonToken *tok)
{
	const char *ptr;

	if (*ptr == '"') {
		wJsonLexString(tok);
		return;
	}

	if (isdigit(*ptr)) {
		wJsonLexNumber(tok);
		return;
	}

	if (isalpha(*ptr)) {
		wJsonLexIdent(tok);
	}

	switch (*ptr) {
		case ',': tok->type = W_JSON_TOK_COMMA; return;
		case ':': tok->type = W_JSON_TOK_COLON; return;
		case '{': tok->type = W_JSON_TOK_LCURLY; return;
		case '}': tok->type = W_JSON_TOK_RCURLY; return;
		case '[': tok->type = W_JSON_TOK_LBRACKET; return;
		case ']': tok->type = W_JSON_TOK_RBRACKET; return;
	}
}

static int wJsonParseNumber(double *ret)
{
}

static int wJsonParseString(wString *ret)
{
	return W_SUCCESS;
}

static int wJsonParseRecurse(wJson *ret)
{
	wJsonToken tok;
	int err;

	wJsonLex(&tok);

	if (tok.type == W_JSON_TOK_NUMBER) {
		ret->type = W_JSON_NUMBER;
		err = wJsonParseNumber(&ret->number);
		if (err)
			goto fail;
	}
	else if (tok.type == W_JSON_TOK_STRING) {
		ret->type = W_JSON_STRING;
		err = wJsonParseString(&ret->str);
		if (err)
			goto fail;
	}
	else if (tok.type == W_JSON_TOK_NULL) {
		ret->type = W_JSON_NULL;
	}
	else if (tok.type == W_JSON_TOK_TRUE) {
		ret->type = W_JSON_BOOL;
		ret->b = true;
	}
	else if (tok.type == W_JSON_TOK_FALSE) {
		ret->type = W_JSON_BOOL;
		ret->b = false;
	}
	else if (tok.type == W_JSON_TOK_LCURLY) {

		ret->type = W_JSON_OBJECT;

		while (tok.type != W_JSON_TOK_RCURLY) {
			wJsonLex(&tok);

			if (tok.type != W_JSON_TOK_STRING) {
				err = W_PARSE_ERROR;
				goto fail;
			}
			wString key;

			wJson item;
			err = wJsonParseRecurse(&item);
			if (err)
				goto fail;

			item.key = key;
		}
	}
	else if (tok.type == W_JSON_TOK_LBRACKET) {

		ret->type = W_JSON_ARRAY;

		while (tok.type != W_JSON_TOK_RBRACKET) {

			wJson item;
			err = wJsonParseRecurse(&item);
			if (err)
				goto fail;
		}
	}
	else {
		wLogError("unexpected token at %d:%d", tok.line, tok.col);
		return W_PARSE_ERROR;
	}

	return W_SUCCESS;

fail:
	wJsonFree(ret);
	return err;
}

wJson *wJsonParse(const wString *str)
{
	int err;
	wJsonToken tok;
	wJson *ret;

	ret = wJsonParseRecurse();
	if (!ret)
		return NULL;

	wJsonLex(&tok);
	if (tok.type != W_JSON_EOF) {
		wLogError("trailing characters");
		wJsonFree(ret);
		return NULL;
	}

	return ret;
}

void wJsonFree(wJson *json)
{
	if (!json)
		return;

	wStringFree(&json->key);
	wStringFree(&json->str);

	for (int i = 0; i < json->numChildren; ++i) {
		wJsonFree(&json->children[i]);
	}

	wMemFree(json->children);

	json->numChildren = 0;
	json->children = NULL;
}

int wJsonGetType(const wJson *json)
{
	wAssert(json != NULL);
	return json->type;
}

bool wJsonIsNull(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_NULL;
}

bool wJsonIsBool(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_BOOL;
}

bool wJsonIsNumber(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_NUMBER;
}

bool wJsonIsString(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_STRING;
}

bool wJsonIsArray(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_ARRAY;
}

bool wJsonIsObject(const wJson *json)
{
	wAssert(json != NULL);
	return json->type == W_JSON_OBJECT;
}

int wJsonGetSize(const wJson *json)
{
	wAssert(json != NULL);
	return json->numChildren;
}

const wString *wJsonGetKey(const wJson *json, int index)
{
	wAssert(json != NULL);
	wAssert(json->type == W_JSON_OBJECT);
	wAssert(index >= 0 && index < json->numChildren);

	return &json->children[index].key;
}

const wJson *wJsonGetValue(const wJson *json, int index)
{
	wAssert(json != NULL);
	wAssert(json->type == W_JSON_OBJECT || json->type == W_JSON_ARRAY);
	wAssert(index >= 0 && index < json->numChildren);

	return &json->children[index];
}

const wJson *wJsonFindValue(const wJson *json, const wString *key)
{
	wAssert(json != NULL);
	wAssert(key != NULL);
	wAssert(json->type == W_JSON_OBJECT);

	for (int i = 0; i < json->numChildren; ++i) {
		if (wStringEquals(&json->children[i].key, key))
			return &json->children[i];
	}

	return NULL;
}
