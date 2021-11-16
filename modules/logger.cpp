
#include "logger.h"
#include "quads.h"
#include "textures.h"

#include <fstream>
#include <iostream>
#include <string>

namespace logger2
{

  void log()
  {
    quads2::log();
    textures2::log();
  }
}