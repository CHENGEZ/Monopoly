cmake_minimum_required(VERSION 3.5)

project(Monopoly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(sockpp/include)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Multimedia REQUIRED)

set(PROJECT_SOURCES
        # Main for Client package
        src/client/main.cpp

        # UI Related
        src/client/QtWindows/LoginWindow.h src/client/QtWindows/LoginWindow.cpp src/client/QtWindows/LoginWindow.ui
        src/client/QtWindows/MainWindow.h src/client/QtWindows/MainWindow.cpp src/client/QtWindows/MainWindow.ui
        src/client/QtWindows/PopupWindow.h src/client/QtWindows/PopupWindow.cpp src/client/QtWindows/PopupWindow.ui
        src/client/QtWindows/LobbyWindow.h src/client/QtWindows/LobbyWindow.cpp src/client/QtWindows/LobbyWindow.ui
        src/client/QtWindows/sourceMainWindow.qrc

        # Client server related files
        src/client/ClientCommunicationToolbox/ClientCommunicationToolbox.cpp src/client/ClientCommunicationToolbox/ClientCommunicationToolbox.h
        src/client/ClientCommunicationToolbox/ResponseListenerThread.cpp src/client/ClientCommunicationToolbox/ResponseListenerThread.h
        src/client/ClientCommunicationToolbox/ClientNetworkManager.h
        src/client/ClientCommunicationToolbox/ClientNetworkManager.cpp
        src/client/HelperFunctions.cpp

        # Client Common related file
        src/common/Cell/Avenue.cpp src/common/Cell/Avenue.hpp
        src/common/Cell/Cell.cpp src/common/Cell/Cell.hpp
        src/common/Cell/EventTriggerer.cpp src/common/Cell/EventTriggerer.hpp
        src/common/Cell/Go2Jail.cpp src/common/Cell/Go2Jail.hpp
        src/common/Cell/GoAndFreeParking.cpp src/common/Cell/GoAndFreeParking.hpp
        src/common/Cell/Jail.cpp src/common/Cell/Jail.hpp
        src/common/Cell/Railway.cpp src/common/Cell/Railway.hpp
        src/common/Cell/Tax.cpp src/common/Cell/Tax.hpp
        src/common/Cell/Utility.cpp src/common/Cell/Utility.hpp
        src/common/exceptions/MonopolyException.hpp
        src/common/game_state/events/Event.h src/common/game_state/events/Event.cpp
        src/common/game_state/player/Player.cpp src/common/game_state/player/Player.hpp
        src/common/game_state/EventExecutor.cpp src/common/game_state/EventExecutor.h
        src/common/game_state/Game.h src/common/game_state/Game.cpp

        # Network assistant part

        #Requests
        src/common/network/requests/ClientRequest.h src/common/network/requests/ClientRequest.cpp
        src/common/network/requests/RollDiceRequest.h src/common/network/requests/RollDiceRequest.cpp
        src/common/network/requests/BuyBuildingRequest.h src/common/network/requests/BuyBuildingRequest.cpp
        src/common/network/requests/BuyPropertyRequest.h src/common/network/requests/BuyPropertyRequest.cpp
        src/common/network/requests/EndTurnRequest.h src/common/network/requests/EndTurnRequest.cpp
        src/common/network/requests/SellBuildingRequest.h src/common/network/requests/SellBuildingRequest.cpp
        src/common/network/requests/SellPropertyRequest.h src/common/network/requests/SellPropertyRequest.cpp
        src/common/network/requests/JoinLobbyRequest.h src/common/network/requests/JoinLobbyRequest.cpp
        src/common/network/requests/StartGameRequest.h src/common/network/requests/StartGameRequest.cpp
        src/common/network/requests/UseGetOutOfJailFreeRequest.h src/common/network/requests/UseGetOutOfJailFreeRequest.cpp
        # Responses
        src/common/network/responses/ServerResponse.h src/common/network/responses/ServerResponse.cpp
        src/common/network/responses/FullStateResponse.h src/common/network/responses/FullStateResponse.cpp


        src/client/QtWindows/ActingWindow.h src/client/ClientCommunicationToolbox/ConnectionThread.h
        src/client/ClientCommunicationToolbox/ConnectionThread.cpp

        src/common/network/responses/RequestResponse.h src/common/network/responses/RequestResponse.cpp

        # Serialisation
        src/common/serialization/json_utils.h
        src/common/serialization/serializable.h
        src/common/serialization/serializable_value.h
        src/common/serialization/unique_serializable.h src/common/serialization/unique_serializable.cpp
        src/common/serialization/uuid_generator.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h)


