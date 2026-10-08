#include "libraries.h"

#include "text.h"
#include "asimov_ttf_bin.h"

int ttf_inited = 0;

int TTFLoadFont(char * path, void * from_memory, int size_from_memory)
{
    if(!ttf_inited) FT_Init_FreeType(&freetype);
    ttf_inited = 1;

    if(path) {if(FT_New_Face(freetype, path, 0, &face)) return -1;}
		else {if(FT_New_Memory_Face(freetype, from_memory, size_from_memory, 0, &face)) return -1;}

    return 0;
}

void TTFUnloadFont()
{
	FT_Done_FreeType(freetype);
	ttf_inited = 0;
}

void TTF_to_Bitmap(u8 chr, u8 * bitmap, short *w, short *h, short *y_correction)
{
    FT_Set_Pixel_Sizes(face, (*w), (*h));
    FT_GlyphSlot slot = face->glyph;
    memset(bitmap, 0, (*w) * (*h));
    
    uint32_t unicode_cp = chr;
    int is_kazakh_qa = 0;
    
    if      (chr == 0xA1) unicode_cp = 0x04D8;
    else if (chr == 0xA2) unicode_cp = 0x04D9;
    else if (chr == 0xA3) unicode_cp = 0x0492;
    else if (chr == 0xA4) unicode_cp = 0x0493;
	
    else if (chr == 0xA5) { unicode_cp = 0x049A; is_kazakh_qa = 1; }
	else if (chr == 0xA6) { unicode_cp = 0x049B; is_kazakh_qa = 2; }
	
    else if (chr == 0xA7) unicode_cp = 0x04A2;
    else if (chr == 0xAA) unicode_cp = 0x04A3;
	
    else if (chr == 0xAB) unicode_cp = 0x04E8;
    else if (chr == 0xAC) unicode_cp = 0x04E9;
    else if (chr == 0xAD) unicode_cp = 0x04AE;
    else if (chr == 0xAE) unicode_cp = 0x04AF;
    else if (chr == 0xAF) unicode_cp = 0x04B0;
    else if (chr == 0xB0) unicode_cp = 0x04B1;
    else if (chr == 0xB1) unicode_cp = 0x04BA;
    else if (chr == 0xB2) unicode_cp = 0x04BB;
    else if (chr == 0xB8) unicode_cp = 0x0451;
	
    else if (chr >= 192 && chr <= 255) unicode_cp = (uint32_t)chr + 0x0350;
    else if (chr == 168) unicode_cp = 0x0401;
    else if (chr == 184) unicode_cp = 0x0451;
	
    if(FT_Load_Char(face, unicode_cp, FT_LOAD_RENDER )) {(*w) = 0; return;}
    
    int n, m, ww;
    int target_w = *w;
    int target_h = *h;

    *y_correction = target_h - 1 - slot->bitmap_top;
    ww = 0;

    for(n = 0; n < slot->bitmap.rows; n++)
    {
        for (m = 0; m < slot->bitmap.width; m++)
        {
            if(m >= target_w || n >= target_h) continue;
            bitmap[m] = (u8) slot->bitmap.buffer[ww + m];
        }
        bitmap += target_w;
        ww += slot->bitmap.width;
    }

    bitmap -= (target_w * slot->bitmap.rows);

    if (is_kazakh_qa > 0) {
        int base_row = slot->bitmap.rows - 1;
        int right_edge = slot->bitmap.width - 1;
        
        if (right_edge > target_w - 4) right_edge = target_w - 4;

        if (base_row > 0 && right_edge > 0) {
            bitmap[(base_row * target_w) + right_edge + 1] = 255;
            bitmap[(base_row * target_w) + right_edge + 2] = 255;
            if (base_row + 1 < target_h) {
                bitmap[((base_row + 1) * target_w) + right_edge + 2] = 255;
            }
        }
    }

    *w = ((slot->advance.x + 31) >> 6) + ((slot->bitmap_left < 0) ? -slot->bitmap_left : 0);
    
    if (is_kazakh_qa > 0) {
        *w += 3; 
    }
    
    *h = slot->bitmap.rows;
}

void DrawFXString(float x, float y, const char* format, ...)
{
	char buffer[1024];
	
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
	
    DrawString(x, y, buffer);
}

void DrawWrappedText(float x, float y, float line_h, int max_chars, const char* text)
{
    char utf8_buffer[1024];
    char cp1251_buffer[1024];
	
    snprintf(utf8_buffer, sizeof(utf8_buffer), text);
    utf8_to_cp1251(utf8_buffer, cp1251_buffer, sizeof(cp1251_buffer));

    char line[1024];
    char word[256];
    int line_len = 0;
    int word_len = 0;
    int i = 0;

    line[0] = '\0';
    word[0] = '\0';

    while(1)
    {
        char c = cp1251_buffer[i];

        if(c != ' ' && c != '\0' && c != '\n')
        {
            if(word_len < sizeof(word) - 1)
            {
                word[word_len++] = c;
                word[word_len] = '\0';
            }
        }

        if(c == ' ' || c == '\0' || c == '\n')
        {
            if(word_len > 0)
            {
                int needed = word_len;
                if(line_len > 0) needed += 1;

                if(line_len + needed > max_chars)
                {
                    DrawFXString(x, y, "%s", line);
                    y += line_h;

                    strcpy(line, word);
                    line_len = word_len;
                }
                else
                {
                    if(line_len > 0)
                    {
                        strcat(line, " ");
                        line_len++;
                    }
                    strcat(line, word);
                    line_len += word_len;
                }

                word[0] = '\0';
                word_len = 0;
            }

            if(c == '\n')
            {
                DrawFXString(x, y, "%s", line);
                y += line_h;
                line[0] = '\0';
                line_len = 0;
            }

            if(c == '\0') break;
        }

        i++;
    }

    if(line_len > 0) DrawFXString(x, y, "%s", line);
}

