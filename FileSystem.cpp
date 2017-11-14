#include "../include/FileSystem.h"

FileSystem:: FileSystem()
{
    rootDirectory=nullptr;
    workingDirectory= nullptr;
}

Directory& FileSystem:: getRootDirectory() const
{
    return *rootDirectory;
}
Directory& FileSystem:: getWorkingDirectory() const
{
   return *workingDirectory;
}
void FileSystem:: setWorkingDirectory(Directory *newWorkingDirectory)
{
    workingDirectory=newWorkingDirectory;
}

