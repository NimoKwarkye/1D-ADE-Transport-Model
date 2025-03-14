#pragma once
#include <Windows.h>
#include <vector>
#include <atomic>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <functional>
#include <map>

#include "MatArray.h"
#include "ReadCSV.h"


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

#define INPUTMASS 1000
#define FLOWRATE 1001
#define ALP 1002
#define DIFCOEF 1003
#define TETHA 1004
#define BETA 1005
#define OMEGA 1006
#define RHO 1007
#define PCOEF 1008
#define RTCOEF 1009
#define K_FWD 1010
#define SMAX 1011
#define HYSTERESIS 1012
#define SOLNDEG 1013
#define SOLIDDEG_EQ 1014
#define SOLIDDEG_KIN 1015

namespace ntrans 
{
	struct ScenarioParams
	{
		double sceneTime{ 0.0 };
		std::vector<int> modifiedParams{};
		std::vector<double> paramValues{};

	};

	enum class CalibrationType {
		None,
		Optimizer,
		SensitivityAnalysis,
		MontecarloAnalysis
	};
	
	struct ConservativeNodes
	{
		double concValue{ 0.0 };
		double prevConcValue{ 0.0 };
		double mainDiagonal{ 0.0 };
		double upperDiagonal{ 0.0 };
		double lowerDiagonal{ 0.0 };
		double rhs{ 0.0 };
		double imConcValue{ 0.0 };
		double prevImConcValue{ 0.0 };

	};

	struct MultiSimData
	{
		std::vector<double*> variablesToModify{};
		std::vector<std::vector<double>> variableData{};
		std::vector<std::string> variableNames{};
	};

	struct LoopData
	{
		int level{ 0 };
		bool enterRange{ false };
		double rangeStart{ 0.0 };
		double rangeEnd{ 0.0 };
		double rangeStep{ 0.0 };
		std::string textInput{ "" };
		std::string paramName{ "" };

		LoopData(int pLevel, std::string pName) : level(pLevel), paramName(pName)
		{
		}
	};

	struct ScenarioLoopInfo
	{

		std::map<int, std::vector<LoopData>>scenarioLoopData;
		std::vector<std::string>paramNames{ "concentration",
											"peclet",
											"flow_rate",
											"smax_nf",
											"isotherm_K",
											"damkohler",
											"hysteresis_coef",
											"sol_deg_rate",
											"eq_sorbed_deg_rate",
											"kin_sorbed_deg_rate"
		};
		std::vector<std::string>addedParams;
		int maxLevels{ 10 };
		int selectedLevel{ 0 };
		int selectedName{ 0 };

		ScenarioLoopInfo()
		{
			for (int i = 0; i < maxLevels; i++)
			{
				scenarioLoopData[i] = std::vector<LoopData>{};
			}
		}

		bool addData(int level, std::string pName)
		{
			if (std::find(addedParams.begin(), addedParams.end(), pName) == addedParams.end())
			{
				addedParams.push_back(pName);
			}
			else
			{
				return false;
			}

			scenarioLoopData[level].push_back(LoopData(level, pName));
			return true;

		}

		void removeData(int level, std::string pName)
		{
			if (scenarioLoopData.find(level) != scenarioLoopData.end())
			{
				auto& loopData = scenarioLoopData[level];
				auto it = std::find_if(loopData.begin(), loopData.end(), [pName](LoopData& lData) {return lData.paramName == pName; });
				if (it != loopData.end())
				{
					loopData.erase(it);
				}
				auto it2 = std::find(addedParams.begin(), addedParams.end(), pName);
				if (it2 != addedParams.end())
				{
					addedParams.erase(it2);
				}
			}
		}

	};

	struct FlowInterrupts
	{
		double startTime{ 0.0 };
		double duration{ 0.0 };
	};
	struct UseLogScale
	{
		bool useLogScale{ false };
	};

	struct SensitivityAnalysisParams
	{
		std::vector<double*>sensitivityParams;
		std::vector<std::vector<double>>paramsRange;
		std::vector<UseLogScale> useLogScale;
		std::vector<std::string>paramsNames;
	};

	struct ReactiveNodes
	{
		double concValue{ 0.0 };
		double prevConcValue{ 0.0 };
		
