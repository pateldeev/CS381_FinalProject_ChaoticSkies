#include <vorbis/vorbisfile.h>

#include "Engine.h"

#include "GfxMgr.h"
#include "InputMgr.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "EntityMgr.h"
#include "UIMgr.h"

SoundMgr::SoundMgr(Engine *engine) :
	Mgr(engine), m_initialised(false), m_sound_on(false), m_EAX_Present(false), m_sound_device(0), m_sound_context(0), m_audio_path(""),
	m_audio_buffers_in_use_count(0), m_audio_sources_in_use_count(0) {
	for (unsigned int i = 0; i < MAX_AUDIO_SOURCES; ++i) {
		m_audio_sources[i] = 0;
		m_audio_source_in_use[i] = false;
	}

	for (unsigned int i = 0; i < MAX_AUDIO_BUFFERS; ++i) {
		m_audio_buffers[i] = 0;
		strcpy(m_audio_buffer_file_name[i], "--");
		m_audio_buffer_in_use[i] = false;
	}

	// Initial position of the listener
	m_position[0] = m_position[1] = m_position[2] = 0.0;

	// Initial velocity of the listener
	m_velocity[0] = m_velocity[1] = m_velocity[2] = 0.0;

	// Initial orientation of the listener = direction + direction up
	m_orientation[0] = 0.0;
	m_orientation[1] = 0.0;
	m_orientation[2] = -1.0;
	m_orientation[3] = 0.0;
	m_orientation[4] = 1.0;
	m_orientation[5] = 0.0;
}

SoundMgr::~SoundMgr(void) {
	Stop();
}

void SoundMgr::Init(void) {
	// It's an error to initialise twice OpenAl
	if (m_initialised)
		return;

	// Open an audio device
	m_sound_device = alcOpenDevice(NULL); // TODO ((ALubyte*) "DirectSound3D");
	// mSoundDevice = alcOpenDevice( "DirectSound3D" );

	// Check for errors
	if (!m_sound_device) {
		printf("SoundManager::init error : No sound device.\n");
		return;
	}

	m_sound_context = alcCreateContext(m_sound_device, NULL);
	//   if ( checkAlError() || !mSoundContext ) // TODO seems not to work! why ?
	if (!m_sound_context) {
		printf("SoundManager::init error : No sound context.\n");
		return;
	}

	// Make the context current and active
	alcMakeContextCurrent(m_sound_context);
	if (CheckALError("Init()")) {
		printf("SoundManager::init error : could not make sound context current and active.\n");
		return;
	}

	// Create the Audio Buffers
	alGenBuffers(MAX_AUDIO_BUFFERS, m_audio_buffers);
	if (CheckALError("init::alGenBuffers:"))
		return;

	// Generate Sources
	alGenSources(MAX_AUDIO_SOURCES, m_audio_sources);
	if (CheckALError("init::alGenSources :"))
		return;

	// Setup the initial listener parameters
	// -> location
	alListenerfv(AL_POSITION, m_position);

	// -> velocity
	alListenerfv(AL_VELOCITY, m_velocity);

	// -> orientation
	alListenerfv(AL_ORIENTATION, m_orientation);

	// Gain
	alListenerf(AL_GAIN, 1.0);

	// Initialise Doppler
	alDopplerFactor(1.0); // 1.2 = exaggerate the pitch shift by 20%
	alDopplerVelocity(343.0f); // m/s this may need to be scaled at some point

	// Ok
	m_initialised = true;
	m_sound_on = true;

	printf("SoundManager initialized.\n\n");
}

void SoundMgr::LoadLevel(void) {
	SetAudioPath(".\\");
}

void SoundMgr::Tick(float dt) {

}

void SoundMgr::Stop(void) {
	if (m_initialised) {
		// Delete the sources and buffers
		alDeleteSources(MAX_AUDIO_SOURCES, m_audio_sources);
		alDeleteBuffers(MAX_AUDIO_BUFFERS, m_audio_buffers);

		// Destroy the sound context and device
		m_sound_context = alcGetCurrentContext();
		m_sound_device = alcGetContextsDevice(m_sound_context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_sound_context);
		if (m_sound_device)
			alcCloseDevice(m_sound_device);

		printf("SoudManager Stopped.\n");
		m_initialised = false;
	}
}

