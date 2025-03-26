#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <string>
#include <vector>
#include <set>
#include <semaphore.h>
#include <functional>
#include "cJSON.h"

#define CFGBOOTPROTOKEY			"bootproto"
#define CFGTYPEKEY				"type"
#define CFGIPADDRKEY			"ipaddr"
#define CFGMASKKEY				"mask"			
#define CFGGATWAYKEY			"gateway"
#define CFGWIFINAME				"wlan-name"
#define CFGWIFIPWD				"wlan-password"

#define INTER_ETHERNET			"eth1"
#define INTER_WLAN				"wlan0"
#define INTER_USBETH			"usbeth0"

#define NAMEDPIPE_PATH			"/var/wifi.pipe"
#define NETCONFIGFILE			"/etc/net.confg"
#define WPA_CONFIG_PATH			"/etc/wpa_supplicant.conf"

typedef char* 		ipaddr_t;
typedef char*		interface_t;

typedef enum{
	MENUIP = 0,
	AUTOIP = 1
} bootproto_t;

typedef enum{
	ETH = 0,
	WLAN = 1,
	USBETH = 2,
	NONE = 10
} nettype_t;

typedef enum{
	RSSI_MINI = 0,
	RSSI_MID,
	RSSI_MAX
} rssi_degree_t;

typedef struct _network_cfg{
	bootproto_t bootproto;
	nettype_t type;
	ipaddr_t ipaddr;
	ipaddr_t mask;
	ipaddr_t gatway;
} network_cfg;


typedef struct _interface_cfg_t{
	bootproto_t bootproto;
	nettype_t type;
	std::string ipaddr;
	std::string mask;
	std::string gatway;
} interface_cfg_t;

typedef struct _wifi_cfg:network_cfg{
	char* wifi_name;
	char* wifi_passwork;
} wifi_cfg;


struct _wlaniface_cfg_t : interface_cfg_t{
	std::string wifi_name;
	std::string wifi_pwd;
};

typedef _wlaniface_cfg_t wlaniface_cfg_t;

class InterfaceBase{
private:
	int sock;
public:
	typedef struct{
		std::string ifaceName;
		std::string dest_ip;
		std::string mask;
		std::string gaitway;
	} RouteItem;

	static std::vector<RouteItem> route_table;
	std::string ifaceName;;
	std::string ip;
	std::string mask;
	std::string mac;
	bool up;
	bool running;
	InterfaceBase();
	InterfaceBase(const std::string & name);
	bool getIPAddr();
	bool getMask();
	bool getMAC();
	bool isRunning();
	bool isUP();
	bool setIPAddr(const std::string & ipaddr);
	bool setMask(const std::string & mask);
	//bool setMAC(const std::string & mac);
	bool setUP(bool up);
	static bool addRoute(const RouteItem & route);
	static bool delRoute(const RouteItem & route);
	static bool parseRouteTable();
};

class InterfaceCFGABS: public InterfaceBase{
protected:
	interface_cfg_t *cfg_ptr;
public:
	InterfaceCFGABS() = delete;
	InterfaceCFGABS(const std::string & ifaceName);
	void interface_base_cfg(const interface_cfg_t & cfg);
	virtual bool prepar(const interface_cfg_t & cfg) = 0;
	virtual bool interface_cfg(const interface_cfg_t & cfg) = 0;
	virtual bool interface_cfg(cJSON* jsoncfg) = 0;
	virtual bool json2config(cJSON* jsoncfg, interface_cfg_t* cfg) = 0;
	virtual bool config2json(interface_cfg_t *cfg, cJSON* jsoncfg) = 0;
};

class EthernetIFaceCFG: public InterfaceCFGABS{
public:
	EthernetIFaceCFG() = delete;
	EthernetIFaceCFG(const std::string & ifaceName);
	~EthernetIFaceCFG();
	virtual bool prepar(const interface_cfg_t & cfg){return true;};
	virtual bool interface_cfg(const interface_cfg_t & cfg);
	virtual bool interface_cfg(cJSON* jsoncfg);
	virtual bool interface_cfg();
	virtual bool json2config(cJSON* jsoncfg, interface_cfg_t* cfg);
	virtual bool config2json(interface_cfg_t *cfg, cJSON* jsoncfg);
};

