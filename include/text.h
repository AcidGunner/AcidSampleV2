#ifndef TEXT_H
#define TEXT_H

#include <libfont.h>
#include <ft2build.h>
#include <freetype/freetype.h> 
#include <freetype/ftglyph.h>

FT_Library freetype;
FT_Face face;

int TTFLoadFont(char * path, void * from_memory, int size_from_memory);
void TTFUnloadFont();
void TTF_to_Bitmap(u8 chr, u8 * bitmap, short *w, short *h, short *y_correction);
void DrawWrappedText(float x, float y, float line_h, int max_chars, const char *text);
void utf8_to_cp1251(const char* src, char* dest, size_t max_len);
void DrawFString(int wrap, float x, float y, u32 textColor, int BW, int size, const char *fmt, ...);
void DrawTextAligned(int wrap, float x1, float x2, float y, int align, u32 tc, int bw, int fsize, int size, const char *text, ...);

#endif