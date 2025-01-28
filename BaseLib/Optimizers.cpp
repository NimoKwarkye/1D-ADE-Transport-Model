#include "Optimizers.h"


double nims_n::sse(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	return std::transform_reduce(lhs.begin(), lhs.end(), rhs.begin(), 0.0, std::plus<>(),
						[](const double& x, const double& y) ->double{
								return (x - y) * (x - y);
							});
}

bool nims_n::MarquardtAlgorithm::optimize()
{
	subtractMat(*fitData->ytrainData, fitData->prediction, residuals);
	currentError = std::transform_reduce(residuals.begin(), residuals.end(), 
											residuals.begin(), 0.0, std::plus<>(), std::multiplies<>());
	if(currentError < oldError)
	{
		fitData->lambda /= fitData->lambdaDown;
		MatArray<double>jcTjcInv;

		for (int i{ 0 }; i < paramsCount; i++)
			oldParams[i] = *fitData->paramsToFit[i];
		
		auto jcobian = jcobianT.getTransposed();
		auto jcTjc = jcobianT & jcobian;
		jcTjc += jcTjc.getDiag() * fitData->lambda;
		try
		{
			jcTjcInv = inv(jcTjc);
		}
		catch (const std::exception& errorMessage)
		{
			std::string message(errorMessage.what());

			std::string logMessage = "Premature termination of marquardt. see error below\n" + message;

			fitData->stopFitting = true;
			return false;
		}
		
		auto jcTres = jcobianT & residuals;
		auto step = jcTjcInv & jcTres;

		for (int i{ 0 }; i < paramsCount; i++)
			*fitData->paramsToFit[i] += step[i];

		relativeChange = std::abs(currentError - oldError);
		oldError = currentError;
	}
	else
	{
		for (int i{ 0 }; i < paramsCount; i++)
			*fitData->paramsToFit[i] = oldParams[i];

		fitData->lambda /= fitData->lambdaUp;
	}
	return true;

}

void nims_n::MarquardtAlgorithm::createJcMat()
{
	double delta = 1e-8;
	
	for (int i{ 0 }; i < paramsCount; i++)
	{
		*fitData->paramsToFit[i] += delta;
		fitData->objFunc(deltaPrediction);

		std::transform(deltaPrediction.begin(), deltaPrediction.end(),
			fitData->prediction.begin(), deltaPrediction.begin(), 
			[delta](const double& y, const double& x) {
				return (y - x) / delta;
			});

		auto begin = jcobianT.begin() + i * smpCount;
		std::copy(deltaPrediction.begin(), deltaPrediction.end(), begin);

		*fitData->paramsToFit[i] -= delta;
	}
}

void nims_n::MarquardtAlgorithm::logResults()
{
	std::string ret{ "" };
	for (int i{ 0 }; i < fitData->paramsNames.size(); i++) {
		ret += fitData->paramsNames[i] + ": " + std::to_string(*fitData->paramsToFit[i]) + "\n";
	}
	ret += "lambda: " + std::to_string(fitData->lambda) + "\n";
	fitData->logger(ret, 0);
}

nims_n::MarquardtAlgorithm::MarquardtAlgorithm(MarquardtInput* _fitData): fitData{_fitData}
{
	
}

void nims_n::MarquardtAlgorithm::operator()()
{
	if (fitData->objFunc == nullptr || fitData->ytrainData == nullptr)
	{
		if (fitData->logger != nullptr)
		{
			std::string msg = "No observation data selected";
			fitData->logger(msg, -1);
		}
		return;
	}

	paramsCount = fitData->paramsToFit.size();
	if (oldParams.size() != paramsCount)
		oldParams.resize(paramsCount);
	fitData->iterations.resize(fitData->cachedErrorCount);
	fitData->relativeErrorChange.resize(fitData->cachedErrorCount);

	smpCount = fitData->ytrainData->size();
	fitData->prediction.resize(smpCount);
	deltaPrediction.resize(smpCount);
	jcobianT = zeros<double>(paramsCount, smpCount);
	residuals = zeros<double>(smpCount, 1);

	fitData->objFunc(fitData->prediction);

	oldError = sse(*fitData->ytrainData, fitData->prediction);

	currentError = 0.0;
	double absChange = oldError - currentError;
	
	if (fitData->logger != nullptr)
		logResults();

	for (int i{ 0 }; i < paramsCount; i++)
		oldParams[i] = *fitData->paramsToFit[i];

	while (absChange > 1e-10)
	{
		if (fitData->stopFitting)
			break;
		createJcMat();
		if (optimize())
			break;
		fitData->dataPoint = fitData->iterationCount % fitData->cachedErrorCount;
		fitData->iterations[fitData->dataPoint] = (double)(fitData->iterationCount + 1);
		fitData->relativeErrorChange[fitData->dataPoint] = relativeChange;

		fitData->iterationCount++;
		if (fitData->logger != nullptr)
			logResults();

	}
	deltaPrediction.clear();
	jcobianT = zeros<double>(1, 1);
	residuals = zeros<double>(1, 1);

	for (int i{ 0 }; i < paramsCount; i++)
		*fitData->paramsToFit[i] = oldParams[i];
}
