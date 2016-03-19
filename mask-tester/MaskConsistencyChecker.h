/*
 * MaskConsistencyChecker.h
 *
 *  Created on: Feb 7, 2016
 *      Author: steven
 */

#ifndef MASKCONSISTENCYCHECKER_H_
#define MASKCONSISTENCYCHECKER_H_

#include <cv.h>
#include <highgui.h>


class MaskConsistencyChecker {
public:
	MaskConsistencyChecker();
	virtual ~MaskConsistencyChecker();

	cv::Mat GetMaskTest() const;
	void SetMaskTest(cv::Mat maskTest);
	cv::Mat GetMaskTruth() const;
	void SetMaskTruth(cv::Mat maskTruth);
	cv::Mat GetMaskResult() const;

	int CheckMaskSizes();
	void CheckMaskConsistency();

	int GetNumPx() const;
	int GetNumRightOff() const;
	int GetNumRightOn() const;
	int GetNumWrongFn() const;
	int GetNumWrongFp() const;

	double GetPercentCorrect() const;
	double GetPercentFalsePositive() const;
	double GetPercentFalseNegative() const;

private:
	cv::Mat mask_truth_;
	cv::Mat mask_test_;
	cv::Mat mask_result_;

	int num_px_;
	int num_right_on_;
	int num_right_off_;
	int num_wrong_fp_;
	int num_wrong_fn_;
};

#endif /* MASKCONSISTENCYCHECKER_H_ */
