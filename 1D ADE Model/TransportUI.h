#pragma once
#include <string>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "implot.h"
#include <Windows.h>
#include <ShlObj.h>
#include <thread>
#include <ctime>
#include "imgui_stdlib.h"


#include "TransportModel.h"
#include "TaskSystemLocal.h"
#include "ReadCSV.h"
#include "FileExplorer.h"
#include "Optimizers.h"

namespace ntrans
{
    struct ImageData {
        int height{ 0 };
        int width{ 0 };
        GLuint data = 0;
    };

    enum class TransportSimEvents {
        First,
        RunTransport,
        RunMarquardt,
        RunAnnealing,
        RunMontecarlo,
        RunAdam,
        CalculateCenterOfMass,
        SensitivityAnalysis,
        EstimateUncertainty,
        LoopScenarios,
        CalculateNPP,
        CustomSensi,
        Last
    };

    struct isSelected {
        bool isFlowRate{ false };
        bool isTetha{ false };
        bool isDisp{ false };
        bool isDiff{ false };
        bool isBeta{ false };
        bool isOmega{ false };
        bool isRho{ false };
        bool isPcoef{ false };
        bool isRtceof{ false };
        bool isK{ false };
        bool isKl{ false };
        bool isSmax{ false };
        bool isKf{ false };
        bool isnf{ false };
        bool isK_kin{ false };
        bool isKl_kin{ false };
        bool isSmax_kin{ false };
        bool isKf_kin{ false };
        bool isnf_kin{ false };
        bool isHysteresis{ false };
        bool isSolnDeg{ false };
        bool isEqDeg{ false };
        bool isKinDeg{ false };
        bool isSmaxInc{ false };
    };

    struct ParamsNames {
        std::string flowVelocity{ "flow rate" };
        std::string dispersion{ "dispersion len." };
        std::string molDiff{ "molecular diff" };
        std::string tetha{ "water content" };
        std::string mobileTetha{ "mobile frac." };
        std::string imoMobMassT{ "imo-mob mass-rt" };
        std::string rho{ "bulk density" };
        std::string pcoef{ "p ceof" };
        std::string rtcoef{ "rt coef" };
        std::string eq_k{ "K" };
        std::string eq_smax{ "Smax/n" };
        std::string hysteresis{ "hysteresis coef" };
        std::string solnDeg{ "soln dg-rt" };
        std::string eqAdsDeg{ "eq-ads dg-rt" };
        std::string kinAdsDeg{ "kin-ads dg-rt" };
    };

    struct UIEventsToggle
    {
        bool showLogs{ true };
        bool showTransportConfigWindow{ true };
        bool isLightTheme{ false };
        bool isDarkTheme{ true };
        bool showLoadObsWindow{ false };
        bool showMarquardtWindow{ false };
        bool showAnnealingWindow{ false };
        bool showAdamWindow{ false };
        bool showSaveDataWindow{ false };
        bool showScenarioWindow{ false };
        bool showSensitivityWindow{ false };
        bool showUncertaintyWindow{ false };
        bool showScenerioLoopWindow{ false };
        bool showUtilitiesWindow{ false };
        bool showSensitivityParamsWindow{ false };
        bool showFlowInterruptsWindow{ false };
        bool loadObsPV{ false };

        bool saveObservationData{ false };
        bool saveSimulationSeries{ false };
        bool saveSimParams{ false };
        bool saveIsothermData{ false };
    };

    struct ObservedDataInfo
    {
        int dataColumn{ 0 };
        int poreColumn{ 0 };
        int timeColumn{ 0 };
        std::vector<std::string> obsDataColumns{};
        std::string obsFileName{ "" };
    };

    struct OptInfo
    {
        int maxStoredData{ 50 };
        int dataPoint{ 0 };
        std::vector<double> relChange;
        std::vector<double> iterations;
    };

    struct SensitivityControls
    {
        int sensiParamColIndex{ 0 };
        int sensiParamsSize{ 0 };
        int currentSensitivity{ 0 };
        int totalSensitivity{ 1 };
        std::string sensiParamName{ "" };
        char** sensParamsColumns = nullptr;
        char** listedSensitivities = nullptr;
    };

	class TransportUI
	{
    private:
        std::vector<std::vector<std::string>>scenarioInputData;
        std::vector<std::vector<std::string>>interruptInputData;
        std::vector<std::string>updateMessages;
        std::vector<int>messageTypes{};
        int maxMessageCount{ 20 };
        std::vector<std::string>lsSensitivities{ "selection 1" };
        std::vector<int>paramsId{};
        
        std::vector<std::string>selectedParamsNames;
        std::vector<double*>selectedParams;
        std::vector<double>paramsLimits;

        std::vector<std::string>paramsNames{
        "Index", "Time", "Input Mass", "Flow Rate",
        "Dispersion Length", "Mol Diffusion", "Theta", "Mobile Theta", "Mob-im Mass T Coef",
        "Bulk Density", "Partition Coef", "Reaction Rate Coef", "isotherm constant K", "Smax/n", "Hyseresis Coef",
        "Sol Deg Rate", "Eq Sorbed Deg Rate", "Kin Sorbed Deg Rate"
        };


        SimulationData* transportData;
        ModelADE modelObject;
        UIEventsToggle uiEvents;
        ObservedDataInfo obsDataInfo;
        nims_n::FileExtensions fileExtensions;
        nims_n::TaskSystemLocal taskExecuter;
        nims_n::MarquardtInput marquardInput;
        

        ImageData loadIcon;
        ImageData runIcon;
        ImageData stopIcon;
        ImageData saveIcon;
        isSelected paramsSelector;
        nims_n::OptimizationInput optInput;
        OptInfo fittingInfo;

        std::vector<isSelected>lsSensitivityParams{ isSelected() };
        std::vector<TransportSimEvents>transportEvents;
        SensitivityControls sensiControls;
        ParamsNames displayNames;

        GLFWwindow* window = nullptr;
        int width{ 10 };
        int height{ 0 };
        int monitors{ 0 };
        std::string name{ "" };
        const char* glsl_version = "#version 130";
        ImGuiWindowFlags window_flags = 0;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        void SetDarkTheme(ImGuiStyle& style);
        void SetLightTheme(ImGuiStyle& style);
        int init();
        void MainMenu();
        void loadModelParameters();
        void PlotWindow();
        void ConfigWindow();
        void LogsWindow();
        void LoadObsDataWindow();
        void SelectParamsWindow();
        void SaveDataWindow();
        void ScenarioWindow();
        void SensitivityParamsWindow();
        void UpdateSensitivityParams(std::string pName, bool& paramState, double* pValue);
        void RemoveSensitivityData();
        void SensitivityWindow();
        void windowBody();
        void updateWindow();
        void GatherSelected(bool includeLimits=false);
        void MarquardtWindow();
        void runMarquardt();
        void UncertaintyWindow();

        ImVec4 heatMapRGBA(double value);

    public:
        void logMessages(std::string mesg, int msgType = 0);
        TransportUI(int _width = 600, int _height = 400, std::string windowName = "My Window", SimulationData* _modelData = nullptr);
        ~TransportUI();
        void operator()();
	};

    void glfw_error_callback(int error, const char* description);
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
}


