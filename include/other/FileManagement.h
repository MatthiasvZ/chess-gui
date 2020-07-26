#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <string>

class FileManagement
{
    public:
        static void setWorkingDir();
        static void createFolders();

    protected:

    private:
        static std::string getDir();
};

#endif // FILEMANAGEMENT_H
