#ifndef DRAWING_H
#define DRAWING_H

#include "libraries.h"

extern u32 *texture_pointer;

void DrawRectangle2D(float x, float y, float w, float h, u32 rgba, float l);
void DrawBackground2D(u32 rgba, float l);

void DrawSprite2D(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float layer, u32 color);
void DrawSpriteUI(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, u32 color);
void DrawFull(float x, float y, int wid, int hei, float layer, u32 color);
void DrawSpriteUIR(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, float angle, u32 color);
void DrawSprite2DR(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float angle, float layer,  u32 color);
void SetTexture(int id);
void drawScene();
void LoadTexture();

#endif