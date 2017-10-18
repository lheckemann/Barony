/*-------------------------------------------------------------------------------

	BARONY
	File: sound.cpp
	Desc: FMOD-based implementation of the sound interface.

	Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
	See LICENSE for details.

-------------------------------------------------------------------------------*/

#include "main.hpp"
#include "entity.hpp"
#include "files.hpp"
#include "game.hpp"
#include "player.hpp"
#include "sound.hpp"
#include "net.hpp"
#include <fmod_errors.h>



/* Wrappers for FMOD functions that accept our own types (autogenerated) */

static void System_CreateSound (
	FMOD_SYSTEM* system,
	const char* name_or_data,
	FMOD_MODE mode,
	FMOD_CREATESOUNDEXINFO* exinfo,
	Sound** sound
) {
	FMOD_System_CreateSound(
		system,
		name_or_data,
		mode,
		exinfo,
		reinterpret_cast<FMOD_SOUND**>(sound)
	);
}

static void System_CreateChannelGroup (
	FMOD_SYSTEM* system,
	const char* name,
	ChannelGroup** channelgroup
) {
	FMOD_System_CreateChannelGroup(
		system,
		name,
		reinterpret_cast<FMOD_CHANNELGROUP**>(channelgroup)
	);
}

static void System_PlaySound (
	FMOD_SYSTEM* system,
	FMOD_CHANNELINDEX channelid,
	Sound* sound,
	FMOD_BOOL paused,
	Channel** channel
) {
	FMOD_System_PlaySound(
		system,
		channelid,
		reinterpret_cast<FMOD_SOUND*>(sound),
		paused,
		reinterpret_cast<FMOD_CHANNEL**>(channel)
	);
}

static void Channel_SetPaused (
	Channel* channel,
	FMOD_BOOL paused
) {
	FMOD_Channel_SetPaused(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		paused
	);
}

static void Channel_SetVolume (
	Channel* channel,
	float volume
) {
	FMOD_Channel_SetVolume(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		volume
	);
}

static void Channel_GetVolume (
	Channel* channel,
	float* volume
) {
	FMOD_Channel_GetVolume(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		volume
	);
}

static void Channel_SetChannelGroup (
	Channel* channel,
	ChannelGroup* channelgroup
) {
	FMOD_Channel_SetChannelGroup(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		reinterpret_cast<FMOD_CHANNELGROUP*>(channelgroup)
	);
}

static void Channel_Set3DAttributes (
	Channel* channel,
	FMOD_VECTOR* pos,
	FMOD_VECTOR* vel
) {
	FMOD_Channel_Set3DAttributes(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		pos,
		vel
	);
}

static void Channel_GetCurrentSound (
	Channel* channel,
	Sound** sound
) {
	FMOD_Channel_GetCurrentSound(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		reinterpret_cast<FMOD_SOUND**>(sound)
	);
}

static void Channel_SetMode (
	Channel* channel,
	FMOD_MODE mode
) {
	FMOD_Channel_SetMode(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		mode
	);
}

static void Channel_GetMode (
	Channel* channel,
	FMOD_MODE* mode
) {
	FMOD_Channel_GetMode(
		reinterpret_cast<FMOD_CHANNEL*>(channel),
		mode
	);
}

/** end autogenerated code **/

/* Some manually-generated code (public-interface FMOD wrappers) */
void Sound_Release(Sound* s)
{
	FMOD_Sound_Release(reinterpret_cast<FMOD_SOUND*>(s));
}

void ChannelGroup_SetVolume(ChannelGroup* group, float vol)
{
	FMOD_ChannelGroup_SetVolume(reinterpret_cast<FMOD_CHANNELGROUP *>(group), vol);
}

void Channel_Stop(Channel* chan)
{
	FMOD_Channel_Stop(reinterpret_cast<FMOD_CHANNEL*>(chan));
}

void ChannelGroup_Stop(ChannelGroup* chan)
{
	FMOD_ChannelGroup_Stop(reinterpret_cast<FMOD_CHANNELGROUP*>(chan));
}

bool Channel_IsPlaying (Channel* channel)
{
	FMOD_BOOL playing;
	FMOD_Channel_IsPlaying(reinterpret_cast<FMOD_CHANNEL*>(channel), &playing);
	return playing;
}

unsigned int Channel_GetPosition(Channel* channel)
{
	unsigned int result;
	FMOD_Channel_GetPosition(reinterpret_cast<FMOD_CHANNEL*>(channel), &result, FMOD_TIMEUNIT_MS);
	return result;
}

unsigned int Sound_GetLength(Sound* sound)
{
	unsigned int result;
	FMOD_Sound_GetLength(reinterpret_cast<FMOD_SOUND*>(sound), &result, FMOD_TIMEUNIT_MS);
	return result;
}


FMOD_SYSTEM* fmod_system = NULL;

FMOD_RESULT fmod_result;

int fmod_maxchannels = 100;
int fmod_flags;
void* fmod_extdriverdata;

