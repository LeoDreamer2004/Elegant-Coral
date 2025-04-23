#include "resources.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Resources::Resources(const string &path) {
    resPath = "resources/" + path;
}

std::string Resources::path() const {
    return resPath;
}

std::string Resources::readAll() const {
    string str;
    ifstream file;

    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file.open(resPath);
        stringstream buffer;
        buffer << file.rdbuf();
        str = buffer.str();
        file.close();
    } catch (ifstream::failure &e) {
        cout << "ERROR::RESOURCES::FILE_NOT_SUCCESSFULLY_READ" << e.what() << endl;
    }
    return str;
}

