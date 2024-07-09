#include "FilePath.h"
#include <sstream>

FilePath::FilePath(std::string path)
{
    std::stringstream res;
    for(int i = 0; i<path.size(); i++) {
        if(path[i]!='\"') {
            res << path[i];
        }
    }

    cleanpath = res.str();
}

FilePath::~FilePath(){}

std::string FilePath::get() { return cleanpath; }