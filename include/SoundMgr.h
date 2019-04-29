#ifndef __SOUNDMGR_H_
#define __SOUNDMGR_H_

#include <AL/al.h>
#include <AL/alc.h>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include <stdio.h>
#include <string>

#include <unordered_map>

#include "Mgr.h"

#define MAX_AUDIO_BUFFERS 256
#define MAX_AUDIO_SOURCES 256
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
	void SetAudioPath(const std::string &path);

	// filen_name = pass in the sound file to play for this source (ex. "myfile.wav")
	// audio_name = name to associate identifier - you will need for the PlayAudioSource();
	bool LoadAudio(const std::string &file_name, const std::string &audio_name, bool loop = false);
	bool ReleaseAudio(const std::string &audio_name);

	// Returns true if the audio is started from the beginning
	// false if error or if already playing
	bool PlayAudio(const std::string &audio_name, bool forceRestart = true);
	bool StopAudio(const std::string &audio_name);
	bool StopAllAudio(void);

	bool PauseAudio(const std::string &audio_name);
	bool PauseAllAudio(void);
	bool ResumeAudio(const std::string &audio_name);
	bool ResumeAllAudio(void);

	bool SetSoundPosition(const std::string &audio_name, const Ogre::Vector3 &position);
	bool SetSoundPosition(const std::string &audio_name, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity,
		const Ogre::Vector3 &direction);

	bool SetSound(const std::string &audio_name, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Vector3 &direction,
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

	std::unordered_map<std::string, unsigned int> m_sources; //maps audio names to sources

	ALfloat m_position[3];
	ALfloat m_velocity[3];
	ALfloat m_orientation[6];
};

#endif
