
#include <string>
#include "../dependencies/AudioFile.h"

#ifndef MODULES_SOUNDS_H
#define MODULES_SOUNDS_H

// PCM format - pulse code modulation
// Bit depth -> the higher, the better sound quality

namespace sounds
{

  // Initialize sounds
  void init();

  // Read audio from path to AudioFile
  AudioFile<float> read_audio(std::string path);

  

}

#endif