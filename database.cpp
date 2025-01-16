#include "database.h"
#include "limlog.h"

int Database::research_callback(void* data, int argc, char** argv, char** colName){
	SQL_Response* resp = reinterpret_cast<SQL_Response*>(data);
	SQL_Row row;
	for(int i = 0; i < argc; i++){
		//row.insert(colName[i], argv[i]);
		LOG_DEBUG << "research data: " << colName[i] << " = " << argv[i];
		row[colName[i]] = argv[i];
	}
	resp->push_back(row);
	return 0;
}

Database::Database(const std::string & database_file)
:databaseFile(database_file)
{

}

Database::~Database(){
	sqlite3_close(sqliteCtx);
}

bool Database::connect(){
	int rc = sqlite3_open(databaseFile.c_str(), &sqliteCtx);
	if(rc){
		LOG_ERROR << "Can't open database: " << sqlite3_errmsg(sqliteCtx);
		_is_connected = false;
		return false;
	}
	_is_connected = true;
	return true;
}

bool Database::excute(const std::string & sql, SQL_Response & resp){
	char* errMsg;
	if(_is_connected == false){
		LOG_ERROR << "Database has not connected yet.";
		return false;
	}
	int rc = sqlite3_exec(sqliteCtx, sql.c_str(), Database::research_callback, &resp, &errMsg);
	if(rc){
		LOG_ERROR << "SQL error: " << errMsg;
		sqlite3_free(errMsg);
		return false;
	}
	return true;
}
