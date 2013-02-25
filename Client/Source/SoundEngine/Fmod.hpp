#ifndef _FMOD_HPP_
#define _FMOD_HPP_

#include <fmod.hpp>
#include <fmod_event.hpp>

#ifdef _DEBUG
#pragma comment(lib, "fmod_eventL.lib")
#pragma comment(lib, "fmodexL_vc.lib")
#else
#pragma comment(lib, "fmod_event.lib")
#pragma comment(lib, "fmodex_vc.lib")
#endif // _DEBUG

#endif // _FMOD_HPP_