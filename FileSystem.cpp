#include "../include/FileSystem.h"

FileSystem:: FileSystem() : rootDirectory(new Directory("", nullptr)), workingDirectory(rootDirectory) {}

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

