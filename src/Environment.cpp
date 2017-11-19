#include "../include/Environment.h"
#include "../include/GlobalVariables.h"

Environment:: Environment(): fs(),commandsHistory() {}

void Environment:: start()
{
    string input="";
    string cmd;
    string content;
    while (input!="exit")
    {
        bool legal=false;
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        getline(cin,input);
        if (input!="exit") {
            int space = input.find(' ');
            if (space == -1) {
                cmd = input;
                content = "";
            } else {
                cmd = input.substr(0, space);
                content = input.substr(space + 1);
            }
            if (verbose==2 || verbose==3)
            {
                cout << input << endl;
            }
            if (cmd == "pwd") {
                PwdCommand * temp = new PwdCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "cd") {
                CdCommand * temp = new CdCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "ls") {
                LsCommand * temp= new LsCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "mkdir") {
                MkdirCommand * temp = new MkdirCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "mkfile") {
                MkfileCommand * temp = new MkfileCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "cp") {
                CpCommand * temp = new CpCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "mv") {
                MvCommand * temp = new MvCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "rename") {
                RenameCommand * temp = new RenameCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "rm") {
                RmCommand * temp = new RmCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "history") {
                HistoryCommand * temp = new HistoryCommand(content, commandsHistory);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "verbose") {
                VerboseCommand * temp = new VerboseCommand(content);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (cmd == "exec") {
                ExecCommand * temp = new ExecCommand(content, commandsHistory);
                temp->execute(fs);
                addToHistory(temp);
                legal = true;
            }

            if (!legal) {
                ErrorCommand * temp = new ErrorCommand(input);
                temp->execute(fs);
                addToHistory(temp);
            }
        }

    }

}
const FileSystem& Environment:: getFileSystem() const
{
    return fs;
}
void Environment:: addToHistory(BaseCommand *command)
{
    commandsHistory.push_back(command);
}
const vector<BaseCommand*>& Environment:: getHistory() const
{
    return commandsHistory;
}
void Environment:: clear()
{
//    vector <BaseCommand*> :: iterator myIt = commandsHistory.begin();
    for (vector <BaseCommand*> :: iterator myIt = commandsHistory.begin(); myIt!=commandsHistory.end() ; myIt++)
    {
        delete (*myIt);
    }
}

Environment ::~Environment()
{
    clear();
}

Environment:: Environment(const Environment &other): commandsHistory(),fs()
{
    vector<BaseCommand*>::const_iterator myIt;
    for( myIt= other.commandsHistory.begin();
        myIt != other.commandsHistory.end(); ++myIt)
    {
        commandsHistory.push_back((*myIt));
    }
    fs = *(new FileSystem(other.getFileSystem()));
}
Environment:: Environment(Environment &&other): commandsHistory(),fs() {
    commandsHistory = other.commandsHistory;
    fs = other.getFileSystem();
    other.commandsHistory.clear();
}

Environment & Environment :: operator=(const Environment &other)
{
    if(this != &other){
        clear();
        vector<BaseCommand*>::const_iterator myIt;
        for( myIt= other.commandsHistory.begin();
             myIt != other.commandsHistory.end(); ++myIt)
        {
            commandsHistory.push_back((*myIt));
        }
        fs = *(new FileSystem(other.getFileSystem()));
    }
    return *this;
}

Environment & Environment:: operator= ( Environment &&other)
{
    if(this != &other){
        clear();
        commandsHistory = other.commandsHistory;
        fs = other.getFileSystem();
        other.commandsHistory.clear();
    }
    return *this;
}
