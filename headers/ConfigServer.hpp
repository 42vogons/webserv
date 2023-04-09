#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <string>
#include <map>
#include <set>
#include <LocationConfigServer.hpp>

class ConfigServer {
    public :
        

    private :
        std::string                 _serverName;
        std::map<int, std::string>  _errorPages;
        std::set<int>               _ports;
        int                         _clientMaxBodySize;
        LocationConfigServer        _locationConfigServer;

};

#endif