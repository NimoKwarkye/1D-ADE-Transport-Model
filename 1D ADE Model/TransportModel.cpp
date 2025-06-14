#include "TransportModel.h"

ntrans::ModelADE::ModelADE(SimulationData* _simData) : simData{ _simData }
{
	dp = &simData->columnParams;
	tp = &simData->transParams;
	sout = &simData->simOut;
}

void ntrans::ModelADE::operator()()
{
	simData->uiControls.isRunning = true;
	init();

	runModel();

	sout->executionLoc = "ADE Main";

	// restore saved parameters
	setTransportParameters(tp, &initialTranspValues);


	if (!simData->uiControls.isCalibration) {

		simData->uiControls.scheduleStop = false;
		simData->uiControls.isRunning = false;
	}

	sout->executionLoc = "NA";
}

void ntrans::ModelADE::saveModelParameters(SimulationData* mdParams, std::string folderName)
{
	std::time_t t = std::time(0);
	struct tm newtime;
	localtime_s(&newtime, &t);
	std::string simDate = std::to_string(newtime.tm_year + 1900) + std::to_string(newtime.tm_mon + 1) +
		std::to_string(newtime.tm_mday) + "_" + std::to_string(newtime.tm_hour);

	std::string filename = folderName + "/" + mdParams->columnParams.simName + "_Params_" + simDate + ".ini";



	std::ofstream out;
	out.open(filename);
	if (out.is_open()) {
		// save space discretization
		out << "Space in X-Direction		:" << mdParams->columnParams.domainLength << "\n";
		out << "Step-Size in X				:" << mdParams->columnParams.domainSteps << "\n";
		//out << "Input Mass Location			:" << mdParams->inputLoc << "\n";
		out << "Cross-sec area				:" << mdParams->columnParams.crossSectionArea << "\n";
		out << "Eff Vessel Vol				:" << mdParams->columnParams.effluentVesselVol << "\n";

		//save time discretization
		out << "Total Simulation Time		:" << mdParams->columnParams.totalTransportTime << "\n";
		out << "Simulation Time Steps		:" << mdParams->columnParams.timestep << "\n";

		//save basic model parameters
		out << "Molecular Diffusion Coef	:" << mdParams->transParams.molecularDiffusion << "\n";
		out << "Dispersion Length			:" << mdParams->transParams.dispersionLength << "\n";
		out << "Water Content (tetha)		:" << mdParams->transParams.waterContent << "\n";
		out << "Flow velocity (L/T)			:" << mdParams->transParams.flowRate << "\n";
		out << "Mass Transfer Rate_im		:" << mdParams->transParams.mo_imExchangeRate << "\n";
		out << "Im<->mobile Ratio			:" << mdParams->transParams.mo_imPartitionCoefficient << "\n";
		out << "Pulse Concentration			:" << mdParams->transParams.pulseConcentration << "\n";

		//save sorption parameters
		out << "Bulk Density 				:" << mdParams->transParams.bulkDensity << "\n";
		out << "Isotherm Type				:" << mdParams->columnParams.isothermType << "\n";
		out << "Isotherm Constant			:" << mdParams->transParams.isothermConstant << "\n";
		out << "Max Adsorption Cons			:" << mdParams->transParams.adsorptionCapacity << "\n";
		out << "Mass Transfer Rate_sop		:" << mdParams->transParams.reactionRateCoefficient << "\n";
		out << "Sol-Liq Partition Coef		:" << mdParams->transParams.eq_kinPartitionCoefficient << "\n";
		out << "Hysteresis Coef				:" << mdParams->transParams.hysteresisCoefficient << "\n";

		//save degradation parameters
		out << "Liquid-Phase Deg Rate		:" << mdParams->transParams.degradationRate_soln << "\n";
		out << "Solid-Phase Deg Rate		:" << mdParams->transParams.degradationRate_eqsb << "\n";
		out << "Solid-Phase_kin Deg Rate	:" << mdParams->transParams.degradationRate_kinsb << "\n";

		//save logistics parameters
		out << "use pore vol				:" << mdParams->uiControls.usePoreVols << "\n";
		out << "Simulation Name				:" << mdParams->columnParams.simName << "\n";
		out << "Simulation Dir				:" << mdParams->columnParams.simDir << "\n";
		//out << "Sampling point_col			:" << mdParams->samplePoint_col << "\n";
		//out << "Observation file			:" << mdParams->obsFile << "\n";

		//save estimations
		out << "Damkohler_obs number		:" << mdParams->simOut.damkohler_obs << "\n";
		out << "Damkohler_prd number		:" << mdParams->simOut.damkohler_prd << "\n";
		out << "Retardation coef			:" << mdParams->simOut.retardation_coef << "\n";
		out << "Peclet number				:" << mdParams->simOut.peclet << "\n";
		out << "Exchange pore volume		:" << mdParams->simOut.exchPoreVol << "\n";
		out << "mass balance				:" << mdParams->simOut.massBalanceValue << "\n";
		out << "adsorbed mass				:" << mdParams->simOut.currentAdsorbedMass << "\n";
		out << "immobile mass				:" << mdParams->simOut.currentImmobileMass << "\n";
		out << "mobile mass					:" << mdParams->simOut.currentMobileMass << "\n";
		out << "outflow mass				:" << mdParams->simOut.totalOutflowMass << "\n";
		out << "inflow mass					:" << mdParams->simOut.totalInflowMass << "\n";
		out << "center of mass				:" << mdParams->simOut.centerMass << "\n";
		if (!mdParams->scenarios.empty()) {
			out << "Begin Scenarios\n";

			for (int i{ 0 }; i < mdParams->scenarios.size(); i++) {
				out << mdParams->scenarios[i].sceneTime << "\t" << mdParams->scenarios[i].modifiedParams.size() << "\t";
				for (int j{ 0 }; j < mdParams->scenarios[i].modifiedParams.size(); j++)
					out << mdParams->scenarios[i].modifiedParams[j] << "\t" << mdParams->scenarios[i].paramValues[j] << "\t";
				out << "\n";
			}

			out << "End Scenarios\n";
		}

		if (!mdParams->flowInterrupts.data.empty()) {
			out << "Begin FlowInterrupts\n";
			out << "use_pv" << "\t" << mdParams->flowInterrupts.usePoreVol << "\n";
			for (int i{ 0 }; i < mdParams->flowInterrupts.data.size(); i++) {
				out << mdParams->flowInterrupts.data[i].startTime << "\t" << mdParams->flowInterrupts.data[i].duration << "\t";

				out << "\n";
			}

			out << "End FlowInterrupts\n";
		}

		if (!mdParams->loopData.addedParams.empty())
		{
			out << "Begin CustomSim\n";
			out << "addedParams:\t";
			for (auto pName : mdParams->loopData.addedParams)
			{
				out << pName << "\t";
			}
			out << "\n";
			out << "maxLevels:\t" << mdParams->loopData.maxLevels << "\n";
			out << "selectedLevel:\t" << mdParams->loopData.selectedLevel << "\n";
			out << "selectedName:\t" << mdParams->loopData.selectedName << "\n";

			for (auto [level, loopData] : mdParams->loopData.scenarioLoopData)
			{
				if (!loopData.empty())
				{
					out << "Level:\t" << level << "\n";
					for (int i{ 0 }; i < loopData.size(); i++)
					{
						out << "name:\t" << loopData[i].paramName << "\n";
						out << "textInput:\t" << loopData[i].textInput << "\n";
						out << "range:\t" << loopData[i].rangeStart << "\t" << loopData[i].rangeEnd << "\t" << loopData[i].rangeStep << "\n";
						out << "usedRange:\t" << loopData[i].enterRange << "\n";
						out << "endLevelData\n";
					}
				}
				
			}
			out << "End CustomSim\n";
		}
		out.close();
	}
	else {
		//std::cout << "Could not open simulation folder " + filename + "\n";
	}
}

