#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>
#include <map>
#include <vector>
extern "C"{
#include "sqlite3.h"
}

#define DATABASE_FILE				"/data/main.db"

typedef std::map<std::string, std::string> SQL_Row;
typedef std::vector<SQL_Row> SQL_Response;

class Database{
private:
	sqlite3* sqliteCtx;
	std::string databaseFile;
	bool _is_connected = false;
public:
	Database() = delete;
	Database(const std::string & database_file);
	~Database();
	bool connect();
	bool is_connected(){return _is_connected;};
	bool excute(const std::string & sql, SQL_Response & resp);
private:
	static int research_callback(void* data, int argc, char** argv, char** colName);
};

#endif