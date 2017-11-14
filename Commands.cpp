#include "../include/Commands.h"

BaseCommand:: BaseCommand(string args) : args(args) {}

string BaseCommand:: getArgs()
{
    return args;
}

PwdCommand:: PwdCommand(string args) : BaseCommand(args) {}
void PwdCommand:: execute(FileSystem & fs)
{
    string path="/"";
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
    string sub;
    bool loop=false;
    size_t found;
    if (args.at(0)=='/') {
        fs.setWorkingDirectory(&fs.getRootDirectory());
        args = args.substr(1);
    }

    while (!loop) {
        found=args.find("/");
        if (found==-1)
                sub=args;
        else
            sub = args.substr(0, found);

            if (sub == "..") {
                if (fs.getWorkingDirectory().getParent() == nullptr) {
                    cout << "The system cannot find the path specified";
                    loop = true;
                }
                else
                    fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            } else {
                vector<BaseFile *>::iterator myIt;
                bool found2 = false;
                for (myIt = fs.getWorkingDirectory().getChildren().begin();
                     myIt != fs.getWorkingDirectory().getChildren().end(); myIt++) {
                    if (sub.compare((**myIt).getName()) == 0 && !((**myIt).isFile()))
                    {
                        fs.setWorkingDirectory(**myIt);
                        found2 = true;
                    }
                }
                if (!found2) {
                    cout << "The system cannot find the path specified";
                    loop = true;
                }
            }
            if (args == sub)
                loop = true;
            else
                args = args.substr(found + 1);

    }

}
string CdCommand:: toString() {return "cd";}



LsCommand:: LsCommand(string args): BaseCommand(args) {}
void LsCommand:: execute(FileSystem & fs)
{
    string args=getArgs();
    bool sortBySize=false;
    if (args.size()>=4 && args.substr(0,4).compare("[-s]"==0))
    {
        sortBySize=true;
        if (args!="[-s]")
        args=args.substr(5);
        else
            args="";
    }

    Directory current = fs.getWorkingDirectory();
    if (args.at(0)=='/') {
        current=fs.getRootDirectory();
        args = args.substr(1);
    }
    size_t found;
    string sub;
    bool loop=false;
    while (!loop) {
        found=args.find("/");
        if (found==-1)
            sub=args;
        else
            sub = args.substr(0, found);

        if (sub == "..") {
            if (current.getParent() == nullptr) {
                cout << "The system cannot find the path specified";
                loop = true;
            }
            else
                current=(*current.getParent());
        } else {
            vector<BaseFile *>::iterator myIt;
            bool found2 = false;
            for (myIt = current.getChildren().begin();
                 myIt != current.getChildren().end(); myIt++) {
                if (sub.compare((**myIt).getName()) == 0 && !((**myIt).isFile()))
                {
                    current=**myIt;
                    found2 = true;
                }
            }
            if (!found2) {
                cout << "The system cannot find the path specified";
                loop = true;
            }
        }
        if (args == sub)
            loop = true;
        else
            args = args.substr(found + 1);

    }
    if (sortBySize)    // SAME SIZE ISSUE (JONATHAN)
        current.sortBySize();
        else
        current.sortByName();
    vector <BaseFile*> :: iterator printIt;
    for (printIt=current.getChildren().begin() ; printIt!=current.getChildren().end() ; printIt++)
    {
        if ((**printIt).isFile())
            cout << "FILE/t" << (**printIt).getName()"/t"  << (**printIt).getSize();
        else
        cout << "DIR/t" << (**printIt).getName() << "/t"  << (**printIt).getSize();
        cout << "/n";
    }


}
string LsCommand:: toString() {return "ls";}

