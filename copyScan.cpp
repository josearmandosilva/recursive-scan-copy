#include <windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Shlwapi.lib")

void addBarAtEnd(std::string * str) {
    if (str->at(str->length() - 1) != '\\' && str->at(str->length() - 1) != '*') {
        str->append("\\");
    }
}

void addAsteriscAtEnd(std::string * str) {
    if (str->at(str->length() - 1) != '*') {
        str->append("*");
    }
}

void addBarAndAsterisc(std::string * str) {
    addBarAtEnd(str);
    addAsteriscAtEnd(str);
}

void deleteBarAndAsterisc(std::string * str) {
    if (str->at(str->length() - 1) == '*') {
        str->erase(str->length() - 1);
    }
    if (str->at(str->length() - 1) == '\\') {
        str->erase(str->length() - 1);
    }
}


bool transferFiles(std::string path, std::string newPath) {
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;
    std::string path_helper(path);
    std::string newPath_helper;

    addBarAndAsterisc(&path); // Add \ and * ate the end of the string if nots
    addBarAtEnd(&newPath); // add the bar at the end to make sure it is a directory division
    newPath_helper = newPath;
    //Verifica todas as pastas e ficheiros

    hFind = FindFirstFile(path.c_str(), &FindFileData); // Find first file
    
    deleteBarAndAsterisc(&path);

    if (hFind == INVALID_HANDLE_VALUE)
        printf("FindFirstFile failed (%d)\n", GetLastError());
    else
    {
        do {
            
            path_helper = path;
            addBarAtEnd(&path_helper);
            newPath_helper = newPath;

            if (FILE_ATTRIBUTE_DIRECTORY == FindFileData.dwFileAttributes) {

                std::string str(FindFileData.cFileName);

                if (str.compare(".") != 0 && str.compare("..") != 0)
                    transferFiles(path_helper.append(FindFileData.cFileName), newPath); // Make path with the founded folder

            }
            else {
                CopyFile(path_helper.append(FindFileData.cFileName).c_str(), newPath_helper.append(FindFileData.cFileName).c_str(), FALSE);
            }
        } while (FindNextFile(hFind, &FindFileData));

        FindClose(hFind);
    }
}

int main(int argc, char* argv[])
{   
    if (argc > 2) {
        bool dirExists = false;
        std::string path(argv[1]), newPath(argv[2]);

        dirExists = (PathIsDirectory(path.c_str()) && PathIsDirectory(newPath.c_str())); //Check if there's folder or not

        if (dirExists == TRUE) {
            transferFiles(path.c_str(), newPath);
            std::cout << "Done !" << std::endl;
        }
        else 
            std::cout << "One of the paths don't exist"; //Enviar erro caso algum dos caminhos seja invalido
    } else {
        std::cout << "copyScan first(path to scan) second(path to copy to)" << std::endl;
    }
    std::cout << "Press key to continue . . ." << std::endl;
    getchar();
    return 0;
}