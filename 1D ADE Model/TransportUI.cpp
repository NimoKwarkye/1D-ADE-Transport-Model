#include "TransportUI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace AtomDarkTheme {
    ImVec4 Background = ImVec4(0.117647f, 0.117647f, 0.117647f, 1.00f);
    ImVec4 Text = ImVec4(0.862745f, 0.862745f, 0.862745f, 1.00f);
    ImVec4 TextDisabled = ImVec4(0.450980f, 0.450980f, 0.450980f, 1.00f);
    ImVec4 WindowBg = ImVec4(0.164706f, 0.164706f, 0.164706f, 0.945098f);
    ImVec4 ChildBg = ImVec4(0.133333f, 0.133333f, 0.133333f, 1.00f);
    ImVec4 PopupBg = ImVec4(0.117647f, 0.117647f, 0.117647f, 1.00f);
    ImVec4 Border = ImVec4(0.286275f, 0.286275f, 0.286275f, 0.314000f);
    ImVec4 BorderShadow = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.000000f);
    ImVec4 FrameBg = ImVec4(0.168627f, 0.168627f, 0.168627f, 1.00f);
    ImVec4 FrameBgHovered = ImVec4(0.305882f, 0.305882f, 0.305882f, 1.00f);
    ImVec4 FrameBgActive = ImVec4(0.407843f, 0.407843f, 0.407843f, 1.00f);
    ImVec4 TitleBg = ImVec4(0.117647f, 0.117647f, 0.117647f, 1.00f);
    ImVec4 TitleBgActive = ImVec4(0.129412f, 0.129412f, 0.129412f, 1.00f);
    ImVec4 TitleBgCollapsed = ImVec4(0.117647f, 0.117647f, 0.117647f, 1.00f);
    ImVec4 MenuBarBg = ImVec4(0.184314f, 0.184314f, 0.184314f, 1.00f);
    ImVec4 ScrollbarBg = ImVec4(0.117647f, 0.117647f, 0.117647f, 1.00f);
    ImVec4 ScrollbarGrab = ImVec4(0.364706f, 0.364706f, 0.364706f, 1.00f);
    ImVec4 ScrollbarGrabHovered = ImVec4(0.482353f, 0.482353f, 0.482353f, 1.00f);
    ImVec4 ScrollbarGrabActive = ImVec4(0.545098f, 0.545098f, 0.545098f, 1.00f);
    ImVec4 CheckMark = ImVec4(0.937255f, 0.937255f, 0.937255f, 1.00f);
    ImVec4 SliderGrab = ImVec4(0.549020f, 0.776471f, 0.239216f, 1.00f);
}

namespace DarkThemeColors {
    ImVec4 Background = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 Text = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 Button = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 ButtonHovered = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 ButtonActive = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 TitleBackground = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleText = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 MenuBarHovered = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 SliderGrab = ImVec4(0.4f, 0.4f, 0.8f, 1.0f);
    ImVec4 InputTextBackground = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
}

namespace LightThemeColors {
    ImVec4 Background = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 Text = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 Button = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 ButtonHovered = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    ImVec4 ButtonActive = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    ImVec4 TitleBackground = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    ImVec4 TitleText = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 MenuBarHovered = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 SliderGrab = ImVec4(0.2f, 0.2f, 0.6f, 1.0f);
    ImVec4 InputTextBackground = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
}

void ntrans::TransportUI::SetDarkTheme(ImGuiStyle& style)
{
    ImGui::StyleColorsDark();



    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg] = DarkThemeColors::Background;
    style.Colors[ImGuiCol_Text] = DarkThemeColors::Text;
    style.Colors[ImGuiCol_Button] = DarkThemeColors::Button;
    style.Colors[ImGuiCol_ButtonHovered] = DarkThemeColors::ButtonHovered;
    style.Colors[ImGuiCol_ButtonActive] = DarkThemeColors::ButtonActive;
    style.Colors[ImGuiCol_TitleBg] = DarkThemeColors::TitleBackground;
    style.Colors[ImGuiCol_TitleBgActive] = DarkThemeColors::TitleBackground;
    style.Colors[ImGuiCol_TitleBgCollapsed] = DarkThemeColors::TitleBackground;
    style.Colors[ImGuiCol_MenuBarBg] = DarkThemeColors::Background;
    style.Colors[ImGuiCol_SliderGrab] = DarkThemeColors::SliderGrab;
    style.Colors[ImGuiCol_FrameBg] = DarkThemeColors::InputTextBackground;
    style.Colors[ImGuiCol_FrameBgHovered] = DarkThemeColors::InputTextBackground;
    style.Colors[ImGuiCol_ScrollbarBg] = AtomDarkTheme::ScrollbarBg;
    style.Colors[ImGuiCol_ScrollbarGrab] = AtomDarkTheme::ScrollbarGrab;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = AtomDarkTheme::ScrollbarGrabHovered;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = AtomDarkTheme::ScrollbarGrabActive;
    style.Colors[ImGuiCol_PopupBg] = AtomDarkTheme::PopupBg;
    style.Colors[ImGuiCol_CheckMark] = AtomDarkTheme::CheckMark;
}

void ntrans::TransportUI::SetLightTheme(ImGuiStyle& style)
{
    ImGui::StyleColorsLight();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg] = LightThemeColors::Background;
    style.Colors[ImGuiCol_Text] = LightThemeColors::Text;
    style.Colors[ImGuiCol_Button] = LightThemeColors::Button;
    style.Colors[ImGuiCol_ButtonHovered] = LightThemeColors::ButtonHovered;
    style.Colors[ImGuiCol_ButtonActive] = LightThemeColors::ButtonActive;
    style.Colors[ImGuiCol_TitleBg] = LightThemeColors::TitleBackground;
    style.Colors[ImGuiCol_TitleBgActive] = LightThemeColors::TitleBackground;
    style.Colors[ImGuiCol_TitleBgCollapsed] = LightThemeColors::TitleBackground;
    style.Colors[ImGuiCol_MenuBarBg] = LightThemeColors::Background;
    style.Colors[ImGuiCol_SliderGrab] = LightThemeColors::SliderGrab;
    style.Colors[ImGuiCol_FrameBg] = LightThemeColors::InputTextBackground;
    style.Colors[ImGuiCol_FrameBgHovered] = LightThemeColors::InputTextBackground;
}

int ntrans::TransportUI::init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5;
    io.IniFilename = "./imgui.ini";
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        if (uiEvents.isLightTheme)
            SetLightTheme(style);
        else
            SetDarkTheme(style);

    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);

    io.Fonts->AddFontFromFileTTF("../assets/misc/fonts/CascadiaCode-Bold.otf", 16.0f);
    bool ret = LoadTextureFromFile("../assets/load.png", &loadIcon.data, &loadIcon.width, &loadIcon.height);
    IM_ASSERT(ret);
    ret = LoadTextureFromFile("../assets/save.png", &saveIcon.data, &saveIcon.width, &saveIcon.height);
    IM_ASSERT(ret);
    ret = LoadTextureFromFile("../assets/run_small.png", &runIcon.data, &runIcon.width, &runIcon.height);
    IM_ASSERT(ret);
    ret = LoadTextureFromFile("../assets/stop_small.png", &stopIcon.data, &stopIcon.width, &stopIcon.height);
    IM_ASSERT(ret);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state


    return 0;
}

