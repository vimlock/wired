#include "../include/wired/wFile.h"
#include "../include/wired/wAssert.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"

#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include <unistd.h>

static int wFile_read(void *opaque, size_t size, void *buf, size_t *nread)
{
	wAssert(opaque != NULL);
	wAssert(buf != NULL);

	size_t tmp;

	tmp = fread(buf, 1, size, opaque);
	if (tmp != size) {
		if (ferror(opaque))
			return W_IO_ERROR;
	}

	*nread = tmp;

	return W_SUCCESS;
}

static int wFile_write(void *opaque, size_t size, const void *buf, size_t *nwriten)
{
	wAssert(opaque != NULL);
	wAssert(buf != NULL);

	size_t tmp;

	if (size == 0)
		return W_SUCCESS;

	tmp = fwrite(buf, 1, size, opaque);
	if (tmp != size) {
		return W_IO_ERROR;
	}

	*nwriten = tmp;

	return W_SUCCESS;
}

static void wFile_close(void *opaque)
{
	wAssert(opaque != NULL);

	fclose(opaque);
}

static int wFile_flush(void *opaque)
{
	wAssert(opaque != NULL);

	int err;

	err = fflush(opaque);
	if (err) {
		return W_IO_ERROR;
	}

	return W_SUCCESS;
}

static int wFile_seek(void *opaque, size_t offset, int whence)
{
	wAssert(opaque != NULL);

	int err;

	switch (whence) {
		case W_FILE_START:   whence = SEEK_SET; break;
		case W_FILE_CURRENT: whence = SEEK_CUR; break;
		case W_FILE_END:     whence = SEEK_END; break;
		default:
			return W_INVALID_ARGUMENT;
	}

	err = fseek(opaque, offset, whence);
	if (err)
		return W_IO_ERROR;

	return W_SUCCESS;
}

static size_t wFile_tell(void *opaque)
{
	long ret;

	ret = ftell(opaque);

	return ret;

}

static size_t wFile_size(void *opaque)
{
	int fd;
	int err;

	struct stat st;

	fd = fileno(opaque);
	err = fstat(fd, &st);
	if (err)
		return 0;

	return st.st_size;
}

int wFileAlloc(wFile **file)
{
	wFile *ret = wMemAlloc(sizeof(wFile));
	if (!ret) {
		*file = NULL;
		return W_OUT_OF_MEMORY;
	}

	memset(ret, 0x0, sizeof(wFile));
	*file = ret;
	return W_SUCCESS;
}

void wFileFree(wFile **file)
{
	if (!*file)
		return;

	wMemFree(*file);
	file = NULL;
}

int wFileOpen(wFile *file, const wString *path, unsigned flags)
{
	wAssert(file != NULL);

	char mode[12];
	char *p;

	if ((flags & W_FILE_READ) && (flags & W_FILE_WRITE))
		return W_INVALID_ARGUMENT;

	p = mode;
	if (flags & W_FILE_READ)
		*p++ = 'r';

	if (flags & W_FILE_WRITE)
		*p++ = 'w';

	if (flags & W_FILE_BINARY)
		*p++ = 'b';

	*p++ = '\0';

	FILE *f = fopen(wStringData(path), mode);
	if (!f) {
		return W_IO_ERROR;
	}

	if (flags & W_FILE_READ)
		file->read = wFile_read;

	if (flags & W_FILE_WRITE)
		file->write = wFile_write;

	file->close = wFile_close;

	file->flush = wFile_flush;
	file->tell = wFile_tell;
	file->seek = wFile_seek;
	file->size = wFile_size;

	return W_SUCCESS;
}

void wFileClose(wFile *file)
{
	if (file == NULL)
		return;

	if (file->close)
		file->close(file->opaque);
}

int wFileSeek(wFile *file, size_t offset, int whence)
{
	wAssert(file != NULL);

	if (file->seek) {
		return file->seek(file->opaque, offset, whence);
	}
	else {
		return W_NOT_IMPLEMENTED;
	}
}

int wFileTell(wFile *file, size_t *ret)
{
	wAssert(file != NULL);

	if (file->tell) {
		*ret = file->tell(file->opaque);
		return W_SUCCESS;
	}
	else {
		return W_NOT_IMPLEMENTED;
	}
}

