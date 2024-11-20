#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
namespace nims_n
{
	struct MarquardtInput
	{
		std::vector<double>* ytrainData = nullptr;
		std::vector<double*> paramsToFit{};
		std::vector<double>relativeErrorChange{};
		std::vector<double>iterations{};
		std::vector<double>prediction{};
		
		std::function<void(std::vector<double>&)> objFunc = nullptr;
		std::function<void(std::string, int)> logger = nullptr;
		
		int iterationCount{ 0 };
		double lambda{ 1.0 };

		bool stopFitting{ false };
	};

	void marquardtAlgorithm(MarquardtInput* fitData);
	double sse(const std::vector<double>& lhs, const std::vector<double>& rhs);
}