set(SERVER_SOURCE_FILES
        # Server Common related file
        src/common/Cell/Avenue.cpp src/common/Cell/Avenue.hpp
        src/common/Cell/Cell.cpp src/common/Cell/Cell.hpp
        src/common/Cell/EventTriggerer.cpp src/common/Cell/EventTriggerer.hpp
        src/common/Cell/Go2Jail.cpp src/common/Cell/Go2Jail.hpp
        src/common/Cell/GoAndFreeParking.cpp src/common/Cell/GoAndFreeParking.hpp
        src/common/Cell/Jail.cpp src/common/Cell/Jail.hpp
        src/common/Cell/Railway.cpp src/common/Cell/Railway.hpp
        src/common/Cell/Tax.cpp src/common/Cell/Tax.hpp
        src/common/Cell/Utility.cpp src/common/Cell/Utility.hpp
        src/common/exceptions/MonopolyException.hpp
        src/common/game_state/events/Event.h src/common/game_state/events/Event.cpp
        src/common/game_state/player/Player.cpp src/common/game_state/player/Player.hpp
        src/common/game_state/EventExecutor.cpp src/common/game_state/EventExecutor.h
        src/common/game_state/Game.h src/common/game_state/Game.cpp

        # Server Files
        src/server/GameInstance.h src/server/GameInstance.cpp
        src/server/GameInstanceManager.h src/server/GameInstanceManager.cpp
        src/server/main.cpp
        src/server/PlayerManager.h src/server/PlayerManager.cpp
        src/server/RequestHandler.h src/server/RequestHandler.cpp
        src/server/ServerNetworkManager.h src/server/ServerNetworkManager.cpp

        # Network assistant part

        # Requests
        src/common/network/requests/ClientRequest.h src/common/network/requests/ClientRequest.cpp
        src/common/network/requests/RollDiceRequest.h src/common/network/requests/RollDiceRequest.cpp
        src/common/network/requests/BuyBuildingRequest.h src/common/network/requests/BuyBuildingRequest.cpp
        src/common/network/requests/BuyPropertyRequest.h src/common/network/requests/BuyPropertyRequest.cpp
        src/common/network/requests/EndTurnRequest.h src/common/network/requests/EndTurnRequest.cpp
        src/common/network/requests/SellBuildingRequest.h src/common/network/requests/SellBuildingRequest.cpp
        src/common/network/requests/SellPropertyRequest.h src/common/network/requests/SellPropertyRequest.cpp
        src/common/network/requests/JoinLobbyRequest.h src/common/network/requests/JoinLobbyRequest.cpp
        src/common/network/requests/StartGameRequest.h src/common/network/requests/StartGameRequest.cpp
        src/common/network/requests/UseGetOutOfJailFreeRequest.h src/common/network/requests/UseGetOutOfJailFreeRequest.cpp
        # Responses
        src/common/network/responses/ServerResponse.h src/common/network/responses/ServerResponse.cpp
        src/common/network/responses/FullStateResponse.h src/common/network/responses/FullStateResponse.cpp
        src/common/network/responses/RequestResponse.h src/common/network/responses/RequestResponse.cpp

        # Serialisation
        src/common/serialization/json_utils.h
        src/common/serialization/serializable.h
        src/common/serialization/serializable_value.h
        src/common/serialization/unique_serializable.h src/common/serialization/unique_serializable.cpp
        src/common/serialization/uuid_generator.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h


        )


# The below is for Qt compile we can ignore it for now
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Monopoly
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET Monopoly APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Monopoly SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Monopoly
                ${PROJECT_SOURCES}
                )
    endif()
endif()

#target_link_libraries(Monopoly PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Monopoly PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS Monopoly
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Monopoly)
endif()

# Additional part for our client compile (mostly setting env var.)
# set compile directives for client-executable
target_compile_definitions(Monopoly PRIVATE MONOPOLY_CLIENT=1 RAPIDJSON_HAS_STDSTRING=1)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Monopoly PRIVATE PRINT_NETWORK_MESSAGES=1)


# This is part for server compile

# set source files for server-executable
add_executable(Monopoly-server ${SERVER_SOURCE_FILES})
# set compile directives for server-executable
target_compile_definitions(Monopoly-server PRIVATE MONOPOLY_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Monopoly-server PRIVATE PRINT_NETWORK_MESSAGES=1)


# linking to sockpp
if(WIN32)
    message("Detected Win32")

    target_link_libraries(Monopoly ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
    target_link_libraries(Monopoly-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)

    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Monopoly wsock32 ws2_32)
else()
    if (APPLE)
        message("Detected Apple device")
        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)

        target_link_libraries(Monopoly PUBLIC ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.0.dylib Threads::Threads Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
        target_link_libraries(Monopoly-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.0.dylib Threads::Threads)
    else()
        message("Not Win32, not Apple, so probably a Linux") # We assume it's Linux in this case

        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)

        target_link_libraries(Monopoly PUBLIC ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.so Threads::Threads PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
        target_link_libraries(Monopoly-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.so Threads::Threads)
    endif ()
endif()


set(CMAKE_CXX_FLAGS "--coverage")
# set source files for server-library
add_library(Monopoly-lib ${SERVER_SOURCE_FILES})
# set compile directives for server-library
target_compile_definitions(Monopoly-lib PRIVATE MONOPOLY_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

add_subdirectory(googletest)
add_subdirectory(unit-tests)

