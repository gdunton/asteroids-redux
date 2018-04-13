//-----------------------------------------------------------
// File: AudioManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "AudioManager.h"

template<>
AudioManager* Singleton<AudioManager>::instance = nullptr;
const int AudioManager::NUM_CHANNELS = 10;

AudioManager::AudioManager()
{
	GetAssetsDir(assetsDir);

	// Create the audio device and mastering voice
	xAudio2 = nullptr;
	masteringVoice = nullptr;

	unsigned int flags = 0;
#ifdef DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	HRESULT hr = XAudio2Create(&xAudio2/*, flags*/);
	if(hr != S_OK)
	{
		MessageBox(nullptr, L"Failed to initialize XAudio2 pointer", L"Initialization Error", MB_OK);
		if(xAudio2)
		{
			xAudio2->Release();
			xAudio2 = nullptr;
		}
		return;
	}
	hr = xAudio2->CreateMasteringVoice(&masteringVoice);
	if(hr != S_OK)
	{
		MessageBox(nullptr, L"Failed to create master voice", L"Initialization Error", MB_OK);
		if(masteringVoice)
		{
			masteringVoice->DestroyVoice();
			masteringVoice = nullptr;
		}
		return;
	}

	// Reset the master volume
	float volume = 0;
	masteringVoice->GetVolume(&volume);
	volume *= 0.2f;
	masteringVoice->SetVolume(volume);

	channels.reserve(NUM_CHANNELS);
}

AudioManager::~AudioManager()
{
	// Clear up all the channels
	for(auto& channel : channels)
	{
		channel.Destroy();
	}
	channels.clear();

	// Destroy all the sounds
	soundMap.clear();

	// Destroy the mastering voice and the audio device
	if(masteringVoice)
	{
		masteringVoice->DestroyVoice();
	}
	if(xAudio2)
	{
		CoUninitialize();
	}
}

void AudioManager::LoadAllAssets()
{
	GetInstance().InLoadAllAssets();
}

void AudioManager::InLoadAllAssets()
{
	// Ensure that xAudio has been created
	if(xAudio2 == nullptr || masteringVoice == nullptr)
	{
		return;
	}

	// Load each asset from the default directory
	LoadWavFile("explode1.wav", "Bang1");
	LoadWavFile("explode2.wav", "Bang2");
	LoadWavFile("explode3.wav", "Bang3");
	LoadWavFile("life.wav", "NewLife", true);
	LoadWavFile("fire.wav", "Fire");
	LoadWavFile("thrust.wav", "Thrust", true);
	LoadWavFile("sfire.wav", "FireAlt");
	LoadWavFile("thump.wav", "Thump");

	// Create all the channels
	WAVEFORMATEX* sampleFormat = soundMap["Thrust"]->GetWavFormat();
	for(int i = 0; i < NUM_CHANNELS; i++)
	{
		// Push the channel into the channel buffer
		channels.emplace_back();
		channels.back().Init(xAudio2, sampleFormat);
	}
}

IStoppable* AudioManager::PlaySoundByName(std::string soundName)
{
	return GetInstance().InPlaySoundByName(soundName);
}

IStoppable* AudioManager::InPlaySoundByName(std::string soundName)
{
	// Ensure that xAudio has been created
	if(xAudio2 == nullptr || masteringVoice == nullptr)
	{
		return nullptr;
	}

	// Search for a free channel. 
	// Loop an iterator until either then end of the array has been reached (return null)
	// or a clear channel has been found
	int i = 0;
	for(auto beg = channels.begin(), end = channels.end(); ; ++beg, i++) // remember to increment i each time
	{
		if(beg == end)
		{
			return nullptr;
		}
		if(!beg->PlayingSound())
		{
			break;
		}
	}

	ASSERT( channels[i].PlayingSound() == false );

	// play the wav through the channel and pass back the channel as IStoppable
	channels[i].PlayWav(soundMap[soundName].get());
	
	return &channels[i];
}

void AudioManager::StopAllSound()
{
	GetInstance().InStopAllSound();
}

void AudioManager::InStopAllSound()
{
	for(auto& channel : channels)
	{
		if(channel.PlayingSound())
		{
			channel.VStop();
		}
	}
}

void AudioManager::LoadWavFile(std::string filename, std::string assetName, bool soundLoops)
{
	// Remove the file from the map if already loaded
	auto iterator = soundMap.find(assetName);
	if(iterator != soundMap.end())
	{
		soundMap.erase(iterator);
	}

	// Load the file
	MyWav* newWave = new MyWav();
	if(newWave->LoadFile(assetsDir + filename, GetInstance().xAudio2, soundLoops))
	{
		// Loaded the file
		soundMap[assetName] = std::shared_ptr<MyWav>(newWave);
	}
	else
	{
		delete newWave;
		newWave = nullptr;
	}
}
