#pragma once
#include "EmotionAnalyzer.h"

extern "C"
{
	class ActReactTranslator : public UserStateObserver //observer
	{
	public:
		~ActReactTranslator();

		void update(AsisstantAction action) override;

		void setAnalyzer(EmotionAnalyzer& analyzer);

		inline const char* resultedAction() 
		{
			return action; 
		}


	private:
		const char* action = "nothing";
		EmotionAnalyzer analyzer;
	};
}