Sound** sounds = NULL;
Uint32 numsounds = 0;
Sound** minesmusic = NULL;
Sound** swampmusic = NULL;
Sound** labyrinthmusic = NULL;
Sound** ruinsmusic = NULL;
Sound** underworldmusic = NULL;
Sound** hellmusic = NULL;
Sound* intromusic = NULL;
Sound* intermissionmusic = NULL;
Sound* minetownmusic = NULL;
Sound* splashmusic = NULL;
Sound* librarymusic = NULL;
Sound* shopmusic = NULL;
Sound* storymusic = NULL;
Sound** minotaurmusic = NULL;
Sound* herxmusic = NULL;
Sound* templemusic = NULL;
Sound* endgamemusic = NULL;
Sound* devilmusic = NULL;
Sound* escapemusic = NULL;
Sound* introductionmusic = NULL;
bool levelmusicplaying = false;

Channel* music_channel = NULL;
Channel* music_channel2 = NULL;
Channel* music_resume = NULL;

ChannelGroup* sound_group = NULL;
ChannelGroup* music_group = NULL;

float fadein_increment = 0.002f;
float default_fadein_increment = 0.002f;
float fadeout_increment = 0.005f;
float default_fadeout_increment = 0.005f;

bool FMODErrorCheck()
{
	if (no_sound)
	{
		return false;
	}
	if (fmod_result != FMOD_OK)
	{
		printlog("[FMOD Error] Error Code (%d): \"%s\"\n", fmod_result, FMOD_ErrorString(fmod_result)); //Report the FMOD error.
		return true;
	}

	return false;
}

void initSound()
{
	printlog("initializing FMOD...\n");
	fmod_result = FMOD_System_Create(&fmod_system);
	if (FMODErrorCheck())
	{
		printlog("Failed to create FMOD.\n");
		no_sound = true;
	}
	if (!no_sound)
	{
		//FMOD_System_SetOutput(fmod_system, FMOD_OUTPUTTYPE_DSOUND);
		fmod_result = FMOD_System_Init(fmod_system, fmod_maxchannels, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
		if (FMODErrorCheck())
		{
			printlog("Failed to initialize FMOD.\n");
			no_sound = true;
		}
		if (!no_sound)
		{
			System_CreateChannelGroup(fmod_system, NULL, &sound_group);
			if (FMODErrorCheck())
			{
				printlog("Failed to create sound channel group.\n");
				no_sound = true;
			}
			if (!no_sound)
			{
				System_CreateChannelGroup(fmod_system, NULL, &music_group);
				if (FMODErrorCheck())
				{
					printlog("Failed to create music channel group.\n");
					no_sound = true;
				}
			}
		}
	}
}

void sound_update()
{
	if (no_sound)
	{
		return;
	}
	if (!fmod_system)
	{
		return;
	}

	FMOD_VECTOR position, forward, up;
	position.x = -camera.y;
	position.y = -camera.z / 32;
	position.z = -camera.x;

	/*double cosroll = cos(0);
	double cosyaw = cos(camera.ang);
	double cospitch = cos(camera.vang);
	double sinroll = sin(0);
	double sinyaw = sin(camera.ang);
	double sinpitch = sin(camera.vang);

	double rx = sinroll*sinyaw - cosroll*sinpitch*cosyaw;
	double ry = sinroll*cosyaw + cosroll*sinpitch*sinyaw;
	double rz = cosroll*cospitch;*/

	forward.x = 1 * sin(camera.ang);
	forward.y = 0;
	forward.z = 1 * cos(camera.ang);
	/*forward.x = rx;
	forward.y = ry;
	forward.z = rz;*/

	/*rx = sinroll*sinyaw - cosroll*cospitch*cosyaw;
	ry = sinroll*cosyaw + cosroll*cospitch*sinyaw;
	rz = cosroll*sinpitch;*/

	up.x = 0;
	up.y = 1;
	up.z = 0;
	/*up.x = rx;
	up.y = ry;
	up.z = rz;*/

	//FMOD_System_Set3DListenerAttributes(fmod_system, 0, &position, &velocity, &forward, &up);
	FMOD_System_Set3DListenerAttributes(fmod_system, 0, &position, 0, &forward, &up);

	//Fade in the currently playing music.
	if (music_channel)
	{
		FMOD_BOOL playing = Channel_IsPlaying(music_channel);
		if (playing)
		{
			float volume = 1.0f;
			Channel_GetVolume(music_channel, &volume);

			if (volume < 1.0f)
			{
				volume += fadein_increment * 2;
				if (volume > 1.0f)
				{
					volume = 1.0f;
				}
				Channel_SetVolume(music_channel, volume);
			}
		}
	}
	//The following makes crossfading possible. Fade out the last playing music. //TODO: Support for saving music so that it can be resumed (for stuff interrupting like combat music).
	if (music_channel2)
	{
		FMOD_BOOL playing = Channel_IsPlaying(music_channel2);
		if (playing)
		{
			float volume = 0.0f;
			Channel_GetVolume(music_channel2, &volume);

			if (volume > 0.0f)
			{
				//volume -= 0.001f;
				//volume -= 0.005f;
				volume -= fadeout_increment * 2;
				if (volume < 0.0f)
				{
					volume = 0.0f;
				}
				Channel_SetVolume(music_channel2, volume);
			}
		}
	}

	FMOD_System_Update(fmod_system);

	//TODO: Mute sound if focus lost.
}

void deinitSound()
{
	FMOD_System_Close(fmod_system);
	FMOD_System_Release(fmod_system);
	fmod_system = NULL;
}

static FMOD_RESULT openDataFileFmodCB(const char *name, int unicode, unsigned int *filesize, void **handle, void **userdata)
{
	std::cout << name << "\n";
	FILE* f;
	if ( !(f = openDataFile(name, "rb")) )
		return FMOD_ERR_FILE_BAD;
	fseek(f, 0, SEEK_END);
	*filesize = ftell(f);
	rewind(f);
	*handle = f;
	return FMOD_OK;
}

static FMOD_CREATESOUNDEXINFO info = { 0 };

Sound* createSound(const char *name)
{
	if (info.cbsize == 0) {
		info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		info.useropen = openDataFileFmodCB;
	}
	Sound * result = NULL;
	System_CreateSound(fmod_system, name, (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), &info, &result);
	return result;
}

Sound* CreateMusic(const char* name)
{
	if (info.cbsize == 0) {
		info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		info.useropen = openDataFileFmodCB;
	}
	Sound * result = NULL;
	System_CreateSound(fmod_system, name, (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D | FMOD_CREATESTREAM), &info, &result);
	return result;
}

Channel* playSound(Uint32 snd, int vol)
{
	if (no_sound)
	{
		return NULL;
	}
	if (!fmod_system || snd < 0 || snd >= numsounds || !sound_group)
	{
		return NULL;
	}
	if (sounds[snd] == NULL || vol == 0)
	{
		return NULL;
	}
	Channel* channel;
	System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sounds[snd], true, &channel);
	//Faux 3D. Set to 0 and then set the channel's mode to be relative  to the player's head to achieve global sound.
	FMOD_VECTOR position;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	Channel_Set3DAttributes(channel, &position, NULL);
	Channel_SetChannelGroup(channel, sound_group);
	Channel_SetVolume(channel, vol / 128.f);
	Channel_SetMode(channel, FMOD_3D_HEADRELATIVE);
	if (FMODErrorCheck())
	{
		return NULL;
	}
	Channel_SetPaused(channel, false);
	return channel;
}

