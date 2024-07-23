add_library(ImGui STATIC imconfig.h imgui.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h misc/cpp/imgui_stdlib.h
                         imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp imgui.cpp misc/cpp/imgui_stdlib.cpp)
set_target_properties(ImGui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Aluminium-Client/lib/ImGui/build/linux-x86_64-${CMAKE_BUILD_TYPE}")

target_include_directories(ImGui PRIVATE .)
