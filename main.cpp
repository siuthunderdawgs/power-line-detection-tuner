/*
 * main.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: steven
 */

#include <cv.h>
#include <highgui.h>

#include <iostream>

#include "power-line-detection/PowerLineDetection.h"
#include "mask-tester/MaskConsistencyChecker.h"

int main(int argc, char** argv)
{
	const char* filename_src;
	const char* filename_truth;

	if(argc >= 3)
	{
		filename_src = argv[1];
		filename_truth = argv[2];
	}
	else
	{
		filename_src = "image.jpg";
		filename_truth = "truth.jpg";
	}

	cv::Mat image_src = cv::imread(filename_src);
	cv::Mat image_truth = cv::imread(filename_truth);

	if(image_src.empty())
	{
		 std::cout << "ERROR: Cannot open " << filename_src << std::endl;
		 return -1;
	}

	if(image_truth.empty())
	{
	 std::cout << "ERROR: Cannot open " << filename_truth << std::endl;
	 return -1;
	}


	cv::Mat image_test;
	PowerLineDetection(image_src, image_test, 0.7971, 15.9820, 200, 10.0, 1.0);

	MaskConsistencyChecker checker;
	checker.SetMaskTruth(image_truth);
	checker.SetMaskTest(image_test);

	if(!checker.CheckMaskSizes())
	{
		std::cout << "Error: Masks sizes do not match!\n";
		return 1;
	}

	checker.CheckMaskConsistency();

	cv::Mat mask_result = checker.GetMaskResult();

	cv::namedWindow("Mask Image");
	cv::imshow("Mask Image", mask_result);

	std::cout << "Correct: " << checker.GetPercentCorrect()*100 << " %" << std::endl;
	std::cout << "False Pos: " << checker.GetPercentFalsePositive()*100 << " %" << std::endl;
	std::cout << "False Neg: " << checker.GetPercentFalseNegative()*100 << " %" << std::endl;

	while(char(cv::waitKey(1)) != 'q'){}

	return 0;
}
