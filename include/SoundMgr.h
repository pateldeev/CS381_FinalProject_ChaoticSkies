#ifndef __SOUNDMGR_H_
#define __SOUNDMGR_H_

#include <AL/al.h>
#include <AL/alc.h>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include <stdio.h>
#include <string>

#include "Mgr.h"

#define MAX_AUDIO_BUFFERS 64
#define MAX_AUDIO_SOURCES 64
#define MAX_FILENAME_LENGTH 512

class SoundMgr: Mgr {
public:
	SoundMgr(Engine *engine);
	virtual ~SoundMgr(void);

public:
	virtual void Init(void) override final;
	virtual void LoadLevel(void) override final;
	virtual void Tick(float dt) override final;
	virtual void Stop(void) override final;

public:
	void SetAudioPath(const char* path);

	// Aquire an Audio Source
	// filename = pass in the sound file to play for this source (ex. "myfile.wav")
	// audioId   = returns the AudioSource identifier you will need for the PlayAudioSource();
	bool LoadAudio(const std::string &file_name, unsigned int* audioId, bool loop = true);
	bool ReleaseAudio(unsigned int audioID);

	// Returns true if the audio is started from the beginning
	// false if error or if already playing
	bool PlayAudio(unsigned int audioId, bool forceRestart);
	bool StopAudio(unsigned int audioID);
	bool StopAllAudio(void);

	bool PauseAudio(unsigned int audioID);
	bool PauseAllAudio(void);
	bool ResumeAudio(unsigned int audioID);
	bool ResumeAllAudio(void);

	bool SetSoundPosition(unsigned int audioID, const Ogre::Vector3 &position);
	bool SetSoundPosition(unsigned int audioID, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Vector3 &direction);

	bool SetSound(unsigned int audioID, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Vector3 &direction,
		float maxDistance, bool playNow, bool forceRestart, float minGain);

	bool SetListenerPosition(const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Quaternion &orientation);

	bool GetIsSoundOn(void) const;
	std::string GetAvailableDevicesList(void) const;

private:
	int LocateAudioBuffer(const std::string &file_name); //Function to check if the soundFile is already loaded into a buffer
	int LoadAudioInToSystem(const std::string &file_name);
	bool LoadOGG(const std::string &file_name, ALuint &pDestAudioBuffer);

	bool CheckALError(void) const;
	bool CheckALError(const std::string & pMsg) const;

private:
	bool m_initialised;
	bool m_sound_on;
	bool m_EAX_Present;

	ALCdevice* m_sound_device;
	ALCcontext* m_sound_context;

	std::string m_audio_path;

	// Audio buffers
	unsigned int m_audio_buffers_in_use_count;
	unsigned int m_audio_buffers[MAX_AUDIO_BUFFERS];
	bool m_audio_buffer_in_use[MAX_AUDIO_BUFFERS];
	char m_audio_buffer_file_name[MAX_AUDIO_BUFFERS][MAX_FILENAME_LENGTH];

	// Audio sources
	unsigned int m_audio_sources_in_use_count;
	unsigned int m_audio_sources[MAX_AUDIO_SOURCES];
	bool m_audio_source_in_use[MAX_AUDIO_SOURCES];

	ALfloat m_position[3];
	ALfloat m_velocity[3];
	ALfloat m_orientation[6];
};

#endif
