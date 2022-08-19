#pragma once
#include "Platform.h"
#include <iostream>

class CORE_EXPORT GaussianCurveBlur
{
public:
	GaussianCurveBlur();
	~GaussianCurveBlur();
	void setPath(const char*);
	int& blurredMatRows();
	int& blurredMatCols();
	const size_t& blurredImStringSize() const;
	void setBlurLevel(const int&);
	void setBlurLevel(const int&, const double& sigmaX, const double& sigmaY = 0.0);
	//unsigned char* blurredOutput();
	cv::Mat blurredOutput();

	static constexpr int channels = 3;

private:
	
	int blurredImageRows, blurredImageCols;
	double sigmaX, sigmaY;
	int kernelSize;//its square so one side size only needed

	size_t ptrSize;
	unsigned char* blurredPtr;
	std::string filepath;

	static constexpr int defSigmaVal = 5;
	static constexpr int defKernelVal = 35;
	
};