void SoundMgr::SetAudioPath(const std::string &path) {
	m_audio_path = path;
}

// Attempts to aquire an empty audio source and assign it given name
// via AudioSourceID. This will lock the source
/*****************************************************************************/
bool SoundMgr::LoadAudio(const std::string &file_name, const std::string &audio_name, bool loop) {
	if (file_name.empty() || file_name.length() > MAX_FILENAME_LENGTH)
		return false;

	if (m_audio_sources_in_use_count == MAX_AUDIO_SOURCES)
		return false;   // out of Audio Source slots!

	if (!m_sources.insert(std::make_pair(audio_name, 0)).second)
		return false; //audio by name already exists

	int bufferID = -1;   // Identity of the Sound Buffer to use
	int sourceID = -1;   // Identity of the Source Buffer to use

	alGetError();    // Clear Error Code

	// Check and see if the pSoundFile is already loaded into a buffer
	bufferID = LocateAudioBuffer(file_name);
	if (bufferID < 0) {
		// The sound file isn't loaded in a buffer, lets attempt to load it on the fly
		bufferID = LoadAudioInToSystem(file_name);
		if (bufferID < 0)
			return false;   // failed!
	}

	// If you are here, the sound the requester wants to reference is in a buffer.
	// Now, we need to find a free Audio Source slot in the sound system
	sourceID = 0;
	while (m_audio_source_in_use[sourceID])
		++sourceID;

	// When you are here, 'mSourceID' now represents a free Audio Source slot
	// The free slot may not be at the end of the array but in the middle of it.
	m_sources[audio_name] = sourceID;  // return the Audio Source ID to the caller
	m_audio_source_in_use[sourceID] = true; // mark this Source slot as in use
	++m_audio_sources_in_use_count;    // bump the 'in use' counter

	// Now inform OpenAL of the sound assignment and attach the audio buffer
	// to the audio source
	alSourcei(m_audio_sources[sourceID], AL_BUFFER, m_audio_buffers[bufferID]);

	// Steven : Not in the original code !!!!!
	alSourcei(m_audio_sources[sourceID], AL_LOOPING, loop);

	if (CheckALError("loadSource()::alSourcei"))
		return false;

	return true;
}

bool SoundMgr::ReleaseAudio(const std::string &audio_name) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	alSourceStop(m_audio_sources[audioID]);
	m_audio_source_in_use[audioID] = false;
	--m_audio_sources_in_use_count;

	m_sources.erase(audio_name);

	return true;
}

bool SoundMgr::PlayAudio(const std::string &audio_name, bool forceRestart) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	// Make sure the audio source ident is valid and usable
	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	int sourceAudioState = 0;

	alGetError();

	// Are we currently playing the audio source?
	alGetSourcei(m_audio_sources[audioID], AL_SOURCE_STATE, &sourceAudioState);

	if (sourceAudioState == AL_PLAYING) {
		if (forceRestart)
			StopAudio(audio_name);
		else
			return false; // Not forced, so we don't do anything
	}

	alSourcePlay(m_audio_sources[audioID]);
	if (CheckALError("playAudio::alSourcePlay: "))
		return false;

	return true;
}

bool SoundMgr::StopAudio(const std::string &audio_name) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	// Make sure the audio source ident is valid and usable
	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	alGetError();

	alSourceStop(m_audio_sources[audioID]);

	if (CheckALError("stopAudio::alSourceStop "))
		return false;

	return true;
}

bool SoundMgr::StopAllAudio(void) {
	if (m_audio_sources_in_use_count >= MAX_AUDIO_SOURCES)
		return false;

	alGetError();

	for (const std::pair<std::string, unsigned int> &s : m_sources)
		StopAudio(s.first);

	if (CheckALError("stopAllAudio::alSourceStop "))
		return false;

	return true;
}