class WlanIFaceCFG: public InterfaceCFGABS{
public:
	WlanIFaceCFG() = delete;
	WlanIFaceCFG(const std::string & ifaceName);
	~WlanIFaceCFG();
	virtual bool prepar(const interface_cfg_t & cfg);
	virtual bool interface_cfg(const interface_cfg_t & cfg);
	virtual bool interface_cfg(cJSON* jsoncfg);
	virtual bool interface_cfg();
	virtual bool json2config(cJSON* jsoncfg, interface_cfg_t* cfg);
	virtual bool config2json(interface_cfg_t *cfg, cJSON* jsoncfg);
};

class USBEtheIFaceCFG : public InterfaceCFGABS{
public:
	USBEtheIFaceCFG() = delete;
	USBEtheIFaceCFG(const std::string & ifaceName);
	~USBEtheIFaceCFG();
	virtual bool prepar(const interface_cfg_t & cfg){return true;};
	virtual bool interface_cfg(const interface_cfg_t & cfg);
	virtual bool interface_cfg(cJSON* jsoncfg){return true;};
	virtual bool interface_cfg();
	virtual bool json2config(cJSON* jsoncfg, interface_cfg_t* cfg){return true;};
	virtual bool config2json(interface_cfg_t *cfg, cJSON* jsoncfg){return true;};
};



class NetConfiger{
private:
	pthread_t switch_thread_pid;
public:
	pthread_mutex_t config_mutex;
	EthernetIFaceCFG ethernetCFG;
	WlanIFaceCFG wlanCFG;
	USBEtheIFaceCFG usbethCFG;
	InterfaceCFGABS* current_iface_ptr = nullptr;
	nettype_t current_iface_type = NONE;

	NetConfiger();
	~NetConfiger();
	bool default_route(InterfaceBase::RouteItem & route);
	void current_interface();

	friend void* switch_thread_handler(void* args);
};

class NetworkManager : public InterfaceBase{
private:
protected:
	pthread_t *fetch_pid = nullptr;
public:
	int rssi;
	NetworkManager() = delete;
	NetworkManager(const std::string & ifaceName);

	virtual void fetch_network_param_daemo() = 0;
	virtual void fetch_network_param_stop() = 0;
	virtual rssi_degree_t get_ssi() = 0;
};

typedef struct{
	std::string wifiName;
	std::string wifiPassword;
	bool read(const std::string & path);
	bool write(const std::string & path);
}wpa_supplicant_info;


class WIFIConnectionCallbackABS{
public:
	virtual void on_success(){};
	virtual void on_failed(){};
};

class WifiManager: public NetworkManager{
public:
	typedef struct _WifiInfo_t{
		std::string wifiName;
	} WifiInfo_t;

private:
	std::vector<WifiInfo_t> _wifi_info;
	wpa_supplicant_info _wpa_info;
	WIFIConnectionCallbackABS* pConnectionCallback_;
	pthread_t _connect_pid;
public:
	WifiManager() = delete;
	WifiManager(const std::string & ifaceName);
	~WifiManager();
	bool prepared();
	bool connect();
	bool connect(const std::string & wifiName, 
				const std::string & wifiPassword,
				WIFIConnectionCallbackABS & callback);
	bool connect_async();
	bool connect_async(const std::string & wifiName, 
					const std::string & wifiPassword);
	bool connect_async(const std::string & wifiName, 
					const std::string & wifiPassword, 
					WIFIConnectionCallbackABS & callback);
	std::vector<WifiInfo_t>* getWifiInfo(){return &_wifi_info;};
	void wifi_scan();

	void set_callback(WIFIConnectionCallbackABS & callback){pConnectionCallback_ = &callback;};

	virtual void fetch_network_param_daemo();
	virtual void fetch_network_param_stop();
	virtual rssi_degree_t get_ssi(){return rssi_degree_t{};};

	friend void* fetch_network_param_thread(void* args);
	friend void* connection_thread(void* args);
};

#endif