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
#include "cpp-opt/cppOpt.h"

using namespace cppOpt;

cv::Mat image_src, image_truth;

template <typename T>
class MySolver : public OptSolverBase<T>
{
public:
	void calculate(OptCalculation<T> &optCalculation) const
	{
		double p1_m = optCalculation.get_parameter("P1_M");
		double p1_b = optCalculation.get_parameter("P1_B");
		double p2 = optCalculation.get_parameter("P2");
		double om = optCalculation.get_parameter("OM");
		double tm = optCalculation.get_parameter("TM");

		cv::Mat image_test;
		PowerLineDetection(image_src, image_test, p1_m, p1_b, p2, om, tm);

		MaskConsistencyChecker checker;
		checker.SetMaskTruth(image_truth);
		checker.SetMaskTest(image_test);
		checker.CheckMaskConsistency();

		double percent_correct = checker.GetPercentCorrect();

		optCalculation.result = percent_correct;

		printf("Iteration complete! Correct: %g %%\n", percent_correct);
	}
};

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

	image_src = cv::imread(filename_src);
	image_truth = cv::imread(filename_truth);

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


	// Create solver
	MySolver<double> mySolver;

	// Define parameters and boundaries
	OptBoundaries<double> optBoundaries;
	optBoundaries.add_boundary(0.0, 2.0, "P1_M");
	optBoundaries.add_boundary(1.0, 25.0, "P1_B");
	optBoundaries.add_boundary(1.0, 400.0, "P2");
	optBoundaries.add_boundary(1.0, 20.0, "OM");
	optBoundaries.add_boundary(0.0, 5.0, "TM");

	// Number of iterations
	unsigned int maxCalculations = 200;

	// Goal of optimization
	OptTarget optTarget = MAXIMIZE;

	// How fast algorithm slows down
	double coolingFactor = 0.95;

	// How likely algorithm follows bad solutions at the beginning of optimization.
	double startChance = 0.25;

	// Create optimizer
	OptSimulatedAnnealing<double> opt(optBoundaries, maxCalculations, &mySolver, optTarget,
										0.0, coolingFactor, startChance);

	// Enable logging of parameters and begin optimization
	OptBase<double>::enable_logging("PARAMETERS.log", optBoundaries);
	OptBase<double>::run_optimisations();

	// Print results
	OptCalculation<double> best = opt.get_best_calculation();
	std::cout << best.to_string_header() << std::endl;
	std::cout << best.to_string_values() << std::endl;

	return 0;
}
