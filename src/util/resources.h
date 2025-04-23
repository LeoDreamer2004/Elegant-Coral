#ifndef FILES_H
#define FILES_H

#include <string>

class Resources {
    std::string resPath;

public:
    explicit Resources(const std::string &path);

    /** returns the actual file path */
    std::string path() const;

    /** read the content of the file */
    std::string readAll() const;
};


#endif //FILES_H