void ntrans::TransportUI::MainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem("Dark Theme", 0, &uiEvents.isDarkTheme)) {
                    uiEvents.isLightTheme = false;
                    uiEvents.isDarkTheme = true;
                    ImGuiStyle& style = ImGui::GetStyle();
                    SetDarkTheme(style);
                }
                if (ImGui::MenuItem("Light Theme", 0, &uiEvents.isLightTheme)) {
                    uiEvents.isLightTheme = true;
                    uiEvents.isDarkTheme = false;
                    ImGuiStyle& style = ImGui::GetStyle();
                    SetLightTheme(style);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Configuration")) {
            if (ImGui::MenuItem("Transport Config")) { uiEvents.showTransportConfigWindow = true; }
            if (ImGui::MenuItem("NN Config")) {}
            if (ImGui::MenuItem("Show Logs")) { uiEvents.showLogs = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {

            if (ImGui::MenuItem("Load Observation Data")) {
                if (!transportData->uiControls.isRunning) {
                    obsDataInfo.dataColumn = 0;
                    obsDataInfo.timeColumn = 0;
                    obsDataInfo.poreColumn = 0;
                    COMDLG_FILTERSPEC csvExtension[] = { fileExtensions.csvFilter };
                    obsDataInfo.obsFileName = nims_n::FileExplorer::openFile(L"Select Observed BT Data File", 1, csvExtension);
                    try
                    {
                        obsDataInfo.obsDataColumns = nims_n::getCSVHeader(obsDataInfo.obsFileName, ',');
                        uiEvents.showLoadObsWindow = true;
                    }
                    catch (const std::exception& er)
                    {
                        
                    }
                    
                    /*dataloader(&filaname);
                    if (filaname.size() > 0) {
                        obsData.clear();
                        obsData.readCSV(filaname);
                        int itemsSize = obsData.columns.size();
                        if (obsDataColumns != nullptr)
                            delete[] obsDataColumns;
                        obsDataColumns = new char* [itemsSize * sizeof(char*)];
                        for (int j{ 0 }; j < obsData.columns.size(); j++) {
                            obsDataColumns[j] = (char*)obsData.columns[j].c_str();
                        }
                        showLoadObsWindow = true;
                    }*/
                }
            }
            if (ImGui::MenuItem("Load Model Parameters")) {
                if (!transportData->uiControls.isRunning) {
                    loadModelParameters();
                }
            }

            if (ImGui::BeginMenu("Optimizer")) {
                if (ImGui::MenuItem("Adam Optimizer")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showAdamWindow = true;
                    }
                }
                if (ImGui::MenuItem("Marquardt")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showMarquardtWindow = true;
                    }
                }
                if (ImGui::MenuItem("Simulated Annealing")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showAnnealingWindow = true;
                    }
                }
                if (ImGui::MenuItem("Estimate Uncertainty")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showUncertaintyWindow = true;
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Custom Simulations")) {
                if (ImGui::MenuItem("Monte Carlo Simulation")) {}

                if (ImGui::MenuItem("Scenario Simulation")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showScenarioWindow = true;
                    }
                }
                if (ImGui::MenuItem("Sensitivity Analysis")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showSensitivityWindow = true;
                    }
                }

                if (ImGui::MenuItem("Flow Interrupts")) {
                    if (!transportData->uiControls.isRunning)
                        uiEvents.showFlowInterruptsWindow = true;
                }

                if (ImGui::MenuItem("Custom Multiple Scenarios")) {
                    if (!transportData->uiControls.isRunning) {
                        uiEvents.showScenerioLoopWindow = true;
                    }
                }

                ImGui::EndMenu();
            }


            if (ImGui::MenuItem("Utilities")) {
                if (!transportData->uiControls.isRunning) {
                    uiEvents.showUtilitiesWindow = true;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 50);
        if (!transportData->uiControls.isRunning) {
            if (ImGui::ImageButton("run transport", (void*)(intptr_t)runIcon.data, ImVec2(runIcon.width, runIcon.height))) {
                if (!transportData->uiControls.isRunning)
                    taskExecuter.async_(modelObject);

            }
        }
        else {
            if (ImGui::ImageButton("stop transport", (void*)(intptr_t)stopIcon.data, ImVec2(stopIcon.width, stopIcon.height))) {
                transportData->uiControls.scheduleStop = true;
                transportData->uiControls.stopCustomLoop = true;
                if (transportData->uiControls.isCalibration && transportData->calibrationType == CalibrationType::Optimizer) {
                    uiEvents.showUncertaintyWindow = true;
                }
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndMainMenuBar();
    }
}

void ntrans::TransportUI::loadModelParameters()
{
    COMDLG_FILTERSPEC iniExtension[] = { fileExtensions.iniFilter };
    std::string filename = nims_n::FileExplorer::openFile(L"Select Fixed Components Data File", 1, iniExtension);
   
    if (std::filesystem::exists(filename)) {
        modelObject.loadModelParameters(filename, transportData);

        if (transportData->scenarios.size() > scenarioInputData.size()) {
            size_t remainingSize = transportData->scenarios.size() - scenarioInputData.size();
            for (size_t rs{ 0 }; rs < remainingSize; rs++) {
                std::vector<std::string> oneRow{};
                for (int j{ 0 }; j < paramsNames.size() - 1; j++) {
                    oneRow.push_back(std::string());
                }
                scenarioInputData.push_back(oneRow);
            }
        }

        for (int i{ 0 }; i < transportData->scenarios.size(); i++) {
            scenarioInputData[i][0] = std::to_string(transportData->scenarios[i].sceneTime);
            for (int j{ 0 }; j < transportData->scenarios[i].modifiedParams.size(); j++) {
                int idx = transportData->scenarios[i].modifiedParams[j] - 999;
                scenarioInputData[i][idx] = std::to_string(transportData->scenarios[i].paramValues[j]);
            }
        }
        if (transportData->flowInterrupts.size() > 0) {
            interruptInputData.clear();

            for (int i{ 0 }; i < transportData->flowInterrupts.size(); i++) {
                std::vector<std::string> oneInt;
                if (transportData->uiControls.usePoreVols) {
                    double vc = transportData->columnParams.domainLength * transportData->transParams.waterContent * transportData->transParams.flowRate / 24.0;
                    double stTime = vc * transportData->flowInterrupts[i].startTime / (transportData->columnParams.domainLength * transportData->transParams.waterContent);
                    double drTime = vc * transportData->flowInterrupts[i].duration / (transportData->columnParams.domainLength * transportData->transParams.waterContent);

                    oneInt.push_back(std::to_string(stTime));
                    oneInt.push_back(std::to_string(drTime));
                }
                else {
                    oneInt.push_back(std::to_string(transportData->flowInterrupts[i].startTime));
                    oneInt.push_back(std::to_string(transportData->flowInterrupts[i].duration));
                }


                interruptInputData.push_back(oneInt);
            }
        }


    }
}

void ntrans::TransportUI::PlotWindow()
{
    ImVec4 scatterColor = uiEvents.isLightTheme ? ImVec4(0.1, 0.1, 0.1, 1.0) : ImVec4(0.6, 0.6, 0.6, 1.0);
    ImVec4 lineColor = uiEvents.isLightTheme ? ImVec4(0.6, 0.0, 0.0, 1.0) : ImVec4(0.8, 0.0, 0.0, 1);
    ImVec4 lineColor2 = ImVec4(0.0, 0.0, 0.8, 1);

    if (ImGui::Begin("Graph")) {
        ImVec2 curWindowPos = ImGui::GetWindowPos();
        ImVec2 curWindowSize = ImGui::GetWindowSize();

        if (transportData->uiControls.canPlot) {
            transportData->uiControls.isPlotting = true;

            ImGui::SetNextWindowPos(ImVec2(curWindowPos.x + 0.02 * curWindowSize.x, curWindowPos.y + 0.05 * curWindowSize.y), ImGuiCond_Always);
            if (ImGui::BeginChild("plot 1", ImVec2(curWindowSize.x * 0.47, curWindowSize.y * 0.7), true)) {
                ImPlotStyle& style = ImPlot::GetStyle();

                if (!transportData->uiControls.isCalibration) {

                    if (ImGui::BeginChild("##distrib")) {

                        if (ImPlot::BeginPlot("Eff Mass", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y * 0.25), ImPlotFlags_NoLegend)) {
                            int dataCount = transportData->simOut.currentSampleStep;

                            ImPlot::SetupAxis(ImAxis_Y1, "x (cm)", ImPlotAxisFlags_NoGridLines);
                            ImPlot::SetupAxis(ImAxis_X1, "", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoLabel);
                            ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImPlotCond_Always);
                            if (transportData->simOut.predictedBT.size() > dataCount)
                            {
                                ImPlot::PushColormap("Jet");

                                ImPlot::PlotHeatmap("mass_v balance", transportData->simOut.predictedBT.data() + dataCount, 1, 1, 0.0, transportData->transParams.maxInputConc, nullptr,
                                    ImPlotPoint(0.0, 0.0), ImPlotPoint(1.0, 1.0));
                                ImPlot::PopColormap();
                            }

                            ImPlot::EndPlot();
                        }



                        if (ImPlot::BeginPlot("Column Mass Distribution", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y), ImPlotFlags_NoLegend)) {
                            ImPlot::SetupAxis(ImAxis_Y1, "x (cm)", ImPlotAxisFlags_NoGridLines);
                            ImPlot::SetupAxis(ImAxis_X1, "", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoLabel);
                            ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, transportData->columnParams.domainLength, ImPlotCond_Always);
                            ImPlot::PushColormap("Jet");
                            ImPlot::PlotHeatmap("mass balance", transportData->simOut.nodesConc.data(), transportData->simOut.nodesConc.size(), 1, 0.0, transportData->transParams.maxInputConc, nullptr,
                                ImPlotPoint(0.0, 0.0), ImPlotPoint(1.0, transportData->columnParams.domainLength));
                            ImPlot::PopColormap();

                            int spaceNodesCount = (int)std::round(transportData->columnParams.domainLength / transportData->columnParams.domainSteps);
                            double columnLength = transportData->columnParams.domainLength;
                            double delta_x = transportData->columnParams.domainSteps;

                            if (transportData->simOut.nodesSorbed.size() > 0 && transportData->simOut.nodesSorbed.size() == spaceNodesCount) {
                                double spIncrementer = columnLength * 0.05;
                                std::vector<double> spaceX{ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };
                                for (double sp{ columnLength * 0.01 }; sp < columnLength; sp += spIncrementer) {
                                    int idx = (int)std::round(sp / delta_x);

                                    std::vector<double> spaceY(spaceX.size(), sp);
                                    std::string plotName = "solidphase mat" + std::to_string(idx);
                                    ImPlot::PushStyleColor(ImPlotCol_MarkerFill, heatMapRGBA(transportData->simOut.nodesSorbed[idx] / transportData->transParams.maxAdsorptionVal));
                                    ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5);
                                    ImPlot::PlotScatter(plotName.c_str(), spaceX.data(), spaceY.data(), spaceX.size());
                                    ImPlot::PopStyleColor(2);
                                }
                                int lastNode = (int)std::round(columnLength * 0.99 / delta_x);
                                double lastNodePos = columnLength * 0.99;
                                std::vector<double> spaceY(spaceX.size(), lastNodePos);
                                std::string plotName = "solidphase mat" + std::to_string(lastNode);
                                ImPlot::PushStyleColor(ImPlotCol_MarkerFill, heatMapRGBA(transportData->simOut.nodesSorbed[lastNode] / transportData->transParams.maxAdsorptionVal));
                                ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5);
                                ImPlot::PlotScatter(plotName.c_str(), spaceX.data(), spaceY.data(), spaceX.size());
                                ImPlot::PopStyleColor(2);
                            }

                            ImPlot::EndPlot();
                        }
                        ImGui::EndChild();
                    }
                    if (transportData->columnParams.isothermType > 0) {
                        ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.35);
                        if (ImGui::BeginChild("##adsorption isotherm at loc")) {
                            if (ImPlot::BeginPlot("Adsorbed Mass at Smp Loc")) {
                                ImPlot::SetupAxis(ImAxis_X1, "c (mg/L)");
                                ImPlot::SetupAxis(ImAxis_Y1, "s (mg/g)");
                                ImPlot::SetupAxesLimits(0.0, transportData->transParams.pulseConcentration, 0.0, transportData->transParams.maxAdsorptionVal);
                                ImPlot::SetNextLineStyle(scatterColor, 3);
                                int dataCount = transportData->simOut.currentSampleStep;
                                ImPlot::PlotLine("Asorption isotherm", transportData->simOut.concAtPoint.data(), transportData->simOut.sorbedAtPoint.data(), dataCount);
                                ImPlot::EndPlot();
                            }
                            ImGui::EndChild();
                        }
                    }


                }

                else {
                    switch (transportData->calibrationType)
                    {
                    case CalibrationType::Optimizer:
                    {
                        /*if (ImPlot::BeginPlot("Change in Parameters")) {
                            ImPlot::SetupAxis(ImAxis_X1, "iterations", ImPlotAxisFlags_AutoFit);
                            ImPlot::SetupAxis(ImAxis_Y1, "absolute change", ImPlotAxisFlags_AutoFit);
                            ImPlot::SetNextLineStyle(lineColor, 3);
                            ImPlot::PlotLine("absolute change", transportData->iterationList.data(), transportData->objectiveFunction.data(), transportData->optIteration);
                            ImPlot::EndPlot();
                        }*/
                        break;
                    }
                    case CalibrationType::SensitivityAnalysis:
                    {
                        /*if (transportData->sensiCol.size() == 0) {
                            if (ImPlot::BeginPlot("Sensitvity Analysis Plot")) {
                                ImPlot::SetupAxis(ImAxis_X1, transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsNames[0].c_str(), ImPlotAxisFlags_AutoFit);
                                ImPlot::SetupAxis(ImAxis_Y1, "log(SSE)", ImPlotAxisFlags_AutoFit);
                                ImPlot::SetNextLineStyle(lineColor, 3);
                                ImPlot::PlotLine("objective function", transportData->sensiRow.data(), transportData->objectiveSurface.get(), transportData->optIteration);
                                ImPlot::EndPlot();
                            }

                        }
                        else {
                            if (!transportData->noObsData)
                                if (ImPlot::BeginPlot("Sensitvity Analysis Plot", ImVec2(-1, 0), ImPlotFlags_NoLegend)) {
                                    double min_y = transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsRange[0][0];
                                    double max_y = transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsRange[0][1];

                                    double min_x = transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsRange[1][0];
                                    double max_x = transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsRange[1][1];
                                    if (transportData->sensitivityAnalysis[transportData->currentSensitivity].useLogScale[1].useLogScale)
                                        ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
                                    if (transportData->sensitivityAnalysis[transportData->currentSensitivity].useLogScale[0].useLogScale)
                                        ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);

                                    ImPlot::SetNextAxisLimits(ImAxis_Y1, min_y, max_y, ImPlotCond_Always);
                                    ImPlot::SetNextAxisLimits(ImAxis_X1, min_x, max_x, ImPlotCond_Always);
                                    ImPlot::SetupAxis(ImAxis_X1, transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsNames[1].c_str(), ImPlotAxisFlags_AutoFit);
                                    ImPlot::SetupAxis(ImAxis_Y1, transportData->sensitivityAnalysis[transportData->currentSensitivity].paramsNames[0].c_str(), ImPlotAxisFlags_AutoFit);
                                    ImPlot::PushColormap("Jet");
                                    ImPlot::PlotHeatmap("objective function", transportData->objectiveSurface.get(), transportData->sensiRow.size(), transportData->sensiCol.size(), 0, 0, nullptr,
                                        ImPlotPoint(min_x, min_y), ImPlotPoint(max_x, max_y));
                                    ImPlot::PopColormap();
                                    ImPlot::EndPlot();
                                }
                        }*/

                        break;
                    }
                    default:
                        break;
                    }


                }

                ImGui::EndChild();
            }
            ImGui::SameLine(0.02 * curWindowSize.x + curWindowSize.x * 0.49);

            if (ImGui::BeginChild("plot 2", ImVec2(curWindowSize.x * 0.47, curWindowSize.y * 0.7), true)) {
                ImPlotStyle& style = ImPlot::GetStyle();
                style.PlotDefaultSize.y = ImGui::GetContentRegionAvail().y;

                if (ImPlot::BeginPlot("Breakthrough")) {

                    if (transportData->uiControls.usePoreVols) {
                        if (transportData->simOut.observedBT.size() == transportData->simOut.obsSamplingPoreVol.size() && transportData->simOut.obsSamplingPoreVol.size()) {

                            ImPlot::SetupAxesLimits(0.0, transportData->simOut.obsSamplingPoreVol[transportData->simOut.obsSamplingPoreVol.size() - (size_t)1], 0.0, transportData->transParams.pulseConcentration);
                            ImPlot::PushStyleColor(ImPlotCol_MarkerFill, scatterColor);
                            ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, scatterColor);
                            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
                            ImPlot::PlotScatter(transportData->columnParams.simName.c_str(), transportData->simOut.obsSamplingPoreVol.data(), transportData->simOut.observedBT.data(), transportData->simOut.obsSamplingPoreVol.size());
                            ImPlot::PopStyleColor(2);
                        }

                        if (!transportData->uiControls.isCalibration) {
                            int dataCount = transportData->simOut.currentSampleStep;
                            ImPlot::SetupAxis(ImAxis_Y1, "c (mg/L)");
                            ImPlot::SetupAxis(ImAxis_X1, "pore volumes (-)");
                            ImPlot::SetNextLineStyle(lineColor, 3);
                            ImPlot::PlotLine("Prediction", transportData->simOut.samplingPoreVol.data(), transportData->simOut.predictedBT.data(), dataCount);
                            ImPlot::SetNextLineStyle(lineColor2, 3);
                            /*if (transportData->iso != Isotherm::noAdsorption)
                                ImPlot::PlotLine("s/smax", transportData->poreVolumes.data(), transportData->ads_smax.data(), dataCount);*/
                            
                        }
                        else {
                            ImPlot::SetupAxis(ImAxis_Y1, "c (mg/L)");
                            ImPlot::SetupAxis(ImAxis_X1, "pore volumes (-)");
                            ImPlot::SetNextLineStyle(lineColor, 3);
                            ImPlot::PlotLine("Prediction", transportData->simOut.samplingPoreVol.data(), transportData->simOut.predictedBT.data(), transportData->simOut.predictedBT.size());
                        }


                    }
                    else {
                        if (transportData->simOut.observedBT.size() == transportData->simOut.obsSamplingTimes.size() && transportData->simOut.obsSamplingTimes.size()) {


                            ImPlot::SetupAxesLimits(0.0, transportData->columnParams.totalTransportTime, 0.0, transportData->transParams.pulseConcentration);
                            ImPlot::PushStyleColor(ImPlotCol_MarkerFill, scatterColor);
                            ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, scatterColor);
                            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
                            ImPlot::PlotScatter(transportData->columnParams.simName.c_str(), transportData->simOut.obsSamplingTimes.data(), transportData->simOut.observedBT.data(), transportData->simOut.obsSamplingTimes.size());
                            ImPlot::PopStyleColor(2);
                        }

                        if (!transportData->uiControls.isCalibration) {
                            int dataCount = transportData->simOut.currentSampleStep;
                            ImPlot::SetupAxis(ImAxis_Y1, "c (mg/L)");
                            ImPlot::SetupAxis(ImAxis_X1, "t (h)");
                            ImPlot::SetNextLineStyle(lineColor, 3);
                            ImPlot::PlotLine("Prediction", transportData->simOut.samplingTimes.data(), transportData->simOut.predictedBT.data(), dataCount);
                            ImPlot::SetNextLineStyle(lineColor2, 3);
                            /*if (transportData->iso != Isotherm::noAdsorption)
                                ImPlot::PlotLine("s/smax", transportData->sampleTimes.data(), transportData->ads_smax.data(), dataCount);*/
                        }
                        else {
                            ImPlot::SetupAxis(ImAxis_Y1, "c (mg/L)");
                            ImPlot::SetupAxis(ImAxis_X1, "t (h)");
                            ImPlot::SetNextLineStyle(lineColor, 3);
                            ImPlot::PlotLine("Prediction", transportData->simOut.samplingTimes.data(), transportData->simOut.predictedBT.data(), transportData->simOut.predictedBT.size());
                        }
                    }



                    ImPlot::EndPlot();
                }
                ImGui::EndChild();
            }

            transportData->uiControls.isPlotting = false;
        }


        ImGui::SetNextWindowPos(ImVec2(curWindowPos.x + 0.02 * curWindowSize.x, curWindowPos.y + 0.71 * curWindowSize.y + 0.05 * curWindowSize.y), ImGuiCond_Always);
        if (ImGui::BeginChild("transport control params", ImVec2(curWindowSize.x * 0.96, curWindowSize.y * 0.235), false)) {
            std::string txt;
            ImVec2 curChildWindowSize = ImGui::GetWindowSize();
            //ImPlotStyle& style = ImPlot::GetStyle();
            if (ImGui::BeginChild("Mass Balance Window", ImVec2(curChildWindowSize.x * 0.32, 0), false, ImGuiWindowFlags_NoScrollbar)) {

                ImGui::SeparatorText("Mass Balance in Column");
                txt = "Mass Balance          = " + std::to_string(transportData->simOut.massBalanceValue);
                ImGui::Text(txt.c_str());
                txt = "Inflow Mass           = " + std::to_string(transportData->simOut.totalInflowMass) + " mg";
                ImGui::Text(txt.c_str());
                txt = "Sorbed Mass           = " + std::to_string(transportData->simOut.currentAdsorbedMass) + " mg";
                ImGui::Text(txt.c_str());
                txt = "Mobile Mass           = " + std::to_string(transportData->simOut.currentMobileMass) + " mg";
                ImGui::Text(txt.c_str());
                txt = "Immobile Mass         = " + std::to_string(transportData->simOut.currentImmobileMass) + " mg";
                ImGui::Text(txt.c_str());
                txt = "Degraded Mass         = " + std::to_string(transportData->simOut.totalDegradedMass) + " mg";
                ImGui::Text(txt.c_str());
                txt = "Outflow Mass          = " + std::to_string(transportData->simOut.totalOutflowMass) + " mg";
                ImGui::Text(txt.c_str());

                ImGui::EndChild();
            }

            ImGui::SameLine(curChildWindowSize.x * 0.34);
            if (ImGui::BeginChild("Transport Properties", ImVec2(curChildWindowSize.x * 0.32, 0), false, ImGuiWindowFlags_NoScrollbar)) {

                ImGui::SeparatorText("Dimensionless Parameters");
                txt = "Damkholer_obs Number  = " + std::to_string(transportData->simOut.damkohler_obs);
                ImGui::Text(txt.c_str());
                txt = "Damkholer_prd Number  = " + std::to_string(transportData->simOut.damkohler_prd);
                ImGui::Text(txt.c_str());
                txt = "Retardation coeff     = " + std::to_string(transportData->simOut.retardation_coef);
                ImGui::Text(txt.c_str());
                txt = "Peclet Number         = " + std::to_string(transportData->simOut.peclet);
                ImGui::Text(txt.c_str());
                txt = "Exchange Pore V       = " + std::to_string(transportData->simOut.exchPoreVol);
                ImGui::Text(txt.c_str());
                txt = "Center of Mass        = " + std::to_string(transportData->simOut.centerMass);
                ImGui::Text(txt.c_str());
                txt = "Diffusion coef        = " + std::to_string(transportData->simOut.disp_app);
                ImGui::Text(txt.c_str());


                ImGui::EndChild();
            }

            ImGui::SameLine(curChildWindowSize.x * 0.68);
            if (ImGui::BeginChild("Simulation Properties", ImVec2(curChildWindowSize.x * 0.32, 0), false, ImGuiWindowFlags_NoScrollbar)) {

                ImGui::SeparatorText("Simulation State");
                txt = "Main Iterations       = " + std::to_string(transportData->simOut.mainIterations);
                ImGui::Text(txt.c_str());
                txt = "Main Iteration Time   = " + std::to_string(transportData->simOut.mainiterationLength) + " ms";
                ImGui::Text(txt.c_str());
                txt = "Time in Jacobian F    = " + std::to_string(transportData->simOut.mainJcTime) + " ms";
                ImGui::Text(txt.c_str());
                txt = "Imobile Iterations    = " + std::to_string(transportData->simOut.imobileIterations);
                ImGui::Text(txt.c_str());
                txt = "Time in Imobile F     = " + std::to_string(transportData->simOut.imobileSolTime) + " ms";
                ImGui::Text(txt.c_str());
                txt = "Execution Currently in " + transportData->simOut.executionLoc;
                ImGui::Text(txt.c_str());
                txt = "Current Time step     =" + std::to_string(transportData->simOut.curSimStep);
                ImGui::Text(txt.c_str());
                ImGui::EndChild();
            }


            ImGui::EndChild();
        }

        ImGui::End();

    }
}