void ntrans::ModelADE::loadModelParameters(std::string fileName, SimulationData* loadParams)
{
	std::ifstream in;
	std::string line_1;
	std::string line;
	std::vector<std::string>savedNames{};
	std::vector<std::string>savedValues{};
	std::vector<std::string>savedScenarios{};
	std::vector<std::string>savedFLT{};
	std::vector<std::string>savedCustomSim{};
	bool flowIntUsePV{ false };
	in.open(fileName);
	if (in.good()) {
		//load space parameters
		while (!in.eof()) {
			line.clear();
			line_1.clear();
			//std::getline(in, line_1, ':');
			std::getline(in, line, '\n');
			if (line == "Begin Scenarios") {
				std::getline(in, line, '\n');
				while (line != "End Scenarios") {
					savedScenarios.push_back(line);
					std::getline(in, line, '\n');
				}
			}
			else if (line == "Begin FlowInterrupts") {
				std::getline(in, line, '\n');
				while (line != "End FlowInterrupts") {
					if (line.starts_with("use_pv"))
					{
						std::string pv_str;
						std::stringstream pv_ss(line);
						std::getline(pv_ss, pv_str, '\t');
						std::getline(pv_ss, pv_str, '\t');
						flowIntUsePV = std::stoi(pv_str);
					}
					else
					{
						savedFLT.push_back(line);
					}
					std::getline(in, line, '\n');
				}
			}
			else if (line == "Begin CustomSim") {
				std::getline(in, line, '\n');
				while (line != "End CustomSim") {
					savedCustomSim.push_back(line);
					std::getline(in, line, '\n');
				}
			}
			else if (!line.empty()) {

				savedNames.push_back(line.substr(0, line.find_last_of(':')));
				savedValues.push_back(line.substr(line.find_last_of(':') + 1, line.size()));
			}

		}
		in.close();

		for (int i{ 0 }; i < savedNames.size(); i++) {
			line_1 = savedNames[i];
			line = savedValues[i];
			if (line_1 == ("Space in X-Direction		")) {
				loadParams->columnParams.domainLength = std::stod(line);
			}
			else if (line_1 == ("Step-Size in X				")) {
				loadParams->columnParams.domainSteps = std::stod(line);
			}
			/*else if (line_1 == ("Input Mass Location			")) {
				loadParams->inputLoc = std::stoi(line);
			}*/
			else if (line_1 == ("Total Simulation Time		")) {
				loadParams->columnParams.totalTransportTime = std::stod(line);
			}
			else if (line_1 == ("Simulation Time Steps		")) {
				loadParams->columnParams.timestep = std::stod(line);
			}
			else if (line_1 == ("Cross-sec area				")) {
				loadParams->columnParams.crossSectionArea = std::stod(line);
			}
			else if (line_1 == ("Eff Vessel Vol				")) {
				loadParams->columnParams.effluentVesselVol = std::stod(line);
			}

			//basic model parameters
			else if (line_1 == ("Molecular Diffusion Coef	")) {
				loadParams->transParams.molecularDiffusion = std::stod(line);
			}
			else if (line_1 == ("Dispersion Length			")) {
				loadParams->transParams.dispersionLength = std::stod(line);
			}
			else if (line_1 == ("Water Content (tetha)		")) {
				loadParams->transParams.waterContent = std::stod(line);
			}
			else if (line_1 == ("Flow velocity (L/T)			")) {
				loadParams->transParams.flowRate = std::stod(line);
			}
			else if (line_1 == ("Mass Transfer Rate_im		")) {
				loadParams->transParams.mo_imExchangeRate = std::stod(line);
			}
			else if (line_1 == ("Im<->mobile Ratio			")) {
				loadParams->transParams.mo_imPartitionCoefficient = std::stod(line);
			}
			else if (line_1 == ("Pulse Concentration			")) {
				loadParams->transParams.pulseConcentration = std::stod(line);
				loadParams->transParams.maxInputConc = loadParams->transParams.pulseConcentration;
			}

			//load sorption parameters
			else if (line_1 == ("Bulk Density 				")) {
				loadParams->transParams.bulkDensity = std::stod(line);
			}
			else if (line_1 == ("Isotherm Type				")) {
				loadParams->columnParams.isothermType = std::stoi(line);
			}
			else if (line_1 == ("Isotherm Constant			")) {
				loadParams->transParams.isothermConstant = std::stod(line);
			}
			
			
			else if (line_1 == ("Max Adsorption Cons			")) {
				loadParams->transParams.adsorptionCapacity = std::stod(line);
			}

			
			else if (line_1 == ("Mass Transfer Rate_sop		")) {
				loadParams->transParams.reactionRateCoefficient = std::stod(line);
			}
			else if (line_1 == ("Sol-Liq Partition Coef		")) {
				loadParams->transParams.eq_kinPartitionCoefficient = std::stod(line);
			}

			else if (line_1 == ("Hysteresis Coef				")) {
				loadParams->transParams.hysteresisCoefficient = std::stod(line);
			}
			

			//load degradation params
			else if (line_1 == ("Liquid-Phase Deg Rate		")) {
				loadParams->transParams.degradationRate_soln = std::stod(line);
			}
			else if (line_1 == ("Solid-Phase Deg Rate		")) {
				loadParams->transParams.degradationRate_eqsb = std::stod(line);
			}
			else if (line_1 == ("Solid-Phase_kin Deg Rate	")) {
				loadParams->transParams.degradationRate_kinsb = std::stod(line);
			}

			//load logistics
			
			else if (line_1 == ("use pore vol				")) {
				loadParams->uiControls.usePoreVols = std::stoi(line);
			}
			
			else if (line_1 == ("Simulation Name				")) {
				loadParams->columnParams.simName = line;
			}
			else if (line_1 == ("Simulation Dir				")) {
				if (std::filesystem::exists(line))
					loadParams->columnParams.simDir = line;
			}
			/*else if (line_1 == ("Sampling point_col			")) {
				loadParams->samplePoint_col = std::stod(line);
			}*/

			else if (line_1 == ("Damkohler_obs number			")) {
				loadParams->simOut.damkohler_obs = std::stod(line);
			}
			else if (line_1 == ("Damkohler_prd number		")) {
			loadParams->simOut.damkohler_prd = std::stod(line);
			}
			else if (line_1 == ("Retardation coef			")) {
			loadParams->simOut.retardation_coef = std::stod(line);
			}
			else if (line_1 == ("Peclet number				")) {
			loadParams->simOut.peclet = std::stod(line);
			}
			else if (line_1 == ("Exchange pore volume		")) {
			loadParams->simOut.exchPoreVol = std::stod(line);
			}
			else if (line_1 == ("mass balance				")) {
			loadParams->simOut.massBalanceValue = std::stod(line);
			}
			else if (line_1 == ("center of mass				")) {
			loadParams->simOut.centerMass = std::stod(line);
			}

			else if (line_1 == ("adsorbed mass				")) {
			loadParams->simOut.currentAdsorbedMass = std::stod(line);
			}
			else if (line_1 == ("immobile mass				")) {
			loadParams->simOut.currentImmobileMass = std::stod(line);
			}
			else if (line_1 == ("mobile mass					")) {
			std::getline(in, line);
			loadParams->simOut.currentMobileMass = std::stod(line);
			}
			else if (line_1 == ("outflow mass				")) {
			loadParams->simOut.totalOutflowMass = std::stod(line);
			}
			else if (line_1 == ("inflow mass					")) {
			loadParams->simOut.totalInflowMass = std::stod(line);
			}
			/*else if (line_1 == ("Observation file			")) {
				if (std::filesystem::exists(line))
					loadParams->obsFile = line;
			}*/


		}
		if (!savedScenarios.empty()) {
			loadParams->scenarios.clear();

			for (int i{ 0 }; i < savedScenarios.size(); i++) {
				std::stringstream scn(savedScenarios[i]);
				ScenarioParams scn_data;
				std::getline(scn, line, '\t');
				scn_data.sceneTime = std::stod(line);
				std::getline(scn, line, '\t');
				int pSize = std::stoi(line);

				for (int j{ 0 }; j < pSize; j++) {
					std::getline(scn, line, '\t');
					scn_data.modifiedParams.push_back(std::stoi(line));
					std::getline(scn, line, '\t');
					scn_data.paramValues.push_back(std::stod(line));
				}
				loadParams->scenarios.push_back(scn_data);
			}
		}

		if (!savedFLT.empty()) {
			loadParams->flowInterrupts.data.clear();
			loadParams->flowInterrupts.usePoreVol = flowIntUsePV;
			for (int i{ 0 }; i < savedFLT.size(); i++) {
				std::stringstream scn(savedFLT[i]);
				FlowInterrupt flt;
				std::getline(scn, line, '\t');
				flt.startTime = std::stod(line);
				std::getline(scn, line, '\t');
				flt.duration = std::stod(line);

				loadParams->flowInterrupts.data.push_back(flt);
			}
		}


		if (!savedCustomSim.empty())
		{
			loadParams->loopData.addedParams.clear();
			for (auto& [level, loopData] : loadParams->loopData.scenarioLoopData)
			{
				loopData.clear();
			}

			std::string pName;
			std::string textInput;
			int level{ 0 };
			double minVal{ 0.0 }, maxVal{ 0.0 }, stepVal{ 0.0 };
			bool useRange{ false };

			for (auto line : savedCustomSim)
			{
				std::stringstream scn(line);
				std::string key;
				std::getline(scn, key, '\t');

				if(key == "addedParams:")
				{
					
				}
				else if (key == "maxLevels:")
				{
					std::getline(scn, key, '\t');
					loadParams->loopData.maxLevels = std::stoi(key);
				}
				else if (key == "selectedLevel:")
				{
					std::getline(scn, key, '\t');
					loadParams->loopData.selectedLevel = std::stoi(key);
				}
				else if (key == "selectedName:")
				{
					std::getline(scn, key, '\t');
					loadParams->loopData.selectedName = std::stoi(key);
				}
				else if (key == "Level:")
				{
					std::getline(scn, key, '\t');
					level = std::stoi(key);
				}
				else if(key == "endLevelData")
				{
					if (std::find(loadParams->loopData.paramNames.begin(), 
						loadParams->loopData.paramNames.end(), pName) != 
						loadParams->loopData.paramNames.end())
					{
						LoopData lData(level, pName);
						lData.textInput = textInput;
						lData.rangeStart = minVal;
						lData.rangeEnd = maxVal;
						lData.rangeStep = stepVal;
						lData.enterRange = useRange;
						loadParams->loopData.scenarioLoopData[level].push_back(lData);
						loadParams->loopData.addedParams.push_back(pName);
					}
					
				}
				else if (key == "name:")
				{
					std::getline(scn, pName, '\t');
				}
				else if (key == "textInput:")
				{
					std::getline(scn, textInput, '\t');
				}
				else if (key == "range:")
				{
					std::getline(scn, key, '\t');
					minVal = std::stod(key);
					std::getline(scn, key, '\t');
					maxVal = std::stod(key);
					std::getline(scn, key, '\t');
					stepVal = std::stod(key);
				}
				else if (key == "usedRange:")
				{
					std::getline(scn, key, '\t');
					useRange = (bool)std::stoi(key);
				}
			}
			
		}

	}
	else {

	}
}

