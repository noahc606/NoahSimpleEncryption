#include <iostream>
#include "FileUtils.h"
#include "FilesysUtils.h"


std::vector<unsigned char> getShiftSetComplement(std::vector<unsigned char> shiftset)
{
    std::vector<unsigned char> res;
    for(int i = 0; i<shiftset.size();i++) {
        res.push_back(256-shiftset[i]);
    }

    return res;
}

std::vector<unsigned char> getShiftSetFromStr(std::string str)
{
    std::vector<unsigned char> res;
    for(int i = 0; i<str.size(); i++) {
        res.push_back(str[i]);
    }

    if(str.size()==0) {
        return getShiftSetFromStr("testing passkey 1234 abcd $fk34#$234fRF$");
    }

    return res;
}

void encryptFile(std::string file, std::vector<unsigned char> shiftset)
{
    //Open file to be encrypted (read+binary)
    FILE* pFile = fopen(file.c_str(), "rb");

    //Find file size
    fseek(pFile, 0, SEEK_END);
    long fileSize = ftell(pFile);
    fseek(pFile, 0, 0);

    //Build char* buffer which has the contents of the entire file
    unsigned char* buffer = (unsigned char*)malloc(fileSize*sizeof(unsigned char));   //Get an (unsigned char*) buffer
    printf("File size: %d bytes.", fileSize);
    for(int i = 0; i<fileSize; i++) {
        buffer[i] = fgetc(pFile);
    }
    fclose(pFile);

    //Re-open file (write+binary)
    fopen(file.c_str(), "wb");

    //Rewrite file where every character is shifted by a certain amount depending on the 'shiftset'.
    for(int i = 0; i<fileSize; i++) {
        unsigned char c = buffer[i]+(unsigned char)shiftset[(i%shiftset.size())];
        FileUtils::writeToFile(pFile, c);
    }
    fclose(pFile);

    //Cleanup malloc'ed buffer
    delete[] buffer;
}

void encryptFile(std::string file, std::string key)
{
    encryptFile(file, getShiftSetFromStr(key));
}

void decryptFile(std::string file, std::string key)
{
    encryptFile(file, getShiftSetComplement( getShiftSetFromStr(key) ));
}

void printNseIntro()
{
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("|               N.S.E. - Noah's Simple Encryption               |\n");
    printf("|                             v1.0                              |\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void printNseWorkingDir(std::vector<std::string> fileList, std::string workingDir)
{
    printf("STATUS:\n");
    printf("- Working directory is \"%s\".\n", workingDir.c_str());
    printf("- Found (%d) files within \"%s\":\n\t", fileList.size(), workingDir.c_str());
    
    for(int i = 0; i<fileList.size() && i<50; i++) {
        if(i!=fileList.size()-1) {
            printf("\"%s\", ", fileList[i].c_str());
        } else {
            printf("\"%s\"", fileList[i].c_str());
        }
    }
    if(fileList.size()>=50) {
        printf(", ...");
    }
}

void printNseNotes(std::string workingDir)
{
    printf("NOTES:\n");
    printf("- All files within the directory \"%s\" (and its subdirectories) could be encrypted and may not be recoverable without knowing the passkey.\n", workingDir.c_str());
    printf("- Files can be encrypted or decrypted multiple times. Recovering these require multiple successive decryptions with the correct keys in the correct order.\n");
    printf("- You MUST enter the correct passkey when decrypting, or else the file may be ruined.\n");
    printf("- Use this program at your own risk.\n");
}

std::string nseUserPrompt()
{
    printf("> ");
    std::string userIn;
    std::getline(std::cin, userIn);
    return userIn;
}

int main()
{
    std::string workingDir = "testingdir";
    auto vec = FilesysUtils::listDirContents(workingDir, FilesysUtils::ONLY_FILES, true);
    
    printNseIntro();
    printNseWorkingDir(vec, workingDir);
    printf("\n\n");
    printNseNotes(workingDir);

    bool running = true;
    int mode = 0;
    while(running) {
        switch(mode) {
        case 0: {
            printf("\n");
            printf("What do you want to do?\n");
            printf("- \"encrypt\": encrypt files.\n");
            printf("- \"decrypt\": decrypt files.\n");
            printf("- \"x\": exit program.\n");
            
            std::string userIn = nseUserPrompt();
            
            if(userIn=="encrypt") {
                mode = 1;
            } else if(userIn=="decrypt") {
                mode = 2;
            } else if(userIn=="x") {
                printf("\n");
                printf("Exiting.\n");
                running = false;
            } else {
                printf("\n");
                printf("Invalid input. Please try again (enter \"encrypt\", \"decrypt\", or \"x\").\n");
            }
        } break;
        case 1: {
            printf("\n");
            printf("WARNING: Attempting to ENCRYPT files within \"%s\". Enter the passkey (can even be blank). Whitespace/lettercase/etc all matter.\n", workingDir.c_str());
            printf("To cancel, type \"!x\" (without the quotes).\n");

            std::string userIn = nseUserPrompt();
        
            if(userIn=="!x") {
                printf("\n");
                printf("Cancelled operation.\n");
                mode = 0;
            } else {
                printf("\n");
                printf("Using encryption key \"%s\"...\n", userIn.c_str());
                for(int i = 0; i<vec.size(); i++) {
                    printf("Encrypting \"%s\"... ", vec[i].c_str());
                    encryptFile(vec[i], userIn);
                    printf(" Done.\n");
                }
                mode = 0;
            }
        } break;
        case 2: {
            printf("\n");
            printf("WARNING: Attempting to DECRYPT files within \"%s\". Enter the passkey used for encryption (may even be blank). Whitespace/lettercase/etc all matter.\n", workingDir.c_str());
            printf("Note that any \"normal\" files here will also be decrypted, potentially ruining them.\n");
            printf("To cancel, type \"!x\" (without the quotes).\n");

            std::string userIn = nseUserPrompt();
        
            if(userIn=="!x") {
                printf("\n");
                printf("Cancelled operation.\n");
                mode = 0;
            } else {
                printf("\n");
                printf("Using encryption key \"%s\"...\n", userIn.c_str());
                for(int i = 0; i<vec.size(); i++) {
                    printf("Decrypting \"%s\"... ", vec[i].c_str());
                    decryptFile(vec[i], userIn);
                    printf(" Done.\n");
                }
                mode = 0;
            }
        } break;
        }
    }
}