Channel* playSoundPosLocal(real_t x, real_t y, Uint32 snd, int vol)
{
	if (no_sound)
	{
		return NULL;
	}

	Channel* channel;

	if (intro)
	{
		return NULL;
	}
	if (snd < 0 || snd >= numsounds)
	{
		return NULL;
	}
	if (sounds[snd] == NULL || vol == 0)
	{
		return NULL;
	}

	if (!fmod_system)   //For the client.
	{
		return NULL;
	}

	System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sounds[snd], true, &channel);
	if (FMODErrorCheck())
	{
		return NULL;
	}
	Channel_SetVolume(channel, vol / 128.f);
	FMOD_VECTOR position;
	position.x = -y / 16; //Left/right.
	position.y = 0; //Up/down. //Should be z, but that's not passed. Ignore? Ignoring. Useful for sounds in the floor and ceiling though.
	position.z = -x / 16; //Forward/backward.
	Channel_Set3DAttributes(channel, &position, NULL);
	Channel_SetChannelGroup(channel, sound_group);
	Channel_SetPaused(channel, false);

	return channel;
}

void playmusic(Sound* sound, bool loop, bool crossfade, bool resume)
{
	if (no_sound)
	{
		return;
	}
	fadein_increment = default_fadein_increment;
	fadeout_increment = default_fadeout_increment;
	if (!fmod_system || !sound)
	{
		printlog("Can't play music.\n");
		return;
	}
	if ( resume && music_channel2 )
	{
		Sound* lastmusic = NULL;
		Channel_GetCurrentSound(music_channel2, &lastmusic);
		if ( lastmusic == sound )
		{
			Channel* tempmusic = music_channel;
			music_channel = music_channel2;
			music_channel2 = tempmusic;
		}
		else
		{
			Channel_Stop(music_channel2);
			music_channel2 = music_channel;
			music_channel = NULL;
			System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sound, true, &music_channel);
		}
	}
	else
	{
		Channel_Stop(music_channel2);
		music_channel2 = music_channel;
		music_channel = NULL;
		System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sound, true, &music_channel);
	}
	Channel_SetChannelGroup(music_channel, music_group);
	if (crossfade == true)
	{
		//Start at volume 0 to get louder.
		Channel_SetVolume(music_channel, 0.0f); //Start at 0 then pop up.
	}
	else
	{
		Channel_Stop(music_channel2);
	}
	if (loop == true)
	{
		//Loop the channel.
		FMOD_MODE mode;
		Channel_GetMode(music_channel, &mode);
		//Channel_SetMode(music_channel, (FMOD_MODE)(mode | FMOD_LOOP_NORMAL));
		Channel_SetMode(music_channel, FMOD_LOOP_NORMAL);
		FMODErrorCheck();
	}
	Channel_SetPaused(music_channel, false);
	if (FMODErrorCheck())
	{
		return;
	}
}
