#pragma once
#include "Platform.h"

#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

class ActReactTranslator;
class EmotionAnalyzer;

extern "C"
{
	namespace Emotion
	{
		class CORE_EXPORT EmotionRecognizer
		{
		public:
			EmotionRecognizer();

			void initModel(const char*);

			std::vector<std::tuple<const char*, float>> emotion(unsigned char* img, int height, int width, bool RGB = true, bool hasAlpha = false);

			const char* getAction();

			void setTiredStateTimeFrame(int seconds);

		private:

            class Impl
            {
            public:
                ActReactTranslator* translator;

            public:
				Impl();
				~Impl()
				{
					delete analyzer;
					delete translator;
					delete sadVietnamFlashbacks;
				}

                void initModel(const char* path);

                std::vector<std::tuple<const char*, float>> emotion(unsigned char* img, int height, int width, bool RGB, bool hasAlpha);

                const char* getAction();

				void setTiredStateTimeFrame(int seconds);

            private:
                EmotionAnalyzer* analyzer;
				unsigned char* sadVietnamFlashbacks;

			private:
				Emotions getEmotion(int maxValue);
            };
			
			std::unique_ptr<Impl> pImpl;
		};
	}
}