void ntrans::ModelADE::init()
{
	simData->uiControls.canPlot = false;
	while (simData->uiControls.isPlotting){}
	
	intDataCopy.clear();
	if(simData->uiControls.usePoreVols)
	{
		simData->transParams.flowVelocity = simData->columnParams.domainLength *
											simData->transParams.waterContent *
											simData->transParams.flowRate / 24.0;
		
	}
	else
	{
		simData->transParams.flowVelocity = simData->transParams.flowRate /
											simData->columnParams.crossSectionArea;

		
	}
	if (simData->flowInterrupts.usePoreVol)
	{
		double eInttime{ 0.0 };
		for (size_t i{ 0 }; i < simData->flowInterrupts.data.size(); i++)
		{
			FlowInterrupt oneInt;
			oneInt.duration = simData->flowInterrupts.data[i].duration *
				simData->transParams.waterContent *
				simData->columnParams.domainLength /
				simData->transParams.flowVelocity;
			oneInt.startTime = (dp->domainLength * tp->waterContent * simData->flowInterrupts.data[i].startTime /
				simData->transParams.flowVelocity) + eInttime;
			eInttime += oneInt.duration;
			intDataCopy.push_back(oneInt);
		}
	}
	else
	{
		for (size_t i{ 0 }; i < simData->flowInterrupts.data.size(); i++)
		{
			FlowInterrupt oneInt;
			oneInt.duration = simData->flowInterrupts.data[i].duration;
			oneInt.startTime = simData->flowInterrupts.data[i].startTime;
			intDataCopy.push_back(oneInt);
		}
	}
	if (!simData->uiControls.isCalibration)
	{
		simData->simOut.damkohler_prd = damkohlerNumber(simData);
		simData->simOut.retardation_coef = retardationCoeff(simData);
	}
	elaspedIntTime = 0.0;
	tp->maxInputConc = max(tp->pulseConcentration, tp->maxInputConc);
	double totalInterruptionTime{ 0.0 };
	for (int i = 0; i < intDataCopy.size(); i++)
		totalInterruptionTime += intDataCopy[i].duration;

	timeCount = (int)std::round((simData->columnParams.totalTransportTime - totalInterruptionTime) / simData->columnParams.timestep) + 1;
	nodeCount = (int)std::round(simData->columnParams.domainLength / simData->columnParams.domainSteps);
	currentScene = 0;
	triDiagHelper.clear();
	triDiagHelper.resize(nodeCount);
	minConc = DBL_MIN * tp->pulseConcentration;

	ignoreImmobileRegions = simData->transParams.mo_imPartitionCoefficient >= 1.0 ||
							simData->transParams.mo_imExchangeRate <= 0.0;
	
	ignoreReactions = simData->columnParams.isothermType == 0;

	simData->reactNodes.clear();
	simData->conserveNodes.clear();
	sout->nodesConc.clear();
	sout->nodesSorbed.clear();

	sout->nodesConc.resize(nodeCount);
	if(ignoreReactions)
	{
		simData->conserveNodes.resize(nodeCount);
	}
	else
	{
		simData->reactNodes.resize(nodeCount);
		sout->nodesSorbed.resize(nodeCount);
		sout->concAtPoint.clear();
		sout->sorbedAtPoint.clear();

		sout->concAtPoint.resize(timeCount);
		sout->sorbedAtPoint.resize(timeCount);

		switch (dp->isothermType)
		{
		case 1:
			tp->maxAdsorptionVal = tp->adsorptionCapacity;
			break;
		case 2:
			tp->maxAdsorptionVal = freundlichIsotherm(tp->maxInputConc);
			break;
		case 3:
			tp->maxAdsorptionVal = linearIsotherm(tp->maxInputConc);
			break;
		default:
			break;
		}
	}

	double pvMult = simData->transParams.flowVelocity / (simData->columnParams.domainLength * simData->transParams.waterContent);
	simData->simOut.init(timeCount, simData->columnParams.totalTransportTime, simData->columnParams.timestep, pvMult, intDataCopy);
	setTransportParameters(&initialTranspValues, tp);
	simData->uiControls.canPlot = true;
}

void ntrans::ModelADE::setTransportParameters(TransportParameters* lhs, TransportParameters* rhs)
{
	lhs->adsorptionCapacity = rhs->adsorptionCapacity;
	lhs->bulkDensity = rhs->bulkDensity;
	lhs->degradationRate_eqsb = rhs->degradationRate_eqsb;
	lhs->degradationRate_kinsb = rhs->degradationRate_kinsb;
	lhs->degradationRate_soln = rhs->degradationRate_soln;
	lhs->dispersionLength = rhs->dispersionLength;
	lhs->eq_kinPartitionCoefficient = rhs->eq_kinPartitionCoefficient;
	lhs->flowRate = rhs->flowRate;
	lhs->hysteresisCoefficient = rhs->hysteresisCoefficient;
	lhs->isothermConstant = rhs->isothermConstant;
	lhs->maxAdsorptionVal = rhs->maxAdsorptionVal;
	lhs->molecularDiffusion = rhs->molecularDiffusion;
	lhs->mo_imExchangeRate = rhs->mo_imExchangeRate;
	lhs->mo_imPartitionCoefficient = rhs->mo_imPartitionCoefficient;
	lhs->pulseConcentration = rhs->pulseConcentration;
	lhs->reactionRateCoefficient = rhs->reactionRateCoefficient;
	lhs->waterContent = rhs->waterContent;
	lhs->flowVelocity = rhs->flowVelocity;

}

void ntrans::ModelADE::solveTriDiag(std::vector<ConservativeNodes>* nodes)
{
	sout->executionLoc = "Scheme Inversion";

	triDiagHelper[0] = nodes->at(0).upperDiagonal / nodes->at(0).mainDiagonal;
	nodes->at(0).rhs = nodes->at(0).rhs / nodes->at(0).mainDiagonal;

	for (int i{ 1 }; i <= nodeCount - (int)1; i++) {
		double m = 1.0 / (nodes->at(i).mainDiagonal - nodes->at(i).lowerDiagonal * triDiagHelper[i - (size_t)1]);
		triDiagHelper[i] = nodes->at(i).upperDiagonal * m;
		nodes->at(i).rhs = (nodes->at(i).rhs - 
							nodes->at(i).lowerDiagonal * nodes->at(i- (size_t)1).rhs) * m;
	}
	for (int i{ nodeCount - 2 }; i >= 0; i--)
		nodes->at(i).rhs -= triDiagHelper[i] * nodes->at(i+ (size_t)1).rhs;
}

void ntrans::ModelADE::solveTriDiag(std::vector<ReactiveNodes>* nodes)
{
	sout->executionLoc = "Scheme Inversion";

	triDiagHelper[0] = nodes->at(0).upperDiagonal / nodes->at(0).mainDiagonal;
	nodes->at(0).rhs = nodes->at(0).rhs / nodes->at(0).mainDiagonal;

	for (int i{ 1 }; i <= nodeCount - (int)1; i++) {
		double m = 1.0 / (nodes->at(i).mainDiagonal - nodes->at(i).lowerDiagonal * triDiagHelper[i - (size_t)1]);
		triDiagHelper[i] = nodes->at(i).upperDiagonal * m;
		nodes->at(i).rhs = (nodes->at(i).rhs -
			nodes->at(i).lowerDiagonal * nodes->at(i - (size_t)1).rhs) * m;
	}
	for (int i{ nodeCount - 2 }; i >= 0; i--)
		nodes->at(i).rhs -= triDiagHelper[i] * nodes->at(i + (size_t)1).rhs;
}