		double prevEqSorbed{ 0.0 };
		double prevKinSorbed{ 0.0 };
		double maxEqConcVal{ 0.0 };
		double maxEqConcVal_tmp{ 0.0 };
		double maxKinConcVal{ 0.0 };
		double eqSorbed_tmp{ 0.0 };
		double kinSorbed_tmp{ 0.0 };
		
		double imConcValue{ 0.0 };
		double prevImConcValue{ 0.0 };
		double imPrevEqSorbed{ 0.0 };
		double imPrevKinSorbed{ 0.0 };
		double imMaxEqConcVal{ 0.0 };
		double imMaxEqConcVal_tmp{ 0.0 };
		double imMaxKinConcVal{ 0.0 };
		double imEqSorbed_tmp{ 0.0 };
		double imKinSorbed_tmp{ 0.0 };

		double totalSorbed{ 0.0 };

		double mainDiagonal{ 0.0 };
		double upperDiagonal{ 0.0 };
		double lowerDiagonal{ 0.0 };
		double rhs{ 0.0 };

	};

	struct TransportParameters
	{
		double pulseConcentration{ 0.0 };
		double maxInputConc{ 0.0 };
		double flowRate{ 0.0 };
		double flowVelocity{ 0.0 };
		double waterContent{ 0.0 };
		double dispersionLength{ 0.0 };
		double molecularDiffusion{ 0.0 };
		double mo_imPartitionCoefficient{ 1.0 };
		double mo_imExchangeRate{ 0.0 };
		double maxAdsorptionVal{ 0.0 };

		double bulkDensity{ 0.0 };
		double isothermConstant{ 0.0 };
		double adsorptionCapacity{ 0.0 }; // replaced with n for Freunlich isotherm.
		double hysteresisCoefficient{ 0.0 };
		double reactionRateCoefficient{ 0.0 };
		double eq_kinPartitionCoefficient{ 1.0 };

		double degradationRate_soln{ 0.0 };
		double degradationRate_eqsb{ 0.0 };
		double degradationRate_kinsb{ 0.0 };
	};

	struct SimulationOutPut
	{
		double totalInflowMass{ 0.0 };
		double totalOutflowMass{ 0.0 };
		double currentAdsorbedMass{ 0.0 };
		double currentImmobileMass{ 0.0 };
		double currentMobileMass{ 0.0 };
		double massBalanceValue{ 0.0 };
		double totalDegradedMass{ 0.0 };

		double centerMass{ 0.0 };
		double damkohler_obs{ 0.0 };
		double damkohler_prd{ 0.0 };
		double retardation_coef{ 0.0 };
		double peclet{ 0.0 };
		double exchPoreVol{ 0.0 };
		double mainiterationLength{ 0.0 };
		double mainJcTime{ 0.0 };
		double imobileSolTime{ 0.0 };
		int mainIterations{ 0 };
		int imobileIterations{ 0 };
		int currentSampleStep{ 0 };
		double curSimStep{ 0.0 };
		double disp_app{ 0.0 };

		std::string executionLoc{ "NA" };

		std::vector<double>predictedBT{};
		std::vector<double>observedBT{};
		std::vector<double>samplingTimes{};
		std::vector<double>samplingPoreVol{};
		std::vector<double>obsSamplingTimes{};
		std::vector<double>obsSamplingPoreVol{};
		std::vector<double>nodesConc{};
		std::vector<double>nodesSorbed{};
		std::vector<double>sorbedAtPoint{};
		std::vector<double>concAtPoint{};

