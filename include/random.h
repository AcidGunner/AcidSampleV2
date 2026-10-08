#ifndef RANDOM_H
#define RANDOM_H

#include <pngdec/pngdec.h>

typedef struct
{
    short *pcm;
    int pcm_size;
    int freq;
    int stereo;
    int voice;
} SoundEffect;

SoundEffect sfx_majora;

#define BGM_LIST 10
#define MUS_DIR "/dev_hdd0/game/ACIDSAMPL/USRDIR/randomshit/"

#define TOTAL_SHEETS 2
pngData spritesheet[TOTAL_SHEETS];
u32 sprite_off[TOTAL_SHEETS];

const char *bgm[BGM_LIST];
int bgm_start[BGM_LIST];
int bgm_end[BGM_LIST];

int LoadSFX();
void init_sfx();
void init_bgm();

int RandRange(int min, int max);
int max(int min, int max);

void PlayBGM(int id, int inf);
void PlaySFX(SoundEffect *sfx);
void StopSFX(SoundEffect *sfx);

void Load_PNG();

void UpdateFPS();
extern int fps;
void Initialize();

void PS3_BUZZ();
void PS3_LED(int type);

float lerp(int x, int y, float amount);

#endif