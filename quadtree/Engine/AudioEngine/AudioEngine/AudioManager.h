#pragma once
#include "FMOD\fmod.hpp"
#include "FMOD\fmod.h"
#include <unordered_map>
#include "Math\Vector\Vector.h"
namespace SoundEngine
{

	enum class eSoundType
	{
		BACKGROUND,
		AMBIENT,
		SAMPLE,
		/* Add more sound types here */
	};


	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();
		/**Note:
		
			-aChannelAmount, specifies the amount of channels that can be created. 
			The amount of channels will represent how many sounds that can be played at the same time.

		*/
		void Initiate(const int aChannelAmount);

		/**Note:

			
			-aFilepath, specifies the path to said file.
			-aKey, specifies a key for the sound to be easily accessed in a map.
			-aSoundIs3D, specifies if the sound is a 3D(true) or 2D(false) sound.

		*/
		void LoadSound(const char* aFilepath, const char* aKey = nullptr, bool aSoundIs3D = false);

		/**Note:

			-const char* aKey, this finds the sound
			-aSoundType, this is which soundtype (Background, Ambient, Sample)
			-const char* aSampleKey, this is only viable if you are playing a sample. This will add it to it's own channel and store it in a map so that you can easily change the volume of each sound.
			-aStartAsPaused, this is if it should be paused or not when you start it.
			-aPosition, specifies the position of the sound, use this if aSoundIs3D for the sound is true.
			-aMinMaxDistance, specifies how far away you're able to hear the sound, use this if aSoundIs3D for the sound is true.

			*/
		void Play(eSoundType aSoundType, const char* aKey, float aVolume, const char* aSampleKey = nullptr, bool aStartAsPaused = false, Vector3f aPosition = (0.f, 0.f, 0.f), Vector2f aMinMaxDistance = (0.f, 0.f));
		/**Note:

			If you are using eSoundType::SAMPLE, also insert a key to find the channel the audio is playing on.
			This is due to the fact that you have to stop a channel from playing a sound and not the sound itself from playing.

			*/
		void Stop(eSoundType aSoundType, const char* aKey = nullptr);
		/**Note:

			If you are using eSoundType::SAMPLE, also insert a key to find the channel the audio is playing on.
			This is due to the fact that you have to stop a channel from playing a sound and not the sound itself from playing.

			*/
		void Pause(eSoundType aSoundType, const char* aKey = nullptr);
		/**Note:

			If you are using eSoundType::SAMPLE, also insert a key to find the channel the audio is playing on.
			This is beacuse the SAMPLE channels are stored in an <unsorted_map> and you can only change the volume on a specific channel.

			*/
		void Volume(eSoundType aSoundType, float aVolumePercent = 0.f, FMOD::Channel* aChannel = nullptr);

		/**Note:
			Must be called else you can't change volume and such.
			*/
		void Update();

		/**Note: 
			
			Returns the volume of the channel asked for.

		*/
		const float GetVolume(eSoundType aSoundType, const char* aSampleKey = nullptr);


		/**Note:
			
			The Listener is usually the player, or the camera. 
			The position of the camera or the position of the player should be inserted into this function.
		*/
		void UpdateListener(CU::Math::Vector3<float> aPosition);

	private:

		inline const unsigned int& GetVersion() const;

		FMOD::System* mySystem;

		FMOD::Channel* myBackgroundMusic;
		FMOD::Channel* myAmbient;
		FMOD::ChannelGroup* mySamples;

		FMOD_RESULT myResult;

		int myChannelAmount;
		unsigned int myVersion;
		int myDrives;

		std::unordered_map<const char*, FMOD::Sound*> mySounds;
		std::unordered_map<const char*, FMOD::Channel*> mySampleChannels;
	};

	inline const unsigned int& AudioManager::GetVersion() const
	{
		return myVersion;
	}

}
namespace SE = SoundEngine;