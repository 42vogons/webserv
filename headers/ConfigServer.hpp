#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <string>
#include <map>
#include <set>
#include <LocationConfigServer.hpp>

class ConfigServer {
    public :
        ConfigServer (void);
        ConfigServer (std::string fileName);
        ConfigServer (const ConfigServer &obj);
        ~ConfigServer (void);

        ConfigServer& operator=(const ConfigServer& obj);
        
        // Setters
        void setServeName(std::string serverName);
        void setErrorPages(int code, std::string page);
        void setPorts(int port);
        void setClientMaxBodySize(int clientMaxBodySize);

        // Getters
        std::string                 getServerName(void);
        std::map<int, std::string>  getErrorPorts(void);
        std::set<int>               getPorts(void);
        int                         getClientMaxBodySize(void);

    private :
        std::string                 _serverName;
        std::map<int, std::string>  _errorPages;
        std::set<int>               _ports;
        int                         _clientMaxBodySize;
        LocationConfigServer        _locationConfigServer;

};

#endif