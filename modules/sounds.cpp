
#include "sounds.h"
// #include <AL/al.h>
// #include <AL/alc.h>
#include <iostream>
#include <string>
#include <vector>

#include "../dependencies/AudioFile.h"

namespace sounds
{

//   #define OpenAL_ErrorCheck(message)\
//   {\
//     ALenum error = alGetError();\
//     if( error != AL_NO_ERROR)\
//     {\
//       std::cerr << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
//     }\
//   }

//   #define alec(FUNCTION_CALL)\
//   FUNCTION_CALL;\
//   OpenAL_ErrorCheck(FUNCTION_CALL)

//   AudioFile<float> read_audio(std::string path)
// {
//   std::cout << " [SOUNDS] Reading sound from " << path << std::endl;
//   AudioFile<float> audioFile;
//   audioFile.load(path);

//   int sampleRate = audioFile.getSampleRate();
//   int bitsPerSample = audioFile.getBitDepth();
//   int numSamples = audioFile.getNumSamplesPerChannel();
//   double lengthInSeconds = audioFile.getLengthInSeconds();

//   int channels = audioFile.getNumChannels();
//   bool isMono = audioFile.isMono();
//   bool isStereo = audioFile.isStereo();
//   audioFile.printSummary();
//   audioFile.setBitDepth(16);
//   return audioFile;
// }

//   void init()
//   {
//     AudioFile<float> audioFile = sounds::read_audio("./data/sounds/intro_music.wav");

//     const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
//     ALCdevice* device = alcOpenDevice(defaultDeviceString);
//     if (!device){
//       std::cerr << " [SOUNDS][ERROR] Failed to get the default device for OpenAL" << std::endl;
//     }

//     std::cout << "[SOUNDS] OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
    
//     ALCcontext* context = alcCreateContext(device, nullptr);
//     if (!alcMakeContextCurrent(context)){
//       std::cerr << " [SOUNDS][ERROR] failed to make the OpenAL context the current context" << std::endl;
//     }
//     alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
//     alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
//     ALfloat forwardAndUpVectors[] = {
//         /*forward = */ 1.f, 0.f, 0.f,
//         /* up = */ 0.f, 1.f, 0.f
//     };
//     alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
//     std::vector<uint8_t> audioFilePCMDataBytes;
//     audioFile.writePCMToBuffer(audioFilePCMDataBytes); //remember, we added this function to the AudioFile library

//     ALuint sound_buffer;
//     alec(alGenBuffers(1, &sound_buffer));
//     alec(alBufferData(sound_buffer, 
//                       AL_FORMAT_STEREO16,
//                       audioFilePCMDataBytes.data(), 
//                       audioFilePCMDataBytes.size(), 
//                       audioFile.getSampleRate()));


//     // create a sound source for our stereo sound; note 3d positioning doesn't work with stereo files because
//     // stereo files are typically used for music. stereo files come out of both ears so it is hard to know
//     // what the sound should be doing based on 3d position data.

//     ALuint stereoSource;
//     alec(alGenSources(1, &stereoSource));
//     //alec(alSource3f(stereoSource, AL_POSITION, 0.f, 0.f, 1.f)); //NOTE: this does not work like mono sound positions!
//     //alec(alSource3f(stereoSource, AL_VELOCITY, 0.f, 0.f, 0.f)); 
//     alec(alSourcef(stereoSource, AL_PITCH, 1.f));
//     alec(alSourcef(stereoSource, AL_GAIN, 1.f));
//     alec(alSourcei(stereoSource, AL_LOOPING, AL_FALSE));
//     alec(alSourcei(stereoSource, AL_BUFFER, sound_buffer));

//     ALint sourceState;
//     alec(alSourcePlay(stereoSource));
//     alec(alGetSourcei(stereoSource, AL_SOURCE_STATE, &sourceState));
//     while (sourceState == AL_PLAYING)
//     {
//       //basically loop until we're done playing the mono sound source
//       alec(alGetSourcei(stereoSource, AL_SOURCE_STATE, &sourceState));
//     }
//     alec(alDeleteSources(1, &stereoSource));
//     alec(alDeleteBuffers(1, &sound_buffer));
//     alcMakeContextCurrent(nullptr);
//     alcDestroyContext(context);
//     alcCloseDevice(device);

//   };

}