void ntrans::ModelADE::subtractNodeValue(std::vector<ConservativeNodes>* lhsNodes)
{
	for (int i{ 0 }; i < nodeCount; i++)
		lhsNodes->at(i).concValue -= lhsNodes->at(i).rhs;
}

void ntrans::ModelADE::subtractNodeValue(std::vector<ReactiveNodes>* lhsNodes)
{
	for (int i{ 0 }; i < nodeCount; i++)
		lhsNodes->at(i).concValue -= lhsNodes->at(i).rhs;
}

void ntrans::ModelADE::resetNegatives(std::vector<ConservativeNodes>* lhsNodes)
{
	for (int i{ 0 }; i < nodeCount; i++)
	{
		if (lhsNodes->at(i).concValue < 0.0)
			lhsNodes->at(i).concValue = 0.0;

	}
}

void ntrans::ModelADE::resetNegatives(std::vector<ReactiveNodes>* lhsNodes)
{
	for (int i{ 0 }; i < nodeCount; i++)
	{
		if (lhsNodes->at(i).concValue < 0.0)
			lhsNodes->at(i).concValue = 0.0;

	}
}

double ntrans::ModelADE::sumNodeRhs(std::vector<ConservativeNodes>* lhsNodes)
{
	double retVal = 0.0;
	for (int i{ 0 }; i < nodeCount; i++)
		retVal += lhsNodes->at(i).rhs;
	return retVal;
}

double ntrans::ModelADE::sumNodeRhs(std::vector<ReactiveNodes>* lhsNodes)
{
	double retVal = 0.0;
	for (int i{ 0 }; i < nodeCount; i++)
		retVal += lhsNodes->at(i).rhs;
	return retVal;
}

void ntrans::ModelADE::calculateMassBalance(std::vector<ConservativeNodes>* lhsNodes)
{
	double columnMobileMass{ 0.0 };
	double columnImobileMass{ 0.0 };
	double outFlowNow{ 0.0 };
	double inFlowNow{ 0.0 };

	double theta_m = tp->waterContent * tp->mo_imPartitionCoefficient;
	double theta_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);

	for (int i{ 0 }; i < nodeCount; i++)
	{
		columnMobileMass += lhsNodes->at(i).prevConcValue;
		columnImobileMass += lhsNodes->at(i).prevImConcValue;
	}

	sout->currentMobileMass = columnMobileMass * (dp->domainSteps * dp->crossSectionArea/1000.0) * theta_m;
	sout->currentImmobileMass = columnImobileMass * (dp->domainSteps * dp->crossSectionArea / 1000.0) * theta_im;

	outFlowNow = lhsNodes->at(nodeCount - (size_t)1).prevConcValue * dp->timestep * tp->flowVelocity * dp->crossSectionArea / 1000.0;
	inFlowNow = tp->pulseConcentration * dp->timestep * tp->flowVelocity * dp->crossSectionArea / 1000.0;

	sout->totalInflowMass += inFlowNow;
	sout->totalOutflowMass += outFlowNow;

	sout->massBalanceValue = sout->currentMobileMass + sout->currentImmobileMass + sout->totalOutflowMass;
	sout->massBalanceValue = std::log10(std::abs(sout->massBalanceValue - sout->totalInflowMass));
}

void ntrans::ModelADE::calculateMassBalance(std::vector<ReactiveNodes>* lhsNodes)
{
	double columnMobileMass{ 0.0 };
	double columnImobileMass{ 0.0 };
	double outFlowNow{ 0.0 };
	double inFlowNow{ 0.0 };
	double sorbedEq{ 0.0 };
	double sorbedKin{ 0.0 };

	double theta_m = tp->waterContent * tp->mo_imPartitionCoefficient;
	double theta_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);

	double volume = (dp->crossSectionArea * dp->domainSteps) / 1000.0;
	double eqmult = tp->bulkDensity * tp->eq_kinPartitionCoefficient * volume;
	double kinmult = tp->bulkDensity * (1.0 - tp->eq_kinPartitionCoefficient) * volume;

	for (int i{ 0 }; i < nodeCount; i++)
	{
		columnMobileMass += lhsNodes->at(i).prevConcValue;
		columnImobileMass += lhsNodes->at(i).prevImConcValue;
		sorbedEq += lhsNodes->at(i).prevEqSorbed + lhsNodes->at(i).imPrevEqSorbed;
		sorbedKin += lhsNodes->at(i).prevKinSorbed + lhsNodes->at(i).imPrevKinSorbed;
	}

	sout->currentMobileMass = columnMobileMass * volume * theta_m;
	sout->currentImmobileMass = columnImobileMass * volume * theta_im;
	sout->currentAdsorbedMass = eqmult * sorbedEq + kinmult * sorbedKin;

	sout->totalDegradedMass += dp->timestep * tp->degradationRate_soln * volume * theta_m * columnMobileMass +
								dp->timestep * tp->degradationRate_soln * volume * theta_im * columnImobileMass +
								dp->timestep * tp->degradationRate_eqsb * eqmult * sorbedEq +
								dp->timestep * tp->degradationRate_eqsb * kinmult * sorbedKin;
								


	outFlowNow = lhsNodes->at(nodeCount - (size_t)1).prevConcValue * dp->timestep * tp->flowVelocity * dp->crossSectionArea / 1000.0;
	inFlowNow = tp->pulseConcentration * dp->timestep * tp->flowVelocity * dp->crossSectionArea / 1000.0;

	sout->totalInflowMass += inFlowNow;
	sout->totalOutflowMass += outFlowNow;

	sout->massBalanceValue = sout->currentMobileMass + sout->currentImmobileMass + sout->totalOutflowMass
							+ sout->totalDegradedMass + sout->currentAdsorbedMass;
	sout->massBalanceValue = std::log10(std::abs(sout->massBalanceValue - sout->totalInflowMass));
}

void ntrans::ModelADE::evaluateScenarios(double currentSimTime)
{
	if (currentScene >= simData->scenarios.size())
		return;

	double scenarioTime = simData->scenarios[currentScene].sceneTime;
	if (simData->uiControls.usePoreVols) {
		scenarioTime = (dp->domainLength * tp->waterContent * scenarioTime / initialTranspValues.flowVelocity) + elaspedIntTime;
	}

	if (currentSimTime >= scenarioTime && currentScene < simData->scenarios.size()) {
		for (int j{ 0 }; j < simData->scenarios[currentScene].modifiedParams.size(); j++) {
			switch (simData->scenarios[currentScene].modifiedParams[j])
			{
			case RHO:
				tp->bulkDensity = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.bulkDensity;
				break;
			case TETHA:
				tp->waterContent = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.waterContent;
				break;
			case FLOWRATE:
			{
				if (simData->uiControls.usePoreVols) {
					double pv = dp->domainLength * tp->waterContent / 24.0;
					tp->flowVelocity = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] * pv : initialTranspValues.flowVelocity;
				}
				else {
					tp->flowVelocity = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.flowVelocity;
				}
				break;
			}
			case ALP:
				tp->dispersionLength = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.dispersionLength;
				break;
			case DIFCOEF:
				tp->molecularDiffusion = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.molecularDiffusion;
				break;			
			case SMAX:
				tp->adsorptionCapacity = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.adsorptionCapacity;
				break;
			
			case K_FWD:
				tp->isothermConstant = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.isothermConstant;
				break;
			case SOLNDEG:
				tp->degradationRate_soln = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.degradationRate_soln;
				break;
			case SOLIDDEG_EQ:
				tp->degradationRate_eqsb = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.degradationRate_eqsb;
				break;
			case SOLIDDEG_KIN:
				tp->degradationRate_kinsb = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.degradationRate_kinsb;
				break;
			case PCOEF:
				tp->eq_kinPartitionCoefficient = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.eq_kinPartitionCoefficient;
				break;
			case RTCOEF:
				tp->reactionRateCoefficient = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.reactionRateCoefficient;
				break;
			case INPUTMASS:
				tp->pulseConcentration = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.pulseConcentration;
				break;
			case BETA:
				tp->mo_imPartitionCoefficient = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.mo_imPartitionCoefficient;
				break;
			case OMEGA:
				tp->mo_imExchangeRate = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.mo_imExchangeRate;
				break;
			case HYSTERESIS:
				tp->hysteresisCoefficient = simData->scenarios[currentScene].paramValues[j] >= 0.0 ? simData->scenarios[currentScene].paramValues[j] : initialTranspValues.hysteresisCoefficient;
				break;
			default:
				break;
			}
		}

		currentScene++;
	}
}

