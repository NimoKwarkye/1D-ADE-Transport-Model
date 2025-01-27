#include "Optimizers.h"


double nims_n::sse(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	return std::transform_reduce(lhs.begin(), lhs.end(), rhs.begin(), 0.0, std::plus<>(),
						[](const double& x, const double& y) ->double{
								return (x - y) * (x - y);
							});
}

nims_n::MarquardtAlgorithm::MarquardtAlgorithm(MarquardtInput* _fitData): fitData{_fitData}
{
	
	
}

void nims_n::MarquardtAlgorithm::operator()()
{
	if (fitData->objFunc == nullptr || fitData->ytrainData == nullptr)
	{
		//log a message
		return;
	}

	int paramsCount = fitData->paramsToFit.size();

	fitData->prediction.resize(fitData->ytrainData->size());
	std::vector<double> deltaPrediction(fitData->ytrainData->size());
	std::vector<double> jc(fitData->ytrainData->size() * paramsCount);

	fitData->objFunc(fitData->prediction);

	double oldError = sse(*fitData->ytrainData, fitData->prediction);
	double currentError = 0.0;

	double absChange = oldError - currentError;
	double delta = 1e-8;
	//perform logging

	std::vector<double> currentOptimalParams(paramsCount);
	for (int i{ 0 }; i < paramsCount; i++)
		currentOptimalParams[i] = *fitData->paramsToFit[i];
	while (absChange > 1e-10)
	{
		if (fitData->stopFitting)
			break;

		for (int i{ 0 }; i < paramsCount; i++)
		{
			*fitData->paramsToFit[i] += delta;
		}

	}
}
