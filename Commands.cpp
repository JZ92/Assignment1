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
            if (location->getParent() == nullptr) {
                return nullptr;
            }
            else
                location=location->getParent();
        } else {
            vector<BaseFile *>::iterator myIt;
            bool found2 = false;
            for (myIt = location->getChildren().begin();
                 myIt != location->getChildren().end(); myIt++) {
                if (sub.compare((**myIt).getName()) == 0 && !((**myIt).isFile()))
                {
                    location=(***myIt);
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
        if (sortBySize)
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
            size=atoi(sizeW.c_str());
            path=args.substr(0,found);
        }
        else
        {
            space=args.find_last_of(' ');
            sub=args.substr(0,space);
            sizeW=sub.substr(space+1);
            size=atoi(sizeW.c_str());
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

        void CpCommand:: execute(FileSystem &fs) {
            string args = getArgs();
            size_t space = args.find(' ');
            string sub1 = args.substr(0, space);
            size_t found = sub1.find_last_of('/');
            string name;
            string path1;
            string path2;
            Directory *location;
            Directory *location2;
            string fileName;
            int size;
            if (found == -1) {
                name = sub1;
                location = (& fs.getWorkingDirectory()  );
            } else {
                name = sub1.substr(found + 1);
                path1 = sub1.substr(0, found);
                if (path1.at(0) == '/')
                    location = goTo(&fs.getRootDirectory(), path1);
                else
                    location = goTo(&fs.getWorkingDirectory(), path1);
            }
            path2=args.substr(space+1);
            if (location == nullptr)
                cout << "The system cannot find the path specified";
            else {
                vector<BaseFile *>::iterator myIt;
                bool found = false;
                for (myIt = location->getChildren().begin(); myIt != location->getChildren().end() && !found; myIt++) {
                    if ((**myIt).getName().compare(name) == 0) {
                        found = true;
                        if ((**myIt).isFile()) {
                            size = (**myIt).getSize();
                            fileName = (**myIt).getName();
                        } else {
                            //DIRECTORY
                        }
                    }
                }
                if (!found)
                    cout << "No such file or directory";
            }
            if (found) {
                if (path2.at(0) == '/')
                    location2 = goTo(&fs.getRootDirectory(), path2.substr(1));
                else
                    location2 = goTo(&fs.getWorkingDirectory(), path2);
                if (location2 == nullptr)
                    cout << "The system cannot find the path specified";
                else {
                    vector<BaseFile *>::iterator myIt;
                    bool found2 = false;
                    for (myIt = location2->getChildren().begin();
                         myIt != location2->getChildren().end() && !found2; myIt++) {
                        if ((**myIt).getName().compare(name) == 0)
                            found2 = true;
                    }
                    if (!found2) {
                        File *newFile = new File(fileName, size);
                        location2->addFile(newFile);
                    }
                }
            }
        }

        string CpCommand:: toString() {return "cp";}

    MvCommand:: MvCommand(string args): BaseCommand(args) {}
    void MvCommand:: execute(FileSystem & fs)
    {
        string args = getArgs();
        size_t space = args.find(' ');
        string sub1 = args.substr(0, space);
        size_t found = sub1.find_last_of('/');
        string name;
        string path1;
        string path2;
        Directory *location;
        Directory *location2;
        string fileName;
        int size;
        if (found == -1) {
            name = sub1;
            location = (& fs.getWorkingDirectory()  );
        } else {
            name = sub1.substr(found + 1);
            path1 = sub1.substr(0, found);
            if (path1.at(0) == '/')
                location = goTo(&fs.getRootDirectory(), path1);
            else
                location = goTo(&fs.getWorkingDirectory(), path1);
        }
        path2=args.substr(space+1);
        if (path2.at(0)=='/')
            location2=goTo(&fs.getRootDirectory(),path2.substr(1));
        else
            location2=goTo(&fs.getWorkingDirectory(),path2);
        if (location==nullptr || location2==nullptr)
            cout << "The system cannot find the path specified";
        else {
            Directory *current = &fs.getWorkingDirectory();
            bool flag=false;
            while (current->getParent()!=nullptr && !flag)
            {
                if (current==location) {
                    cout << "Can't move directory";
                    flag=true;
                }
                current=current->getParent();
            }
            if (!flag)
            {
                vector<BaseFile *>::iterator myIt;
                bool found2 = false;
                for (myIt = location->getChildren().begin();
                     myIt != location->getChildren().end() && !found2; myIt++) {
                    if ((**myIt).getName().compare(name) == 0) {
                        found2 = true;
                        if((**myIt).isFile())
                        {
                            fileName=(**myIt).getName();
                            size=(**myIt).getSize();
                            File * newFile = new File(fileName,size);
                            location->removeFile(fileName);
                            vector<BaseFile *>::iterator myIt2;
                            bool found3 = false;
                            for (myIt2 = location2->getChildren().begin();
                                 myIt2 != location2->getChildren().end() && !found3; myIt2++) {
                                if ((**myIt).getName().compare(name) == 0)
                                    found3 = true;
                            }
                            if (!found3)
                            {
                                location2->addFile(newFile);
                            }
                        }
                        else
                        {
                            //DIRECTORY
                        }
                    }
                }
                if (!found2)
                    cout << "No such file or directory";
            }

        }

    }

    string MvCommand:: toString() {return "mv";}

    RenameCommand:: RenameCommand(string args):BaseCommand(args){}
    void RenameCommand:: execute(FileSystem & fs)
    {
        string args = getArgs();
        size_t space = args.find(' ');
        string sub1 = args.substr(0, space);
        size_t found = sub1.find_last_of('/');
        string path;
        string name1;
        string name2;
        Directory *location;
        if (found == -1) {
            name1 = sub1;
            location = (& fs.getWorkingDirectory()  );
        } else {
            name1 = sub1.substr(found + 1);
            path = sub1.substr(0, found);
            if (path.at(0) == '/')
                location = goTo(&fs.getRootDirectory(), path);
            else
                location = goTo(&fs.getWorkingDirectory(), path);
        }
        name2=args.substr(space+1);
        if (location==nullptr)
            cout << "No such file or directory";
        else
        {
            vector<BaseFile *>::iterator myIt;
            bool found = false;
            for (myIt = location->getChildren().begin();
                 myIt != location->getChildren().end() && !found; myIt++) {
                if ((**myIt).getName().compare(name1) == 0) {
                    found=true;
                    if ((**myIt).isFile())
                        (**myIt).setName(name2);
                    else
                    {
                        if((**myIt)==&fs.getWorkingDirectory())
                            cout << "Can't rename the working directory";
                        else
                            (**myIt).setName(name2);
                    }
                }
            }
            if (!found)
                cout <<"No such file or directory";
        }
    }
    string RenameCommand::toString() {return "rename";}




    RmCommand:: RmCommand(string args):BaseCommand(args) {}
    void RmCommand:: execute(FileSystem & fs) {
        string args = getArgs();
        size_t found = args.find_last_of('/');
        string path;
        string name;
        Directory *location;
        if (found == -1) {
            name = args;
            location = (&fs.getWorkingDirectory());
        } else {
            name = args.substr(found + 1);
            path = args.substr(0, found);
            if (path.at(0) == '/')
                location = goTo(&fs.getRootDirectory(), path);
            else
                location = goTo(&fs.getWorkingDirectory(), path);
        }
        if (location == nullptr)
            cout << "no such file or directory";
        else {
            vector<BaseFile *>::iterator myIt;
            bool found = false;
            for (myIt = location->getChildren().begin();
                 myIt != location->getChildren().end() && !found; myIt++) {
                if ((**myIt).getName().compare(name) == 0) {
                    location->removeFile(name);
                    found = true;
                }
            }
            if (!found)
                cout << "No such file or directory";
        }
    }

    string RmCommand:: toString() {return "rm";}

    HistoryCommand:: HistoryCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}
    void HistoryCommand:: execute(FileSystem & fs)
    {
        vector<BaseCommand*>::const_iterator myIt;
        int i(0);
        for (myIt=history.begin() ; myIt!=history.end() ; myIt++, i++)
        {
            cout << i << "/t" << (**myIt).toString() << " " << (**myIt).getArgs();
            cout << "/n";
        }
    }
    string HistoryCommand:: toString() {return "history";}

    VerboseCommand:: VerboseCommand(string args):BaseCommand(args) {}
    void VerboseCommand:: execute(FileSystem & fs)
    {

    }
    string VerboseCommand:: toString() {return "verbose";}

    ErrorCommand:: ErrorCommand(string args) : BaseCommand(args) {}
    void ErrorCommand:: execute(FileSystem & fs)
    {
        string args=getArgs();
        size_t space = args.find(' ');
        if (space!=-1)
            args=args.substr(0,space);
        cout << args << ": Unknown command";
    }
    string ErrorCommand:: toString() {return "error";}

    ExecCommand:: ExecCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args),history(history) {}

    void ExecCommand:: execute(FileSystem & fs)
    {
        string args=getArgs();
        int a =atoi((args.substr(5)).c_str());
        if (a>=history.size())
            cout << "Command not found";
        else
            history.at(a)->execute(fs);
    }

    string ExecCommand:: toString() {return "exec";}