// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include"ProductManager.h"
#include<set>
#include <chrono>
#include "Personalnfo.h"
#include "VIP.h"
#include "ProcessPersonalInfo.h"
#include "Deliver.h"
#include <fstream>
// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool showMenu = false;

void RenderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                // Handle the "Open" action
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                // Handle the "Save" action
            }
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // Handle the "Exit" action
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                // Handle the "Undo" action
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                // Handle the "Redo" action
            }
            ImGui::EndMenu();
        }
        // Add more menu items as needed
        ImGui::EndMainMenuBar();
    }
}



// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Laboratorul 3", WS_OVERLAPPEDWINDOW, 0, 0, screenWidth, screenHeight, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    io.Fonts->AddFontDefault();
    ImFont* fallbackFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\Mihai\\Desktop\\ImGui_project\\Fonts\\MotleyForcesRegular-w1rZ3.ttf", 15.5f);
    IM_ASSERT(fallbackFont != NULL);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();

    
    
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    Bogo bogo1("Chips", "Apa");
    Bogo bogo2("Caiete", "Pixuri");
    Bogo bogo3("Nutella", "Biscuiti");
    Bogo bogo4("Suc de portocale", "Paine integrala");
    Bogo bogo5("Cafea 'Nescafe'", "Ulei de masline");

    std::vector<std::string> b1 = { "Detergent de vase","Pasta de dinti" };
    std::vector<std::string> b2 = { "Sampon pentru par","Sampon pentru dus"};
    std::vector<std::string> b3 = { "Banane","Pere" };
    std::vector<std::string> b4 = { "Sare","Piper" };
    std::vector<std::string> b5 = { "Orez","Cartofi"};

    Bundle BundleInfo1(b1, 51.99f);
    Bundle BundleInfo2(b2, 167.99f);
    Bundle BundleInfo3(b3, 49.99f);
    Bundle BundleInfo4(b4, 13.99f);
    Bundle BundleInfo5(b5, 30.99f);


    ProductManager store;
    store.AddProduct("Piine", Product(15, 11.50f, 10.0f, false));
    store.AddProduct("Lapte", Product(20, 15.99f, 12.0f, false));
    store.AddProduct("Snickers", Product(12, 21.00f, 25.0f, false));
    store.AddProduct("Fanta", Product(50, 17.55f, 15.0f, false));
    store.AddProduct("Cola", Product(60, 21.00f, 20.0f, false));
    store.AddProduct("Chips", Product(88, 40.00f, 0.0f, false));
    store.AddProduct("Caiete", Product(10, 5.50f, 0.0f, false));
    store.AddProduct("Pixuri", Product(12, 6.00f, 0.0f, false));
    store.AddProduct("Salam", Product(7, 69.80f, 0.0f, false));
    store.AddProduct("Cafea 'Nescafe'", Product(6, 110.0f, 0.0f, false));
    store.AddProduct("Nutella", Product(30, 89.99f, 0.0f, false));
    store.AddProduct("Apa", Product(40, 13.50f, 0.0f, false));
    store.AddProduct("Paine integrala", Product(20, 12.75f, 0.0f, false));
    store.AddProduct("Suc de portocale", Product(25, 31.99f, 0.0f, false));
    store.AddProduct("Biscuiti", Product(50, 25.25f, 0.0f, false));
    store.AddProduct("Ulei de masline", Product(15, 40.50f, 0.0f, false));
    store.AddProduct("Detergent de vase", Product(10, 33.99f, 0.0f, false));
    store.AddProduct("Sampon pentru dus", Product(30, 120.00f, 0.0f, false));
    store.AddProduct("Hartie igienica", Product(35, 7.75f, 0.0f, false));
    store.AddProduct("Sampon pentru par", Product(18, 68.50f, 0.0f, false));
    store.AddProduct("Pasta de dinti", Product(25, 21.99f, 0.0f, false));
    store.AddProduct("Pere", Product(40, 32.75f, 0.0f, false));
    store.AddProduct("Banane", Product(60, 21.99f, 0.0f, false));
    store.AddProduct("Cascaval", Product(22, 55.75f, 0.0f, false));
    store.AddProduct("Sare", Product(15, 11.25f, 0.0f, false));
    store.AddProduct("Piper", Product(10, 7.50f, 0.0f, false));
    store.AddProduct("Sos de rosii", Product(28, 25.99f, 0.0f, false));
    store.AddProduct("Cartofi", Product(50, 12.25f, 0.0f, false));
    store.AddProduct("Orez", Product(38, 16.50f, 0.0f, false));
    store.AddProduct("Hartie de copt", Product(20, 28.25f, 0.0f, false));

    // noi produse
    store.AddProduct("Laptop", Product(5, 12000.0f, 0.0f, true));
    store.AddProduct("Telefon mobil", Product(10, 3500.0f, 0.0f, true));
    store.AddProduct("Televizor", Product(8, 2500.0f, 0.0f, true));
    store.AddProduct("Tableta", Product(15, 4000.0f, 0.0f, true));
    store.AddProduct("Casti wireless", Product(20, 458.0f, 0.0f, true));
    store.AddProduct("Mouse", Product(30, 220.0f, 0.0f, true));
    store.AddProduct("Tastatura", Product(25, 320.0f, 0.0f, true));
    store.AddProduct("Monitor", Product(10, 440.0f, 0.0f, true));
    store.AddProduct("Camera web", Product(12, 250.0f, 0.0f, true));
    store.AddProduct("Router wireless", Product(5, 550.0f, 0.0f, true));
    store.AddProduct("Ceas de mana Rolex Submariner", Product(2, 500000.0f, 0.0f, true));
    store.AddProduct("Iphone 14 Pro Max",Product(5,250000.0f,0.0f,true));
    store.AddProduct("Samsung S23 Ultra", Product(5, 50000.0f, 0.0f, true));

    store.add_bogo_products("Chips + Apa", bogo1);
    store.add_bogo_products("Caiete + Pixuri", bogo2);
    store.add_bogo_products("Nutella + Biscuiti", bogo3);
    store.add_bogo_products("Suc de portocale + Paine integrala", bogo4);
    store.add_bogo_products("Cafea 'Nescafe' + Ulei de masline", bogo5);

    store.add_bundle_product("Detergent de vase + Pasta de dinti", BundleInfo1);
    store.add_bundle_product("Sampon pentru par + Sampon pentru dus", BundleInfo2);
    store.add_bundle_product("Banane + Pere", BundleInfo3);
    store.add_bundle_product("Sare + Piper", BundleInfo4);
    store.add_bundle_product("Orez + Cartofi", BundleInfo5);

    //std::string code = "I Love Metro";
    Vouchere voucher1("Cascaval", "i love cheese", 13.25f,false);
    Vouchere voucher2("Hartie de copt", "i use baking paper", 5.50f,false);
    Vouchere voucher3("Hartie igienica", "i use toilet paper", 3.55f,false);
    Vouchere voucher4("Sos de rosii", "i love tomato souce", 21.20f,false);
    Vouchere voucher5("Salam", "i love saussage", 31.99f,false);

    store.add_Voucheres("Cascaval", voucher1);
    store.add_Voucheres("Hartie de copt", voucher2);
    store.add_Voucheres("Hartie igienica", voucher3);
    store.add_Voucheres("Sos de rosii", voucher4);
    store.add_Voucheres("Salam", voucher5);
    PersonalInfo personal;
    GoldVIP gold;
    SilverVIP silver;
    PlatinumVIP platinum;
    VIPS vips;
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!)
        ImGui::PushFont(fallbackFont);
        store.display_products(personal,store.total);
        store.display_bundle_products();
        store.display_bogo_products();
        store.display_cart(vips, personal, gold, silver, platinum);




        DeliverService<bool> user_street;
        DeliverService<bool> user_street_nr;
        store.deliver_window(user_street,user_street_nr);
        if (user_street.user_data && user_street_nr.user_data) {
            store.can_submit = true;
        }
        ImGui::PopFont();
        
        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)   
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
//class VIPS :public GoldVIP ,public SilverVIP,public PlatinumVIP {
//public:
//    void add_vip_clients(std::string& idnp, Invitations& invite) override {
//        VIP[idnp] = invite;
//    }
//
//    virtual void btn_invitation() {
//        float centerPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Invitatii").x) * 0.5f;
//        ImGui::SetCursorPosX(centerPosX);
//        if (ImGui::Button("Invitatii")) {
//            std::string desktopPath = get_desktop_path();
//
//            if (!desktopPath.empty()) {
//                std::string folderPath = desktopPath;
//                system(("start explorer.exe \"" + folderPath + "\"").c_str());
//                showFileContent = true;
//
//            }
//        }
//
//    }
//
//    virtual void open_files() {
//        if (fileIconClicked) {
//            std::string desktopPath = get_desktop_path();
//            if (!desktopPath.empty()) {
//                std::string folderPath = desktopPath;
//                std::string command = "start explorer.exe \"" + folderPath + "\"";
//                system(command.c_str());
//            }
//            fileIconClicked = false;
//        }
//
//    }
//
//
//    virtual void show_file() {
//        if (showFileContent) {
//            if (ImGui::Begin("Invitations Display", &showFileContent)) {
//                ImGui::Text("Invitations Content:");
//                ImGui::TextWrapped("%s", fileContent.c_str());
//
//                if (ImGui::Button("Open Folder")) {
//                    fileIconClicked = true;
//                }
//
//                if (ImGui::Button("Close Invitation")) {
//                    showFileContent = false;
//                    fileContent.clear();
//                }
//
//                ImGui::End();
//            }
//        }
//    }
//};