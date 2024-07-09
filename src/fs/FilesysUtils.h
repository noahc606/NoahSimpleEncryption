#pragma once
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>


class FilesysUtils
{
public:
    FilesysUtils(/* args */);
    ~FilesysUtils();

    static bool dirExists(std::string dirPath);
    static std::vector<std::string> listDirContents(std::string dirPath, int listType, bool recursive);

    enum ListTypes {
        ONLY_DIRS,
        ONLY_FILES,
        ALL
    };

private:
    
};