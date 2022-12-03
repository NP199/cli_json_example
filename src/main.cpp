#include <iostream>
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

std::string globalname = "Hello!";

struct config{
    config(std::string loginName_temp, std::string host_temp, std::uint8_t id_temp)
    : loginName{loginName_temp}, host{host_temp}, id{id_temp}{
        std::cout << "new config constructed!\n";
    }
    config() = default;
    void print(){
        std::cout << globalname << "\n";
    }
    std::string loginName{"Name"};
    std::string host{"Host"};
    int id{}; //0-255

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(config, loginName, host, id);
};


int main(int argc, char** argv) {
    std::cout << globalname << "\n";
    CLI::App app{"CLI_minimal example"};

    std::string filepath;
    app.add_option("-r,--read", filepath,"Path to config file")
        ->required()
        ->check(CLI::ExistingFile);

    try
    {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& error) {
        std::cout << "Es ist etwas schief gelaufen!" << "\n";
        return app.exit(error);
    }

    std::cout << "Wird das auch noch ausgegeben?" << "\n";

    std::ifstream file{filepath};
    if(!file.is_open()){
        std::cout << "Error opening file!\n";
        exit(0);
    }

    nlohmann::json FileToJson;
    file >> FileToJson;

    std::cout << FileToJson.dump() << "\n";

    config myConfig{};
    config myConfig2{};
    config myConfig3{FileToJson};
    config myConfig4{"Hello","Host3",10};
    myConfig.print();
    std::string temp{FileToJson.dump()};

    std::cout << "String:" << temp << "\n";
    std::cout << "myConfig: "<< myConfig.loginName << ", " << myConfig.host <<"\n";
    std::cout << "myConfig2: "<< myConfig2.loginName << "\n";
    std::cout << "myConfig: "<< myConfig3.loginName << ", " << myConfig3.host << ", " << myConfig3.id <<"\n";
    std::cout << "myConfig: "<< myConfig4.loginName << ", " << myConfig4.host <<  ", " << myConfig4.id << "\n";

    return 0;
}
