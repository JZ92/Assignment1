#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
    FileSystem fs;
	vector<BaseCommand*> commandsHistory;


public:
	Environment();
	~Environment();
	void start();
	void clear();
	const FileSystem& getFileSystem() const;// Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
	Environment(const Environment &other);
	Environment(Environment &&other);
	Environment & operator=(Environment &&other);
	Environment & operator=(const Environment &other);
};

#endif