void ntrans::ModelADE::runModel()
{
	int sampleStep{ 0 };
	double prevEffluent{ 0.0 };
	currentScene = 0;
	currentInterrupt = 0;
	double theta_m = tp->waterContent * tp->mo_imPartitionCoefficient;
	double theta_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);

	if(ignoreReactions)
	{
		for (double simstep{ dp->timestep }; simstep <= dp->totalTransportTime + dp->timestep; simstep += dp->timestep)
		{
			if (simData->uiControls.scheduleStop)
				break;
			sout->executionLoc = "Main Timestep";
			sout->curSimStep = simstep;

			if (simData->scenarios.size() > 0) {
				evaluateScenarios(simstep);
			}
			

			if (sout->samplingTimes.size() > 0) {
				if (sampleStep < sout->samplingTimes.size()) {
					sout->currentSampleStep = sampleStep;
					if (sout->samplingTimes.at(sampleStep) - (simstep - dp->timestep) <= dp->timestep/1000.0) {
						sout->predictedBT[sampleStep] = prevEffluent;

						sampleStep++;
					}
				}

			}

			double velocity = theta_m > 0.0 ? tp->flowVelocity / theta_m : 0.0;
			tp->maxInputConc = tp->maxInputConc < tp->pulseConcentration ? tp->pulseConcentration :
				tp->maxInputConc;


			sout->disp_app = (tp->molecularDiffusion + velocity * tp->dispersionLength);

			sout->peclet = (tp->molecularDiffusion + velocity * tp->dispersionLength) > 0.0 ? (dp->domainLength * velocity) /
				(tp->molecularDiffusion + velocity * tp->dispersionLength) : 0.0;

			sout->exchPoreVol = (velocity * simstep) / dp->domainLength;


			if (intDataCopy.size() > 0 && currentInterrupt < intDataCopy.size() &&
				simstep >= intDataCopy[currentInterrupt].startTime)
			{
				double fv = tp->flowVelocity;
				tp->flowVelocity = 0.0;
				double interruptTime{ 0.0 };
				while (interruptTime < intDataCopy[currentInterrupt].duration) {
					conservativeLoop();
					interruptTime += dp->timestep;
					simstep += dp->timestep;
				}
				elaspedIntTime += intDataCopy[currentInterrupt].duration;
				tp->flowVelocity = fv;
				currentInterrupt++;
			}
			else
			{
				conservativeLoop();

			}
			sout->executionLoc = "Main Timestep";
			prevEffluent = effVessel(simData->conserveNodes[nodeCount - (size_t)1].prevConcValue, prevEffluent);
		}
	}
	else
	{
		for (double t{ dp->timestep }; t <= dp->totalTransportTime + dp->timestep; t += dp->timestep)
		{
			if (simData->uiControls.scheduleStop)
				break;
			sout->executionLoc = "Main Timestep";
			sout->curSimStep = t;

			if (simData->scenarios.size() > 0) {
				evaluateScenarios(t);
			}

			if (sout->samplingTimes.size() > 0) {
				if (sampleStep < sout->samplingTimes.size()) {
					sout->currentSampleStep = sampleStep;
					if (sout->samplingTimes.at(sampleStep) - (t - dp->timestep) < dp->timestep / 1000.0) {
						sout->predictedBT[sampleStep] = prevEffluent;
						sout->concAtPoint[sampleStep] = simData->reactNodes[nodeCount - (size_t)1].prevConcValue;
						sout->sorbedAtPoint[sampleStep] = simData->reactNodes[nodeCount - (size_t)1].totalSorbed;
						sampleStep++;
					}
				}

			}

			double velocity = theta_m > 0.0 ? tp->flowVelocity / theta_m : 0.0;
			tp->maxInputConc = tp->maxInputConc < tp->pulseConcentration ? tp->pulseConcentration :
				tp->maxInputConc;


			sout->disp_app = (tp->molecularDiffusion + velocity * tp->dispersionLength);

			sout->peclet = (tp->molecularDiffusion + velocity * tp->dispersionLength) > 0.0 ? (dp->domainLength * velocity) /
				(tp->molecularDiffusion + velocity * tp->dispersionLength) : 0.0;

			sout->exchPoreVol = (velocity * t) / dp->domainLength;


			if (intDataCopy.size() > 0 && currentInterrupt < intDataCopy.size() &&
				t >= intDataCopy[currentInterrupt].startTime)
			{
				double fv = tp->flowVelocity;
				tp->flowVelocity = 0.0;
				double interruptTime{ 0.0 };
				while (interruptTime < intDataCopy[currentInterrupt].duration) {
					reactiveLoop();
					interruptTime += dp->timestep;
					t += dp->timestep;
				}
				tp->flowVelocity = fv;
				elaspedIntTime += intDataCopy[currentInterrupt].duration;
				currentInterrupt++;
			}
			else
			{
				reactiveLoop();

			}
			sout->executionLoc = "Main Timestep";
			prevEffluent = effVessel(simData->reactNodes[nodeCount - (size_t)1].prevConcValue, prevEffluent);
		}
	}
}

