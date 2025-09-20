#pragma once

#include <stdbool.h>

#include "wBuffer.h"
#include "wString.h"

enum wFileFlags
{
	W_FILE_BINARY = 0 << 0,
	W_FILE_TEXT   = 1 << 0,
	W_FILE_READ   = 1 << 1,
	W_FILE_WRITE  = 1 << 2,
};

enum wFileWhence
{
	W_FILE_START,
	W_FILE_END,
	W_FILE_CURRENT,
};


typedef struct _wFile
{
	int (*read)(void *opaque, size_t size, void *buf, size_t *nread);
	int (*write)(void *opaque, size_t size, const void *buf, size_t *nwriten);

	void (*close)(void *opaque);

	int (*flush)(void *opaque);

	int (*seek)(void *opaque, size_t offset, int whence);

	size_t (*tell)(void *opaque);
	size_t (*size)(void *opaque);

	void *opaque;
} wFile;

int wFileAlloc(wFile **file);
void wFileFree(wFile **file);

int wFileOpen(wFile *file, const wString *path, unsigned flags);
void wFileClose(wFile *file);
int wFileSeek(wFile *file, size_t offset, int whence);
int wFileTell(wFile *file, size_t *ret);
int wFileSize(wFile *file, size_t *ret);

int wFileRead(wFile *file, size_t size, void *buf, size_t *nread);
int wFileWrite(wFile *file, size_t size, void *buf, size_t *nwriten);

int wFileReadAll(const wString *path, wBuffer *buf);
int wFileReadAllText(const wString *path, wString *buf);
int wFileWriteAll(const wString *path, const wBuffer *buf);
int wFileWriteAllText(const wString *path, const wString *buf);

int wFileMakeDirectory(const wString *path);
bool wFileExists(const wString *path);
bool wFileIsDirectory(const wString *path);
int wFileRemove(const wString *path);
