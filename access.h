#ifndef __ACCESS_H__
#define __ACCESS_H__
#include <functional>
#include "database.h"

typedef enum{
	MANUPULATOR = 0,
	MANAGER
} identity_t;

class Identity{
protected:
	identity_t id_type;
	uint32_t id;
	std::string password;
	Database database;
public:
	Identity();
	~Identity();

	identity_t curent_identity() const;
	bool identity2manu();
	bool identity2manager(const std::string & pwd);
	bool get_password(int id, std::string & password);
	bool modify_password(int id, const std::string & password);
};


typedef std::function<void(identity_t)> IDSwitchCallback;

class AccessContrl: public Identity{
private:
	IDSwitchCallback IDSwitchCallback_;
public:
	void setCallback(IDSwitchCallback cbk){IDSwitchCallback_ = cbk;};

	bool login(const std::string & pwd);
	bool logout();

	static AccessContrl* getInstance();
private:
	AccessContrl(){};
	~AccessContrl(){};
};

#endif