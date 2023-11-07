#include <graphing.h>
#include <helper.h>
#include <defines.h>


#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imguipp.h>

#include <implot.h>
#include <implot_internal.h>

#include <string.h>


ImColor float4toImColorRead(float input[4]) {
	return ImColor(input[0], input[1], input[2], input[3]);
}

ImVec4 float4toImVec4Read(float input[4]) {
	return ImVec4(input[0], input[1], input[2], input[3]);
}




extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);


LRESULT __stdcall WindowProcess(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam))
        return TRUE;

    switch (message)
    {
        case WM_SIZE:
        {
            if (gui::device && wParam != SIZE_MINIMIZED)
            {
                gui::presentParameters.BackBufferWidth = LOWORD(lParam);
                gui::presentParameters.BackBufferHeight = HIWORD(lParam);
                gui::ResetDevice();
            }
            return 0;
        }

        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        return 0;

        case WM_LBUTTONDOWN:
        {
            gui::position = MAKEPOINTS(lParam);
        }
        return 0;

        case WM_MOUSEMOVE:
        {
            if (wParam == MK_LBUTTON)
            {
                POINTS points = MAKEPOINTS(lParam);
                RECT rect = {};

                GetWindowRect(gui::window, &rect);

                rect.left += points.x - gui::position.x;
                rect.top += points.y - gui::position.y;

                if (gui::position.x >= 0 &&
                    gui::position.x <= gui::WIDTH &&
                    gui::position.y >= 0 && gui::position.y <= 19)
                {
                    SetWindowPos(
                        gui::window,
                        HWND_TOPMOST,
                        rect.left,
                        rect.top,
                        0,
                        0,
                        SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
                }
            }
        }
        return 0;
    }

    return DefWindowProcW(window, message, wParam, lParam);
}


void gui::CreateHWindow(
	const char* windowName,
	const char* className) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);
	
	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);


	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
	
	
}



void gui::DestroyHWindow() noexcept
{
	
	DestroyWindow(window);
	UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
	
	
}


bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)
		return false;
	
	ZeroMemory(&presentParameters, sizeof(presentParameters));
	
	presentParameters.Windowed = true;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = true;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) <0 )
		return false;
	
	return true;
	
}


void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	
	const auto result = device->Reset(&presentParameters);
	
	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
	
}


void gui::DestroyDevice() noexcept
{

	if (device)
	{
		device->Release();
		device = nullptr;
	}
	
	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}


void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	
}


void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

}


void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);


	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}


void gui::EndRender() noexcept
{
	
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();

	}

	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();

}


void gui::Theme() noexcept
{
	ImGuiStyle* style = &ImGui::GetStyle();
	
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	
	style->WindowMinSize = ImVec2(WIDTH, HEIGHT);

	style->FramePadding = ImVec2(8, 6);

	
	
	style->Colors[ImGuiCol_TitleBg] = { 0.32f, 0.00f, 0.47f, 1.00f };
	style->Colors[ImGuiCol_TitleBgActive] = { 0.32f, 0.00f, 0.47f, 1.00f };
	style->Colors[ImGuiCol_TitleBgCollapsed] = { 0.00f, 0.00f, 0.00f, 1.00f };

	style->Colors[ImGuiCol_Button] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_ButtonHovered] = { 0.12f, 0.11f, 0.12f, 1.00f };
	style->Colors[ImGuiCol_ButtonActive] = { 0.71f, 0.00f, 0.29f, 1.00f };
	
	style->Colors[ImGuiCol_Separator] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_SeparatorHovered] = { 0.12f, 0.11f, 0.12f, 1.00f };
	style->Colors[ImGuiCol_SeparatorActive] = { 0.71f, 0.00f, 0.29f, 1.00f };

	style->Colors[ImGuiCol_Text] = { 1.00f, 1.00f, 1.00f, 1.00f };
	style->Colors[ImGuiCol_ChildBg] = { 0.00f, 0.00f, 0.00f, 0.00f };
	style->Colors[ImGuiCol_WindowBg] = { 0.06f, 0.06f, 0.06f, 0.94f };
	style->Colors[ImGuiCol_Border] = { 0.27f, 0.00f, 0.11f, 1.00f };
	
	style->Colors[ImGuiCol_FrameBg] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_FrameBgHovered] = { 0.12f, 0.11f, 0.12f, 1.00f };
	style->Colors[ImGuiCol_FrameBgActive] = { 0.30f, 0.00f, 0.13f, 1.00f };

	style->Colors[ImGuiCol_CheckMark] = { 0.71f, 0.00f, 0.29f, 1.00f };

	style->Colors[ImGuiCol_SliderGrab] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_SliderGrabActive] = { 0.71f, 0.00f, 0.29f, 1.00f };

	style->Colors[ImGuiCol_ScrollbarBg] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_ScrollbarGrab] = { 0.71f, 0.00f, 0.29f, 1.00f };
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.12f, 0.11f, 0.12f, 1.00f };
	style->Colors[ImGuiCol_ScrollbarGrabActive] = { 0.71f, 0.00f, 0.29f, 1.00f };

	style->Colors[ImGuiCol_Header] = { 0.30f, 0.00f, 0.13f, 1.00f };
	style->Colors[ImGuiCol_HeaderHovered] = { 0.12f, 0.11f, 0.12f, 1.00f };
	style->Colors[ImGuiCol_HeaderActive] = { 0.30f, 0.00f, 0.13f, 1.00f };
	
}

