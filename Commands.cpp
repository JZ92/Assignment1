#include <sstream>
#include "../include/Commands.h"
#include "../include/GlobalVariables.h"

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
            vector <BaseFile *> myChildren = location->getChildren();
            for (myIt = myChildren.begin();
                 myIt != myChildren.end(); myIt++) {
                if (sub==(**myIt).getName() && !((**myIt).isFile()))
                {
                    location=(Directory *)(*myIt);
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
    Directory * current=&fs.getWorkingDirectory();
    output.push_back(current->getName());
    while (current->getParent()!= nullptr)
    {
        current=(current->getParent());
        output.push_back(current->getName());
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
        cout << "The system cannot find the path specified" <<endl;
    else
        fs.setWorkingDirectory(current);
}

string CdCommand:: toString() {return "cd";}



LsCommand:: LsCommand(string args): BaseCommand(args) {}
void LsCommand:: execute(FileSystem & fs)
{
    string args=getArgs();
    bool sortBySize=false;
    if (args.size()>=2 && args.substr(0,2)=="-s")
    {
        sortBySize=true;
        if (args!="-s")
        args=args.substr(3);
        else
            args="";
    }

    Directory * current;
    if (args=="")
        current=&fs.getWorkingDirectory();
    else {
        if (args.at(0) == '/')
            current = goTo((&fs.getRootDirectory()), args.substr(1));
        else
            current = goTo((&fs.getWorkingDirectory()), args);
    }
        if (current == nullptr)
            cout << "The system cannot find the path specified" << endl;

    else {
        if (sortBySize)
            (*current).sortBySize();
        else
            (*current).sortByName();
        vector<BaseFile *>::iterator printIt;
            vector <BaseFile *> myChildren = ((Directory *)(current))->getChildren();
        for (printIt = myChildren.begin(); printIt != myChildren.end(); printIt++) {
            if ((*printIt)->isFile())
                cout << "FILE\t" << (**printIt).getName() << "\t" << (**printIt).getSize();
            else
            cout << "DIR\t" << (**printIt).getName() << "\t" << (**printIt).getSize();

            cout << endl;
        }
    }

}

    string LsCommand:: toString() {return "ls";}

    MkdirCommand:: MkdirCommand(string args) : BaseCommand(args) {}
    void MkdirCommand:: execute(FileSystem & fs)
    {
        string args=getArgs();
        Directory * current;
        if (args.at(0)=='/')
        {
            current=&fs.getRootDirectory();
            args=args.substr(1);
        } else
            current=&fs.getWorkingDirectory();
        istringstream ss(args);
        string slash;
        string curr;
        int k,i;
        vector <string> path;
        while (getline(ss, slash, '/'))
            path.insert(path.begin(), slash);
        while (!path.empty())
        {
            curr=path.back();
            path.pop_back();
            bool found=false;
            vector <BaseFile*> :: iterator myIt;
            vector <BaseFile*> myChildren=current->getChildren();
            for (myIt=myChildren.begin(), k=0 ; myIt!=myChildren.end() && !found; myIt++, k++)
            {
                if ((**myIt).getName()==curr)
                {
                    found=true;
                    if (!(**myIt).isFile())
                    {
                        i=k;
                        if (path.empty())
                            cout << "The directory already exists" << endl;
                        else
                            current=((Directory *)(*myIt));
                    }
                }
            }
            if (!found)
            {
                current->addFile(new Directory(curr,current));
                current= (Directory *)(current->getChildren().at(current->getChildren().size()-1));
            }
        }

    }
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
        Directory * location;
        if (found!=-1)
        {
            sub=args.substr(found+1);
            space=args.find(' ');
            sub=sub.substr(0,sub.find(' '));
            sizeW=args.substr(space+1);
            size=stoi(sizeW);
            path=args.substr(0,found);
            if (args.at(0)=='/') {
                if (path.size()!=0)
                location = goTo((&fs.getRootDirectory()), path.substr(1));
                else
                    location=&fs.getRootDirectory();
            }
            else
                location = goTo((&fs.getWorkingDirectory()),path);
        }
        else
        {
            location=&fs.getWorkingDirectory();
            space=args.find_last_of(' ');
            sub=args.substr(0,space);
            sizeW=args.substr(space+1);
            size=stoi(sizeW);
        }
        if (location==nullptr)
            cout << "The system cannot find the path specified" << endl;
        else
        {
            vector <BaseFile*>:: iterator myIt;
            vector <BaseFile*> myChildren=location->getChildren();
            bool found=false;
            for(myIt=myChildren.begin(); myIt!=myChildren.end() && !found; myIt++)
            {
                if ((**myIt).getName()==sub)
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
            Directory * newDir;
            int size;
            bool file;
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
                            file=true;
                        }
                        else
                        {
                            newDir = new Directory(*(Directory *)(*myIt));
                            file=false;
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
                        if (file) {
                            File *newFile = new File(fileName, size);
                            location2->addFile(newFile);
                        }
                        else
                            location2->addFile(newDir);
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
                            Directory * newDir = new Directory(*(Directory *)(*myIt));
                            location->removeFile(newDir->getName());
                            vector<BaseFile *>::iterator myIt2;
                            bool found3 = false;
                            for (myIt2 = location2->getChildren().begin();
                                 myIt2 != location2->getChildren().end() && !found3; myIt2++) {
                                if ((**myIt).getName().compare(name) == 0)
                                    found3 = true;
                            }
                            if (!found3)
                            {
                                location2->addFile(newDir);
                            }
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
                        if((Directory *)(*myIt)==&fs.getWorkingDirectory())
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
        string args=getArgs();
        int ver=stoi(args);
        if (ver<0 || ver>3)
            cout << "Wrong verbose input";
        else
            verbose=ver;
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