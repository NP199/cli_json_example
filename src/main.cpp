#include <iostream>
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>

struct config{
    //Eigener Konstruktor
    config(std::string loginName_temp, std::string host_temp, int id_temp)
    : loginName{loginName_temp}, host{host_temp}, id{id_temp}{
        std::cout << "new config constructed!\n";
    }
    //Default Konstruktor
    config() = default;
    std::string loginName{"Name"};
    std::string host{"Host"};
    int id{};
    //Nlohmann json mein config Datentypen mitteilen
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(config, loginName, host, id);
};

class accescontrol{
private:
    std::vector<config> allowedUser;
public:
    void addAllowedUser(config user_){
        std::cout << "added user:"  << user_.loginName
                                    << ", "
                                    << user_.host
                                    << ", "
                                    << user_.id
                                    << "\n";
        allowedUser.push_back(user_);
    }

    void showAllowedUser(){
        for(config & user_ : allowedUser){
            std::cout   << user_.loginName
                        << ", "
                        << user_.host
                        << ", "
                        << user_.id
                        << "\n";
        }
    }

    void isUserAllowed(config const& userToCheck){
    int counter = 0;
    for(config const& user_ : allowedUser){
                    counter++;
                    if(     user_.loginName == userToCheck.loginName
                         && user_.host == userToCheck.host
                         && user_.id == userToCheck.id){
                        std::cout << user_.loginName << ": acces granted!\n";
                        break;
                    }else if(counter == allowedUser.size()){
                        std::cout << user_.loginName <<": no acces rights!\n";
                        std::exit(1);
                    }
                }
    }

    void isUserAllowed(std::string const& loginName_, std::string const& host_, int const& id_){
    int counter = 0;
    for(config const& user_ : allowedUser){
                    counter++;
                    if(     user_.loginName == loginName_
                         && user_.host == host_
                         && user_.id == id_){
                        std::cout << loginName_ << ", " << host_ <<", " << id_ << ": acces granted!\n";
                        break;
                    }else if(counter == allowedUser.size()){
                        std::cout << loginName_ << ", " << host_ <<", " << id_ << ": no acces rights!\n";
                        std::exit(1);
                    }
                }
    }

    void saveAllowedUser(){
        std::ofstream saveFile{"alloweduser.json"};
        if(!saveFile.is_open()){
            std::cerr << "Error opening file!\n";
            std::exit(1);
        }
        nlohmann::json json2file{};
        for(config const& user_ : allowedUser){
            json2file.push_back(user_);
        }
            saveFile << json2file;
    }

    void loadFromFile(std::string const& filepath_){
        std::ifstream file{filepath_};
        if(!file.is_open()){
            std::cerr << "Error opening file!\n";
            std::exit(1);
        }
        nlohmann::json FileToJson{};
        file >> FileToJson;
        for(nlohmann::json& json_element : FileToJson){
            std::cout << "from file:" << json_element << "\n";
            allowedUser.push_back(config{json_element.get<config>()});
            //config myConfig{ FileToJson.get<config>()};
        }

    }
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
    /*
    config myConfig{};
    std::cout << myConfig.loginName << ", " << myConfig.host << ", " << myConfig.id << "\n";
    */
    /*
    file >> FileToJson;
    config myConfig{ FileToJson.get<config>()};
    std::cout << myConfig.loginName << ", " << myConfig.host << ", " << myConfig.id << "\n";
    std::cout << FileToJson.dump() << "\n";
*/
    accescontrol myAccescontrol;

    config user2{"Heinrich", "AwesomeHost", 2};
    config user3{"Sabine", "AwesomeHost", 3};
    config user4{"Gabi", "AwesomeHost", 4};
    config user_no_acces1{"Lisa", "AwesomeHost", 8};
    config user_no_acces2{"Björn", "AwesomeHost", 9};
    config user_no_acces3{"Sina", "AwesomeHost", 10};
    myAccescontrol.addAllowedUser(user2);
    myAccescontrol.addAllowedUser(user3);
    myAccescontrol.addAllowedUser(user4);

    myAccescontrol.showAllowedUser();
    std::cout << "\n";
    myAccescontrol.loadFromFile(filepath);
    std::cout << "\n";
    myAccescontrol.showAllowedUser();
    std::cout << "\n";
    myAccescontrol.isUserAllowed(user2);
    myAccescontrol.isUserAllowed(user3);
    myAccescontrol.isUserAllowed("Sabine", "AwesomeHost", 3);
    myAccescontrol.isUserAllowed("Björn", "AwesomeHost", 9);
    //myAccescontrol.isUserAllowed(user_no_acces1);
    std::cout << "\n";
    myAccescontrol.saveAllowedUser();
    std::cout << "\n";


    return 0;
}
