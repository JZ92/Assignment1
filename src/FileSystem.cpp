#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"

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
    if (verbose==1 || verbose==3)
    {
        cout <<"FileSystem:: ~FileSystem()" << endl;
    }
    clear();
}

void FileSystem:: clear()
{
    delete workingDirectory;
    delete rootDirectory;
}
FileSystem::FileSystem(const FileSystem & other): rootDirectory(new Directory (* other.rootDirectory)), workingDirectory()
{
    if (verbose==1 || verbose==3)
    {
        cout <<"FileSystem::FileSystem(const FileSystem & other)" << endl;
    }
    Directory * current =  other.workingDirectory ;
    vector <string> path;
    while (current->getParent()!=nullptr)
    {
        path.push_back(current->getName());
        current=current->getParent();
    }
    current=other.rootDirectory;
    vector <BaseFile*> :: iterator myIt;
    while (!path.empty())
    {
        for (myIt = current->getChildren().begin(); myIt != current->getChildren().end(); myIt++) {
            if ((**myIt).getName().compare(path.back()) == 0) {
                current = (Directory *)(*myIt);
                path.pop_back();
            }
        }
    }
    setWorkingDirectory(current);
}

FileSystem:: FileSystem(FileSystem && other): rootDirectory(&other.getRootDirectory()), workingDirectory(&other.getWorkingDirectory())
{
    if (verbose==1 || verbose==3)
    {
        cout <<"FileSystem:: FileSystem(FileSystem && other)" << endl;
    }
}
FileSystem & FileSystem:: operator=(FileSystem && other)
{
    if (verbose==1 || verbose==3)
    {
        cout <<"FileSystem & FileSystem:: operator=(FileSystem && other)" << endl;
    }
    if (this!=&other)
    {
        clear();
        setWorkingDirectory(&other.getWorkingDirectory());
        rootDirectory=(&other.getRootDirectory());
        other.setWorkingDirectory(nullptr);
        other.rootDirectory=nullptr;
    }
    return *this;
}
FileSystem & FileSystem:: operator=(const FileSystem & other)
{
    if (verbose==1 || verbose==3)
    {
        cout <<"FileSystem & FileSystem:: operator=(const FileSystem & other)" << endl;
    }
    if (this==& other)
        return *this;
    else
    {
        clear();
        Directory * newRoot = new Directory ("",&other.getRootDirectory());
        rootDirectory=newRoot;
        Directory * current =other.workingDirectory ;
        vector <string> path;
        while (current->getParent()!=nullptr)
        {
            path.push_back(current->getName());
            current=current->getParent();
        }
        current= newRoot;
        vector <BaseFile*> :: iterator myIt;
        while (!path.empty()) {
            for (myIt = current->getChildren().begin(); myIt != current->getChildren().end(); myIt++) {
                if ((**myIt).getName().compare(path.back()) == 0) {
                    current = (Directory *)(*myIt);
                    path.pop_back();
                }
            }
        }
        setWorkingDirectory(current);
        return *this;
    }
}
