#include "TransportModel.h"
#include "TransportUI.h"

//int main()
//{
//	using namespace ntrans;
//	SimulationData simData;
//	TransportUI ui(1280, 720, "ADE Simulator", &simData);
//	ui();
//
//	return 0;
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	using namespace ntrans;
	SimulationData simData;
	TransportUI ui(1280, 720, "ADE Simulator", &simData);
	ui();
    
    return 0;
}