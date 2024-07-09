#include "FilesysUtils.h"
#include "FilePath.h"

bool FilesysUtils::dirExists(std::string dirPath)
{
    if(std::filesystem::is_directory(dirPath)) {
        return true;
    }
    return false;
}

std::vector<std::string> FilesysUtils::listDirContents(std::string dirPath, int listType, bool recursive)
{
    std::string mfp = dirPath;

    std::vector<std::string> res;
    if(dirExists(dirPath)) {

        std::vector<std::filesystem::directory_entry> dev;  //Directory entry vector
        if(recursive) {
            using dritr = std::filesystem::recursive_directory_iterator;
            dritr dirListRec = dritr(mfp);
            for (const auto& dir : dirListRec) { dev.push_back(dir); }
        } else {
            using ditr = std::filesystem::directory_iterator;
            ditr dirList = ditr(mfp);
            for (const auto& dir : dirList) { dev.push_back(dir); }
        }

        for (const auto& dir : dev) {
            std::stringstream ss; ss << dir;

            FilePath fp(ss.str());
            
            
            if(listType==ONLY_DIRS) {
                if(dirExists(fp.get())) {
                    res.push_back(fp.get());
                }
            } else if(listType==ONLY_FILES) {
                if(!dirExists(fp.get())) {
                    res.push_back(fp.get());
                }
            } else if(listType==ALL) {
                res.push_back(fp.get());
            } else {
                printf("Unknown listType, returning empty vector...");
                return res;
            }
        }

    } else {
        printf("Warning: \"%s\" is not a directory - returning empty vector.\n", dirPath.c_str());
    }


    return res;
}