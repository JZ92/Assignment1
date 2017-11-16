#include "../include/Environment.h"

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
            size_t space = input.find(' ');
            if (space == -1) {
                cmd = input;
                content = "";
            } else {
                cmd = input.substr(0, space);
                content = input.substr(space + 1);
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
FileSystem& Environment:: getFileSystem() const
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
