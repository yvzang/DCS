#include <strstream>
#include "access.h"
#include "limlog.h"

#define DATABASE_FILE				"/data/main.db"
#define ACCOUNT_TABLE				"manager_account"

Identity::Identity()
:id_type(MANUPULATOR), database(DATABASE_FILE)
{
	database.connect();
}

Identity::~Identity(){

}

identity_t Identity::curent_identity() const{
	return this->id_type;
}

bool Identity::identity2manu(){
	id_type = MANUPULATOR;
}

bool Identity::identity2manager(const std::string & pwd){
	std::string password;
	if(get_password(0, password)){
		if(pwd == password){
			id_type = MANAGER;
			return true;
		}
	}
	return false;
}

bool Identity::get_password(int id, std::string & password){
	char sql_str[64]{0};
	SQL_Response resp;
	std::string sql = "SELECT * FROM "ACCOUNT_TABLE" WHERE id=%d";
	sprintf(sql_str, sql.c_str(), id);
	bool ret = database.excute(sql_str, resp);
	if(ret){
		for(auto ptr = resp.begin(); ptr != resp.end(); ptr++){
			auto it = ptr->find("password");
			if(it != ptr->end()){
				password.assign(it->second);
				return true;
			}
		}
	}
	return false;
}

bool Identity::modify_password(int id, const std::string & password){
	char sql_str[64]{0};
	SQL_Response resp;
	std::string sql = "UPDATE "ACCOUNT_TABLE" SET password = %s WHERE id = %d";
	sprintf(sql_str, sql.c_str(), id);
	return database.excute(sql_str, resp);
}

