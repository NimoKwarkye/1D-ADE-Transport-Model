#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "MatArray.h"
namespace nims_n
{
	struct MarquardtInput
	{
		std::vector<double>* ytrainData = nullptr;
		std::vector<double*> paramsToFit{};
		std::vector<double>*relativeErrorChange = nullptr;
		std::vector<double>*iterations = nullptr;
		std::vector<double>prediction{};
		std::vector<std::string>paramsNames;
		
		std::function<void(std::vector<double>&)> objFunc = nullptr;
		std::function<void(std::string, int)> logger = nullptr;
		
		int iterationCount{ 0 };
		int* dataPoint{ 0 };
		int cachedErrorCount{ 20 };
		double lambda{ 1.0 };
		double lambdaUp{ 1.5 };

		double lambdaDown{ 3.0 };
		bool* stopFitting{ nullptr };
	};

	class MarquardtAlgorithm
	{
	private:
		bool optimize();
		void createJcMat();
		void logResults();
	public:

		MarquardtAlgorithm(MarquardtInput* _fitData);

		void operator()();


	private:
		MarquardtInput* fitData = nullptr;
		MatArray<double> jcobianT;
		std::vector<double>oldParams;
		MatArray<double>residuals;
		std::vector<double> deltaPrediction;
		double oldError = DBL_MAX;
		double currentError = 0.0;
		double relativeChange{ 0.0 };
		int paramsCount{0};
		int smpCount{ 0 };
	};
	double sse(const std::vector<double>& lhs, const std::vector<double>& rhs);
}