		inline void init(int timeCount, double delta_t, double poreVolMult)
		{
			totalDegradedMass = 0.0;
			totalInflowMass = 0.0;
			totalOutflowMass = 0.0;
			currentAdsorbedMass = 0.0;
			currentImmobileMass = 0.0;
			currentMobileMass = 0.0;
			massBalanceValue = 0.0;
			currentSampleStep = 0;


			if(obsSamplingTimes.size() > 0)
			{
				samplingTimes.resize(obsSamplingTimes.size());
				samplingPoreVol.resize(obsSamplingTimes.size());

				std::copy(obsSamplingTimes.begin(), obsSamplingTimes.end(), samplingTimes.begin());
				if(obsSamplingTimes.size() == obsSamplingPoreVol.size())
				{
					std::copy(obsSamplingPoreVol.begin(), obsSamplingPoreVol.end(), samplingPoreVol.begin());
				}
				else
				{
					obsSamplingPoreVol.resize(obsSamplingTimes.size());
					double it{ 0.0 };
					for (int  i = 0; i < obsSamplingTimes.size(); i++)
					{
						obsSamplingPoreVol[i] = obsSamplingTimes[i] * poreVolMult;
						samplingPoreVol[i] = obsSamplingTimes[i] * poreVolMult;
					}

				}
			}
			else
			{
				samplingTimes.resize(timeCount);
				samplingPoreVol.resize(timeCount);
				double it{ 0.0 };
				for (int i = 0; i < timeCount; i++)
				{
					samplingTimes[i] = it;
					samplingPoreVol[i] = it * poreVolMult;
					it += delta_t;
				}
			}
			predictedBT.resize(samplingTimes.size());
			sorbedAtPoint.resize(samplingTimes.size());
			concAtPoint.resize(samplingTimes.size());
			executionLoc = "ADE Main";
		}
	};

	struct DomainParameters
	{
		double totalTransportTime{ 1.0 };
		double timestep{ 0.1 };

		double domainLength{ 1.0 };
		double domainSteps{ 0.1 };
		double crossSectionArea{ 1.0 };

		double effluentVesselVol{ 1.0 };
		int isothermType{ 0 };

		std::string simName{"ADE Model"};
		std::string simDir{""};
	};

	struct UIControls
	{
		int currentSensitivity{ 0 };
		std::atomic_bool canPlot{ false };
		std::atomic_bool isPlotting{ false };
		int simDelay{ 0 };
		bool isCalibration{ false };
		bool isRunning{ false };
		bool scheduleStop{ false };
		bool usePoreVols{ false };
		bool stopCustomLoop{ false };
		bool noObsData{ false };

	};


	struct SimulationData
	{
		DomainParameters columnParams;
		TransportParameters transParams;
		SimulationOutPut simOut;
		UIControls uiControls;
		
		std::vector<ConservativeNodes> conserveNodes;
		std::vector<ReactiveNodes> reactNodes;
		std::vector<ScenarioParams>scenarios{};
		std::vector<FlowInterrupts>flowInterrupts{ };
		std::vector<SensitivityAnalysisParams>sensitivityAnalysis{ SensitivityAnalysisParams() };
		std::vector<MultiSimData>multiSimData{ };
		std::vector<std::function<void(SimulationData*)>> multiSimDependencies{};
		ScenarioLoopInfo loopData;

		CalibrationType calibrationType{ CalibrationType::None };

		int maxIterations{ 200 };
		double noiseLevel{ 0.01 };
	};


	class ModelADE
	{
	public:
		ModelADE(SimulationData* _simData);
		ModelADE() = default;
		~ModelADE()
		{
			tp = nullptr;
			dp = nullptr;
			sout = nullptr;
			simData = nullptr;
		}
		void operator()();
		void saveModelParameters(SimulationData* mdParams, std::string fileName);
		void loadModelParameters(std::string fileName, SimulationData* loadParams);

	private:
		void init();
		void setTransportParameters(TransportParameters* lhs, TransportParameters* rhs);
		void solveTriDiag(std::vector<ConservativeNodes>* nodes);
		void solveTriDiag(std::vector<ReactiveNodes>* nodes);
		void subtractNodeValue(std::vector<ConservativeNodes>* lhsNodes);
		void subtractNodeValue(std::vector<ReactiveNodes>* lhsNodes);
		void resetNegatives(std::vector<ConservativeNodes>* lhsNodes);
		void resetNegatives(std::vector<ReactiveNodes>* lhsNodes);
		double sumNodeRhs(std::vector<ConservativeNodes>* lhsNodes);
		double sumNodeRhs(std::vector<ReactiveNodes>* lhsNodes);
		void calculateMassBalance(std::vector<ConservativeNodes>* lhsNodes);
		void calculateMassBalance(std::vector<ReactiveNodes>* lhsNodes);
		void evaluateScenarios(double currentSimTime);
		void runModel();

