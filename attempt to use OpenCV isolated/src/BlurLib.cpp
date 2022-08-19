#include "BlurLib.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>

using namespace cv;

GaussianCurveBlur::GaussianCurveBlur()
	: kernelSize(defKernelVal), sigmaX(defSigmaVal), sigmaY(0.0),
	blurredImageCols(0), blurredImageRows(0), ptrSize(0)
{}

GaussianCurveBlur::~GaussianCurveBlur()
{
	delete[] blurredPtr;
}

void GaussianCurveBlur::setPath(const char* filename)
{
	if (imread(filename).empty())
	{
		throw std::invalid_argument("No image found");
	}
	else
	{
		this->filepath = std::string(filename);
	}
}

int& GaussianCurveBlur::blurredMatRows()
{
	return blurredImageRows;
}

int& GaussianCurveBlur::blurredMatCols()
{
	return blurredImageCols;
}

const size_t& GaussianCurveBlur::blurredImStringSize() const
{
	return this->ptrSize;
}

void GaussianCurveBlur::setBlurLevel(const int& kernelSize)
{
	if (kernelSize % 2 == 0)
	{
		this->kernelSize = kernelSize + 1;
	}
	else
	{
		this->kernelSize = kernelSize;
	}
}

void GaussianCurveBlur::setBlurLevel(const int& kernelSize, const double& sigmaX, const double& sigmaY)
{
	this->sigmaX = sigmaX;
	this->sigmaY = sigmaY;
	setBlurLevel(kernelSize);
}

unsigned char* GaussianCurveBlur::blurredOutput()
{
	Mat origin(imread(filepath));
	Mat blurredImage;
	GaussianBlur(origin, blurredImage, Size(kernelSize, kernelSize), sigmaX, sigmaY);

#ifndef _WIN32
	cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2RGB);
#endif

	blurredImageCols = blurredImage.cols;
	blurredImageRows = blurredImage.rows;

	ptrSize = (origin.rows - 1) * origin.cols * origin.channels() + (origin.cols - 1) * origin.channels() + origin.channels();
	blurredPtr = new unsigned char[ptrSize];

	std::copy(blurredImage.data, blurredImage.data + ptrSize, blurredPtr);
	return blurredPtr;
}