#include "EmotionDetection.h"
#include "ActReactTranslator.h"
#include "EmotionAnalyzer.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/dnn/dnn.hpp"
#include "opencv2/opencv.hpp"

#include <string>
#include <algorithm>

using namespace cv;
using namespace cv::dnn;

namespace Utils
{
    Net net;
    Point minLoc, maxLoc = {-1, -1};
    CascadeClassifier faceCascade;
    std::vector<std::string> emotToStr = { "Neutral", "Happy", "Sad" };
    Mat imgCopy, faceImage, gray, reshaped, outImage;

    std::vector<Rect> haarDetect(Mat img)
    {
        std::vector<Rect> faces;

        if (faceCascade.empty())
        {
            std::cout << "\033[1;31mERROR: Couldn't open haarcascade_frontalface_default.xml\033[0m\n";
            return std::vector<Rect>();
        }

        faceCascade.detectMultiScale(img, faces, 1.1, 10);
        return faces;
    }
}

Emotion::EmotionRecognizer::Impl::Impl()
    : translator(new ActReactTranslator), analyzer(new EmotionAnalyzer),
    sadVietnamFlashbacks(nullptr)
{
    translator->setAnalyzer(*analyzer);
}

void Emotion::EmotionRecognizer::Impl::initModel(const char* path)
{
    if ((path != NULL) && (path[0] == '\0'))
    {
        Utils::faceCascade.load("..\\Model\\haarcascade_frontalface_default.xml");
        Utils::net = cv::dnn::readNetFromONNX("..\\Model\\emotion-ferplus-8.onnx");
    }
    else
    {
        std::string name = std::string(path) + "haarcascade_frontalface_default.xml";
        Utils::faceCascade.load(name);
        Utils::net = cv::dnn::readNetFromONNX(std::string(path) + "emotion-ferplus-8.onnx");
    }
}

std::vector<std::tuple<const char*, float>> Emotion::EmotionRecognizer::Impl::emotion(unsigned char* img, int height, int width, bool RGB, bool hasAlpha)
{
#ifdef _WIN32
    Utils::imgCopy = Mat(height, width, CV_8UC3, img);
#else
    Utils::imgCopy = Mat(height, width, CV_8UC4, img);
#endif
        
    Utils::maxLoc = { -1, -1 };
    std::vector<Rect> faces = Utils::haarDetect(Utils::imgCopy);
    Mat forwardedRes, probability;
    std::vector<std::tuple<const char*, float>> emotionResult;
    
    std::for_each(faces.begin(), faces.end(), [&](Rect& face)
        {
            Utils::faceImage = Mat(Utils::imgCopy, face);
            if (RGB)
            {
                if (hasAlpha)
                    cvtColor(Utils::imgCopy, Utils::gray, COLOR_RGBA2GRAY);
                else
                    cvtColor(Utils::imgCopy, Utils::gray, COLOR_RGB2GRAY);
            }
            else cvtColor(Utils::imgCopy, Utils::gray, COLOR_BGR2GRAY);

            Utils::reshaped = blobFromImage(Utils::gray, 1.0, Size(64, 64));
            Utils::net.setInput(Utils::reshaped);
            forwardedRes = Utils::net.forward();

            //some hard calibration. i`ve got a code snippet to automatize it but collegues had no time left to create UI so we left it to the better times:)
#ifdef _WIN32 
            forwardedRes.at<float>(0, 0) += 0;//neutral
            forwardedRes.at<float>(0, 1) += 0;//happy
            forwardedRes.at<float>(0, 3) += 0;//sad
#else
            forwardedRes.at<float>(0, 0) += 1;
            forwardedRes.at<float>(0, 1) += 3.5;
            forwardedRes.at<float>(0, 3) += 1;
#endif
            
            minMaxLoc(forwardedRes, NULL, NULL, &Utils::minLoc, &Utils::maxLoc);
            int counter = 0;
            std::for_each(Utils::emotToStr.begin(), Utils::emotToStr.end(), [&](std::string& emotion)
                {
                    if (counter == 2) ++counter;
                        emotionResult.push_back(std::make_tuple(emotion.c_str(), forwardedRes.at<float>(0, counter)));
                    ++counter;
                });
        });

    analyzer->determineAction(getEmotion(Utils::maxLoc.x));

    return emotionResult;
}

const char* Emotion::EmotionRecognizer::Impl::getAction()
{
    return translator->resultedAction();
}

void Emotion::EmotionRecognizer::Impl::setTiredStateTimeFrame(int seconds)
{
    analyzer->setTiredStateTimeFrame(seconds);
}

Emotions Emotion::EmotionRecognizer::Impl::getEmotion(int maxValue)
{
    return maxValue == 3? Emotions::Sad : (maxValue == 1? Emotions::Happy : (maxValue == 0? Emotions::Neutral : Emotions::userAway));
}

//pointer to implementation
Emotion::EmotionRecognizer::EmotionRecognizer()
{
    pImpl = std::make_unique<Impl>();
}

void Emotion::EmotionRecognizer::initModel(const char* path)
{
    pImpl->initModel(path);
}

std::vector<std::tuple<const char*, float>> Emotion::EmotionRecognizer::emotion(unsigned char* img, int height, int width, bool RGB, bool hasAlpha)
{
    return pImpl->emotion(img, height, width, RGB, hasAlpha);
}

const char* Emotion::EmotionRecognizer::getAction()
{
    return pImpl->getAction();
}

void Emotion::EmotionRecognizer::setTiredStateTimeFrame(int seconds)
{
    pImpl->setTiredStateTimeFrame(seconds);
}