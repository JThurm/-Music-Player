#include <iostream>

#include <GLFW/glfw3.h>
#include <string>
#include <array>
#include <cstddef>
#include <iomanip>
#include <sstream>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

struct Album
{
    std::string title;
    std::string artist;
};

Album albums[] = {
          {"Midnights", "Taylor Swift"},
          {"Red", "Taylor Swift"},
          {"Folklore", "Taylor Swift"},
          {"Reputation", "Taylor Swift"},
          {"Evermore", "Taylor Swift"}
};
struct Song
{
    std::string title;
    std::string artist;
    float durationSeconds;
};

Song songs[] = {
    { "Wonderwall", "Oasis",             258.0f }, // 4:18
    { "1979",       "Smashing Pumpkins", 247.0f }, // 4:07
    { "My Hero",    "Foo Fighters",      260.0f }  // 4:20
};

void DrawAlbumCard(const Album& album)
{
    ImGui::BeginGroup();
    ImGui::Button("ART", ImVec2(120, 120)); // album cover placeholder
    ImGui::Text("%s", album.title.c_str());
    ImGui::Text("%s", album.artist.c_str());
    ImGui::EndGroup();
}

std::string FormatDuration (float seconds){
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds/60;
    int remainingSeconds = totalSeconds%60;

    std::ostringstream oss;
    oss << minutes << ":" << std::setw(2) << std::setfill('0') << remainingSeconds;
    return oss.str();
}


int main()
{

    // state for the player

    bool isPlaying = false;

    int selectedSongIndex = -1;
    // --------------------------------------------------------
    // Initialize GLFW
    // --------------------------------------------------------

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return 1;
    }


    // --------------------------------------------------------
    // Configure OpenGL
    // --------------------------------------------------------

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // --------------------------------------------------------
    // Create Window
    // --------------------------------------------------------

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Pulse",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;

        glfwTerminate();

        return 1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);


    // --------------------------------------------------------
    // Initialize Dear ImGui
    // --------------------------------------------------------

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    (void)io;


    // --------------------------------------------------------
    // Initialize ImGui Backends
    // --------------------------------------------------------

    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true
    );

    ImGui_ImplOpenGL3_Init(
        "#version 330"
    );


    // --------------------------------------------------------
    // Main Loop
    // --------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        // Start ImGui frame

        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // ----------------------------------------------------
        // Sidebar
        // ----------------------------------------------------

        // removing gui decos
        ImGuiWindowFlags flags = 
                            ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;

        // setting the size of the sidebar
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2( 250, io.DisplaySize.y)); // sidebar width
        

        ImGui::Begin("Sidebar", nullptr, flags);
        ImGui::Text("Pulse");

        ImGui::Separator();
        ImGui::Button("Home");
        ImGui::Button("Library");

        ImGui::Spacing();
        
        ImGui::Text("Playlists");

        ImGui::Button("+ New Playlists");
        ImGui::End();


        // ----------------------------------------------------
        // Library
        // ----------------------------------------------------

        float sidebarWidth = 250.0f;
        float playerBarHeight = 90.0f;
        float libraryHeight = io.DisplaySize.y - playerBarHeight;

        ImGui::SetNextWindowPos(ImVec2(sidebarWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - sidebarWidth, libraryHeight)); // library width and height

        ImGui::Begin("Library", nullptr, flags);

       // Album cards

        ImGui::Text("Library");
        ImGui::Separator();
        ImGui::Text("Recently Added");
        ImGui::Separator();

        for(int i = 0; i < static_cast<int>(std::size(albums)); i++){
            ImGui::PushID(i);
            DrawAlbumCard(albums[i]);
            ImGui::PopID();
            
            if(i < static_cast<int>(std::size(albums))-1)
                ImGui::SameLine();
        }

        // song list
        ImGui::Separator();
        ImGui::Text("Songs");
        ImGui::Separator();

        if (ImGui::BeginTable("SongTable", 3))
        {
            ImGui::TableSetupColumn("Title");
            ImGui::TableSetupColumn("Artist");
            ImGui::TableSetupColumn("Time");
            ImGui::TableHeadersRow();

            for (int i = 0; i < static_cast<int>(std::size(songs)); i++)
            {
                const Song& song = songs[i];
                bool isSelected = (selectedSongIndex == i);

                ImGui::TableNextRow();

                // Column 1: Title (selectable)
                ImGui::TableNextColumn();
                if (ImGui::Selectable(song.title.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
                {
                    selectedSongIndex = i;
                }

                // Column 2: Artist
                ImGui::TableNextColumn();
                ImGui::Text("%s", song.artist.c_str());

                // Column 3: Time
                ImGui::TableNextColumn();
                std::string durationText = FormatDuration(song.durationSeconds);
                ImGui::Text("%s", durationText.c_str());
            }

            ImGui::EndTable();
        }

        if (selectedSongIndex >= 0)
        {
            const Song& song = songs[selectedSongIndex];
            std::string nowPlaying = song.title + " - " + song.artist;
            ImGui::Text("%s", nowPlaying.c_str());
        }
        else
        {
            ImGui::Text("No song selected");
        }


        ImGui::End();

        // player bar

        // position at the bottom of the screen
        ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - playerBarHeight));
        // full width fixed height
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, playerBarHeight));


        ImGui::Begin("PlayerBar", nullptr, flags);

        //player controls

        const char* playLabel = isPlaying ? "||" : "|>";
        ImGui::Button("<<");
        ImGui::SameLine();
        if (ImGui::Button(playLabel)) isPlaying = !isPlaying;
        ImGui::SameLine();
        ImGui::Button(">>");
        ImGui::SameLine();

        if (selectedSongIndex >= 0)
        {
            const Song& song = songs[selectedSongIndex];

            std::string nowPlaying = song.title + " - " + song.artist;
            ImGui::Text("%s", nowPlaying.c_str());
        }
        else
        {
            ImGui::Text("No song selected");
        }


        // later updatte the progress bar weith timestamps

        ImGui::End();

        // ----------------------------------------------------
        // Render
        // ----------------------------------------------------

        ImGui::Render();

        int displayWidth;
        int displayHeight;

        glfwGetFramebufferSize(
            window,
            &displayWidth,
            &displayHeight
        );

        glViewport(
            0,
            0,
            displayWidth,
            displayHeight
        );

        glClearColor(
            0.08f,
            0.08f,
            0.10f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT
        );

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }


    // --------------------------------------------------------
    // Cleanup
    // --------------------------------------------------------

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();


    glfwDestroyWindow(window);

    glfwTerminate();


    return 0;
}