bool SoundMgr::PauseAudio(const std::string &audio_name) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	// Make sure the audio source ident is valid and usable
	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	alGetError();

	alSourcePause(m_audio_sources[audioID]);

	if (CheckALError("pauseAudio::alSourceStop "))
		return false;

	return true;
}

bool SoundMgr::PauseAllAudio(void) {
	if (m_audio_sources_in_use_count >= MAX_AUDIO_SOURCES)
		return false;

	alGetError();

	alSourcePausev(m_audio_sources_in_use_count, m_audio_sources);

	if (CheckALError("pauseAllAudio::alSourceStop "))
		return false;

	return true;
}

// We could use playAudio instead !
bool SoundMgr::ResumeAudio(const std::string &audio_name) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	// Make sure the audio source ident is valid and usable
	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	alGetError();

	// If the sound was paused the sound will resume, else it will play from
	// the beginning !
	// TODO No check for forced restart. Verify if it is what you want ?
	alSourcePlay(m_audio_sources[audioID]);

	if (CheckALError("resumeAudio::alSourceStop "))
		return false;

	return true;
}

bool SoundMgr::ResumeAllAudio(void) {
	if (m_audio_sources_in_use_count >= MAX_AUDIO_SOURCES)
		return false;

	alGetError();

	int sourceAudioState = 0;

	for (const std::pair<std::string, unsigned int> &s : m_sources) {
		alGetSourcei(m_audio_sources[s.second], AL_SOURCE_STATE, &sourceAudioState);

		if (sourceAudioState == AL_PAUSED)
			ResumeAudio(s.first);
	}

	if (CheckALError("resumeAllAudio::alSourceStop "))
		return false;

	return true;
}

bool SoundMgr::SetSoundPosition(const std::string &audio_name, const Ogre::Vector3 &position) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	// Set the position
	ALfloat pos[3] = { position.x, position.y, position.z };

	alSourcefv(m_audio_sources[audioID], AL_POSITION, pos);

	if (CheckALError("setSound::alSourcefv:AL_POSITION"))
		return false;

	return true;
}

bool SoundMgr::SetSoundPosition(const std::string &audio_name, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity,
	const Ogre::Vector3 &direction) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	// Set the position
	ALfloat pos[3] = { position.x, position.y, position.z };

	alSourcefv(m_audio_sources[audioID], AL_POSITION, pos);

	if (CheckALError("setSound::alSourcefv:AL_POSITION"))
		return false;

	// Set the veclocity
	ALfloat vel[3] = { velocity.x, velocity.y, velocity.z };

	alSourcefv(m_audio_sources[audioID], AL_VELOCITY, vel);

	if (CheckALError("setSound::alSourcefv:AL_VELOCITY"))
		return false;

	// Set the direction
	ALfloat dir[3] = { velocity.x, velocity.y, velocity.z };

	alSourcefv(m_audio_sources[audioID], AL_DIRECTION, dir);

	if (CheckALError("setSound::alSourcefv:AL_DIRECTION"))
		return false;

	return true;
}

bool SoundMgr::SetSound(const std::string &audio_name, const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Vector3 &direction,
	float maxDistance, bool playNow, bool forceRestart, float minGain) {
	if (m_sources.find(audio_name) == m_sources.end())
		return false; //no audio with given name
	unsigned int audioID = m_sources[audio_name];

	if (audioID >= MAX_AUDIO_SOURCES || !m_audio_source_in_use[audioID])
		return false;

	// Set the position
	ALfloat pos[] = { position.x, position.y, position.z };

	alSourcefv(m_audio_sources[audioID], AL_POSITION, pos);

	if (CheckALError("setSound::alSourcefv:AL_POSITION"))
		return false;

	// Set the veclocity
	ALfloat vel[] = { velocity.x, velocity.y, velocity.z };

	alSourcefv(m_audio_sources[audioID], AL_VELOCITY, vel);

	if (CheckALError("setSound::alSourcefv:AL_VELOCITY"))
		return false;

	// Set the direction
	ALfloat dir[] = { velocity.x, velocity.y, velocity.z };

	alSourcefv(m_audio_sources[audioID], AL_DIRECTION, dir);

	if (CheckALError("setSound::alSourcefv:AL_DIRECTION"))
		return false;

	// Set the max audible distance
	alSourcef(m_audio_sources[audioID], AL_MAX_DISTANCE, maxDistance);

	// Set the MIN_GAIN ( IMPORTANT - if not set, nothing audible! )
	alSourcef(m_audio_sources[audioID], AL_MIN_GAIN, minGain);

	// Set the max gain
	alSourcef(m_audio_sources[audioID], AL_MAX_GAIN, 1.0f); // TODO as parameter ? global ?

	// Set the rollof factor
	alSourcef(m_audio_sources[audioID], AL_ROLLOFF_FACTOR, 1.0f); // TODO as parameter ?

	// Do we play the sound now ?
	if (playNow)
		return PlayAudio(audio_name, forceRestart); // TODO bof... not in this fct

	return true;
}