		void createJcReactive(bool isJc = false);
		double createJcReactiveImmobile(int loc, double mobile_c, double i_conc, bool isJc = false);
		double oneDfuncIm(double c1, double cOld, double sbEq, double sbEqKn_ex, double sbKin,
			double sbDeg, double imPhaseConst);
		double solveImobilePhaseWithSorption(double mobile_c, int loc);
		double oneDFunctReactive(double c1, double c2, double c3, double cOld, double mD, double uD,
			double lD, double sbEq, double sbEqKn_ex, double sbKin, double sbDeg, int loc);
		double langmuirIsotherm(double solConc, double eqConc, double maxSolConc);
		double freundlichIsotherm(double solConc);
		double linearIsotherm(double solConc);
		double getEqConcFromSorbed(double sorbed);
		void updateSorbedKinPhase();
		void reactiveLoop();

		void createJcConservative(bool isJc=false);
		double oneDFuncConservative(double c1, double c2, double c3, double oldC, double md, double ud, double ld);
		void updateImmobilePhase();
		void conservativeLoop();

		double effVessel(double cOut, double cvOld);


	private:
		SimulationData* simData{ nullptr };
		TransportParameters* tp{ nullptr };
		DomainParameters* dp{ nullptr };
		SimulationOutPut* sout{ nullptr };
		TransportParameters initialTranspValues;

		int nodeCount{ 1 };
		int timeCount{ 1 };
		int currentScene{ 0 };
		int currentInterrupt{ 0 };

		bool ignoreImmobileRegions{ true };
		bool ignoreReactions{ true };
		double minConc{ 1e-315 };

		std::vector<double> triDiagHelper{};
	};


	


	class MultipleSimulation
	{
	public:
		MultipleSimulation(MultiSimData simData, std::function<void(std::string, bool& stopSims)>funct):
			variablesToModify(simData.variablesToModify), 
			variableData(simData.variableData), 
			variableNames(simData.variableNames),
			innerLoop(funct)
		{
			min_cnt = variableData[0].size();
			for (size_t i = 1; i < variableData.size(); i++)
			{
				if (variableData[i].size() < min_cnt)
				{
					min_cnt = variableData[i].size();
				}
			}
			cachedVariables.resize(variablesToModify.size());
			for (size_t i = 0; i < variablesToModify.size(); i++)
			{
				cachedVariables[i] = *variablesToModify[i];
			}
		}

		inline size_t size()
		{

			return min_cnt;
		}

		inline void operator()(std::string fileName, bool& stopSims)
		{
			size_t varCount{ 0 };
			while (varCount < min_cnt && !stopSims)
			{
				std::string passdown = "";
				for (size_t i{ 0 }; i < variableData.size(); i++)
				{
					*variablesToModify[i] = variableData[i][varCount];

					std::string c_name = std::to_string(*variablesToModify[i]);
					c_name = c_name.substr(0, c_name.find_first_of('.')) + "_" + 
						c_name.substr(c_name.find_first_of('.') + 1, c_name.size());

					passdown += variableNames[i] + "_" + c_name + "_";
				}
				innerLoop(fileName + passdown, stopSims);
				varCount++;
			}

			for (size_t i{ 0 }; i < variablesToModify.size(); i++)
				*variablesToModify[i] = cachedVariables[i];

		}
		

	private:
		std::vector<double*> variablesToModify{};
		std::vector<std::vector<double>> variableData{};
		std::vector<std::string> variableNames{};
		size_t min_cnt{ 0 };
		std::function<void(std::string, bool& stopSims)> innerLoop;
		std::vector<double>cachedVariables{};
	};

	inline double langmuirIsotherm(double eqConc, double kl, double smax)
	{
		return (kl * eqConc * smax) / (1 + (eqConc * kl));
	}

	inline double freundlichIsotherm(double eqConc, double kn, double nf)
	{
		return kn * std::pow(eqConc, nf);
	}
	
	inline double linearIsotherm(double eqConc, double k)
	{
		return k * eqConc;
	}

