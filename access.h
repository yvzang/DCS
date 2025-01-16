#ifndef __ACCESS_H__
#define __ACCESS_H__
#include "database.h"

typedef enum{
	MANUPULATOR = 0,
	MANAGER
} identity_t;

class Identity{
private:
	identity_t id_type;
	uint32_t id;
	std::string password;
	Database database;
public:
	static Identity* getInstance(){
		static Identity id;
		return &id;
	}
	identity_t curent_identity() const;
	bool identity2manu();
	bool identity2manager(const std::string & pwd);
	bool get_password(int id, std::string & password);
	bool modify_password(int id, const std::string & password);
private:
	Identity();
	~Identity();
};


#endif