bool SoundMgr::SetListenerPosition(const Ogre::Vector3 &position, const Ogre::Vector3 &velocity, const Ogre::Quaternion &orientation) {
	Ogre::Vector3 axis;

	// Set the position
	ALfloat pos[3] = { position.x, position.y, position.z };

	alListenerfv(AL_POSITION, pos);

	if (CheckALError("setListenerPosition::alListenerfv:AL_POSITION"))
		return false;

	// Set the veclocity
	ALfloat vel[3] = { velocity.x, velocity.y, velocity.z };

	alListenerfv(AL_VELOCITY, vel);

	if (CheckALError("setListenerPosition::alListenerfv:AL_VELOCITY"))
		return false;

	// Orientation of the listener : look at then look up
	axis = Ogre::Vector3::ZERO;
	axis.x = orientation.getYaw().valueRadians();
	axis.y = orientation.getPitch().valueRadians();
	axis.z = orientation.getRoll().valueRadians();

	// Set the direction
	ALfloat dir[3] = { axis.x, axis.y, axis.z };

	alListenerfv(AL_ORIENTATION, dir);

	if (CheckALError("setListenerPosition::alListenerfv:AL_DIRECTION"))
		return false;

	// TODO as parameters ?
	alListenerf(AL_MAX_DISTANCE, 10000.0f);
	alListenerf(AL_MIN_GAIN, 0.0f);
	alListenerf(AL_MAX_GAIN, 1.0f);
	alListenerf(AL_GAIN, 1.0f);

	return true;
}

bool SoundMgr::GetIsSoundOn(void) const {
	return m_sound_on;
}

std::string SoundMgr::GetAvailableDevicesList(void) const {
	std::string str = "Sound Devices available : ";

	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {
		str = "List of Devices : ";
		str += (char*) alcGetString(NULL, ALC_DEVICE_SPECIFIER);
		str += "\n";
	} else
		str += " ... eunmeration error.\n";

	return str;
}

// Function to check and see if the pSoundFile is already loaded into a buffer
int SoundMgr::LocateAudioBuffer(const std::string &file_name) {
	for (unsigned int b = 0; b < MAX_AUDIO_BUFFERS; ++b) {
		if (file_name == m_audio_buffer_file_name[b])
			return int(b); // TODO Careful : converts unsigned to int!
	}
	return -1;      // not found
}

// Function to load a sound file into an AudioBuffer
int SoundMgr::LoadAudioInToSystem(const std::string &file_name) {
	if (file_name.empty())
		return -1;

	// Make sure we have audio buffers available
	if (m_audio_buffers_in_use_count == MAX_AUDIO_BUFFERS)
		return -1;

	// Find a free Audio Buffer slot
	int bufferID = 0;      // Identity of the Sound Buffer to use

	while (m_audio_buffer_in_use[bufferID] == true)
		++bufferID;

	// load .wav, .ogg or .au
	/*  if ( filename.find( ".wav", 0 ) != std::string::npos )
	 {
	 printf(" ---> found Wav\n");
	 // When you are here, bufferID now represents a free Audio Buffer slot
	 // Attempt to load the SoundFile into the buffer
	 if ( !loadWAV( filename, mAudioBuffers[ bufferID ] ) ) return -1;
	 }
	 */
	if (file_name.find(".ogg", 0) != std::string::npos) {
		printf(" ---> found ogg\n");
		if (!LoadOGG(file_name, m_audio_buffers[bufferID]))
			return -1;
	}
	/* else if ( filename.find( ".au", 0 ) != std::string::npos )
	 {
	 printf(" ---> found au\n");
	 // TODO if ( !loadAU( filename, mAudioBuffers[mBufferID]) ) return -1;
	 }
	 */

	// Successful load of the file into the Audio Buffer.
	m_audio_buffer_in_use[bufferID] = true; // Buffer now in use

	strcpy(m_audio_buffer_file_name[bufferID], file_name.c_str()); // save the file descriptor

	++m_audio_buffers_in_use_count; // bump the 'in use' counter

	return bufferID;
}

