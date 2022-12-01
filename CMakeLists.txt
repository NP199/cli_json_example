cmake_minimum_required(VERSION 3.12)

project(cli_Config_loader)

include(FetchContent)

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  fetchcontent_declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG master
  )
  fetchcontent_makeavailable(nlohmann_json)
endif()

find_package(CLI11 QUIET)
if(NOT CLI11_FOUND)
  fetchcontent_declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG main
  )
  fetchcontent_makeavailable(CLI11)
endif()

add_executable(loadConfig src/main.cpp)

target_link_libraries(
    loadConfig
    CLI11::CLI11
    nlohmann_json::nlohmann_json
    )
