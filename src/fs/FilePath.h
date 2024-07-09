#pragma once
#include <string>

class FilePath
{
public:
    FilePath(std::string path);
    ~FilePath();

    std::string get();


private:
    std::string cleanpath = "";
    /* data */
};
