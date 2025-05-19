#include "passserver.h"
#include <algorithm>
#include <iostream>

//PassServer Class Public Implementations

//constructor
PassServer::PassServer(size_t size) : h(size) {}

//destructor
PassServer::~PassServer() {
    h.clear(); //clears the hash table
}

//loads the contents of the file into the hash table
bool PassServer::load(const char *filename) {
    return h.load(filename); //load the file
}

//add new user 
bool PassServer::addUser(std::pair<string,  string> & kv) {
    if (h.insert(make_pair(kv.first, encrypt(kv.second)))) {
        size_t hashval = std::hash<std::string>()(kv.first);
        std::cout << "hashed position of " << kv.first << " is " << hashval << std::endl;        
        std::cout << "User " << kv.first << " added." << std::endl;
        return true;
    } else {
        std::cout << "*****Error: User already exists. Could not add user." << std::endl;
        return false;
    }
}

//add new user but by rvalue reference
bool PassServer::addUser(std::pair<string,  string> && kv) {
    if (h.insert(std::move(make_pair(kv.first, encrypt(kv.second))))) {
        size_t hashval = std::hash<std::string>()(kv.first);
        std::cout << "hashed position of " << kv.first << " is " << hashval << std::endl;
        std::cout << "User " << kv.first << " added." << std::endl;
        return true;
    } else {
        std::cout << "*****Error: User already exists. Could not add user." << std::endl;
        return false;
    }
}

//remove user 
bool PassServer::removeUser(const string & k) {
    return h.remove(k); 
}

//change password
bool PassServer::changePassword(const std::pair<string, string> &p, const string & newpassword) {
    return h.insert(make_pair(p.first, encrypt(newpassword))); 
}

//find user
bool PassServer::find(const string & user) const {
    return h.contains(user); 
}

//print the table
void PassServer::dump() const {
    h.dump(); //print the table
}

//size getter
size_t PassServer::size() const {
    return h.size();
}

//write to file
bool PassServer::write_to_file(const char *filename) const {
    return h.write_to_file(filename); 
}

//encryption function definition
string PassServer::encrypt(const string & str) { 
    char salt[] = "$1$########";
    char * password = new char [35];
    strcpy(password, crypt(str.c_str(), salt));

    string pword(password);
    string delim = "$";
    int pos = pword.find(delim, 0);
    pos = pword.find(delim, pos+1);
    pos = pword.find(delim, pos+1);
    pword.erase(0, pos+1);
    return pword;
}