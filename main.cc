#include <drogon/drogon.h>
int main() {
    std::cout << "Starting server..." << std::endl;
    drogon::app().setDocumentRoot("public");
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 8800);
    drogon::app().setLogLevel(trantor::Logger::kDebug);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    std::cout << "Server listening on port 8800" << std::endl;
    drogon::app().run();
    return 0;
}
