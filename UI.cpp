//
// Created by xAbdoMo on 4/14/2024.
//

#include <imgui.h>
#include "UI.h"
#include "Chat.h"
#include <iostream>
#include <fstream>

int hidden_parameters = 0;

SigType Q1 , a1;
SigType Q2 , a2;

SigType Yb1;
SigType Yb2;

//some string operations
static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

ImVector<const char*> chat;
bool AutoScroll = true;
char InputBuf[1024];

//add to the "log" (chat)
static void AddLog(const char* fmt, ...)
{
    char buf[4024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    chat.push_back(Strdup(buf));
}


//int arr[PRIMES_LIMIT] = { 0 };

//current font
ImFont* fnt;

void UI::InitUI(){
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig cfg;
    cfg.OversampleH = 1;
    cfg.OversampleV = 1;
    cfg.RasterizerMultiply = 1.5f;

    fnt = io.Fonts->AddFontFromFileTTF("res\\JetBrainsMono-Medium.ttf", 18.0f , &cfg);

    std::ifstream keys;
    keys.open("public.txt");
    if (!keys.is_open()){
        std::cout << "failed to open file" << std::endl;
        return;
    }

    keys >> Q1 >> a1;
    keys >> Q2 >> a2;

    Chat::init(Q1 , a1 , Q2 , a2);
}

void UI::DrawUI(bool& exit){
    ImGui::PushFont(fnt);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (!Chat::isConnecting() && !Chat::isConnected()) {
                if (ImGui::MenuItem("Start server")) {
                    Chat::startConnection(true);
                }
                if (ImGui::MenuItem("Start client")) {
                    Chat::startConnection(false);
                }
            }
            else {
                if (ImGui::MenuItem("Disconnect")) {
                    Chat::disconnect();
                }
            }

            if (hidden_parameters){
                if (ImGui::MenuItem("Hide Extra Parameters")) {
                    hidden_parameters ^= 1;
                }
            }else{
                if (ImGui::MenuItem("Show Extra Parameters")) {
                    hidden_parameters ^= 1;
                }
            }

            if (ImGui::MenuItem("Exit")) {
                exit = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    static int window_flags = 0;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBackground;

    static bool p_open = true;
    ImGui::Begin("Here we go ", &p_open, window_flags);
    ImGui::PopStyleVar(2);

    { // widgets before the chat area
        ImGui::Text("Statues : ");
        ImGui::SameLine();

        if (Chat::isConnected()){
            ImGui::TextColored({ 0,1,0,1 }, "Connected");
        }else if (Chat::isConnecting()){
            ImGui::TextColored({ 0,1,1,1 }, "Connecting..");
        }else{
            ImGui::TextColored({ 1,0,0,1 }, "Disconnected");
        }

        if (Chat::isConnected()) {
            ImGui::Text("External Parameters: ");
            ImGui::TextColored({0, 0.5, 1, 1}, "Ya|ELGamal : ");
            ImGui::SameLine();
            ImGui::Text(Chat::getEternalElGamalYa().toString(16).c_str());
            ImGui::SameLine(350);
            ImGui::TextColored({0, 0.5, 1, 1}, "Ya|DH : ");
            ImGui::SameLine();
            ImGui::Text(Chat::getExternalDHYa().toString(16).c_str());
        }

        ImGui::SeparatorText("Elgamal Parameters: ");
        ImGui::TextColored({1,1,0,1} , "Ya : ");
        ImGui::SameLine();
        ImGui::Text(Chat::getSig().getPublicKey().toString(16).c_str());
        ImGui::SameLine(300);
        ImGui::TextColored({1,0,0,1} , "Xa : ");
        ImGui::SameLine();
        ImGui::Text(Chat::getSig().getPrivateKey().toString(16).c_str());

        if (hidden_parameters){
            ImGui::TextColored({1,1,0,1} , "K  : ");
            ImGui::SameLine();
            ImGui::Text(Chat::getSig().getRandomK().toString(16).c_str());
            ImGui::SameLine(300);
            ImGui::TextColored({1,0,0,1} , "K-1: ");
            ImGui::SameLine();
            ImGui::Text(Chat::getSig().getRandomKInv().toString(16).c_str());
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20.0f, 0.0f));
        ImGui::Spacing();
        ImGui::SameLine();
        ImGui::Text("DH Parameters: ");
        ImGui::PopStyleVar();
        ImGui::TextColored({1,1,0,1} , "Ya : ");
        ImGui::SameLine();
        ImGui::Text(Chat::getDaffieHellman().first.toString(16).c_str());
        ImGui::SameLine(300);
        ImGui::TextColored({1,0,0,1} , "Xa : ");
        ImGui::SameLine();
        ImGui::Text(Chat::getDaffieHellman().second.toString(16).c_str());

    }

    { // chat area

        // Reserve enough left-over height for 1 separator + 1 input text
        ImGui::SeparatorText("Chat");

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) {
                    chat.clear();
                }
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            for (int i = 0; i < chat.Size; i++)
            {
                const char* item = chat[i];

                ImVec4 color = {1,1,1,1};
                if (item[0] == '0') {
                    color = { 1,0,0,1 }; //error
                }

                if (item[0] == '1') {
                    color = { 1,0,1,1 }; //out message
                }

                if (item[0] == '2') {
                    color = { 1,1,1,1 }; //normal message
                }

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item + 1);
                ImGui::PopStyleColor();
            }

            if ((AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();
    }

    { //input area
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags , nullptr , nullptr))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0]) {
                Chat::send(s);
                AddLog("2%s", s);
            }
            strcpy_s(InputBuf, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    }


    ImGui::End();

    ImGui::PopFont();

    //demo saves lifes
    //ImGui::ShowDemoWindow();
}

void UI::Terminate(){
    Chat::disconnect();
    Chat::waitForChat(); //wait until join so libStd don't fk us up
}

void UI::message(const char *ptr) {
    AddLog("1%s", ptr);
}

void UI::info(const char *ptr) {
    AddLog("2%s", ptr);
}

void UI::error(const char *ptr) {
    AddLog("0%s", ptr);
}