void ntrans::ModelADE::createJcReactive(bool isJc)
{
	auto t1 = high_resolution_clock::now();
	sout->executionLoc = "Main Jacobian F";

	double tetha_m = tp->waterContent * tp->mo_imPartitionCoefficient;
	double tetha_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);
	double mbPhaseConst = tp->mo_imPartitionCoefficient < 1.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_m : 0.0;


	// handle degredation
	double degConstant = (tp->degradationRate_soln * dp->timestep);

	// advection and diffusion
	double velocity = tetha_m > 0.0 ? tp->flowVelocity / tetha_m : 0.0;
	double hDisCoef = tp->molecularDiffusion + (velocity * tp->dispersionLength);
	double alpha = (dp->timestep * hDisCoef) / std::pow(dp->domainSteps, 2);
	double gamma = (dp->timestep * velocity) / dp->domainSteps;

	// handle diagonals
	double mDiag = 1.0 + (2.0 * alpha) + gamma + degConstant + mbPhaseConst;
	double upDiag = -alpha, lwDiag = -(alpha + gamma);
	double change{ 1e-8 };

	// handle adsorption
	double sorbedEqMult = tetha_m > 0.0 ? (tp->bulkDensity * tp->eq_kinPartitionCoefficient) / tetha_m : 0.0;
	double sorbedKinMult = tetha_m > 0.0 ? (tp->bulkDensity * dp->timestep * tp->reactionRateCoefficient) / tetha_m : 0.0;
	double sorbedKinEq_ex = tp->eq_kinPartitionCoefficient < 1.0 ? (1.0 - tp->eq_kinPartitionCoefficient) * sorbedKinMult : 0.0;
	double sbEqDeg = sorbedEqMult * tp->degradationRate_eqsb;
	double sbKineticConst = sorbedKinMult * (1.0 - tp->eq_kinPartitionCoefficient) * ((dp->timestep * tp->reactionRateCoefficient)) /
		(1.0 + (dp->timestep * tp->reactionRateCoefficient) + (dp->timestep * tp->degradationRate_kinsb));
	double sbKinOldConst = tp->reactionRateCoefficient > 0.0 ? sbKineticConst / (dp->timestep * tp->reactionRateCoefficient) : 0.0;


	for (int i{ 0 }; i < nodeCount; i++) {
		double dm = mDiag, du = upDiag, dl = lwDiag;
		double sorbed{ 0.0 }; double sorbedDelta{ 0.0 }; double sorbedOld{ 0.0 };
		double sorbedKin{ 0.0 }, sorbedKin_dt{ 0.0 }, sorbedEq{ 0.0 }, sorbedEq_dt{ 0.0 };
		double sorbedEq_exch{ 0.0 }, sorbedEq_exch_dt{ 0.0 };
		double sorbed_kin_delta{ 0.0 }, sorbed_kin{ 0.0 };
		double sorbedDegEq{ 0.0 }, sorbedDegEq_dt{ 0.0 };
		double c1 = simData->reactNodes.at(i).concValue, c2 = 0.0, c3 = 0.0; // c1 = Cx, c2 = Cx+1, c3 = Cx-1

		if (i == 0) {
			dm -= alpha;
			dl += alpha;
			c3 = tp->pulseConcentration;
		}


		if (i == nodeCount - 1) {
			dm -= alpha;
			du = 0;
		}
		if (i < nodeCount - 1)
			c2 = simData->reactNodes.at(i + (size_t)1).concValue;
		if (i > 0)
			c3 = simData->reactNodes.at(i - (size_t)1).concValue;

		simData->reactNodes[i].maxEqConcVal_tmp = c1 > simData->reactNodes[i].maxEqConcVal ? c1 : simData->reactNodes[i].maxEqConcVal;



		switch (dp->isothermType)
		{
		case 1:

			sorbed = langmuirIsotherm(c1, simData->reactNodes[i].maxEqConcVal_tmp);
			sorbedDelta = langmuirIsotherm(c1 + change, simData->reactNodes[i].maxEqConcVal_tmp + change);

			
			if (simData->reactNodes[i].maxKinConcVal > c1) {
				sorbed_kin = langmuirIsotherm(c1, simData->reactNodes[i].maxKinConcVal);
				sorbed_kin_delta = langmuirIsotherm(c1 + change, simData->reactNodes[i].maxKinConcVal + change);
			}
			else {
				sorbed_kin = langmuirIsotherm(c1, c1);
				sorbed_kin_delta = langmuirIsotherm(c1 + change, c1 + change);
			}



			sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
			sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

			sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
			sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;
			break;
		case 2:
			sorbed = freundlichIsothermHysteresis(c1, simData->reactNodes[i].maxEqConcVal_tmp);
			sorbedDelta = freundlichIsothermHysteresis(c1 + change, simData->reactNodes[i].maxEqConcVal_tmp + change);


			if (simData->reactNodes[i].maxKinConcVal > c1) {
				sorbed_kin = freundlichIsothermHysteresis(c1, simData->reactNodes[i].maxKinConcVal);
				sorbed_kin_delta = freundlichIsothermHysteresis(c1 + change, simData->reactNodes[i].maxKinConcVal + change);
			}
			else {
				sorbed_kin = freundlichIsothermHysteresis(c1, c1);
				sorbed_kin_delta = freundlichIsothermHysteresis(c1 + change, c1 + change);
			}
						
			sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
			sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

			sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
			sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;
			break;
		case 3:
			sorbed = linearIsotherm(c1);
			sorbedDelta = linearIsotherm(c1 + change);
			sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
			sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

			sorbed_kin = linearIsotherm(c1);
			sorbed_kin_delta = linearIsotherm(c1 + change);
			sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
			sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;
			break;
		default:

			break;
		}
		double oldConc = simData->reactNodes[i].prevConcValue + (simData->reactNodes[i].prevEqSorbed * sorbedEqMult) +
						(simData->reactNodes[i].prevKinSorbed * sbKinOldConst);
		
		simData->reactNodes[i].eqSorbed_tmp = sorbed; 
		simData->reactNodes[i].kinSorbed_tmp = sorbed_kin;


		sout->executionLoc = "Main Jacobian F";
		if (isJc) {
			double f_1 = oneDFunctReactive(c1 + change, c2, c3, oldConc, dm, du,
				dl, sorbedEq_dt, sorbedEq_exch_dt, sorbedKin_dt, sorbedDegEq_dt, i);
			double f_2 = oneDFunctReactive(c1, c2 + change, c3, oldConc, dm, du,
				dl, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, i);
			double f_3 = oneDFunctReactive(c1, c2, c3 + change, oldConc, dm, du,
				dl, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, i);
			double f_0 = oneDFunctReactive(c1, c2, c3, oldConc, dm, du,
				dl, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, i);

			simData->reactNodes[i].mainDiagonal = (f_1 - f_0) / change;
			if (i < nodeCount - 1) {
				simData->reactNodes[i].upperDiagonal = (f_2 - f_0) / change;
			}
			if (i > 0) {
				simData->reactNodes[i].lowerDiagonal = (f_3 - f_0) / change;
			}
		}
		else {
			simData->reactNodes[i].rhs = oneDFunctReactive(c1, c2, c3, oldConc, dm, du,
				dl, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, i);
		}

		sout->executionLoc = "Main Jacobian F";
	}


	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double_nm = t2 - t1;
	sout->mainJcTime = ms_double_nm.count();

}

double ntrans::ModelADE::createJcReactiveImmobile( int loc, double mobile_c, double i_conc, bool isJc)
{
	sout->executionLoc = "Immobile Jacobian F";
	// imobile phase discretization	
	double tetha_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);
	double imPhaseConst = tetha_im > 0.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_im : 0.0;

	// handle degredation
	double degConstant = (tp->degradationRate_soln * dp->timestep);

	double change{ 1e-8 };

	// handle adsorption
	double sorbedEqMult = tetha_im > 0.0 ? 
						(tp->bulkDensity * tp->eq_kinPartitionCoefficient) / 
						tetha_im : 0.0;
	double sorbedKinMult = tetha_im > 0.0 ? 
							(tp->bulkDensity * dp->timestep * tp->reactionRateCoefficient) / 
							tetha_im : 0.0;

	double sorbedKinEq_ex = tp->eq_kinPartitionCoefficient < 1.0 ? 
							(1.0 - tp->eq_kinPartitionCoefficient) * 
							sorbedKinMult : 0.0;
	double sbEqDeg = sorbedEqMult * tp->degradationRate_eqsb;
	double sbKineticConst = sorbedKinMult * ((dp->timestep * tp->reactionRateCoefficient) *
											(1.0 - tp->eq_kinPartitionCoefficient)) /
											(1.0 + (dp->timestep * tp->reactionRateCoefficient) +
											(dp->timestep * tp->degradationRate_kinsb));
	double sbKinOldConst = tp->reactionRateCoefficient > 0.0 ? sbKineticConst / 
							(dp->timestep * tp->reactionRateCoefficient) : 0.0;




	double prvEqIso{ 0.0 };
	double sorbed{ 0.0 }; double sorbedDelta{ 0.0 }; double sorbedOld{ 0.0 };
	double sorbedKin{ 0.0 }, sorbedKin_dt{ 0.0 }, sorbedEq{ 0.0 }, sorbedEq_dt{ 0.0 };
	double sorbedEq_exch{ 0.0 }, sorbedEq_exch_dt{ 0.0 };
	double sorbed_kin_delta{ 0.0 }, sorbed_kin{ 0.0 };
	double sorbedDegEq{ 0.0 }, sorbedDegEq_dt{ 0.0 };
	double c1 = i_conc;
	simData->reactNodes[loc].imMaxEqConcVal_tmp = c1 > simData->reactNodes[loc].imMaxEqConcVal ? c1 : simData->reactNodes[loc].imMaxEqConcVal;
	
	switch (dp->isothermType)
	{
	case 1:
		//sorbed = langmuirIsotherm(c1, im->oldImobilePhase.at(loc,0), im->sorbedEq.at(loc, 0), 
		//	mdParams->Kl_rev, mdParams->Smax_store, loc, im->klArray, im->smaxArray, im->isforwardArray, mdParams->hysteresis);
		//sorbedDelta = langmuirIsotherm(c1 + change, *im->klArray[loc], *im->smaxArray[loc]);

		sorbed = langmuirIsotherm(c1, simData->reactNodes[loc].imMaxEqConcVal_tmp);
		sorbedDelta = langmuirIsotherm(c1 + change, simData->reactNodes[loc].imMaxEqConcVal_tmp + change);


		if (simData->reactNodes[loc].imMaxKinConcVal > c1) {
			sorbed_kin = langmuirIsotherm(c1, simData->reactNodes[loc].imMaxKinConcVal);
			sorbed_kin_delta = langmuirIsotherm(c1 + change, simData->reactNodes[loc].imMaxKinConcVal + change);
		}
		else {
			sorbed_kin = langmuirIsotherm(c1, c1);
			sorbed_kin_delta = langmuirIsotherm(c1 + change, c1 + change);
		}



		sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
		sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

		sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
		sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;

		break;
	case 2:
		sorbed = freundlichIsothermHysteresis(c1, simData->reactNodes[loc].imMaxEqConcVal_tmp);
		sorbedDelta = freundlichIsothermHysteresis(c1 + change, simData->reactNodes[loc].imMaxEqConcVal_tmp + change);


		if (simData->reactNodes[loc].imMaxKinConcVal > c1) {
			sorbed_kin = freundlichIsothermHysteresis(c1, simData->reactNodes[loc].imMaxKinConcVal);
			sorbed_kin_delta = freundlichIsothermHysteresis(c1 + change, simData->reactNodes[loc].imMaxKinConcVal + change);
		}
		else {
			sorbed_kin = freundlichIsothermHysteresis(c1, c1);
			sorbed_kin_delta = freundlichIsothermHysteresis(c1 + change, c1 + change);
		}
		
		sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
		sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

		sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
		sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;
		break;
	case 3:
		sorbed = linearIsotherm(c1);
		sorbedDelta = linearIsotherm(c1 + change);
		sorbedEq = sorbed * sorbedEqMult; sorbedEq_dt = sorbedDelta * sorbedEqMult;
		sorbedDegEq = sorbed * sbEqDeg; sorbedDegEq_dt = sorbedDelta * sbEqDeg;

		sorbed_kin = linearIsotherm(c1);
		sorbed_kin_delta = linearIsotherm(c1 + change);
		sorbedEq_exch = sorbed_kin * sorbedKinEq_ex; sorbedEq_exch_dt = sorbed_kin_delta * sorbedKinEq_ex;
		sorbedKin = sorbed_kin * sbKineticConst; sorbedKin_dt = sorbed_kin_delta * sbKineticConst;
		break;
	default:

		break;
	}
	sout->executionLoc = "Immobile Jacobian F";
	simData->reactNodes[loc].imEqSorbed_tmp = sorbed;
	simData->reactNodes[loc].imKinSorbed_tmp = sorbed_kin;

	double oldConc = simData->reactNodes[loc].prevImConcValue + 
					(simData->reactNodes[loc].imPrevEqSorbed * sorbedEqMult) +
					(simData->reactNodes[loc].imPrevKinSorbed * sbKinOldConst) + (mobile_c * imPhaseConst);

	//pmbSorbedMat[i] = sorbed; // +sorbedEq_exch;
	if (isJc) {
		double f_0 = oneDfuncIm(c1, oldConc, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, imPhaseConst + degConstant);
		double f_1 = oneDfuncIm(c1 + change, oldConc, sorbedEq_dt, sorbedEq_exch_dt, sorbedKin_dt,
			sorbedDegEq_dt, imPhaseConst + degConstant);

		return (f_1 - f_0) / change;

	}
	else {
		return oneDfuncIm(c1, oldConc, sorbedEq, sorbedEq_exch, sorbedKin, sorbedDegEq, imPhaseConst + degConstant);
	}
	sout->executionLoc = "Immobile Jacobian F";
}