bool SoundMgr::LoadOGG(const std::string &file_name, ALuint &pDestAudioBuffer) {
	OggVorbis_File oggfile;

	if (ov_fopen(const_cast<char*>(file_name.c_str()), &oggfile)) {
		printf("SoundManager::loadOGG() : ov_fopen failed.\n");
		return false;
	}

	vorbis_info* info = ov_info(&oggfile, -1);

	ALenum format;
	switch (info->channels) {
	case 1:
		format = AL_FORMAT_MONO16;
		break;
	case 2:
		format = AL_FORMAT_STEREO16;
		break;
	case 4:
		format = alGetEnumValue("AL_FORMAT_QUAD16");
		break;
	case 6:
		format = alGetEnumValue("AL_FORMAT_51CHN16");
		break;
	case 7:
		format = alGetEnumValue("AL_FORMAT_61CHN16");
		break;
	case 8:
		format = alGetEnumValue("AL_FORMAT_71CHN16");
		break;
	default:
		format = 0;
		break;
	}

	std::vector<char> samples;
	char tmpbuf[4096];
	int section = 0;
	bool firstrun = true;
	while (1) {
		int result = ov_read(&oggfile, tmpbuf, 4096, 0, 2, 1, &section);
		if (result > 0) {
			firstrun = false;
			samples.insert(samples.end(), tmpbuf, tmpbuf + (result));
		} else {
			if (result < 0) {
				printf("SoundManager::loadOGG() : Loading ogg sound data failed!");
				ov_clear(&oggfile);
				return false;
			} else {
				if (firstrun)
					return false;
				break;
			}
		}
	}

	alBufferData(pDestAudioBuffer, format, &samples[0], samples.size(), info->rate);

	return true;
}

bool SoundMgr::CheckALError(void) const {
	ALenum errCode;
	if ((errCode = alGetError()) != AL_NO_ERROR) {
		std::string err = "ERROR SoundManager:: ";
		err += (char*) alGetString(errCode);

		printf("%s\n", err.c_str());
		return true;
	}
	return false;
}

bool SoundMgr::CheckALError(const std::string &pMsg) const {
	ALenum error = 0;

	if ((error = alGetError()) == AL_NO_ERROR)
		return false;

	char mStr[256];
	switch (error) {
	case AL_INVALID_NAME:
		sprintf(mStr, "ERROR SoundManager::%s Invalid Name", pMsg.c_str());
		break;
	case AL_INVALID_ENUM:
		sprintf(mStr, "ERROR SoundManager::%s Invalid Enum", pMsg.c_str());
		break;
	case AL_INVALID_VALUE:
		sprintf(mStr, "ERROR SoundManager::%s Invalid Value", pMsg.c_str());
		break;
	case AL_INVALID_OPERATION:
		sprintf(mStr, "ERROR SoundManager::%s Invalid Operation", pMsg.c_str());
		break;
	case AL_OUT_OF_MEMORY:
		sprintf(mStr, "ERROR SoundManager::%s Out Of Memory", pMsg.c_str());
		break;
	default:
		sprintf(mStr, "ERROR SoundManager::%s Unknown error (%i) case in testALError()", pMsg.c_str(), error);
		break;
	};

	printf("%s\n", mStr);
	return true;
}
