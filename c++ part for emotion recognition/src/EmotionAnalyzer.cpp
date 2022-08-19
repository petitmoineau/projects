#include "EmotionAnalyzer.h"

EmotionAnalyzer::EmotionAnalyzer()
	: startPoint(std::chrono::steady_clock::now()), tiredStateDuration(std::chrono::duration<double>(std::chrono::seconds(5)))
{}

void EmotionAnalyzer::attach(UserStateObserver* observer)
{
	this->observer = observer;
}

void EmotionAnalyzer::detach()
{
	this->observer = nullptr;
}

void EmotionAnalyzer::notify()
{
	this->observer->update(currentAction);
}

std::chrono::duration<double> EmotionAnalyzer::countEmotionDuration(Emotions emotion)
{
	if (emotion != previousEmotion)
	{
		emotionDuration = std::chrono::steady_clock::now() - startPoint;
		startPoint = std::chrono::steady_clock::now();
		setEmotion(emotion);
	}
	emotionDuration = std::chrono::steady_clock::now() - startPoint;
	return emotionDuration;
}

AsisstantAction EmotionAnalyzer::emotionValidation(std::chrono::duration<double> duration, Emotions emotion)
{
	if (Emotions::userAway == emotion)
	{
		return AsisstantAction::userAway;
	}
	else if (duration >= std::chrono::seconds(3))
	{
		if (Emotions::Neutral == emotion && duration >= tiredStateDuration)
		{
			startPoint = std::chrono::steady_clock::now();
			return AsisstantAction::takeBreak; 
		}
		else if (Emotions::Happy == emotion)
		{
			return AsisstantAction::keepMood;
		}
		else if (Emotions::Sad == emotion)
		{
			return AsisstantAction::cheerUp;
		}
	}
	else return AsisstantAction::nothing;
}

void EmotionAnalyzer::determineAction(Emotions emotion)
{
	if (observer != nullptr)
		observer->update(emotionValidation(countEmotionDuration(emotion), emotion));
}

void EmotionAnalyzer::EmotionAnalyzer::setEmotion(Emotions emotion)
{
	previousEmotion = emotion;
}

void EmotionAnalyzer::setTiredStateTimeFrame(int seconds)
{
	this->tiredStateDuration = std::chrono::duration<double>(std::chrono::seconds(seconds));
}
