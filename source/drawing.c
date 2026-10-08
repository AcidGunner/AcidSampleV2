#include "drawing.h"
#include "text.h"
#include "random.h"
#include "asimov_ttf_bin.h"

u32 c_white = 0xffffffff;
u32 c_black = 0x000000ff;
u32 c_green = 0x00ff00ff;
u32 c_red = 0xff0000ff;
u32 c_blue = 0x0000ffff;
u32 c_yellow = 0xffff00ff;
u32 c_pink = 0xff00ffff;
u32 c_cyan = 0x00ffffff;

void DrawBackground2D(u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(0, 0, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(1000, 0, l);
    tiny3d_VertexPos(1000, 1000, l);
    tiny3d_VertexPos(0, 1000, l);
    tiny3d_End();
}

void DrawRectangle2D(float x, float y, float w, float h, u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(x+w, y, l);
    tiny3d_VertexPos(x+w, y+h, l);
    tiny3d_VertexPos(x, y+h, l);
    tiny3d_End();
}

void DrawSprite2D(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float layer, u32 color)
{
	int cols = wid0 / wid1;
    int col = id % cols;
    int row = id / cols;
    float u0 = (float)(col * wid1) / wid0;
    float v0 = (float)(row * hei1) / hei0;
    float u1 = (float)((col + 1) * wid1) / wid0;
    float v1 = (float)((row + 1) * hei1) / hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid2, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid2, y+hei2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei2, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void DrawFull(float x, float y, int wid, int hei, float layer, u32 color)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(0, 0);
    tiny3d_VertexPos(x+wid, y, layer);
    tiny3d_VertexTexture(0.99f, 0);
    tiny3d_VertexPos(x+wid, y+hei, layer);
    tiny3d_VertexTexture(0.99f, 0.99f);
    tiny3d_VertexPos(x, y+hei, layer);
    tiny3d_VertexTexture(0, 0.99f);

    tiny3d_End();
}

void DrawSpriteUI(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, u32 color)
{
    float u0 = (float)wid1 / (float)wid0;
    float v0 = (float)hei1 / (float)hei0;
    float u1 = (float)wid2 / (float)wid0;
    float v1 = (float)hei2 / (float)hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid3, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid3, y+hei3, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei3, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void DrawSpriteUIR(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, float angle, u32 color)
{
    float u0 = (float)wid1 / (float)wid0;
    float v0 = (float)hei1 / (float)hei0;
    float u1 = (float)wid2 / (float)wid0;
    float v1 = (float)hei2 / (float)hei0;
	
    float cx = x + wid3 * 0.5f;
    float cy = y + hei3 * 0.5f;
    float hw = wid3 * 0.5f;
    float hh = hei3 * 0.5f;

    float rad = angle * (M_PI / 180.0f);
    float c = cosf(rad);
    float s = sinf(rad);

    float x0 = (-hw * c) - (-hh * s);
    float y0 = (-hw * s) + (-hh * c);
    float x1 = ( hw * c) - (-hh * s);
    float y1 = ( hw * s) + (-hh * c);
    float x2 = ( hw * c) - ( hh * s);
    float y2 = ( hw * s) + ( hh * c);
    float x3 = (-hw * c) - ( hh * s);
    float y3 = (-hw * s) + ( hh * c);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(cx + x0, cy + y0, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(cx + x1, cy + y1, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(cx + x2, cy + y2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(cx + x3, cy + y3, layer);
    tiny3d_VertexTexture(u0, v1);
	
    tiny3d_End();
}

void DrawSprite2DR(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float angle, float layer, u32 color)
{
    int cols = wid0 / wid1;
    int col = id % cols;
    int row = id / cols;

    float u0 = (float)(col * wid1) / wid0;
    float v0 = (float)(row * hei1) / hei0;
    float u1 = (float)((col + 1) * wid1) / wid0;
    float v1 = (float)((row + 1) * hei1) / hei0;

    float cx = x + wid2 * 0.5f;
    float cy = y + hei2 * 0.5f;
    float hw = wid2 * 0.5f;
    float hh = hei2 * 0.5f;

    float rad = angle * (M_PI / 180.0f);
    float c = cosf(rad);
    float s = sinf(rad);

    float x0 = (-hw * c) - (-hh * s);
    float y0 = (-hw * s) + (-hh * c);
    float x1 = ( hw * c) - (-hh * s);
    float y1 = ( hw * s) + (-hh * c);
    float x2 = ( hw * c) - ( hh * s);
    float y2 = ( hw * s) + ( hh * c);
    float x3 = (-hw * c) - ( hh * s);
    float y3 = (-hw * s) + ( hh * c);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(cx + x0, cy + y0, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(cx + x1, cy + y1, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(cx + x2, cy + y2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(cx + x3, cy + y3, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void SetTexture(int id)
{
	tiny3d_SetTexture(0, sprite_off[id], spritesheet[id].width, spritesheet[id].height,
		spritesheet[id].pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_NEAREST);
}

void drawScene()
{
	static int framexd = 0;
	int offset_x = 10;
	
	framexd++;
    tiny3d_Project2D();
	
    SetFontColor(c_white, c_black);
    SetCurrentFont(0);
	
	SetTexture(0);
	DrawFull(0, 0, 916, 582, 1, 0xccccccff);
	
	SetFontSize(28, 32);
	DrawFString(0, offset_x, 10, c_white, 0, 28, "Welcome to AcidSample v2!");
	
	SetFontSize(16, 16);
	DrawFString(0, offset_x, 60, c_white, 0, 16, "I don't know what to include here, lel.");
	DrawFString(0, offset_x, 80, c_white, 0, 16, "Я поддерживаю кириллицу!!! Ура!");
	
	DrawFString(0, offset_x, 120, c_white, 0, 16, "Pressing CROSS plays \"Lunar Abyss x Bad Apple!! x Cry for Me\" mashup (made by yours truly!) [AUDIOPLAYER/NOLOOP/.mp3]");
	DrawFString(0, offset_x, 140, c_white, 0, 16, "Pressing CIRCLE plays music, that was used in old flash game where you slap Tenshi. [AUDIOPLAYER/NOLOOP/.ogg]");
	DrawFString(0, offset_x, 160, c_white, 0, 16, "Pressing TRIANGLE plays \"Hey, it's me, it's oddcard\" mashup (made by yours truly!) [AUDIOPLAYER/LOOP/.ogg]");
	DrawFString(0, offset_x, 180, c_white, 0, 16, "Pressing SQUARE plays Majora's Mask Tower clock bell SFX [SOUNDLIB]");
	DrawFString(0, offset_x, 220, c_white, 0, 16, "Pressing L1 makes PS3 beep 3 times.");
	DrawFString(0, offset_x, 260, c_white, 0, 16, "Pressing START returns to XMB.");
	DrawFString(0, offset_x+512, 220, c_white, 0, 16, "Pressing R1 makes PS3 LED Green.");
	DrawFString(0, offset_x+512, 240, c_white, 0, 16, "Pressing R2 makes PS3 LED Red.");
	DrawFString(0, offset_x+512, 260, c_white, 0, 16, "Pressing R3 makes PS3 LED Yellow.");
	
	SetTexture(1);
	DrawSprite2DR(0, offset_x, 350, 128, 128, 128, 128, 384, 128, framexd, 0, c_white);
	
	DrawSprite2D(1, offset_x+128+10, 350, 128, 128, 128, 128, 384, 128, 0, c_white);
	float width = sinf((float)framexd / 100) * 32;
	DrawSprite2D(2, offset_x+256+10, 382-width, 128, 96+width, 128, 128, 384, 128, 0, c_white);
	float width2 = (float)RandRange(0, 100) / 100 * 32;
	DrawSprite2D(2, offset_x+384+10, 382-width2, 128, 96+width2, 128, 128, 384, 128, 0, c_white);
	
	DrawFString(0, offset_x+5, 500, c_white, 0, 16, "DrawSprite2DR");
	DrawFString(0, offset_x+128+15, 500, c_white, 0, 16, "DrawSprite2D");
	DrawFString(0, offset_x+256+15, 500, c_white, 0, 16, "sinf test?");
	DrawFString(0, offset_x+384+15, 500, c_white, 0, 16, "RandRange(0, 100);");
}

u32 *texture_pointer = NULL;

void LoadTexture()
{
	int g;
	
    u32 * texture_mem = tiny3d_AllocTexture(48*1024*1024);
    if(!texture_mem) return;
    texture_pointer = texture_mem;
	
    Load_PNG();
	
	ResetFont();
    TTFLoadFont(NULL, (void *) asimov_ttf_bin, asimov_ttf_bin_size);
    texture_pointer = (u32 *) AddFontFromTTF((u8 *) texture_pointer, 32, 255, 64, 64, TTF_to_Bitmap);
    TTFUnloadFont();
	
    for(g = 0; g < TOTAL_SHEETS; g++)
	{
		memcpy(texture_pointer, spritesheet[g].bmp_out, spritesheet[g].pitch * spritesheet[g].height);
		sprite_off[g] = tiny3d_TextureOffset(texture_pointer);
		free(spritesheet[g].bmp_out);
		spritesheet[g].bmp_out = NULL;
		
		texture_pointer += ((spritesheet[g].pitch * spritesheet[g].height + 15) & ~15) / 4;
    }
}