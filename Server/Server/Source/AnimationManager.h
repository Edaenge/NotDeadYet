#pragma once

#include "AnimationFileReader.h"
#include "AnimationStates.h"
#include "BioActor.h"
#include <vector>
#include <map>

class AnimationManager
{
private:
	AnimationFileReader zAnimationFileReader[4];
	std::map<std::string, AnimationFileReader> zModelToReaderMap;

public:
	AnimationManager();
	virtual ~AnimationManager();

	AnimationQueue CreatePlayerAnimationQueue(BioActor* bActor);
	AnimationQueue CreateAnimalAnimationQueue(BioActor* bActor);
	const std::string& GetPlayerAnimation(const std::string model, const std::string& animation);
	const float GetAnimationTime(const std::string model, const std::string& animation);
};