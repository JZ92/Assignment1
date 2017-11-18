#include "../include/Files.h"
#include "../include/GlobalVariables.h"


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

int File:: getSize()
{
    return size;
}

bool File:: isFile() {return true;}

Directory::Directory(string name, Directory *parent):BaseFile(name),children(),parent(parent) {}

Directory:: ~Directory()
{
    if (verbose==1 || verbose==3)
    {
        cout <<"Directory:: ~Directory()" << endl;
    }
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
    if (verbose==1 || verbose==3)
    {
        cout <<"Directory::Directory(Directory && other)" << endl;
    }
    setName(other.getName());
    other.setParent(nullptr);
}

Directory::Directory(const Directory & other):BaseFile(""),children(),parent(other.getParent())
{
    if (verbose==1 || verbose==3)
    {
        cout <<"Directory::Directory(const Directory & other)" << endl;
    }
    setName((other.getName()));
    vector <BaseFile*> :: const_iterator myIt;
    for (myIt=(other.children).begin() ; myIt!=other.children.end() ; myIt++)
    {
        if ((**myIt).isFile())
        {
            children.push_back(new File(((File *)(*myIt))->getName(),((File *)(*myIt))->getSize()));
        }
        else
        {
            children.push_back(new Directory (*((Directory *)(*myIt))));
            ((Directory *)children.back())->parent=this;
        }
    }
}

Directory & Directory:: operator=(const Directory & other)
{
    if (verbose==1 || verbose==3)
    {
        cout <<"Directory & Directory:: operator=(const Directory & other)" << endl;
    }
    if (this==& other)
        return *this;
    else
    {
        clear();
        setParent(other.getParent());
        vector <BaseFile*> :: const_iterator myIt;
        for (myIt=other.children.begin() ; myIt!=other.children.end() ; myIt++) {
            if ((**myIt).isFile()) {
                File *newFile = new File(((File *)(*myIt))->getName(), (((File *)(*myIt))->getSize()));
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
    if (verbose==1 || verbose==3)
    {
        cout <<"Directory & Directory:: operator=(Directory && other)" << endl;
    }
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
    parent->removeFile(this);
    parent=newParent;
    newParent->addFile(this);
}

void Directory:: addFile(BaseFile* file)
{
    children.push_back(file);
}

void Directory:: removeFile(string name)
{
    vector<BaseFile*>::iterator myIt;
    for(myIt=children.begin(); myIt!=children.end();) {
        if ((**myIt).getName()==name) {
            children.erase(myIt);
        } else
            myIt++;
    }
}

void Directory:: removeFile(BaseFile* file)
{
    vector<BaseFile*>::iterator myIt;
    for(myIt=children.begin(); myIt!=children.end();) {
        if ((*myIt)==file) {
            children.erase(myIt);
        } else
            myIt++;
    }
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
vector<BaseFile*> Directory:: getChildren()
{
    return children;
}
int Directory:: getSize()
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
