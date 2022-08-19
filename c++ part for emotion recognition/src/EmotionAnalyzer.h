#pragma once
#include "Platform.h"
#include "UserStateObserver.h"

#include <chrono>
#include <memory>
#include <vector>
#include <string>

extern "C"
{
	class EmotionAnalyzer : public UserStateSubject //subject
	{
	public:
		EmotionAnalyzer();

		~EmotionAnalyzer() { delete observer; };

		void attach(UserStateObserver* observer) override;

		void detach() override;

		void notify() override;

		void setEmotion(Emotions emotion);

		void setTiredStateTimeFrame(int seconds);

		std::chrono::duration<double> countEmotionDuration(Emotions emotion);

		AsisstantAction emotionValidation(std::chrono::duration<double> duration, Emotions emotion);

		void determineAction(Emotions emotion);

	private:

		UserStateObserver* observer;
		AsisstantAction currentAction;
		std::chrono::steady_clock::time_point startPoint;
		std::chrono::duration<double> tiredStateDuration;
		std::chrono::duration<double> emotionDuration{};//zero-initialize
		Emotions previousEmotion = Emotions::Neutral;
	};
}
