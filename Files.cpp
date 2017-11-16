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

File:: File(string name, int size): BaseFile(name),size(size){}

int File:: getSize()
{
    return size;
}

bool File:: isFile() {return true;}

Directory::Directory(string name, Directory *parent):BaseFile(name),parent(parent)
{
    children=vector <BaseFile*>();
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
    children.push_back(file);
}

void removeAll (Directory * target)
{
    vector <BaseFile*> :: iterator myIt;
    for (myIt=target->getChildren().begin() ; myIt!=target->getChildren().end() ; myIt++)
    {
        if ((**myIt).isFile())
            (target->removeFile(**myIt))
        else
            removeAll(**myIt);
    }
}

void Directory:: removeFile(string name)
{
    int i(0);
    vector<BaseFile*>::iterator myIt;
    for(myIt=children.begin(); myIt!=children.end() ; myIt++,i++) {
        if ((**myIt).getName().compare(name)==0) {
            if ((**myIt).isFile())
            children.erase(children.begin() + i);
            else
                removeAll(**myIt);
        }
    }
}

void Directory:: removeFile(BaseFile* file)
{
        int i(0);
        vector<BaseFile*>::iterator myIt;
        for(myIt=children.begin(); myIt!=children.end() ; myIt++,i++) {
            if ((**myIt)==file) {
                if ((**myIt).isFile())
                    children.erase(children.begin() + i);
                else
                    removeAll(**myIt);
            }
    }
}

bool sortAlpha( BaseFile *a, BaseFile *b)
{
    return (((*a).getName())<((*b).getName()));
}

bool sortSize( BaseFile *a, BaseFile *b)
{
    return (((*a).getSize())<((*b).getSize()));
}

void Directory:: sortByName()
{
    sort(children.begin(),children.end(),sortAlpha);
}
void Directory:: sortBySize()
{
    sort(children.begin(),children.end(),sortSize);
}
vector<BaseFile*> Directory:: getChildren()
{
    return children;
}
int Directory:: getSize()
{
    int output(0);
    vector<BaseFile*>::iterator myIt;
    int i(0);
    for(myIt=children.begin(); myIt!=children.end() ; myIt++,i++) {
        output=output+(*(children.at(i))).getSize();
    }
    return output;

}
string Directory:: getAbsolutePath()
{
    string output="/";
    vector <string> path;
    Directory current=*this;
    while (current.getParent()!= nullptr)
    {
        path.push_back(current.getName());
        current=*(current.getParent());
    }
    vector<string>::reverse_iterator myIt;
    for (myIt=path.rbegin() ; myIt!=path.rend(); ++myIt)
    {
        output.append(*myIt);
        output.append("/");
    }
    output.pop_back();
    return output;

}

bool Directory:: isFile() {return false;}
