#include <iostream>
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>


struct config{
    config(std::string loginName_temp, std::string host_temp, std::uint8_t id_temp)
    : loginName{loginName_temp}, host{host_temp}, id{id_temp}{
        std::cout << "new config constructed!\n";
    }
    config() = default;
    std::string loginName{"Name"};
    std::string host{"Host"};
    int id{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(config, loginName, host, id);
};


int main(int argc, char** argv) {
    CLI::App app{"CLI_minimal example"};

    std::string filepath;
    app.add_option("-r,--read", filepath,"Path to config file")
        ->required()
        ->check(CLI::ExistingFile);


    try
    {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        return app.exit(e);
    }

    std::ifstream file{filepath};
    if(!file.is_open()){
        std::cerr << "Error opening file!\n";
        std::exit(1);
    }

    nlohmann::json FileToJson{};
    config myConfig{};

    file >> FileToJson;
    myConfig = FileToJson.get<config>();
    std::cout << myConfig.loginName << ", " << myConfig.host << ", " << myConfig.id << "\n";
    std::cout << FileToJson.dump() << "\n";

    return 0;
}
