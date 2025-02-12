#include "Optimizers.h"


double nims_n::sse(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
	double initVal{ 0.0 };
	return std::transform_reduce(lhs.begin(), lhs.end(), rhs.begin(), initVal, std::plus<>(),
						[](const double& x, const double& y) ->double{
								return (x - y) * (x - y);
							});
}

void nims_n::createJacobianMatrix(OptimizationInput* _fitData, std::vector<double>& deltaPd, MatArray<double>& outJcT)
{
	double delta = 1e-8;
	int paramsCount = _fitData->paramsToFit.size();
	int smpCount = _fitData->ytrainData->size();
	for (int i{ 0 }; i < paramsCount; i++)
	{
		*_fitData->paramsToFit[i] += delta;
		_fitData->objFunc(deltaPd);

		std::transform(deltaPd.begin(), deltaPd.end(),
			_fitData->prediction.begin(), deltaPd.begin(),
			[delta](const double& y, const double& x) {
				return (y - x) / delta;
			});

		auto begin = outJcT.begin() + i * smpCount;
		std::copy(deltaPd.begin(), deltaPd.end(), begin);

		*_fitData->paramsToFit[i] -= delta;
	}
}

bool nims_n::MarquardtAlgorithm::optimize()
{
	subtractMat(*fitData->ytrainData, fitData->prediction, residuals);
	/*currentError = std::transform_reduce(residuals.begin(), residuals.end(), 
											residuals.begin(), 0.0, std::plus<>(), std::multiplies<>());*/
	if(currentError < oldError)
	{
		mqParams->lambda /= mqParams->lambdaDown;
		MatArray<double>jcTjcInv;

		for (int i{ 0 }; i < paramsCount; i++)
			oldParams[i] = *fitData->paramsToFit[i];
		
		auto jcobian = jcobianT.getTransposed();
		auto jcTjc = jcobianT & jcobian;
		jcTjc += jcTjc.getDiag() * mqParams->lambda;
		try
		{
			jcTjcInv = inv(jcTjc);
		}
		catch (const std::exception& errorMessage)
		{
			std::string message(errorMessage.what());

			std::string logMessage = "Premature termination of marquardt. see error below\n" + message;
			fitData->logger(logMessage, -1);
			*fitData->stopFitting = true;
			return false;
		}
		
		auto jcTres = jcobianT & residuals;
		auto step = jcTjcInv & jcTres;

		for (int i{ 0 }; i < paramsCount; i++)
			*fitData->paramsToFit[i] += step[i];

		
	}
	else
	{
		for (int i{ 0 }; i < paramsCount; i++)
			*fitData->paramsToFit[i] = oldParams[i];

		mqParams->lambda *= mqParams->lambdaUp;
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
	ret += "\n";
	ret += "lambda: " + std::to_string(mqParams->lambda) + "\n";
	ret += "relativeChange: " + std::to_string(relativeChange) + "\n";
	ret += "iterations: " + std::to_string(fitData->iterationCount) + "\n";
	fitData->logger(ret, 0);
}

nims_n::MarquardtAlgorithm::MarquardtAlgorithm(OptimizationInput* _fitData, MarquardtInput* _mqParams): fitData{_fitData}, mqParams{_mqParams}
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
	fitData->iterations->resize(fitData->cachedErrorCount);
	fitData->relativeErrorChange->resize(fitData->cachedErrorCount);

	smpCount = fitData->ytrainData->size();
	fitData->prediction.resize(smpCount);
	deltaPrediction.resize(smpCount);
	jcobianT = zeros<double>(paramsCount, smpCount);
	residuals = zeros<double>(smpCount, 1);

	fitData->objFunc(fitData->prediction);

	currentError = sse(*fitData->ytrainData, fitData->prediction);

	oldError = DBL_MAX;
	relativeChange = std::abs(currentError);
	
	if (fitData->logger != nullptr)
		logResults();
	for (int i{ 0 }; i < paramsCount; i++)
		oldParams[i] = *fitData->paramsToFit[i];

	while (relativeChange > 1e-10)
	{
		createJcMat();
		if (!optimize())
			break;
		*fitData->dataPoint = fitData->iterationCount % fitData->cachedErrorCount;
		fitData->iterations->at(*fitData->dataPoint) = (double)(fitData->iterationCount + (size_t)1);
		fitData->relativeErrorChange->at(*fitData->dataPoint) = relativeChange;

		fitData->iterationCount++;
		fitData->objFunc(fitData->prediction);
		if (fitData->logger != nullptr)
			logResults();
		if (currentError < oldError)
			oldError = currentError;
		currentError = sse(*fitData->ytrainData, fitData->prediction);

		relativeChange = std::abs(currentError - oldError);
		if (*fitData->stopFitting)
		{
			if (currentError > oldError)
				for (int i{ 0 }; i < paramsCount; i++)
					*fitData->paramsToFit[i] = oldParams[i];
			break;
		}
	}
	deltaPrediction.clear();
	jcobianT = zeros<double>(1, 1);
	residuals = zeros<double>(1, 1);
}

void nims_n::ParamUncertainty::uncertainty()
{
	size_t smpCount = fitData->ytrainData->size();
	size_t paramsCount = fitData->paramsToFit.size();
	_unc.resize(paramsCount);

	fitData->prediction.resize(smpCount);
	std::vector<double> deltaPredictions(smpCount, 0.0);

	fitData->objFunc(fitData->prediction);
	MatArray<double> jcT = zeros<double>(smpCount, paramsCount);

	createJacobianMatrix(fitData, deltaPredictions, jcT);
	double err = sse(fitData->prediction, *fitData->ytrainData);
	double tdis{ 1.96 };
	if (smpCount - 1 < 30) {
		tdis = studentT[smpCount - 1];

	}
	auto jc = jcT.getTransposed();
	auto jcTjc = jcT & jc;
	auto jcTjc_inv = inv(jcTjc);
	jcTjc_inv *= err / (double)(smpCount - paramsCount);
	covarianceMat = jcTjc_inv;

	for (int i{0}; i < paramsCount; i++)
	{
		for (int j{ 0 }; j < paramsCount; j++)
		{
			if(i == j)
			{
				_unc[i] = std::sqrt(jcTjc_inv.at(i, j)) * tdis;
			}

		}
	}
	correlation();

	if (fitData->logger != nullptr && fitData->paramsNames.size() > 0)
		logResults();
}

void nims_n::ParamUncertainty::correlation()
{
	auto covDiag = covarianceMat.getDiag();
	std::transform(covDiag.begin(), covDiag.end(), covDiag.begin(), [](const double& x) {return std::sqrt(x); });
	covDiag = inv(covDiag);
	correlationMat = covDiag & covarianceMat & covDiag;
}

void nims_n::ParamUncertainty::logResults()
{
	size_t paramsCount = fitData->paramsToFit.size();
	std::string ret{ "Estimated Uncertainty for Selected Parameters\n" };
	for (int i{ 0 }; i < paramsCount; i++) {
		ret += fitData->paramsNames[i] + " = " + std::to_string(*fitData->paramsToFit[i]) + 
				std::string(" \u00b1 ") + std::to_string(_unc[i]) + "\n";
	}
	std::stringstream corrStr;
	correlationMat.print(corrStr);
	ret += "\n\n" + std::string("Correlation Matrix\n") + corrStr.str();

	fitData->logger(ret, 0);
}

nims_n::ParamUncertainty::ParamUncertainty(OptimizationInput* _fitData) : fitData{ _fitData }
{

}

void nims_n::ParamUncertainty::operator()()
{
	uncertainty();
}