double ntrans::ModelADE::oneDfuncIm(double c1, double cOld, double sbEq, double sbEqKn_ex, 
							double sbKin, double sbDeg, double imPhaseConst)
{
	 
	return c1 + (c1 * imPhaseConst) + sbEq + sbDeg + sbEqKn_ex - sbKin - cOld;
}

double ntrans::ModelADE::solveImobilePhaseWithSorption(double mobile_c, int loc)
{
	sout->executionLoc = "Immobile Iteration";
	double iConc = simData->reactNodes[loc].imConcValue;
	double error = 1e10;//createJacobianIm(im, iConc, loc, mdParams, mobilePhase, false);
	double delta{ 0.0 };
	int iteration{ 0 };
	while (std::abs(error) > 1e-10 && iteration < simData->maxIterations) 
	{

		delta = createJcReactiveImmobile(loc, mobile_c, iConc, true);
		error = createJcReactiveImmobile(loc, mobile_c, iConc, false);

		iConc -= error / delta;
		iConc = iConc < 0.0 ? 0.0 : iConc;
		error = createJcReactiveImmobile(loc, mobile_c, iConc, false);
		iteration++;
	}
	sout->imobileIterations = iteration;
	return iConc;
}

double ntrans::ModelADE::oneDFunctReactive(double c1, double c2, double c3, double cOld, double mD, double uD, double lD, double sbEq, double sbEqKn_ex, double sbKin, double sbDeg, int loc)
{
	double imobilePhase{ 0.0 };
	if (!ignoreImmobileRegions)
	{
		auto t1 = high_resolution_clock::now();
		imobilePhase = solveImobilePhaseWithSorption(c1, loc);
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double_nm = t2 - t1;
		sout->imobileSolTime = ms_double_nm.count();
		simData->reactNodes[loc].imConcValue = imobilePhase;
	}
	double tetha_m =tp->waterContent * tp->mo_imPartitionCoefficient;
	double imPhaseConst = tetha_m > 0.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_m : 0.0;

	double val = (mD * c1) + sbEq + sbDeg + sbEqKn_ex - ((imobilePhase * imPhaseConst)) - sbKin +
		(uD * c2) + (lD * c3) - cOld;
	return val;
}

double ntrans::ModelADE::langmuirIsotherm(double solConc, double maxSolConc)
{
	/*if (eqConc > minConc)
	{
		return (tp->isothermConstant * tp->adsorptionCapacity * solConc) /
			(tp->isothermConstant * solConc + std::pow(solConc / maxSolConc, tp->hysteresisCoefficient));
	}*/

	if (maxSolConc > 0.0 && tp->hysteresisCoefficient >= 1.0)
	{
		return (tp->isothermConstant * tp->adsorptionCapacity) /
			(tp->isothermConstant  + 1.0 / maxSolConc);
	}

	if (maxSolConc > 0.0) {
		return (tp->isothermConstant * tp->adsorptionCapacity * std::pow(solConc, 1.0 - tp->hysteresisCoefficient)) /
			(tp->isothermConstant * std::pow(solConc, 1.0 - tp->hysteresisCoefficient) +
				(1.0 / std::pow(maxSolConc, tp->hysteresisCoefficient)));
	}
	return (tp->isothermConstant * tp->adsorptionCapacity * solConc) /
		(tp->isothermConstant * solConc + 1.0);
}

double ntrans::ModelADE::freundlichIsothermHysteresis(double solConc, double maxSolConc)
{
	if(maxSolConc > 0.0)
	{
		return tp->isothermConstant * std::pow(maxSolConc, tp->adsorptionCapacity * tp->hysteresisCoefficient) *
			std::pow(solConc, tp->adsorptionCapacity - tp->adsorptionCapacity * tp->hysteresisCoefficient);
	}
	return tp->isothermConstant * std::pow(solConc, tp->adsorptionCapacity);
}

double ntrans::ModelADE::freundlichIsotherm(double solConc)
{
	return tp->isothermConstant * std::pow(solConc, tp->adsorptionCapacity);
}

double ntrans::ModelADE::linearIsotherm(double solConc)
{
	return tp->isothermConstant * solConc;
}

double ntrans::ModelADE::getEqConcFromSorbed(double sorbed)
{
	switch (dp->isothermType)
	{
	case 1:
		return sorbed / (tp->adsorptionCapacity * tp->isothermConstant - sorbed * tp->isothermConstant);
		break;
	case 2:
		return std::pow(sorbed / tp->isothermConstant, 1.0 / tp->adsorptionCapacity);
	default:
		return sorbed / tp->isothermConstant;
	}
}

void ntrans::ModelADE::updateSorbedKinPhase()
{
	double sorbed{ 0.0 };
	double sbDivConst = 1.0/ (1.0 + (dp->timestep *tp->reactionRateCoefficient) + (dp->timestep * tp->degradationRate_kinsb));
	for (int i{ 0 }; i < nodeCount; i++) {


		simData->reactNodes[i].prevKinSorbed = (simData->reactNodes[i].prevKinSorbed * sbDivConst) +
			((dp->timestep *tp->reactionRateCoefficient * simData->reactNodes[i].kinSorbed_tmp) * sbDivConst);

		if(!ignoreImmobileRegions)
		{
			simData->reactNodes[i].imPrevKinSorbed = (simData->reactNodes[i].imPrevKinSorbed * sbDivConst) +
				((dp->timestep * tp->reactionRateCoefficient * simData->reactNodes[i].imKinSorbed_tmp) * sbDivConst);
		}
		
	}
}