void ntrans::TransportUI::ConfigWindow()
{
    if (ImGui::Begin("Config", &uiEvents.showTransportConfigWindow, window_flags)) {

        ImGuiInputTextFlags inputTextActive = transportData->uiControls.isRunning ? ImGuiInputTextFlags_ReadOnly : 0;
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        if (ImGui::TreeNode("Discretization")) {


            {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("ChildR", ImVec2(0, 490), false);
                if (ImGui::Button("reset obs data") && !transportData->uiControls.isRunning) {
                    transportData->simOut.observedBT.clear();
                    transportData->simOut.obsSamplingTimes.clear();
                    transportData->simOut.obsSamplingPoreVol.clear();
                }
                ImGui::SameLine();
                if (ImGui::Button("add noise") && transportData->simOut.predictedBT.size() > 0 && !transportData->uiControls.isRunning) {

                    //addNoise();
                }

                if (ImGui::BeginTable("split", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextColumn();
                    ImGui::SeparatorText("Time Settings");
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Simulation Time (T)", &transportData->columnParams.totalTransportTime, 0.01f, 1.0f, "%.3f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Time Step (T)", &transportData->columnParams.timestep, 0.01f, 1.0f, "%.8f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputInt("Simulation Delay (T)", &transportData->uiControls.simDelay, 1, 1, inputTextActive);
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputInt("Max Iterations", &transportData->maxIterations, 1, 1, inputTextActive);
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Noise Level", &transportData->noiseLevel, 0.001f, 0.1f, "%.6f", inputTextActive);
                    ImGui::Separator();

                    ImGui::NewLine();
                    ImGui::TableNextColumn();
                    ImGui::SeparatorText("Space Settings");
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Space (L)", &transportData->columnParams.domainLength, 0.01f, 1.0f, "%.4f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Space Step (L)", &transportData->columnParams.domainSteps, 0.01f, 1.0f, "%.6f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Cross-Sectional Area (L*L)", &transportData->columnParams.crossSectionArea, 0.01f, 1.0f, "%.4f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Eff Vessel's vol (L*L*L)", &transportData->columnParams.effluentVesselVol, 0.01, 0.1, "%.4f", inputTextActive);
                    ImGui::Separator();







                    ImGui::EndTable();
                }


                ImGui::EndChild();
                ImGui::PopStyleVar();
            }



            ImGui::TreePop();

            ImGui::NewLine();
        }

        if (ImGui::TreeNode("Transport")) {


            {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                ImGui::BeginChild("ADE", ImVec2(0, 380), false);

                if (ImGui::BeginTable("ADE_Table", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextColumn();
                    ImGui::SeparatorText("Advection Parameters");
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::Checkbox("Use Pore V.", &transportData->uiControls.usePoreVols);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Input Amount (M/L*L*L)", &transportData->transParams.pulseConcentration, 0.01f, 1.0f, "%.4f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Water Content (-)", &transportData->transParams.waterContent, 0.01f, 0.1f, "%.3f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    if (transportData->uiControls.usePoreVols) {
                        ImGui::InputDouble("Pumping Rate (pv/day)", &transportData->transParams.flowRate, 0.01f, 0.1f, "%.5f", inputTextActive);
                    }
                    else {
                        ImGui::InputDouble("Pumping Rate (L*L*L/T)", &transportData->transParams.flowRate, 0.01f, 0.1f, "%.5f", inputTextActive);
                    }

                    ImGui::Separator();
                    ImGui::NewLine();

                    ImGui::SeparatorText("Diffusion Parameters");
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Dispersion Len. (L)", &transportData->transParams.dispersionLength, 0.01f, 1.0f, "%.8f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Molecular Diff Coeff (L*L/T)", &transportData->transParams.molecularDiffusion, 0.01f, 1.0f, "%.8f", inputTextActive);
                    ImGui::Separator();
                    ImGui::NewLine();

                    ImGui::SeparatorText("Mobile-Immobile Transfer Parameters");
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Fraction of Mobile Water (-)", &transportData->transParams.mo_imPartitionCoefficient, 0.01f, 1.0f, "%.4f", inputTextActive);
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(220.0f);
                    ImGui::InputDouble("Imo-Mob Mass Transfer Coef (1/T)", &transportData->transParams.mo_imExchangeRate, 0.01f, 1.0f, "%.8f", inputTextActive);




                    ImGui::EndTable();
                }


                ImGui::EndChild();
                ImGui::PopStyleVar();
            }



            ImGui::TreePop();
            ImGui::NewLine();
        }

        if (ImGui::TreeNode("Reactions")) {

            ImGui::SeparatorText("Select Isotherm Model");
            ImGui::RadioButton("No Adsorption", &transportData->columnParams.isothermType, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Linear Adsorption", &transportData->columnParams.isothermType, 3);
            ImGui::SameLine();
            ImGui::RadioButton("Langmuir Adsorption", &transportData->columnParams.isothermType, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Freundlich Adsorption", &transportData->columnParams.isothermType, 2);
            ImGui::NewLine();

            if (transportData->columnParams.isothermType > 0)
            {
                ImGui::SeparatorText("Sorption Parameters");

                {
                    //ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
                    ImGui::BeginChild("Reactions", ImVec2(0, 680), false);

                    if (ImGui::BeginTable("Reaction_Table", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
                    {
                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Bulk Density (M/L*L*L)", &transportData->transParams.bulkDensity, 0.01f, 0.1f, "%.3f", inputTextActive);
                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Partion Coefficient (-)", &transportData->transParams.eq_kinPartitionCoefficient, 0.01f, 0.1f, "%.8f", inputTextActive);

                        switch (transportData->columnParams.isothermType)
                        {
                        case 1:
                        {
                            ImGui::NewLine();

                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("Isotherm Constant (L*L*L/M)", &transportData->transParams.isothermConstant, 0.01f, 0.1f, "%.8f", inputTextActive);


                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("Smax (M/M)", &transportData->transParams.adsorptionCapacity, 0.01f, 0.1f, "%.8f", inputTextActive);

                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("Hysteresis Coefficient (-)", &transportData->transParams.hysteresisCoefficient, 0.01f, 0.1f, "%.8f", inputTextActive);




                            break;
                        }
                        case 2:
                        {
                            ImGui::NewLine();

                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("Isotherm Constant (L*L*L/M)", &transportData->transParams.isothermConstant, 0.01f, 0.1f, "%.8f", inputTextActive);

                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("nf (-)", &transportData->transParams.adsorptionCapacity, 0.01f, 0.1f, "%.8f", inputTextActive);

                            break;
                        }
                        case 3:
                        {
                            ImGui::NewLine();

                            ImGui::SetNextItemWidth(220.0f);
                            ImGui::InputDouble("Isotherm Constant (L*L*L/M)", &transportData->transParams.isothermConstant, 0.01f, 0.1f, "%.8f", inputTextActive);
                            break;
                        }
                        default:
                            break;
                        }

                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Reaction Rate Coefficient (1/T)", &transportData->transParams.reactionRateCoefficient, 0.01f, 0.1f, "%.8f", inputTextActive);
                        ImGui::Separator();

                        ImGui::NewLine();
                        ImGui::SeparatorText("Degradation Parameters");

                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Solution Deg Rate (1/T)", &transportData->transParams.degradationRate_soln, 0.01f, 0.1f, "%.8f", inputTextActive);

                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Eq-Sorbed Deg Rate (1/T)", &transportData->transParams.degradationRate_eqsb, 0.01f, 0.1f, "%.8f", inputTextActive);

                        ImGui::SetNextItemWidth(220.0f);
                        ImGui::InputDouble("Kin-Sorbed Deg Rate (1/T)", &transportData->transParams.degradationRate_kinsb, 0.01f, 0.1f, "%.8f", inputTextActive);

                        ImGui::EndTable();
                    }

                    ImGui::EndChild();
                    //ImGui::PopStyleVar();
                }
            }


            ImGui::TreePop();
        }
        ImGui::NewLine();
        ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y + 0.99 * windowSize.y - (loadIcon.height)), ImGuiCond_Always);
        ImGui::BeginChild("save_load_window", ImVec2(windowSize.x, loadIcon.height));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.4, -1.0f);
        if (ImGui::ImageButton("load transport data", (void*)(intptr_t)loadIcon.data, ImVec2(loadIcon.width, loadIcon.height))) {
            if (!transportData->uiControls.isRunning) {
                loadModelParameters();
            }
        }
        ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6, -1.0f);
        if (ImGui::ImageButton("save transport", (void*)(intptr_t)saveIcon.data, ImVec2(saveIcon.width, saveIcon.height))) {

            if (!transportData->uiControls.isRunning) {
                uiEvents.showSaveDataWindow = true;
            }
        }


        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::End();
    }
}

void ntrans::TransportUI::LogsWindow()
{
    static float f = 0.0f;
    static int counter = 0;

    if (ImGui::Begin("Logs", &uiEvents.showLogs, window_flags)) {
        ImVec2 curWindowPos = ImGui::GetWindowPos();
        ImVec2 curWindowSize = ImGui::GetWindowSize();

        //ImGui::SetNextWindowPos(ImVec2(curWindowPos.x + 0.02 * curWindowSize.x, curWindowPos.y), ImGuiCond_Always);
        ImGui::BeginChild("update logs", ImVec2(curWindowSize.x * 0.47, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::SeparatorText("Output");
        if (updateMessages.size() > 0) {
            int sz = (int)updateMessages.size() - 1;
            for (int i{ sz }; i >= 0; i--) {
                ImGui::Text(updateMessages[i].c_str());
                ImGui::Separator();
            }
        }



        ImGui::EndChild();

        ImGui::SameLine(0.02 * curWindowSize.x + curWindowSize.x * 0.49);
        ImGui::BeginChild("error logs", ImVec2(curWindowSize.x * 0.47, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::SeparatorText("Error List");
        if (errorMessages.size() > 0) {
            int sz = (int)errorMessages.size() - 1;
            for (int i{ sz }; i >= 0; i--) {
                ImGui::Text(errorMessages[i].c_str());
                ImGui::Separator();
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}

void ntrans::TransportUI::LoadObsDataWindow()
{
    ImGui::SetNextWindowSize(ImVec2(950.0, 312.0), ImGuiCond_Always);
    //ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    if (ImGui::Begin("Load Observation Data", &uiEvents.showLoadObsWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        

        int itemsSize = obsDataInfo.obsDataColumns.size();

        const char* combo_preview_value = obsDataInfo.obsDataColumns[obsDataInfo.timeColumn].data();  // Pass in the preview value visible before opening the combo (it could be anything)

        ImGui::SetNextItemWidth(300.0f);
        if (ImGui::BeginCombo("Time Column", combo_preview_value))
        {
            for (int n = 0; n < itemsSize; n++)
            {
                const bool is_selected = (obsDataInfo.timeColumn == n);
                if (ImGui::Selectable(obsDataInfo.obsDataColumns[n].data(), is_selected))
                    obsDataInfo.timeColumn = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine(500.0f);
        const char* combo_preview_value2 = obsDataInfo.obsDataColumns[obsDataInfo.dataColumn].data();
        ImGui::SetNextItemWidth(300.0f);
        if (ImGui::BeginCombo("Data Column", combo_preview_value2))
        {
            for (int n = 0; n < itemsSize; n++)
            {
                const bool is_selected = (obsDataInfo.dataColumn == n);
                if (ImGui::Selectable(obsDataInfo.obsDataColumns[n].data(), is_selected))
                    obsDataInfo.dataColumn = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Load PV", &uiEvents.loadObsPV);
        if (uiEvents.loadObsPV) {
            ImGui::SameLine(500.0f);

            const char* combo_preview_value3 = obsDataInfo.obsDataColumns[obsDataInfo.poreColumn].data();
            ImGui::SetNextItemWidth(300.0f);
            if (ImGui::BeginCombo("PV Column", combo_preview_value3))
            {
                for (int n = 0; n < itemsSize; n++)
                {
                    const bool is_selected = (obsDataInfo.poreColumn == n);
                    if (ImGui::Selectable(obsDataInfo.obsDataColumns[n].data(), is_selected))
                        obsDataInfo.poreColumn = n;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
        ImGui::SetNextWindowPos(ImVec2(windowPos.x + 0.5 * windowSize.x - 60, windowPos.y + 0.8 * windowSize.y));
        ImGui::BeginChild("button window");

        if (ImGui::Button("Load Data", ImVec2(120, 40))) {
            try
            {
                nims_n::ReadCSV<double>obsData(obsDataInfo.obsFileName, ',');

                int dataLen = obsData.rowSize();
                nims_n::ReadCSV<double> data = obsData[obsDataInfo.dataColumn];
                nims_n::ReadCSV<double> ts = obsData[obsDataInfo.timeColumn];

                transportData->simOut.observedBT.clear();
                transportData->simOut.observedBT.resize(dataLen);
                std::copy(data.begin(), data.end(), transportData->simOut.observedBT.begin());

                transportData->simOut.obsSamplingTimes.clear();
                transportData->simOut.obsSamplingTimes.resize(dataLen);
                std::copy(ts.begin(), ts.end(), transportData->simOut.obsSamplingTimes.begin());

                if (uiEvents.loadObsPV) {
                    nims_n::ReadCSV<double> pv = obsData[obsDataInfo.poreColumn];
                    transportData->simOut.obsSamplingPoreVol.clear();
                    transportData->simOut.obsSamplingPoreVol.resize(dataLen);
                    std::copy(pv.begin(), pv.end(), transportData->simOut.obsSamplingPoreVol.begin());

                }

                //transportEvents.push_back(TransportSimEvents::CalculateCenterOfMass);
                //TODO calculate center of mass
                transportData->uiControls.canPlot = true;
                uiEvents.showLoadObsWindow = false;

            }
            catch (const std::exception& er)
            {

            }
            
        }
        ImGui::EndChild();


        ImGui::End();
    }
}

void ntrans::TransportUI::SelectParamsWindow()
{
}

void ntrans::TransportUI::SaveDataWindow()
{
    ImGui::SetNextWindowSize(ImVec2(750.0, 312.0), ImGuiCond_Always);
    if (ImGui::Begin("Save Simulation Data", &uiEvents.showSaveDataWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::NewLine();
        ImGui::Checkbox("Save Observed BT?", &uiEvents.saveObservationData);
        ImGui::SameLine(450.0f);
        ImGui::Checkbox("Save Predicted BT?", &uiEvents.saveSimulationSeries);
        ImGui::Checkbox("Save Simulation Parameters?", &uiEvents.saveSimParams);
        ImGui::SameLine(450.0f);
        ImGui::Checkbox("Save Isotherm Data?", &uiEvents.saveIsothermData);


        ImGui::SetNextWindowPos(ImVec2(windowPos.x + 0.5 * windowSize.x - 60, windowPos.y + 0.8 * windowSize.y));
        ImGui::BeginChild("save data button window");
        if (ImGui::Button("Save Data", ImVec2(120, 40)) && !transportData->uiControls.isRunning) {
            int columnSize{ 0 };
            std::string folderName = nims_n::FileExplorer::openFolder(L"Select Folder to Save Model Files");
            if (!std::filesystem::exists(folderName)) {
                return;
            }
            if (uiEvents.saveObservationData && transportData->simOut.observedBT.size() > 0) {
                columnSize++;
            }
            if (uiEvents.saveSimulationSeries && transportData->simOut.predictedBT.size() > 0) {
                columnSize++;
            }

            if (columnSize > 1) {
                if (transportData->simOut.samplingPoreVol.size() == transportData->simOut.observedBT.size())
                    columnSize++;
                int rows = transportData->simOut.observedBT.size();
                int cols = columnSize + 1;
                std::vector<double> datToSave(rows * cols);
                int idx{ 0 };
                std::vector<std::string>headers{ "Time", "obs", "prd" };
                if (columnSize > 2) {
                    headers = std::vector<std::string>{ "Time", "pore vol", "obs", "prd" };
                    for (int i{ 0 }; i < transportData->simOut.observedBT.size(); i++) {
                        idx = i * 4;
                        datToSave[idx] = transportData->simOut.samplingTimes[i];
                        idx = i * 4 + 1;
                        datToSave[idx] = transportData->simOut.samplingPoreVol[i];
                        idx = i * 4 + 2;
                        datToSave[idx] = transportData->simOut.observedBT[i];
                        idx = i * 4 + 3;
                        datToSave[idx] = transportData->simOut.predictedBT[i];
                    }
                }
                else {
                    for (int i{ 0 }; i < transportData->simOut.observedBT.size(); i++) {
                        idx = i * 3;
                        datToSave[idx] = transportData->simOut.samplingTimes[i];
                        idx = i * 3 + 1;
                        datToSave[idx] = transportData->simOut.observedBT[i];
                        idx = i * 3 + 2;
                        datToSave[idx] = transportData->simOut.predictedBT[i];
                    }
                }


                std::string filename = folderName + "/" + transportData->columnParams.simName + "_obs_prd.csv";
                nims_n::saveCSV(filename, datToSave, rows, cols, ',', headers);

            }
            else if (columnSize == 1)
            {
                int saveSize = transportData->simOut.observedBT.size() > transportData->simOut.predictedBT.size() ?
                    transportData->simOut.observedBT.size() : transportData->simOut.predictedBT.size();

                if (transportData->simOut.samplingPoreVol.size() == saveSize)
                    columnSize++;

                std::vector<std::string>headers{ "Time", "obs" };
                std::vector<double> datToSave (saveSize * (columnSize + 1));
                int idx{ 0 };

                if (transportData->simOut.observedBT.size() > 0 && uiEvents.saveObservationData) {
                    if (columnSize > 1) {
                        headers = std::vector<std::string>{ "Time", "pore vol", "obs" };
                        for (int i{ 0 }; i < transportData->simOut.observedBT.size(); i++) {
                            idx = i * 3;
                            datToSave[idx] = transportData->simOut.samplingTimes[i];
                            idx = i * 3 + 1;
                            datToSave[idx] = transportData->simOut.samplingPoreVol[i];
                            idx = i * 3 + 2;
                            datToSave[idx] = transportData->simOut.observedBT[i];

                        }
                    }
                    else {
                        for (int i{ 0 }; i < transportData->simOut.observedBT.size(); i++) {
                            idx = i * 2;
                            datToSave[idx] = transportData->simOut.samplingTimes[i];
                            idx = i * 2 + 1;
                            datToSave[idx] = transportData->simOut.observedBT[i];

                        }
                    }


                    std::string filename = folderName + "/" + transportData->columnParams.simName + "_obs.csv";
                    nims_n::saveCSV(filename, datToSave, saveSize, columnSize + 1, ',', headers);
                }
                else if (transportData->simOut.predictedBT.size() > 0 && uiEvents.saveSimulationSeries) {
                    std::vector<std::string>headers{ "Time", "prd" };

                    if (columnSize > 1) {
                        headers = std::vector<std::string>{ "Time","pore vol", "prd" };
                        for (int i{ 0 }; i < transportData->simOut.predictedBT.size(); i++) {
                            idx = i * 3;
                            datToSave[idx] = transportData->simOut.samplingTimes[i];
                            idx = i * 3 + 1;
                            datToSave[idx] = transportData->simOut.samplingPoreVol[i];
                            idx = i * 3 + 2;
                            datToSave[idx] = transportData->simOut.predictedBT[i];

                        }
                    }
                    else {
                        for (int i{ 0 }; i < transportData->simOut.predictedBT.size(); i++) {
                            idx = i * 2;
                            datToSave[idx] = transportData->simOut.samplingTimes[i];
                            idx = i * 2 + 1;
                            datToSave[idx] = transportData->simOut.predictedBT[i];

                        }
                    }


                    std::string filename = folderName + "/" + transportData->columnParams.simName + "_prd.csv";
                    nims_n::saveCSV(filename, datToSave, saveSize, columnSize + 1, ',', headers);
                }
            }

            if (uiEvents.saveSimParams)
                modelObject.saveModelParameters(transportData, folderName);
            if (uiEvents.saveIsothermData && transportData->simOut.concAtPoint.size() > 0) {
                int isoLen = transportData->simOut.concAtPoint.size();

                std::vector<double> isomat (isoLen * 2);
                std::vector<std::string> heads{ "c (mg/L)", "s (mg/L)" };

                for (int i{ 0 }; i < isoLen; i++) {
                    int idx_c = i * 2;
                    int idx_s = i * 2 + 1;
                    isomat[idx_c] = transportData->simOut.concAtPoint[i];
                    isomat[idx_s] = transportData->simOut.sorbedAtPoint[i];
                }
                std::string filename = folderName + "/" + transportData->columnParams.simName + "_iso.csv";
                nims_n::saveCSV(filename, isomat, isoLen, 2, ',', heads);
            }

            uiEvents.showSaveDataWindow = false;
        }
        ImGui::EndChild();


        ImGui::End();
    }
}

void ntrans::TransportUI::windowBody()
{
    PlotWindow();
    if (uiEvents.showLogs)
        LogsWindow();
    if (uiEvents.showTransportConfigWindow)
        ConfigWindow();
    if (uiEvents.showSaveDataWindow)
        SaveDataWindow();
    if (uiEvents.showLoadObsWindow)
        LoadObsDataWindow();

}

void ntrans::TransportUI::updateWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        MainMenu();
        ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport());
        windowBody();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
}

ImVec4 ntrans::TransportUI::heatMapRGBA(double value)
{
    ImVec4 rgbColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    int colorValue{ 0 };
    if (value >= 0.0 && value < 0.25) {
        rgbColor.z = 1.0f;
        colorValue = round((value / 0.25) * 1.0);
        rgbColor.y = colorValue;
    }

    else if (value >= 0.25 && value < 0.5) {
        rgbColor.z = 1.0f;
        rgbColor.y = 1.0f;
        colorValue = round(((value - 0.25) / 0.25) * 1.0);
        rgbColor.z -= colorValue;
    }

    else if (value >= 0.5 && value < 0.75) {
        rgbColor.y = 1.0f;
        colorValue = round(((value - 0.5) / 0.25) * 1.0);
        rgbColor.x = colorValue;
    }

    else if (value >= 0.75 && value < 1.0) {
        rgbColor.y = 1.0f;
        rgbColor.x = 1.0f;
        colorValue = round(((value - 0.75) / 0.25) * 1.0);
        rgbColor.y -= colorValue;
    }

    else if (value >= -0.5 && value < 0.0) {
        rgbColor.z = 1.0;
        rgbColor.x = 1.0;
        colorValue = round(((value - 0.75) / 0.25) * 1.0);
        rgbColor.x -= colorValue;
    }

    else if (value >= -1.0 && value < -0.5) {
        rgbColor.x = 1.0f;
        colorValue = round(((value - 0.75) / 0.25) * 1.0);
        rgbColor.z = colorValue;
    }
    else if (value >= 1.0) {
        rgbColor.x = 1.0f;
    }

    return rgbColor;
}

ntrans::TransportUI::TransportUI(int _width, int _height, std::string windowName, SimulationData* _modelData) :
    height(_height), width(_width), name(windowName), transportData(_modelData)
{
    modelObject = ModelADE(transportData);

    int error = init();
    if (error > 0)
        fprintf(stderr, "Failed to create GLFW window");
    for (int i{ 0 }; i < 5; i++) {
        std::vector<std::string> oneRow{};
        for (int j{ 0 }; j < paramsNames.size() - 1; j++) {
            oneRow.push_back(std::string());
        }
        scenarioInputData.push_back(oneRow);
    }
    for (int i{ 0 }; i < paramsNames.size() - 2; i++) {
        paramsId.push_back(1000 + i);
    }
    window_flags |= ImGuiWindowFlags_NoCollapse;
}

ntrans::TransportUI::~TransportUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ntrans::TransportUI::operator()()
{
    updateWindow();
}

void ntrans::glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool ntrans::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
