#pragma once

#if defined (_WIN32)
#define CORE_EXPORT _declspec(dllexport)
#else
#define CORE_EXPORT
#endif

enum class Emotions
{
	Neutral,
	Happy,
	Sad,
	userAway
};

enum class AsisstantAction
{
	nothing,
	takeBreak,
	keepMood,
	cheerUp,
	userAway
};