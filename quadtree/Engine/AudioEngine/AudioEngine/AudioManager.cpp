#include "AudioManager.h"
#include "DL_Debug.h"

typedef std::pair<const char*, FMOD::Sound*> Sound;
typedef std::pair<const char*, FMOD::Channel*> Channel;
typedef std::pair<const char*, FMOD::ChannelGroup*> Group;

namespace SoundEngine
{

	AudioManager::AudioManager()
	{
	}

	AudioManager::~AudioManager()
	{
	}

	void AudioManager::Initiate(int aChannelAmount)
	{
		myChannelAmount = aChannelAmount;

		myResult = FMOD::System_Create(&mySystem);
		DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Initiate) : Couldn't create FMOD System!");

		myResult = mySystem->getNumDrivers(&myDrives);
		if (myDrives <= 0)
		{
			myResult = mySystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Initiate) : Couldn't set output type!");
		}

		myResult = mySystem->getVersion(&myVersion);
		DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Initiate) : Couldn't get FMOD Version!");

		myResult = mySystem->init(aChannelAmount, FMOD_INIT_NORMAL, nullptr);
		DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Initiate) : Couldn't initiate Fmod System");


		myAmbient = nullptr;
		mySamples = nullptr;
		myBackgroundMusic = nullptr;
	}

	void AudioManager::LoadSound(const char* aFilepath, const char* aKey, bool aSoundIs3D)
	{
		FMOD::Sound* sound = nullptr;

		const char*  key = nullptr;
		if (aKey == nullptr)
			key = aFilepath;
		else
			key = aKey;

		if (aSoundIs3D == true)
		{
			myResult = mySystem->createSound(aFilepath, FMOD_3D, 0, &sound);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](LoadSound) : Couldn't Load Sound! (3D)");
		}
		else if (aSoundIs3D == false)
		{
			myResult = mySystem->createSound(aFilepath, FMOD_LOOP_OFF, 0, &sound);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](LoadSound) : Couldn't Load Sound! (2D)");
		}

		mySounds.insert(Sound(key, sound));
	}

	void AudioManager::Play(eSoundType aSoundType, const char* aKey, float aVolume, const char* aSampleKey, bool aStartAsPaused, Vector3f aPosition, Vector2f aMinMaxDistance)
	{
		DL_ASSERT_EXP(aKey == nullptr, "[AudioManager](Play) : aKey is null! Please enter a value!");
		bool playing = false;
		bool paused = false;
		
		FMOD_VECTOR pos;
		pos.x = aPosition.x;
		pos.y = aPosition.y;
		pos.z = aPosition.z;

		switch (aSoundType)
		{
#pragma region BACKGROUND
		case eSoundType::BACKGROUND:
			
			DL_ASSERT_EXP(aSampleKey != nullptr, "aSample has to be a nullptr when using a different type than SAMPLE!");
			if (myBackgroundMusic != nullptr)
			{
				myResult = myBackgroundMusic->isPlaying(&playing);
				DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't check if [BACKGROUND] already is Playing!");
			}

			if (playing == true)
				Stop(aSoundType);

			myResult = mySystem->playSound(mySounds[aKey], 0, aStartAsPaused, &myBackgroundMusic);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't Play [BACKGROUD] Sound!");
		
			//This should also be in the SAMPLE and AMBIENT	
			myResult = myBackgroundMusic->set3DMinMaxDistance(aMinMaxDistance.x*1.f, aMinMaxDistance.y*1.f);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't set distance to sound!");
			myResult = myBackgroundMusic->set3DAttributes(&pos, 0);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't set [BACKGROUND] 3D attribute!");


			Volume(eSoundType::BACKGROUND, aVolume);

			break;
#pragma endregion
#pragma region AMBIENT
		case eSoundType::AMBIENT:

			DL_ASSERT_EXP(aSampleKey != nullptr, "aSample has to be a nullptr when using a different type than SAMPLE!");
			if (myAmbient != nullptr)
			{
				myResult = myAmbient->isPlaying(&playing);
				DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't check if [AMBIENT] already is playing!");
			}

			if (playing == true)
				Stop(aSoundType);

			myResult = mySystem->playSound(mySounds[aKey], 0, aStartAsPaused, &myAmbient);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't Play [AMBIENT] Sound!");
			Volume(eSoundType::AMBIENT, aVolume);

			break;
#pragma endregion
#pragma region SAMPLE
		case eSoundType::SAMPLE:
			if (aSampleKey != nullptr)
			{
				FMOD::Channel* sampleChannel;

				myResult = mySystem->playSound(mySounds[aKey], mySamples, aStartAsPaused, &sampleChannel);
				DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Play) : Couldn't Play [SAMPLE] Sound!");

				mySampleChannels.insert(Channel(aSampleKey, sampleChannel));


				Volume(eSoundType::SAMPLE, aVolume, sampleChannel);
			}
			else
				DL_ASSERT_EXP(aSampleKey == nullptr, "[AudioManager](Play) : aSampleKey were a nullptr!");

			break;

		default:
			DL_ASSERT_EXP(false, "[AudioManager](Play) : No valid SoundType entered!");
			break;
#pragma endregion
		}
	}

	void AudioManager::Pause(eSoundType aSoundType, const char* aSampleKey)
	{
		bool paused = false;

		switch (aSoundType)
		{

		case eSoundType::BACKGROUND:

			myResult = myBackgroundMusic->getPaused(&paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't get pause bool on [BACKGROUND]!");

			myResult = myBackgroundMusic->setPaused(!paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't set pause bool on [BACKGROUND]!");

			break;

		case eSoundType::AMBIENT:

			myResult = myAmbient->getPaused(&paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't get pause bool on [AMBIENT]!");

			myResult = myAmbient->setPaused(!paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't set pause bool on [AMBIENT]!");

			break;

		case eSoundType::SAMPLE:

			myResult = mySampleChannels[aSampleKey]->getPaused(&paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't get pause bool on [SAMPLE]!");

			myResult = mySampleChannels[aSampleKey]->setPaused(!paused);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Pause) : Couldn't set pause bool on [SAMPLE]!");

			break;

		default:
			break;
		}
	}

	void AudioManager::Stop(eSoundType aSoundType, const char* aSampleKey)
	{
		switch (aSoundType)
		{
		case eSoundType::BACKGROUND:
			myResult = myBackgroundMusic->stop();
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Stop) : Couldn't stop [BACKGROUND]!");
			break;
		case eSoundType::AMBIENT:
			myResult = myAmbient->stop();
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Stop) : Couldn't stop [AMBIENT]!");
			break;
		case eSoundType::SAMPLE:
			myResult = mySampleChannels[aSampleKey]->stop();
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Stop) : Couldn't stop [SAMPLE]!");
			break;
		default:
			break;
		}
	}

	void AudioManager::Volume(eSoundType aSoundType, float aVolumePercent, FMOD::Channel* aChannel)
	{
		float volume = 0.f;
		float vol = 100.f - aVolumePercent;
		switch (aSoundType)
		{
		case eSoundType::BACKGROUND:
			myBackgroundMusic->getVolume(&volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Volume) : Couldn't Get the volume from [BACKGROUND] Channel!");
			volume -= (vol / 100.f);

			if (volume > 1.f)
				volume = 1.f;

			if (volume < 0.f)
				volume = 0.f;

			myBackgroundMusic->setVolume(volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "Couldn't Increase volume!");
			break;
		case eSoundType::AMBIENT:
			myAmbient->getVolume(&volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Volume) : Couldn't Get the volume from [AMBIENT] Channel!");
			volume -= (vol / 100.f);

			if (volume > 1.f)
				volume = 1.f;

			if (volume < 0.f)
				volume = 0.f;

			myAmbient->setVolume(volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "Couldn't Increase volume!");
			break;
		case eSoundType::SAMPLE:


			myResult = aChannel->getVolume(&volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Volume) : Couldn't Get the volume from [SAMPLE] Channel!");
			volume -= (vol / 100.f);

			if (volume > 1.f)
				volume = 1.f;

			if (volume < 0.f)
				volume = 0.f;

			myResult = aChannel->setVolume(volume);
			DL_ASSERT_EXP(myResult != FMOD_OK, "Couldn't Increase volume!");

			break;
		}
	}

	void AudioManager::Update()
	{
		myResult = mySystem->update();
		DL_ASSERT_EXP(myResult != FMOD_OK, "[AudioManager](Update) : Update Failed!");
	}

	const float AudioManager::GetVolume(eSoundType aSoundType, const char* aSampleKey)
	{
		float volume;
		switch (aSoundType)
		{
		case eSoundType::BACKGROUND:
			myBackgroundMusic->getVolume(&volume);
			break;
		case eSoundType::AMBIENT:
			myAmbient->getVolume(&volume);
			break;
		case eSoundType::SAMPLE:
			mySampleChannels[aSampleKey]->getVolume(&volume);
			break;
		}
		return volume * 100.f;
	}

	void AudioManager::UpdateListener(CU::Math::Vector3<float> aPosition)
	{
		static int listeners = 0;

		FMOD_VECTOR position;
		position.x = aPosition.x;
		position.y = aPosition.y;
		position.z = aPosition.z;

		myResult = mySystem->set3DListenerAttributes(listeners, &position, 0, 0, 0);
		if (myResult != FMOD_OK)
		{
			DL_DEBUG("[AudioManager](UpdateListener) : Couldn't update the position of the listener! %i", listeners);
			DL_ASSERT("[AudioManager](UpdateListener) : Couldn't update the position of the listener!");
		}
	}


}