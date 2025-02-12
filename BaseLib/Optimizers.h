#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

#include "MatArray.h"
namespace nims_n
{
	struct OptimizationInput
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
		
		bool* stopFitting{ nullptr };
	};

	struct MarquardtInput
	{
		double lambda{ 1.0 };
		double lambdaUp{ 1.5 };
		double lambdaDown{ 3.0 };
	};

	class MarquardtAlgorithm
	{
	private:
		bool optimize();
		void createJcMat();
		void logResults();
	public:

		MarquardtAlgorithm(OptimizationInput* _fitData, MarquardtInput* _mqParams);

		void operator()();


	private:
		OptimizationInput* fitData = nullptr;
		MarquardtInput* mqParams = nullptr;
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
	
	class ParamUncertainty
	{
		std::vector<double>studentT{
		12.706,4.303,3.182,2.776,2.571,2.447,
		2.365,2.306,2.262,2.228,2.201,2.179,
		2.16,2.145,2.131,2.12,2.11,2.101,2.093,
		2.086,2.08,2.074,2.069,2.064,2.06,2.056,
		2.052,2.048,2.045,2.042
		};
	private:
		void uncertainty();
		void correlation();
		void logResults();


	public:
		ParamUncertainty(OptimizationInput* _fitData);

		void operator ()();
	private:
		OptimizationInput* fitData = nullptr;
		MatArray<double> correlationMat;
		MatArray<double> covarianceMat;
		std::vector<double>_unc;
	};
	
	double sse(const std::vector<double>& lhs, const std::vector<double>& rhs);
	void createJacobianMatrix(OptimizationInput* _fitData, std::vector<double>& deltaPd, MatArray<double>& outJcT);
}


