#include "../include/Commands.h"

Directory * goTo (Directory * location, string args)
{
    string sub;
    bool loop=false;
    size_t found;
    while (!loop) {
        found=args.find("/");
        if (found==-1)
            sub=args;
        else
            sub = args.substr(0, found);

        if (sub == "..") {
            if ((location&).getParent() == nullptr) {
                return nullptr;
            }
            else
                (location&)=(location&).getParent();
        } else {
            vector<BaseFile *>::iterator myIt;
            bool found2 = false;
            for (myIt = (location&).getChildren().begin();
                 myIt != (location&).getChildren().end(); myIt++) {
                if (sub.compare((**myIt).getName()) == 0 && !((**myIt).isFile()))
                {
                    location=**myIt;
                    found2 = true;
                }
            }
            if (!found2) {
                return nullptr;
            }
        }
        if (args == sub)
            loop = true;
        else
            args = args.substr(found + 1);
    }
    return location;
}

BaseCommand:: BaseCommand(string args) : args(args) {}

string BaseCommand:: getArgs()
{
    return args;
}

PwdCommand:: PwdCommand(string args) : BaseCommand(args) {}
void PwdCommand:: execute(FileSystem & fs)
{
    string path="/";
    vector <string> output;
    Directory current=fs.getWorkingDirectory();
    output.push_back(current.getName());
    while (current.getParent()!= nullptr)
    {
        current=*(current.getParent());
        output.push_back(current.getName());
    }
    vector<string>::reverse_iterator myIt;
    for (myIt=output.rbegin() ; myIt!=output.rend(); ++myIt)
    {
        path.append(*myIt);
        path.append("/");
    }
    path.pop_back();
    cout << path;
}
string PwdCommand:: toString() { return "pwd"; }

CdCommand:: CdCommand(string args) :BaseCommand(args) {}

void CdCommand:: execute(FileSystem & fs)
{
    string args=getArgs();
    Directory * current;
    if (args.at(0)=='/')
        current=goTo((&fs.getRootDirectory()),args.substr(1));
        else
        current=goTo((&fs.getWorkingDirectory()),args);
    if (current== nullptr)
        cout << "The system cannot find the path specified";
    else
        fs.setWorkingDirectory(current);
}

string CdCommand:: toString() {return "cd";}



LsCommand:: LsCommand(string args): BaseCommand(args) {}
void LsCommand:: execute(FileSystem & fs)
{
    string args=getArgs();
    bool sortBySize=false;
    if (args.size()>=4 && args.substr(0,4).compare("[-s]")==0)
    {
        sortBySize=true;
        if (args!="[-s]")
        args=args.substr(5);
        else
            args="";
    }

    Directory * current;
    if (args.at(0)=='/')
        current=goTo((&fs.getRootDirectory()),args.substr(1));
    else
        current=goTo((&fs.getWorkingDirectory()),args);
    if (current== nullptr)
        cout << "The system cannot find the path specified";
    else {
        if (sortBySize)    // SAME SIZE ISSUE (JONATHAN)
            (*current).sortBySize();
        else
            (*current).sortByName();
        vector<BaseFile *>::iterator printIt;
        for (printIt = (*current).getChildren().begin(); printIt != (*current).getChildren().end(); printIt++) {
            if ((**printIt).isFile())
                cout << "FILE/t" << (**printIt).getName() << "/t" << (**printIt).getSize();
            else
            cout << "DIR/t" << (**printIt).getName() << "/t" << (**printIt).getSize();

            cout << "/n";
        }
    }

}

    string LsCommand:: toString() {return "ls";}

    MkdirCommand:: MkdirCommand(string args) : BaseCommand(args) {}
    void MkdirCommand:: execute(FileSystem & fs)
    {}
    string MkdirCommand:: toString() {return "mkdir";}

    MkfileCommand:: MkfileCommand(string args) :BaseCommand(args) {}
    void MkfileCommand:: execute(FileSystem & fs)
    {
        string args=getArgs();
        size_t found = args.find_last_of('/');
        string sub;
        size_t space;
        string sizeW;
        int size;
        string path;
        if (found!=-1)
        {
            sub=args.substr(found+1);
            space=sub.find(' ');
            sub=sub.substr(0,space);
            sizeW=sub.substr(space+1);
            size=atoi(sizeW);
            path=args.substr(0,found);
        }
        else
        {
            space=args.find_last_of(' ');
            sub=args.substr(0,space);
            sizeW=sub.substr(space+1);
            size=atoi(sizeW);
            path=args.substr(0,space);
        }
        Directory * location;
        if (args.at(0)=='/')
            location = goTo((&fs.getRootDirectory()),path.substr(1));
        else
            location = goTo((&fs.getWorkingDirectory()),path);
        if (location==nullptr)
            cout << "The system cannot find the path specified";
        else
        {
            vector <BaseFile*>:: iterator myIt;
            bool found=false;
            for(myIt=location->getChildren().begin(); myIt!=location->getChildren().end() && !found; myIt++)
            {
                if ((**myIt).getName().compare(sub) == 0)
                    found = true;
            }
            if (found)
                cout << "The file already exists";
            else
            {
                File * newFile=new File(sub,size);
                location->addFile(newFile);
            }
        }

    }

    string MkfileCommand:: toString() {return "mkfile";}

    CpCommand :: CpCommand(string args) : BaseCommand(args){}

        void CpCommand:: execute(FileSystem &fs)
        {
            string args=getArgs();
            size_t space=args.find(' ');
            string sub1=args.substr(0, found);
            size_t found=args.find_last_of('/');
            string name;
            string path1;
            Directory * location;
            if(found==-1)
            {
                name = sub1;
                location=(fs.getWorkingDirectory()*);
            }
                else
            {
                name=sub1.substr(found+1);
                path1=sub1.substr(0,found);
                if(path1.at(0)=='/')
                    location=goTo(&fs.getRootDirectory(), path1);
                else
                location=goTo(&fs.getWorkingDirectory(), path1);
            }
            if(location==nullptr)
                cout << "The system cannot find the path specified";
            else
            {
                vector <BaseFile*>:: iterator myIt;
                bool found=false;
                for(myIt=location->getChildren().begin(); myIt!=location->getChildren().end() && !found; myIt++) {
                    if ((**myIt).getName().compare(name) == 0)
                    {
                    found = true;
                        if((**myIt).isFile)
                        {
                            int size=(**myIt).getSize();
                            string fileName=(**myIt).getName();
                        }
                        else
                        {
                            
                        }

                    }


                }
        }

        string CpCommand:: toString() {return "cp";}


