cmake_minimum_required(VERSION 3.9)
project(MyApp)

find_package(Threads REQUIRED)

# if ENV BEAUTY = TRUE
if("$ENV{BEAUTY}" STREQUAL "TRUE")
    find_package(ftxui 5 REQUIRED)
    add_executable(chatclient 
    src/ChatClientBeauty.cpp 

    # Engine
    src/engine/chat_client_engine.cpp 

    # UI
    src/ui/custom_btn.cpp 
    src/ui/custom_msg_option.cpp
    )

    target_link_libraries(chatclient
      PRIVATE ftxui::screen
      PRIVATE ftxui::dom
      PRIVATE ftxui::component
    )

else()
    add_executable(chatclient src/ChatClient.cpp src/engine/chat_client_engine.cpp)
endif()