int wFileSize(wFile *file, size_t *ret)
{
	wAssert(file != NULL);

	if (file->size) {
		*ret = file->size(file->opaque);
		return W_SUCCESS;
	}
	else {
		return W_NOT_IMPLEMENTED;
	}
}

int wFileRead(wFile *file, size_t size, void *buf, size_t *nread)
{
	wAssert(file != NULL);

	size_t discard;
	if (!nread)
		nread = &discard;

	if (!file->read)
		return W_INVALID_OPERATION;

	return file->read(file->opaque, size, buf, nread);
}

int wFileWrite(wFile *file, size_t size, void *buf, size_t *nwriten)
{
	wAssert(file != NULL);

	size_t discard;
	if (!nwriten)
		nwriten = &discard;

	if (!file->write)
		return W_INVALID_OPERATION;

	return file->write(file->opaque, size, buf, nwriten);
}

int wFileReadAll(const wString *path, wBuffer *buf)
{
	wAssert(path != NULL);
	wAssert(buf != NULL);

	int err;
	wFile *file;
	size_t size;
	char tmp[4096];
	size_t nread;

	wBufferClear(buf);

	err = wFileAlloc(&file);
	if (err)
		return err;

	err = wFileOpen(file, path, W_FILE_READ | W_FILE_BINARY);
	if (err)
		goto fail;

	err = wFileSize(file, &size);
	if (!err)
		wBufferReserve(buf, size);

	while (true) {
		err = wFileRead(file, sizeof(tmp), tmp, &nread);
		if (err)
			goto fail;

		if (nread == 0)
			break;

		err = wBufferAppend(buf, nread, tmp);
		if (err)
			goto fail;
	}

	wFileClose(file);
	wFileFree(&file);

	return W_SUCCESS;

fail:
	wFileClose(file);
	wFileFree(&file);

	return err;
}

int wFileReadAllText(const wString *path, wString *buf)
{
	wAssert(path != NULL);
	wAssert(buf != NULL);

	int err;
	wFile *file;
	size_t size;
	char tmp[4096];
	size_t nread;

	wStringClear(buf);

	err = wFileAlloc(&file);
	if (err)
		return err;

	err = wFileOpen(file, path, W_FILE_READ | W_FILE_TEXT);
	if (err)
		goto fail;

	err = wFileSize(file, &size);
	if (!err)
		wStringReserve(buf, size);

	while (true) {
		err = wFileRead(file, sizeof(tmp), tmp, &nread);
		if (err)
			goto fail;

		if (nread == 0)
			break;

		err = wStringAppend(buf, nread, tmp);
		if (err)
			goto fail;
	}

	wFileClose(file);
	wFileFree(&file);

	return W_SUCCESS;

fail:
	wFileClose(file);
	wFileFree(&file);

	return err;
}

int wFileWriteAll(const wString *path, const wBuffer *buf)
{
	wAssert(path != NULL);
	wAssert(buf != NULL);

	int err;
	wFile *file;

	err = wFileAlloc(&file);
	if (err)
		return err;

	err = wFileOpen(file, path, W_FILE_WRITE | W_FILE_BINARY);
	if (err)
		goto fail;

	err = wFileWrite(file, buf->size, buf->data, NULL);
	if (err)
		goto fail;

	wFileClose(file);
	wFileFree(&file);

	return W_SUCCESS;

fail:
	wFileClose(file);
	wFileFree(&file);

	return err;
}

int wFileWriteAllText(const wString *path, const wString *buf)
{
	wAssert(path != NULL);
	wAssert(buf != NULL);

	int err;
	wFile *file;

	err = wFileAlloc(&file);
	if (err)
		return err;

	err = wFileOpen(file, path, W_FILE_WRITE | W_FILE_TEXT);
	if (err)
		goto fail;

	err = wFileWrite(file, buf->size, buf->data, NULL);
	if (err)
		goto fail;

	wFileClose(file);
	wFileFree(&file);

	return W_SUCCESS;

fail:
	wFileClose(file);
	wFileFree(&file);

	return err;
}