void utf8_to_cp1251(const char* src, char* dest, size_t max_len)
{
    size_t i = 0, j = 0;
    while (src[i] != '\0' && j < max_len - 1)
	{
        unsigned char c1 = (unsigned char)src[i];
        unsigned char c2 = (unsigned char)src[i + 1];

        if (c1 == 0xD3 && c2 == 0x98) { dest[j++] = (char)0xA1; i += 2; continue; } // Ә
        if (c1 == 0xD3 && c2 == 0x99) { dest[j++] = (char)0xA2; i += 2; continue; } // ә
        if (c1 == 0xD2 && c2 == 0x92) { dest[j++] = (char)0xA3; i += 2; continue; } // Ғ
        if (c1 == 0xD2 && c2 == 0x93) { dest[j++] = (char)0xA4; i += 2; continue; } // ғ
        
        if (c1 == 0xD2 && c2 == 0x9A) { dest[j++] = (char)0xA5; i += 2; continue; } // Қ
        if (c1 == 0xD2 && c2 == 0x9B) { dest[j++] = (char)0xA6; i += 2; continue; } // қ
        
        if (c1 == 0xD2 && c2 == 0xA2) { dest[j++] = (char)0xA7; i += 2; continue; } // Ң
        if (c1 == 0xD2 && c2 == 0xA3) { dest[j++] = (char)0xAA; i += 2; continue; } // ң
        if (c1 == 0xD3 && c2 == 0xA8) { dest[j++] = (char)0xAB; i += 2; continue; } // Ө
        if (c1 == 0xD3 && c2 == 0xA9) { dest[j++] = (char)0xAC; i += 2; continue; } // ө
        if (c1 == 0xD2 && c2 == 0xAE) { dest[j++] = (char)0xAD; i += 2; continue; } // Ұ
        if (c1 == 0xD2 && c2 == 0xAF) { dest[j++] = (char)0xAE; i += 2; continue; } // ұ
        if (c1 == 0xD2 && c2 == 0xB0) { dest[j++] = (char)0xAF; i += 2; continue; } // Ү
        if (c1 == 0xD2 && c2 == 0xB1) { dest[j++] = (char)0xB0; i += 2; continue; } // ү
        if (c1 == 0xD2 && c2 == 0xBA) { dest[j++] = (char)0xB1; i += 2; continue; } // Һ
        if (c1 == 0xD2 && c2 == 0xBB) { dest[j++] = (char)0xB2; i += 2; continue; } // һ
        if (c1 == 0xD0 && c2 == 0x86) { dest[j++] = (char)0x49; i += 2; continue; } // І
        if (c1 == 0xD1 && c2 == 0x96) { dest[j++] = (char)0x69; i += 2; continue; } // і

        if (c1 == 0xD0 && c2 == 0x81) { dest[j++] = (char)0xA8; i += 2; continue; } // Ё
        if (c1 == 0xD1 && c2 == 0x91) { dest[j++] = (char)0xB8; i += 2; continue; } // ё
        
        if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0xBF) { dest[j++] = (char)(c2 + 0x30); i += 2; continue; }
        if (c1 == 0xD1 && c2 >= 0x80 && c2 <= 0x8F) { dest[j++] = (char)(c2 + 0x70); i += 2; continue; }

        dest[j++] = src[i++];
    }
    dest[j] = '\0';
}

void MyDrawFormatString(float x, float y, const char* format, ...)
{
    char utf8_buffer[1024];
    char cp1251_buffer[1024];
	
    va_list args;
    va_start(args, format);
    vsnprintf(utf8_buffer, sizeof(utf8_buffer), format, args);
    va_end(args);
	
    utf8_to_cp1251(utf8_buffer, cp1251_buffer, sizeof(cp1251_buffer));
    DrawString(x, y, cp1251_buffer);
}

void DrawFString(int wrap, float x, float y, u32 textColor, int BW, int size, const char *fmt, ...)
{
    char buffer[1024];
	u32 shadowColor = 0x000000ff;
	
	if(BW) shadowColor = 0xffffffff;
	
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    SetFontColor(shadowColor, 0x00000000);
	
	if(wrap==0) MyDrawFormatString(x + 2, y + 2, buffer);
	else DrawWrappedText(x + 2, y + 2, size, wrap, buffer);
	
    SetFontColor(textColor, 0x00000000);
	
	if(wrap==0) MyDrawFormatString(x, y, buffer);
	else DrawWrappedText(x, y, size, wrap, buffer);
}

int GetFontWidth(const char *text, int fz)
{
    return (int)(strlen(text) * fz * 0.55f);
}

void DrawTextAligned(int wrap, float x1, float x2, float y, int align, u32 tc, int bw, int fsize, int size, const char *text, ...)
{
    char bufferz[1024];
    va_list args;
    va_start(args, text);
    vsnprintf(bufferz, sizeof(bufferz), text, args);
    va_end(args);
	
    int width = GetFontWidth(bufferz, fsize);
    float x;

    if(align == 0) x = x1;
    else if(align == 1) x = x1 + ((x2 - x1) - width) / 2.0f;
    else x = x2 - width;

	DrawFString(wrap, x, y, tc, bw, size, bufferz);
}
