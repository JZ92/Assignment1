#include "../include/Environment.h"
#include "../include/GlobalVariables.h"

Environment:: Environment(): fs(),commandsHistory() {
    Directory *d1 = new Directory("dir1", &fs.getRootDirectory());
    Directory *d2 = new Directory("dir2", &fs.getRootDirectory());
    Directory *d3 = new Directory("dir3", &fs.getRootDirectory());
    Directory *d4 = new Directory("dir4", &fs.getRootDirectory());
    Directory *d5 = new Directory("dir5", d1);
    Directory *d6 = new Directory("dir6", d1);

    fs.getRootDirectory().addFile(d1);
    fs.getRootDirectory().addFile(d2);
    fs.getRootDirectory().addFile(d3);
    fs.getRootDirectory().addFile(d4);
    d1->addFile(d5);
    d1->addFile(d6);
    d1->addFile(new File("file4", 12));
    d1->addFile(new File("file3", 12));

}

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
            size_t space = input.find(' ');
            if (space == -1) {
                cmd = input;
                content = "";
            } else {
                cmd = input.substr(0, space);
                content = input.substr(space + 1);
            }
            if (verbose==2 || verbose==3)
            {
                cout << cmd << endl;
            }
            if (cmd == "pwd") {
                PwdCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "cd") {
                CdCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "ls") {
                LsCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "mkdir") {
                MkdirCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "mkfile") {
                MkfileCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "cp") {
                CpCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "mv") {
                MvCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "rename") {
                RenameCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "rm") {
                RmCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "history") {
                HistoryCommand temp(content, commandsHistory);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "verbose") {
                VerboseCommand temp(content);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (cmd == "exec") {
                ExecCommand temp(content, commandsHistory);
                temp.execute(fs);
                addToHistory(&temp);
                legal = true;
            }

            if (!legal) {
                ErrorCommand temp(input);
                temp.execute(fs);
                addToHistory(&temp);
            }
        }

    }

}
FileSystem& Environment:: getFileSystem()
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
