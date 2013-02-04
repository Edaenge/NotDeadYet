#pragma once

#include <fmod.h> 
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_codec.h>
#include <fmod_output.h>
#include <fmod_dsp.h>

#ifdef _DEBUG
#pragma comment(lib, "fmodexL_vc.lib")
#else
#pragma comment(lib, "fmodex_vc.lib")
#endif