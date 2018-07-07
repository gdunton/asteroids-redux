//-----------------------------------------------------------
// File: AudioManager.cpp
//-----------------------------------------------------------

#include "AudioManager.h"
#include <algorithm>
#include "Content.h"
#include "../Debugging/Error.h"

const int AudioManager::NUM_CHANNELS = 10;

AudioManager::AudioManager()
{
	const auto result = CoInitialize(nullptr);
	ASSERT(result == S_OK);
	assetsDir = Content::GetAssetsDir();

	// Create the audio device and mastering voice
	xAudio2 = nullptr;
	masteringVoice = nullptr;

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

	LoadAllAssets();
}

AudioManager::~AudioManager()
{
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

Channel* AudioManager::PlaySoundByName(const std::string& soundName)
{
	// Ensure that xAudio has been created
	if(xAudio2 == nullptr || masteringVoice == nullptr)
	{
		return nullptr;
	}

	auto beg = std::find_if(channels.begin(), channels.end(),
	                        [](const Channel& channel) { return !channel.PlayingSound(); });
	if(beg == channels.end())
	{
		return nullptr;
	}

	beg->PlayWav(soundMap.at(soundName).get());
	return &(*beg);
}

void AudioManager::StopAllSound()
{
	for(auto& channel : channels)
	{
		if(channel.PlayingSound())
		{
			channel.Stop();
		}
	}
}

void AudioManager::LoadWavFile(const std::string& filename, const std::string& assetName, bool soundLoops)
{
	// Remove the file from the map if already loaded
	auto iterator = soundMap.find(assetName);
	if(iterator != soundMap.end())
	{
		soundMap.erase(iterator);
	}

	// Load the file
	auto* newWave = new MyWav();
	if(newWave->LoadFile(assetsDir + filename, soundLoops))
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