	inline double retardationCoeff(SimulationData* mdp)
	{
		if (mdp->columnParams.isothermType == 1)
		{
			double s_eq = langmuirIsotherm(mdp->transParams.pulseConcentration,
				mdp->transParams.isothermConstant, mdp->transParams.adsorptionCapacity);


			return 1.0 + (s_eq * mdp->transParams.bulkDensity / 
						mdp->transParams.pulseConcentration) /
						mdp->transParams.waterContent;
		}
		else if (mdp->columnParams.isothermType == 2)
		{
			double s_eq = freundlichIsotherm(mdp->transParams.pulseConcentration,
				mdp->transParams.isothermConstant, mdp->transParams.adsorptionCapacity);

			return 1.0 + (s_eq * mdp->transParams.bulkDensity / 
				mdp->transParams.pulseConcentration) / mdp->transParams.waterContent;
		}
		else if (mdp->columnParams.isothermType == 3)
		{
			double s_eq = linearIsotherm(mdp->transParams.pulseConcentration,
				mdp->transParams.isothermConstant);
			return 1.0 + (s_eq * mdp->transParams.bulkDensity) /
				mdp->transParams.waterContent;
		}

		return 0.0;
	}


	inline void set_rt_from_da(SimulationData* simData)
	{
		double rd = retardationCoeff(simData);
		double velocity{ 0 };
		
		if (simData->uiControls.usePoreVols) {
			velocity = simData->columnParams.domainLength * 
				simData->transParams.waterContent * simData->transParams.flowRate / 24.0;
		}
		else {
			velocity = simData->transParams.flowRate;
		}
		double vt = (velocity / simData->transParams.waterContent) / rd;
		simData->transParams.reactionRateCoefficient = simData->simOut.damkohler_prd * 
							vt / simData->columnParams.domainLength;
	}

	inline void set_disp_from_pe(SimulationData* simData)
	{
		simData->transParams.dispersionLength = simData->columnParams.domainLength / simData->simOut.peclet;
	}

	inline std::vector<double> delimited_string_to_vec(std::string str)
	{
		std::stringstream ss(str);
		std::string on_val{ "" };
		std::vector<double> ret_data{};
		while (std::getline(ss, on_val, ','))
		{
			ret_data.push_back(std::stod(on_val));
		}
		return ret_data;
	}

	inline std::vector<double> generate_range(double min, double max, double step)
	{
		std::vector<double> ret_data{};
		for (double val{ min }; val < max; val += step)
			ret_data.push_back(val);
		return ret_data;
	}


	inline void prepareLoopData(ScenarioLoopInfo& data, SimulationData* simData)
	{
		auto set_data = [](double* varToModify, std::string pName, LoopData& loopData, MultiSimData& one_data)
			{ 
				one_data.variablesToModify.push_back(varToModify);
				one_data.variableNames.push_back(pName);

				if (loopData.enterRange)
				{
					one_data.variableData.push_back(generate_range(loopData.rangeStart, loopData.rangeEnd, loopData.rangeStep));
				}
				else
				{
					one_data.variableData.push_back(delimited_string_to_vec(loopData.textInput));
				}
			};

		for (auto [level, loopData] : data.scenarioLoopData)
		{
			MultiSimData oneLevelData;
			for (size_t j{0}; j < loopData.size(); j++)
			{
				if (loopData[j].paramName == "concentration")
				{
					set_data(&simData->transParams.pulseConcentration, "c", loopData[j], oneLevelData);

				}
				else if(loopData[j].paramName == "peclet")
				{
					set_data(&simData->simOut.peclet, "Pe", loopData[j], oneLevelData);
					simData->multiSimDependencies.push_back(set_disp_from_pe);
				}
				else if (loopData[j].paramName == "flow_rate")
				{
					set_data(&simData->transParams.flowRate, "fr", loopData[j], oneLevelData);
				}
				else if (loopData[j].paramName == "smax_nf")
				{
					set_data(&simData->transParams.adsorptionCapacity, "smax_nf", loopData[j], oneLevelData);
				}

				else if (loopData[j].paramName == "isotherm_K")
				{
					set_data(&simData->transParams.isothermConstant, "K", loopData[j], oneLevelData);
				}
				else if (loopData[j].paramName == "damkohler")
				{
					set_data(&simData->simOut.damkohler_prd, "Da", loopData[j], oneLevelData);
					simData->multiSimDependencies.push_back(set_rt_from_da);
				}
				else if (loopData[j].paramName == "hysteresis_coef")
				{
					set_data(&simData->transParams.hysteresisCoefficient, "h", loopData[j], oneLevelData);
				}
				else if (loopData[j].paramName == "sol_deg_rate")
				{
					set_data(&simData->transParams.degradationRate_soln, "c_deg", loopData[j], oneLevelData);
				}
				else if (loopData[j].paramName == "eq_sorbed_deg_rate")
				{
					set_data(&simData->transParams.degradationRate_eqsb, "s_deg", loopData[j], oneLevelData);
				}
				else if (loopData[j].paramName == "kin_sorbed_deg_rate")
				{
					set_data(&simData->transParams.degradationRate_kinsb, "s_deg_kin", loopData[j], oneLevelData);
				}
				
				simData->multiSimData.push_back(oneLevelData);
			}
		}
	}

