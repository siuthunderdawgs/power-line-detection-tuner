/*
 * MaskConsistencyChecker.cpp
 *
 *  Created on: Feb 7, 2016
 *      Author: steven
 */

#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "MaskConsistencyChecker.h"

MaskConsistencyChecker::MaskConsistencyChecker() {
	// TODO Auto-generated constructor stub

}

MaskConsistencyChecker::~MaskConsistencyChecker() {
	// TODO Auto-generated destructor stub
}

cv::Mat MaskConsistencyChecker::GetMaskTest() const
{
    return mask_test_;
}

void MaskConsistencyChecker::SetMaskTest(cv::Mat maskTest)
{
	cv::Mat temp;

	cv::cvtColor(maskTest, temp, CV_BGR2GRAY );
	cv::threshold(temp, mask_test_, 10, 255, cv::THRESH_BINARY);
}

cv::Mat MaskConsistencyChecker::GetMaskTruth() const
{
    return mask_truth_;
}

void MaskConsistencyChecker::SetMaskTruth(cv::Mat maskTruth)
{
	cv::Mat temp;

	cv::cvtColor(maskTruth, temp, CV_BGR2GRAY );
	cv::threshold(temp, mask_truth_, 10, 255, cv::THRESH_BINARY);
}
cv::Mat MaskConsistencyChecker::GetMaskResult() const
{
    return mask_result_;
}

int MaskConsistencyChecker::CheckMaskSizes()
{
	if(mask_truth_.size() == mask_test_.size())
		return 1;
	else
		return 0;
}

void MaskConsistencyChecker::CheckMaskConsistency()
{
	if(!CheckMaskSizes())
	{
		std::cout << "Error: The mask sizes do not match!\n";
		return;
	}

	cv::Mat mask_result = cv::Mat::zeros(mask_truth_.size(), CV_8UC3);

	int rows = mask_truth_.rows;
	int cols = mask_truth_.cols;

	int n = 0;
	int n_right_on = 0;
	int n_right_off = 0;
	int n_wrong_fp = 0;
	int n_wrong_fn = 0;

	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			int px_truth = mask_truth_.at<uchar>(i,j);
			int px_test = mask_test_.at<uchar>(i,j);

			int right_on = (px_truth && px_test);
			int right_off = (!px_truth && !px_test);
			int wrong_fp = (px_test && !px_truth);
			int wrong_fn = (px_truth && !px_test);

			++n;
			if(right_on)
			{
				++n_right_on;
				mask_result.at<cv::Vec3b>(i,j) = cv::Vec3b(255,255,255);
			}
			else if(right_off)
			{
				++n_right_off;
				mask_result.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
			}
			else if(wrong_fp)
			{
				++n_wrong_fp;
				mask_result.at<cv::Vec3b>(i,j) = cv::Vec3b(255,0,0);
			}
			else if(wrong_fn)
			{
				++n_wrong_fn;
				mask_result.at<cv::Vec3b>(i,j) = cv::Vec3b(0,255,0);
			}
		}
	}

	mask_result_ = mask_result.clone();

	num_px_ = n;
	num_right_on_ = n_right_on;
	num_right_off_ = n_right_off;
	num_wrong_fp_ = n_wrong_fp;
	num_wrong_fn_ = n_wrong_fn;
}

int MaskConsistencyChecker::GetNumPx() const {
	return num_px_;
}

int MaskConsistencyChecker::GetNumRightOff() const {
	return num_right_off_;
}

int MaskConsistencyChecker::GetNumRightOn() const {
	return num_right_on_;
}

int MaskConsistencyChecker::GetNumWrongFn() const {
	return num_wrong_fn_;
}

int MaskConsistencyChecker::GetNumWrongFp() const {
	return num_wrong_fp_;
}

double MaskConsistencyChecker::GetPercentCorrect() const {
	double rval = double(num_right_on_ + num_right_off_)/num_px_;
	return rval;
}

double MaskConsistencyChecker::GetPercentFalsePositive() const {
	double rval = double(num_wrong_fp_)/num_px_;
	return rval;
}

double MaskConsistencyChecker::GetPercentFalseNegative() const {
	double rval = double(num_wrong_fn_)/num_px_;
	return rval;
}
