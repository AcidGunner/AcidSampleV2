#include "random.h"
#include "assets.h"
#include "libraries.h"
#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"
#include "spu_soundmodule.bin.h"

float lerp(int x, int y, float amount)
{
	// gamemaker 1.4 function, hehe
	return x + amount * (y - x);
}

int fps = 0;
static int frames = 0;
static u64 last_time = 0;

void UpdateFPS()
{
    u64 now = sysGetSystemTime();
    frames++;

    if(now - last_time >= 1000000ULL)
    {
        fps = frames;
        frames = 0;
        last_time = now;
    }
}

int sex = 2147483647;

void init_bgm()
{
	for(int z = 0; z < BGM_LIST; z++)
	{
		bgm[z] = "";
		bgm_start[z] = 0;
		bgm_end[z] = sex;
	}
	
	bgm[0] = MUS_DIR "ultimate mashup.mp3";
	bgm[1] = MUS_DIR "tenshi_slap.ogg";
	bgm[2] = MUS_DIR "hey its me its oddcard.ogg";
}

int LoadSFX(SoundEffect *sfx, const void *data, int size)
{
    sfx->pcm_size = size * 20;
	sfx->pcm = memalign(32, sfx->pcm_size);

    if(!sfx->pcm) return 0;
    if(DecodeAudio((void *)data,size,sfx->pcm,&sfx->pcm_size,&sfx->freq,&sfx->stereo) != 0)
    {
        free(sfx->pcm);
        sfx->pcm = NULL;
        return 0;
    }

    return 1;
}

void init_sfx()
{
	LoadSFX(&sfx_majora, majora_bin, majora_bin_size);
}

int RandRange(int min, int max)
{
	int rndom;
	rndom = rand() % (max-min+1) + min;
	return rndom;
}

int max(int min, int max)
{
	int biggest;
	biggest = (min > max) ? min : max;
	
	return biggest;
}

void PlayBGM(int id, int inf)
{
	StopAudio();
	FILE *fp = fopen(bgm[id], "rb");
	
	if(!fp)
    {
        printf("Error! No music found.\n");
        return;
    }
	
    PlayAudiofd(fp, 0, inf ? AUDIO_INFINITE_TIME : AUDIO_ONE_TIME);
}

void PlaySFX(SoundEffect *sfx)
{
	int voice = SND_GetFirstUnusedVoice();
    if (voice < 1) return;
	
    SND_SetVoice(voice, sfx->stereo ? VOICE_STEREO_16BIT : VOICE_MONO_16BIT,
        sfx->freq, 0, sfx->pcm, sfx->pcm_size, 255, 255, NULL);
	
	sfx->voice = voice;
}

void StopSFX(SoundEffect *sfx)
{
    if(sfx->voice >= 1)
    {
        SND_StopVoice(sfx->voice);
        sfx->voice = -1;
    }
}

void Load_PNG()
{
	pngLoadFromBuffer(koishi_fumo_bg_bin, koishi_fumo_bg_bin_size, &spritesheet[0]);
	pngLoadFromBuffer(iconz_bin, iconz_bin_size, &spritesheet[1]);
}

void Initialize()
{
	printf("Initialization...\n");
	u32 spu;
	sysSpuImage spu_image;
	sysSpuInitialize(1, 5);
	sysSpuRawCreate(&spu, NULL);
	sysSpuImageImport(&spu_image, spu_soundmodule_bin, 0);
	sysSpuRawImageLoad(spu, &spu_image);
	SND_Init(spu);
}

void PS3_BUZZ()
{
	printf("PS3 Buzzer should be triggered. If not, well, idrk then\n");
	lv2syscall3(392, 0x1004, 0xA, 0x1B6);
}

void syscall2_0()
{
	lv2syscall2(386, 1, 1);
}
void syscall2_1()
{
	lv2syscall2(386, 1, 0);
}
void syscall2_2()
{
	lv2syscall2(386, 2, 1);
}
void syscall2_3()
{
	lv2syscall2(386, 2, 0);
}

void PS3_LED(int type)
{
	if(type == 0)
	{
		printf("PS3 LED should be GREEN.\n");
		syscall2_0();
		syscall2_3();
	}
	else if(type == 1)
	{
		printf("PS3 LED should be RED.\n");
		syscall2_1();
		syscall2_2();
	}
	else
	{
		printf("PS3 LED should be YELLOW.\n");
		syscall2_0();
		syscall2_2();
	}
}