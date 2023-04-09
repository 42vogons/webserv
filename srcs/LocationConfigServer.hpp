#ifndef LOCATION_CONFIG_SERVER_HPP
#define LOCATION_CONFIG_SERVER_HPP

#include <string>
#include <map>
#include <set>

class LocationConfigServer {
    public :
        

    private :
        bool                         _autoIndex;
        std::string                  _name;
        std::string                  _root;
        std::map<std::string, bool>  _allowedMethods;
        std::set<std::string>        _pagesIndex;
        std::string                  _cgi;
};

#endif