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

	

	class MultipleSimualation
	{
	public:
		MultipleSimualation(MultiSimData simData, std::function<void(std::string, bool& stopSims)>funct):
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

					passdown += fileName + variableNames[i] + "_" + c_name + "_";
				}
				innerLoop(passdown, stopSims);
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
	

	inline void runMultiScenarioLoop(SimulationData* simData)
	{

	}
}

