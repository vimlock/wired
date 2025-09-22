#include "../include/wired/wImage.h"
#include "../include/wired/wError.h"
#include "../include/wired/wMemory.h"
#include "../include/wired/wString.h"
#include "../include/wired/wLog.h"

#include <png.h>

#include <stdio.h>
#include <stdlib.h>

int wImagePNGLoad(wImage *img, const wString *path)
{
    int width, height;
    int color_type, bit_depth;

    FILE *fp = fopen(wStringData(path), "rb");
    if (!fp) {
        wLogError("Could not open file: %s", wStringData(path));
        return W_INVALID_OPERATION;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        return W_INVALID_OPERATION;

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        return W_INVALID_OPERATION;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return W_INVALID_OPERATION;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    bit_depth = png_get_bit_depth(png, info);
    color_type = png_get_color_type(png, info);
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    wImageInit(img, width, height, W_IMAGE_RGBA8);

    png_bytep *row_pointers = (png_bytep *)wMemAlloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
        row_pointers[y] = (png_byte *)wImageData(img) + y * png_get_rowbytes(png, info);

    png_read_image(png, row_pointers);

    wMemFree(row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);

    return W_SUCCESS;
}