void ntrans::ModelADE::reactiveLoop()
{
	auto t1 = high_resolution_clock::now();

	double volume = (dp->crossSectionArea * dp->domainSteps) / 1000.0;
	double eqmult = tp->bulkDensity * tp->eq_kinPartitionCoefficient * volume;
	double kinmult = tp->bulkDensity * (1.0 - tp->eq_kinPartitionCoefficient) * volume;

	int iteration{ 0 };

	while (iteration < simData->maxIterations) {

		if (simData->uiControls.scheduleStop)
			break;
		sout->executionLoc = "Main Iteration";

		createJcReactive(true);
		sout->executionLoc = "Main Iteration";
		createJcReactive();
		sout->executionLoc = "Main Iteration";

		solveTriDiag(&simData->reactNodes);
		sout->executionLoc = "Main Iteration";

		subtractNodeValue(&simData->reactNodes);
		resetNegatives(&simData->reactNodes);
		createJcReactive();

		sout->executionLoc = "Main Iteration";
		double sumerr = std::abs(sumNodeRhs (&simData->reactNodes));
		iteration++;
		if (sumerr < 1e-10) {
			break;
		}
	}

	sout->mainIterations = iteration;
	updateSorbedKinPhase();

	for (int px{ 0 }; px < nodeCount; px++)
	{
		simData->reactNodes[px].maxEqConcVal = simData->reactNodes[px].maxEqConcVal <
			simData->reactNodes[px].concValue ? simData->reactNodes[px].concValue : simData->reactNodes[px].maxEqConcVal;

		double kin_ca = getEqConcFromSorbed(simData->reactNodes[px].prevKinSorbed);
		simData->reactNodes[px].maxKinConcVal = simData->reactNodes[px].maxKinConcVal < kin_ca ? kin_ca : simData->reactNodes[px].maxKinConcVal;

		simData->reactNodes[px].prevConcValue = simData->reactNodes[px].concValue;
		simData->reactNodes[px].prevEqSorbed = simData->reactNodes[px].eqSorbed_tmp;

		if (!ignoreImmobileRegions) 
		{
			simData->reactNodes[px].imMaxEqConcVal = simData->reactNodes[px].imMaxEqConcVal < simData->reactNodes[px].imConcValue ?
													simData->reactNodes[px].imConcValue : simData->reactNodes[px].imMaxEqConcVal;
			simData->reactNodes[px].prevImConcValue = simData->reactNodes[px].imConcValue;
			
			simData->reactNodes[px].imPrevEqSorbed = simData->reactNodes[px].imEqSorbed_tmp;
			double kin_ca_imph = getEqConcFromSorbed(simData->reactNodes[px].prevKinSorbed);
			simData->reactNodes[px].imMaxKinConcVal = simData->reactNodes[px].imMaxKinConcVal < kin_ca_imph ? kin_ca_imph : simData->reactNodes[px].imMaxKinConcVal;

			sout->nodesSorbed[px] = (simData->reactNodes[px].imPrevEqSorbed + simData->reactNodes[px].prevEqSorbed) * tp->eq_kinPartitionCoefficient +
									(simData->reactNodes[px].prevKinSorbed + simData->reactNodes[px].imPrevKinSorbed) * (1.0 - tp->eq_kinPartitionCoefficient);

		}
		else {
			sout->nodesSorbed[px] = (simData->reactNodes[px].prevEqSorbed) * tp->eq_kinPartitionCoefficient +
				(simData->reactNodes[px].prevKinSorbed ) * (1.0 - tp->eq_kinPartitionCoefficient);
		}




		simData->reactNodes[px].totalSorbed = simData->reactNodes[px].prevEqSorbed;/*(simData->reactNodes[px].prevEqSorbed + simData->reactNodes[px].imPrevEqSorbed) *
			tp->eq_kinPartitionCoefficient + (simData->reactNodes[px].prevKinSorbed + simData->reactNodes[px].imPrevKinSorbed) *
									(1.0 - tp->eq_kinPartitionCoefficient);*/

		sout->nodesConc[px] = simData->reactNodes[px].prevConcValue;

		
	}
	calculateMassBalance(&simData->reactNodes);
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double_nm = t2 - t1;
	sout->mainiterationLength = ms_double_nm.count();
	if (simData->uiControls.simDelay > 0 && !simData->uiControls.isCalibration)
		Sleep(simData->uiControls.simDelay);
}

void ntrans::ModelADE::createJcConservative(bool isJc)
{
	
	sout->executionLoc = "Main Jacobian F";
	double tetha_m = tp->waterContent * tp->mo_imPartitionCoefficient;

	double tetha_im = tp->waterContent * (1.0 - tp->mo_imPartitionCoefficient);
	double imPhaseConst = tetha_im > 0.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_im : 0.0;
	double mbPhaseConst = tp->mo_imPartitionCoefficient < 1.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_m : 0.0;
	double im_mbPhaseExchange = (imPhaseConst * mbPhaseConst) / (1.0 + imPhaseConst);
	double oldImPhaseMult = mbPhaseConst / (1.0 + imPhaseConst);

	double velocity = tetha_m > 0.0 ? tp->flowVelocity/ tetha_m : 0.0;
	double hDisCoef = tp->molecularDiffusion + (velocity * tp->dispersionLength);
	double alpha = (dp->timestep * hDisCoef) / std::pow(dp->domainSteps, 2.0);
	double gamma = (dp->timestep * velocity) / dp->domainSteps;

	double mDiag = 1.0 + (2.0 * alpha) + gamma + mbPhaseConst - im_mbPhaseExchange;
	double upDiag = -alpha, lwDiag = -(alpha + gamma);

	double change = 1e-8;

	for (int i{ 0 }; i < nodeCount; i++) {
		double dm = mDiag, du = upDiag, dl = lwDiag;
		double c1 = simData->conserveNodes[i].concValue, c2 = 0.0, c3 = 0.0;
		if (i == 0) {
			dm -= alpha;
			dl += alpha;
			c3 = tp->pulseConcentration;
		}
		if (i == nodeCount - 1) {
			dm -= alpha;
			du = 0;
		}

		if (i < nodeCount - 1) {
			c2 = simData->conserveNodes[i + (size_t)1].concValue;
		}
		if (i > 0) {
			c3 = simData->conserveNodes[i - (size_t)1].concValue;
		}

		double oldConc = !ignoreImmobileRegions ? simData->conserveNodes[i].prevConcValue +
						simData->conserveNodes[i].prevImConcValue * oldImPhaseMult : simData->conserveNodes[i].prevConcValue;

		sout->executionLoc = "Main Jacobian F";
		if (isJc) {
			double f_1 = oneDFuncConservative(c1 + change, c2, c3, oldConc, dm, du, dl);
			double f_2 = oneDFuncConservative(c1, c2 + change, c3, oldConc, dm, du, dl);
			double f_3 = oneDFuncConservative(c1, c2, c3 + change, oldConc, dm, du, dl);
			double f_0 = oneDFuncConservative(c1, c2, c3, oldConc, dm, du, dl);

			simData->conserveNodes[i].mainDiagonal = (f_1 - f_0) / change;
			if (i < nodeCount - 1) {
				simData->conserveNodes[i].upperDiagonal = (f_2 - f_0) / change;
			}
			if (i > 0) {
				simData->conserveNodes[i].lowerDiagonal = (f_3 - f_0) / change;
			}
		}
		else {
			simData->conserveNodes[i].rhs = oneDFuncConservative(c1, c2, c3, oldConc, dm, du, dl);
		}

		sout->executionLoc = "Main Jacobian F";
	}
}

double ntrans::ModelADE::oneDFuncConservative(double c1, double c2, double c3, double oldC, double md, double ud, double ld)
{
	return (md * c1) + (ud * c2) + (ld * c3) - oldC;
}

void ntrans::ModelADE::updateImmobilePhase()
{
	double tetha_im = ((1.0 - tp->mo_imPartitionCoefficient) * tp->waterContent);
	double mult = tetha_im > 0.0 ? (dp->timestep * tp->mo_imExchangeRate) / tetha_im : 0.0;
	double div = 1.0 / (1.0 + mult);
	for (int i{ 0 }; i < nodeCount; i++) {
		simData->conserveNodes[i].prevImConcValue = ((mult * simData->conserveNodes[i].concValue) + 
													simData->conserveNodes[i].prevImConcValue) * div;
	}
}

void ntrans::ModelADE::conservativeLoop()
{
	auto t1 = high_resolution_clock::now();

	int iteration{ 0 };

	while (iteration < simData->maxIterations) {

		if (simData->uiControls.scheduleStop)
			break;
		sout->executionLoc = "Main Iteration";

		createJcConservative(true);
		createJcConservative();
		sout->executionLoc = "Main Iteration";
		createJcConservative();
		sout->executionLoc = "Main Iteration";

		solveTriDiag(&simData->conserveNodes);
		sout->executionLoc = "Main Iteration";

		subtractNodeValue(&simData->conserveNodes);
		resetNegatives(&simData->conserveNodes);
		createJcConservative();

		sout->executionLoc = "Main Iteration";
		double sumerr = std::abs(sumNodeRhs(&simData->conserveNodes));
		iteration++;
		if (sumerr < 1e-10) {
			break;
		}
	}
	sout->mainIterations = iteration;
	if (!ignoreImmobileRegions)
		updateImmobilePhase();

	for (int px{ 0 }; px < nodeCount; px++) {
		simData->conserveNodes[px].prevConcValue = simData->conserveNodes[px].concValue;
		sout->nodesConc[px] = simData->conserveNodes[px].prevConcValue;
	}


	calculateMassBalance(&simData->conserveNodes);
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms_double_nm = t2 - t1;
	sout->mainiterationLength = ms_double_nm.count();
	if (simData->uiControls.simDelay > 0 && !simData->uiControls.isCalibration)
		Sleep(simData->uiControls.simDelay);
}

double ntrans::ModelADE::effVessel(double cOut, double cvOld)
{
	double q_out = tp->flowVelocity* dp->crossSectionArea * dp->timestep / dp->effluentVesselVol;
	return cvOld / (1.0 + q_out) + q_out * cOut / (1.0 + q_out);
}