void gui::Menu() noexcept
{
	ImPlot::BeginPlot("Winning Generations");
	{

		ImPlot::PlotLine("Average", Helper::average_fitnesses, NUMBER_OF_GENERATIONS);

		#ifdef FIND_BEST
			ImPlot::PlotLine("Best", Helper::best_fitnesses, NUMBER_OF_GENERATIONS);
		#else
			ImPlot::PlotLine("Worst", Helper::worst_fitnesses, NUMBER_OF_GENERATIONS);
		#endif

	}
	ImPlot::EndPlot();

	#ifdef _PLOT_GRAPHS
			ImPlot::BeginPlot("Population");
			{
				#ifdef _GRAPH_POPULATION

					#ifdef FIND_BEST
						float population_best[NUMBER_OF_GENERATIONS];
					#else
						float population_worst[NUMBER_OF_GENERATIONS];
					#endif

					float population_average[NUMBER_OF_GENERATIONS];

					for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
						#ifdef FIND_BEST
							population_best[i] = Helper::generations[i].population.best;
						#else
							population_worst[i] = Helper::generations[i].population.worst;
						#endif

						population_average[i] = Helper::generations[i].population.average;
					}

					#ifdef FIND_BEST
						ImPlot::PlotLine("Population Best", population_best, NUMBER_OF_GENERATIONS);
					#else
						ImPlot::PlotLine("Population Worst", population_worst, NUMBER_OF_GENERATIONS);
					#endif

					ImPlot::PlotLine("Population Average", population_average, NUMBER_OF_GENERATIONS);

				#endif

				#ifdef _GRAPH_GLADIATOR

					#ifdef FIND_BEST
						float gladiator_best[NUMBER_OF_GENERATIONS];
					#else
						float gladiator_worst[NUMBER_OF_GENERATIONS];
					#endif

					float gladiator_average[NUMBER_OF_GENERATIONS];

					for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
						#ifdef FIND_BEST
							gladiator_best[i] = Helper::generations[i].gladiator.best;
						#else
							gladiator_worst[i] = Helper::generations[i].gladiator.worst;
						#endif

						gladiator_average[i] = Helper::generations[i].gladiator.average;
					
					}

					#ifdef FIND_BEST
						ImPlot::PlotLine("Gladiator Best", gladiator_best, NUMBER_OF_GENERATIONS);
					#else
						ImPlot::PlotLine("Gladiator Worst", gladiator_worst, NUMBER_OF_GENERATIONS);
					#endif

					ImPlot::PlotLine("Gladiator Average", gladiator_average, NUMBER_OF_GENERATIONS);
				
				#endif

				#ifdef _GRAPH_CROSSPOINT

					#ifdef FIND_BEST
						float crosspoint_best[NUMBER_OF_GENERATIONS];
					#else
						float crosspoint_worst[NUMBER_OF_GENERATIONS];
					#endif

					float crosspoint_average[NUMBER_OF_GENERATIONS];

					for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
						#ifdef FIND_BEST
							crosspoint_best[i] = Helper::generations[i].crosspoint.best;
						#else
							crosspoint_worst[i] = Helper::generations[i].crosspoint.worst;
						#endif

						crosspoint_average[i] = Helper::generations[i].crosspoint.average;
					}

					#ifdef FIND_BEST
						ImPlot::PlotLine("Crosspoint Best", crosspoint_best, NUMBER_OF_GENERATIONS);
					#else
						ImPlot::PlotLine("Crosspoint Worst", crosspoint_worst, NUMBER_OF_GENERATIONS);
					#endif

					ImPlot::PlotLine("Crosspoint Average", crosspoint_average, NUMBER_OF_GENERATIONS);

				#endif

				#ifdef _GRAPH_MUTATION

					#ifdef FIND_BEST
						float mutation_best[NUMBER_OF_GENERATIONS];
					#else
						float mutation_worst[NUMBER_OF_GENERATIONS];
					#endif

					float mutation_average[NUMBER_OF_GENERATIONS];

					for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
						#ifdef FIND_BEST
							mutation_best[i] = Helper::generations[i].mutation.best;
						#else
							mutation_worst[i] = Helper::generations[i].mutation.worst;
						#endif

						mutation_average[i] = Helper::generations[i].mutation.average;
					}

					#ifdef FIND_BEST
						ImPlot::PlotLine("Mutation Best", mutation_best, NUMBER_OF_GENERATIONS);
					#else
						ImPlot::PlotLine("Mutation Worst", mutation_worst, NUMBER_OF_GENERATIONS);
					#endif

					ImPlot::PlotLine("Mutation Average", mutation_average, NUMBER_OF_GENERATIONS);

				#endif

			}
			ImPlot::EndPlot();
		

	#endif

}



void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	
	ImGui::Begin(
		"AI Assignment",
		0,
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize 
		
	);
	{	
		gui::Theme();
		{
			gui::Menu();
		}
		
	}

	ImGui::End();
}


bool gui::graphing() noexcept
{
	CreateHWindow("AI Assignment", "AI Assignment Window");
	if (!CreateDevice())
	{
		DestroyDevice();
		DestroyHWindow();
		return false;
	}
	CreateImGui();
	
	while (!(GetAsyncKeyState(VK_END) & 1))
	{
		BeginRender();
		{
			Render();
		}
		EndRender();
	}
	
	DestroyImGui();
	DestroyDevice();
	DestroyHWindow();
	
	return true;
	
}