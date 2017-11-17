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
FileSystem:: ~FileSystem()
{
    clear();
}

void FileSystem:: clear()
{
    delete workingDirectory;
    delete rootDirectory;
}
FileSystem::FileSystem(const FileSystem & other)
{
    Directory * newWorking = new Directory (other.getWorkingDirectory().getName(),other.getWorkingDirectory().getParent());
    Directory * newRoot = new Directory ("",&other.getRootDirectory());
    setWorkingDirectory(newWorking);
    rootDirectory=newRoot;
}
FileSystem:: FileSystem(FileSystem && other)
{
    setWorkingDirectory(&other.getWorkingDirectory());
    rootDirectory=(&other.getRootDirectory());
    other.setWorkingDirectory(nullptr);
    other.rootDirectory=nullptr;
}
FileSystem & FileSystem:: operator=(FileSystem && other)
{
    if (this!=&other)
    {
        clear();
        setWorkingDirectory(&other.getWorkingDirectory())
        rootDirectory=(&other.getRootDirectory());
        other.setWorkingDirectory(nullptr);
        other.rootDirectory=nullptr;
    }
    return *this;
}
FileSystem & FileSystem:: operator=(const FileSystem & other)
{
    if (this==& other)
        return *this;
    else
    {
        clear();
        Directory * newWorking = new Directory (other.getWorkingDirectory().getName(),other.getWorkingDirectory().getParent());
        Directory * newRoot = new Directory ("",&other.getRootDirectory());
        setWorkingDirectory(newWorking);
        rootDirectory=newRoot;
        return *this;
    }
}
