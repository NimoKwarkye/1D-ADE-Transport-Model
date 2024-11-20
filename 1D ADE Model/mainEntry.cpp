#include <iostream>

#include "TransportModel.h"
#include "TransportUI.h"
#include "MatArray.h"

int main()
{
	using namespace ntrans;
	SimulationData simData;
	TransportUI ui(1280, 720, "ADE Simulator", &simData);
	ui();

	return 0;
}