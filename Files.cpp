#include "../include/Files.h"


BaseFile::BaseFile(string name):name(name) {}
string BaseFile:: getName() const
{
    return name;
}
void BaseFile:: setName(string newName)
{
    name=newName;
}

BaseFile:: ~BaseFile() {}

File:: File(string name, int size): BaseFile(name),size(size){}

int File:: getSize() const
{
    return size;
}

bool File:: isFile() {return true;}

Directory::Directory(string name, Directory *parent):BaseFile(name),parent(parent),children() {}

Directory:: ~Directory()
{
    clear();
}

void Directory:: clear()
{
    vector <BaseFile*> :: iterator myIt;
    for (myIt=getChildren().begin() ; myIt!=getChildren().end(); myIt++)
    {
        delete (*myIt);
    }

}

Directory::Directory(Directory && other):BaseFile(other),children(other.getChildren()),parent(other.getParent())
{
    setName(other.getName());
    other.setParent(nullptr);
}

Directory::Directory(const Directory & other):BaseFile(""),children(),parent(nullptr)
{
    setName((other.getName()));
    vector <BaseFile*> :: iterator myIt;
    for (myIt=(other.getChildren()).begin() ; myIt!=other.getChildren().end() ; myIt++)
    {
        if ((**myIt).isFile())
        {
            File * newFile = new File(other.getName(),other.getSize());
            addFile(newFile);
        }
        else
        {
            Directory * newDirectory = new Directory ((Directory &)**myIt );
            addFile(newDirectory);
        }
        setParent(other.getParent());
    }
}

Directory & Directory:: operator=(const Directory & other)
{
    if (this==& other)
        return *this;
    else
    {
        clear();
        setParent(other.getParent());
        vector <BaseFile*> :: iterator myIt;
        for (myIt=(other.getChildren()).begin() ; myIt!=other.getChildren().end() ; myIt++) {
            if ((**myIt).isFile()) {
                File *newFile = new File(other.getName(), other.getSize());
                addFile(newFile);
            } else {
                Directory *newDirectory = new Directory((Directory &) **myIt);
                addFile(newDirectory);
            }
            setParent(other.getParent());
        }
        return *this;
    }
}

Directory & Directory:: operator=(Directory && other)
{
    if (this!=&other)
    {
        clear();
        setParent(other.getParent());
        children=other.getChildren();
        setName(other.getName());
        other.setParent(nullptr);
    }
    return *this;
}

Directory* Directory:: getParent() const
{
    return parent;
}



void Directory:: setParent(Directory *newParent)
{
    parent=newParent;
}

void Directory:: addFile(BaseFile* file)
{
    vector <BaseFile*> :: iterator myIt;
    bool found=false;
    for (myIt=getChildren().begin() ; myIt!=getChildren().end() && !found ; myIt++)
    {
        if ((**myIt).getName().compare(file->getName())==0)
            found=true;
    }
    if (!found)
    children.push_back(file);
}

void Directory:: removeFile(string name)
{
    int i(0);
    vector<BaseFile*>::iterator myIt;
    for(myIt=children.begin(); myIt!=children.end() ; myIt++,i++) {
        if ((**myIt).getName().compare(name)==0) {
            delete (*myIt);
            children.erase(children.begin() + i);
        }
    }
}

void Directory:: removeFile(BaseFile* file)
{
        delete file;
}

bool sortAlpha( BaseFile *a, BaseFile *b)
{
    return (((*a).getName())<((*b).getName()));
}

bool sortSize( BaseFile *a, BaseFile *b)
{
    if ((*a).getSize()!=((*b).getSize()))
    return (((*a).getSize())<((*b).getSize()));
    else
    return (((*a).getName())<((*b).getName()));
}

void Directory:: sortByName()
{
    sort(children.begin(),children.end(),sortAlpha);
}
void Directory:: sortBySize()
{
    sort(children.begin(),children.end(),sortSize);
}
vector<BaseFile*> Directory:: getChildren() const
{
    return children;
}
int Directory:: getSize() const
{
    int output(0);
    vector<BaseFile*>::const_iterator myIt;
    for(myIt=children.begin(); myIt!=children.end() ; myIt++) {
        output=output+(**myIt).getSize();
    }
    return output;
}

string Directory:: getAbsolutePath()
{
    string output="/";
    vector <string> path;
    path.push_back(getName());
    Directory * current=getParent();
    while (current!= nullptr)
    {
        path.push_back(current->getName());
        current=current->getParent();
    }
    path.pop_back();
    vector<string>::reverse_iterator myIt;
    for (myIt=path.rbegin() ; myIt!=path.rend(); ++myIt)
    {
        output.append(*myIt);
        output.append("/");
    }
    if (output.size()!=1)
    output.pop_back();
    return output;
}

bool Directory:: isFile() {return false;}