	inline void runMultiScenarioLoop(SimulationData* simData)
	{
		simData->uiControls.isRunning = true;
		simData->uiControls.scheduleStop = false;
		simData->uiControls.isCalibration = true;
		simData->calibrationType = CalibrationType::SensitivityAnalysis;
		prepareLoopData(simData->loopData, simData);
		size_t lastIndex = simData->multiSimData.size() - 1;
		std::string simDir = simData->columnParams.simDir + "/multiple_simulation/";
		if (!std::filesystem::exists(simDir))
			std::filesystem::create_directories(simDir);

		MultipleSimulation lastSim (simData->multiSimData[lastIndex],
						[simData, simDir](std::string passdown, bool& stopSims) 
						{
						
								for (auto& dep : simData->multiSimDependencies)
									dep(simData);
								for (int scn{0}; scn < simData->scenarios.size(); scn++)
								{
									for (int dt{0}; dt < simData->scenarios[scn].modifiedParams.size(); dt++)
									{
										if (simData->scenarios[scn].modifiedParams[dt] == INPUTMASS &&
											simData->scenarios[scn].paramValues[dt] > 1e-10)
										{
											simData->scenarios[scn].paramValues[dt] = simData->transParams.pulseConcentration;
										}
									}
								}

								ModelADE model(simData);
								model();
								size_t dataLen = simData->simOut.predictedBT.size();
								nims_n::MatArray<double> dataMat = nims_n::zeros<double>(dataLen, 4);
								for (size_t i{ 0 }; i < dataLen; i++)
								{
									size_t idx = i * 4;
									dataMat[idx] = simData->simOut.samplingTimes[i];
									dataMat[idx + 1] = simData->simOut.samplingPoreVol[i];
									dataMat[idx + 2] = simData->simOut.predictedBT[i]/simData->transParams.pulseConcentration;
									dataMat[idx + 3] = simData->simOut.sorbedAtPoint[i];
								}
								std::string saveName = simDir + passdown + ".csv";
								std::vector<std::string> header{ "time [h]", "pv [-]", "c [mg/L]", "s [mg/g]" };
								nims_n::saveCSV(saveName, dataMat, ',', header);
						});

		for (int j = (int)simData->multiSimData.size() - 2; j >= 0; j--)
		{
			MultipleSimulation nextSim(simData->multiSimData[j], lastSim);
			lastSim = nextSim;
		}

		lastSim("", simData->uiControls.scheduleStop);

		for (auto& dep : simData->multiSimDependencies)
			dep(simData);
		for (int scn{ 0 }; scn < simData->scenarios.size(); scn++)
		{
			for (int dt{ 0 }; dt < simData->scenarios[scn].modifiedParams.size(); dt++)
			{
				if (simData->scenarios[scn].modifiedParams[dt] == INPUTMASS &&
					simData->scenarios[scn].paramValues[dt] > 1e-10)
				{
					simData->scenarios[scn].paramValues[dt] = simData->transParams.pulseConcentration;
				}
			}
		}

		simData->uiControls.isRunning = false;
		simData->uiControls.scheduleStop = false;

	}
}

