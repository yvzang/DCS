#include <string.h>
#include <stdlib.h>
#include <ifaddrs.h>
//#include <linux/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/route.h>
#include <sstream>
#include "limlog.h"
#include "cJSON.h"
#include "network.h"
#include "utils.h"
#include "myregex.h"
#include "Format.h"

#define ETHERNETCFGKEY				"ethernet"
#define WIFICFGKEY					"wlan"

#define ETHEINTERFACE0				"eth0"
#define ETHEINTERFACE1				"eth1"
#define WLANINTERFACE				"wlan0"

#define USBETH_AT_CTL_FILE			"/dev/ttyUSB2"
#define ROUTE_TABLE_FILE			"/proc/net/route"

#define TR(str)						std::string(str)

#define SYSTEMCMD(fmt, ...)				{ 									\
											char cmd[128]{0};				\
											sprintf(cmd, fmt, __VA_ARGS__);	\
											system(cmd);					\
										}

/*=======================InterfaceBase=========================*/

std::vector<InterfaceBase::RouteItem> InterfaceBase::route_table;

static int hex2num(char c){
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	return -1;
}

static void hexstr2mac(char* mac, const char* hexstr){
	int i = 0;
	while(i < 6){
		if(' ' == *hexstr || ':' == *hexstr || '"' == *hexstr ||
			'\'' == *hexstr){
			hexstr++;
			continue;
		}
		*(mac + i) = ((hex2num(*hexstr) << 4) | (hex2num(*(hexstr+1))));
		i++;
		hexstr += 2;
	}
}

InterfaceBase::InterfaceBase(){
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
}

InterfaceBase::InterfaceBase(const std::string & name)
:ifaceName(name)
{
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	getIPAddr();
	getMask();
	getMAC();
	isUP();
	isRunning();
}

bool InterfaceBase::getIPAddr(){
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) == 0){
		if(ifr.ifr_ifru.ifru_flags & IFF_UP && ioctl(sock, SIOCGIFADDR, &ifr) == 0){
			ip.assign(inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_ifru.ifru_addr))->sin_addr));
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::getMask(){
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFNETMASK, &ifr) == 0){
		mask.assign(inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_ifru.ifru_netmask))->sin_addr));
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::getMAC(){
	struct ifreq ifr;
	char mac_cs[32]{0};
	unsigned char mac_ul[7]{0};
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFHWADDR, &ifr) == 0){
		memcpy(mac_ul, ifr.ifr_ifru.ifru_hwaddr.sa_data, 6);
		snprintf(mac_cs, sizeof(mac_cs), 
					"%02X:%02X:%02X:%02X:%02X:%02X",
					*(mac_ul), *(mac_ul+1), *(mac_ul+2),
					*(mac_ul+3), *(mac_ul+4), *(mac_ul+5));
		mac.assign(mac_cs);
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::isUP(){
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) == 0){
		this->up = !!(ifr.ifr_ifru.ifru_flags&IFF_UP);
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::isRunning(){
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) == 0){
		this->running = !!(ifr.ifr_ifru.ifru_flags&IFF_RUNNING);
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::setIPAddr(const std::string & ipaddr){
	struct ifreq ifr;
	unsigned long ip_ul;
	memset(&ifr, 0, sizeof(ifr));
	ip_ul = inet_addr(ipaddr.c_str());
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;
	struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_ifru.ifru_addr;
	memcpy(&(addr->sin_addr.s_addr), &ip_ul, 4);
	if(ioctl(sock, SIOCSIFADDR, &ifr) == 0){
		ip = ipaddr;
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::setMask(const std::string & mask){
	struct ifreq ifr;
	unsigned long ip_ul;
	memset(&ifr, 0, sizeof(ifr));
	ip_ul = inet_addr(mask.c_str());
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;
	struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_ifru.ifru_netmask;
	memcpy(&(addr->sin_addr.s_addr), &ip_ul, 4);
	if(ioctl(sock, SIOCSIFNETMASK, &ifr) == 0){
		this->mask = mask;
	}
	else{
		return false;
	}
	return true;
}

/*bool InterfaceBase::setMAC(const std::string & imac){
	struct ifreq ifr;
	char mac_uc[6]{0};

	bool tmpup = isUP();
	if(tmpup){
		setUP(false);
	}
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	sscanf(imac.c_str(), "%x:%x:%x:%x:%x:%x", 
			&mac_uc[0], &mac_uc[1], &mac_uc[2],
			&mac_uc[3], &mac_uc[4], &mac_uc[5]);
	memcpy(ifr.ifr_ifru.ifru_hwaddr.sa_data, mac_uc, 6);
	if(ioctl(sock, SIOCSIFHWADDR, &ifr) == 0){
		this->mac = imac;
	}
	else{
		return false;
	}
	if(tmpup){
		setUP(true);
	}
	return true;
}*/

bool InterfaceBase::setUP(bool up){
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifaceName.c_str());
	if(ioctl(sock, SIOCGIFFLAGS, &ifr) == 0){
		if(up){
			ifr.ifr_ifru.ifru_flags |= IFF_UP;
		}
		else{
			ifr.ifr_ifru.ifru_flags &= ~IFF_UP;
		}
		if(ioctl(sock, SIOCSIFFLAGS, &ifr) == 0){
			this->up = up;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
	return true;
}

bool InterfaceBase::addRoute(const RouteItem & route){
	struct rtentry rt;
	struct sockaddr_in* sockaddr_ptr;
	memset(&rt, 0, sizeof(rt));
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_gateway;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.gaitway.c_str());

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_dst;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.dest_ip.c_str());

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_genmask;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.mask.c_str());

	rt.rt_flags = RTF_GATEWAY | RTF_UP;
	rt.rt_dev = (char*)route.ifaceName.c_str();

	if(ioctl(sock, SIOCADDRT, &rt) < 0){
		return false;
	}
	else{
		route_table.push_back(route);
	}
	return true;
}

bool InterfaceBase::delRoute(const RouteItem & route){
	struct rtentry rt;
	struct sockaddr_in* sockaddr_ptr;
	memset(&rt, 0, sizeof(rt));
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_gateway;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.gaitway.c_str());

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_dst;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.dest_ip.c_str());

	sockaddr_ptr = (struct sockaddr_in*)&rt.rt_genmask;
	sockaddr_ptr->sin_family = AF_INET;
	sockaddr_ptr->sin_addr.s_addr = inet_addr(route.mask.c_str());

	rt.rt_flags = RTF_GATEWAY | RTF_UP;
	rt.rt_dev = (char*)route.ifaceName.c_str();

	if(ioctl(sock, SIOCDELRT, &rt) < 0){
		return false;
	}
	else{
		for(auto route_ptr = route_table.begin(); 
			route_ptr != route_table.end(); route_ptr++){
			if(route_ptr->ifaceName == route.ifaceName){
				if(route_ptr->dest_ip == route.dest_ip ||
					route_ptr->mask == route.mask){
					route_table.erase(route_ptr);
					break;
				}
			}
		}
	}
	return true;
}

bool InterfaceBase::parseRouteTable(){
	int ret;
	unsigned long dest, gaitway, mask;
	int flags, ref, use, metric, mtu, wind, irtt;
	FILE* fp = fopen(ROUTE_TABLE_FILE, "r");
	char readbuf[1024]{0};
	char devname[64]{0};
	uint8_t dest_ip_s[4]{0};
	uint8_t gaitway_s[4]{0};
	uint8_t mask_s[4]{0};
	uint8_t dest_ip_str[16]{0};
	uint8_t gaitway_str[16]{0};
	uint8_t mask_str[16]{0};
	// skip first line
	fgets(readbuf, sizeof(readbuf), fp);
	while(!feof(fp)){
		InterfaceBase::RouteItem route;
		memset(readbuf, 0, sizeof(readbuf));
		memset(devname, 0, sizeof(devname));
		fgets(readbuf, sizeof(readbuf), fp);
		ret = sscanf(readbuf, "%s%lx%lx%x%d%d%d%lx%d%d%d\n",
					devname, &dest, &gaitway, &flags, &ref, 
					&use, &metric, &mask, &mtu, &wind, &irtt);
		if(ret != 11){
			continue;
		}
		if((flags & RTF_UP) && (flags & RTF_GATEWAY)){
			memcpy(dest_ip_s, &dest, 4);
			memcpy(gaitway_s, &gaitway, 4);
			memcpy(mask_s, &mask, 4);
			memset(dest_ip_str, 0, sizeof(dest_ip_str));
			memset(gaitway_str, 0, sizeof(gaitway_str));
			memset(mask_str, 0, sizeof(mask_str));
			sprintf((char*)dest_ip_str, "%d.%d.%d.%d", 
					dest_ip_s[0], dest_ip_s[1], dest_ip_s[2], dest_ip_s[3]);
			sprintf((char*)gaitway_str, "%d.%d.%d.%d",
					gaitway_s[0], gaitway_s[1], gaitway_s[2], gaitway_s[3]);
			sprintf((char*)mask_str, "%d.%d.%d.%d",
					mask_s[0], mask_s[1], mask_s[2], mask_s[3]);
			route.ifaceName.assign(devname);
			route.dest_ip.assign((char*)dest_ip_str);
			route.gaitway.assign((char*)gaitway_str);
			route.mask.assign((char*)mask_str);
			InterfaceBase::route_table.push_back(route);
		}
	}
	fclose(fp);
	return true;
}

/*=======================InterfaceCFGABS======================*/

InterfaceCFGABS::InterfaceCFGABS(const std::string & ifaceName)
:InterfaceBase(ifaceName)
{

}

void InterfaceCFGABS::interface_base_cfg(const interface_cfg_t & cfg){
	if(cfg.bootproto == MENUIP){
		InterfaceBase::RouteItem route;
		setIPAddr(cfg.ipaddr);
		setMask(cfg.mask);
		route.ifaceName = ifaceName;
		route.dest_ip = "0.0.0.0";
		route.mask = "0.0.0.0";
		route.gaitway = cfg.gatway;
		InterfaceBase::addRoute(route);
	}
	else if(cfg.bootproto == AUTOIP){
		char cmd_buf[127]{0};
		sprintf(cmd_buf, "udhcpc -i %s -n -p -t 2 -T 2", 
				ifaceName.c_str());
		system(cmd_buf);
	}
}

/*=====================EthernetIFaceCFG======================*/

EthernetIFaceCFG::EthernetIFaceCFG(const std::string & ifaceName)
:InterfaceCFGABS(ifaceName)
{
	cfg_ptr = new interface_cfg_t;
}

EthernetIFaceCFG::~EthernetIFaceCFG(){
	delete cfg_ptr;
}

bool EthernetIFaceCFG::json2config(cJSON* jsoncfg, interface_cfg_t* cfg){
	interface_cfg_t* config = cfg;
	if(jsoncfg == NULL || config == NULL ||
		!cJSON_HasObjectItem(jsoncfg, CFGBOOTPROTOKEY)||
		!cJSON_HasObjectItem(jsoncfg, CFGTYPEKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGIPADDRKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGGATWAYKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGMASKKEY)){
			return false;
	}
	config->bootproto = (bootproto_t)cJSON_GetObjectItem(jsoncfg, CFGBOOTPROTOKEY)->valueint;
	config->type = (nettype_t)cJSON_GetObjectItem(jsoncfg, CFGTYPEKEY)->valueint;
	config->ipaddr = (ipaddr_t)cJSON_GetObjectItem(jsoncfg, CFGIPADDRKEY)->valuestring;
	config->gatway = (ipaddr_t)cJSON_GetObjectItem(jsoncfg, CFGGATWAYKEY)->valuestring;
	config->mask = (ipaddr_t)cJSON_GetObjectItem(jsoncfg, CFGMASKKEY)->valuestring;
	return true;
}

bool EthernetIFaceCFG::config2json(interface_cfg_t *cfg, cJSON* jsoncfg){
	interface_cfg_t* config = cfg;
	if(config == NULL || cfg == NULL)
		return false;
	cJSON_AddNumberToObject(jsoncfg, CFGBOOTPROTOKEY, config->bootproto);
	cJSON_AddNumberToObject(jsoncfg, CFGTYPEKEY, config->type);
	cJSON_AddStringToObject(jsoncfg, CFGIPADDRKEY, config->ipaddr.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGGATWAYKEY, config->gatway.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGMASKKEY, config->mask.c_str());
	return true;
}

bool EthernetIFaceCFG::interface_cfg(const interface_cfg_t & cfg){
	interface_base_cfg(cfg);
	return true;
}

bool EthernetIFaceCFG::interface_cfg(){
	cfg_ptr->bootproto = AUTOIP;
	return interface_cfg(*cfg_ptr);
}

bool EthernetIFaceCFG::interface_cfg(cJSON* jsoncfg){
	if(json2config(jsoncfg, cfg_ptr)){
		return interface_cfg(*cfg_ptr);
	}
	else{
		return false;
	}
	return true;
}

/*====================WlanIFaceCFG===========================*/

WlanIFaceCFG::WlanIFaceCFG(const std::string & ifaceName)
:InterfaceCFGABS(ifaceName)
{
	cfg_ptr = new wlaniface_cfg_t;
}

WlanIFaceCFG::~WlanIFaceCFG(){
	delete cfg_ptr;
}

bool WlanIFaceCFG::json2config(cJSON* jsoncfg, interface_cfg_t* cfg){
	wlaniface_cfg_t* config = (wlaniface_cfg_t*)cfg;
	if(jsoncfg == NULL || config == NULL ||
		!cJSON_HasObjectItem(jsoncfg, CFGBOOTPROTOKEY)||
		!cJSON_HasObjectItem(jsoncfg, CFGTYPEKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGIPADDRKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGGATWAYKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGMASKKEY) ||
		!cJSON_HasObjectItem(jsoncfg, CFGWIFINAME) ||
		!cJSON_HasObjectItem(jsoncfg, CFGWIFIPWD)){
			return false;
	}
	config->bootproto = (bootproto_t)cJSON_GetObjectItem(jsoncfg, CFGBOOTPROTOKEY)->valueint;
	config->type = (nettype_t)cJSON_GetObjectItem(jsoncfg, CFGTYPEKEY)->valueint;
	config->ipaddr.assign(cJSON_GetObjectItem(jsoncfg, CFGIPADDRKEY)->valuestring);
	config->gatway.assign(cJSON_GetObjectItem(jsoncfg, CFGGATWAYKEY)->valuestring);
	config->mask.assign(cJSON_GetObjectItem(jsoncfg, CFGMASKKEY)->valuestring);
	config->wifi_name.assign(cJSON_GetObjectItem(jsoncfg, CFGWIFINAME)->valuestring);
	config->wifi_pwd.assign(cJSON_GetObjectItem(jsoncfg, CFGWIFIPWD)->valuestring);
	return true;
}

bool WlanIFaceCFG::config2json(interface_cfg_t* cfg, cJSON* jsoncfg){
	wlaniface_cfg_t* config = (wlaniface_cfg_t*)cfg;
	if(cfg == NULL || jsoncfg == NULL)
		return false;
	cJSON_AddNumberToObject(jsoncfg, CFGBOOTPROTOKEY, config->bootproto);
	cJSON_AddNumberToObject(jsoncfg, CFGTYPEKEY, config->type);
	cJSON_AddStringToObject(jsoncfg, CFGIPADDRKEY, config->ipaddr.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGGATWAYKEY, config->gatway.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGMASKKEY, config->mask.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGWIFINAME, config->wifi_name.c_str());
	cJSON_AddStringToObject(jsoncfg, CFGWIFIPWD, config->wifi_pwd.c_str());
	return true;
}

bool WlanIFaceCFG::prepar(const interface_cfg_t & cfg){
	char cmd_buf[128] = {0};
	wlaniface_cfg_t & wificfg = (wlaniface_cfg_t &)cfg;
	// disconnect from current wifi
	system("wifi -d");
	LOG_DEBUG << "wifi disconnected.";
	// reconnect to wifi
	sprintf(cmd_buf, "wifi -c %s %s", 
			wificfg.wifi_name.c_str(), wificfg.wifi_pwd.c_str());
	system(cmd_buf);
	LOG_DEBUG << "construct command: " << cmd_buf;

	int interval = 2, time = 5;
	for(;time > 0; time--){
		if(this->isRunning() && this->running){
			return true;
		}
		sleep(2);
	}
	return false;
}

bool WlanIFaceCFG::interface_cfg(const interface_cfg_t & cfg){
	if(prepar(cfg) == false)
		return false;
	interface_base_cfg(cfg);
	return true;
}

bool WlanIFaceCFG::interface_cfg(){
	return interface_cfg(*cfg_ptr);
}

bool WlanIFaceCFG::interface_cfg(cJSON* jsoncfg){
	if(json2config(jsoncfg, cfg_ptr)){
		return interface_cfg(*cfg_ptr);
	}
	else{
		return false;
	}
	return true;
}

/*================USBEtheIFaceCFG=====================*/

USBEtheIFaceCFG::USBEtheIFaceCFG(const std::string & ifaceName)
:InterfaceCFGABS(ifaceName)
{
	cfg_ptr = new interface_cfg_t;
	this->setUP(true);
}

USBEtheIFaceCFG::~USBEtheIFaceCFG(){
	delete cfg_ptr;
}

bool USBEtheIFaceCFG::interface_cfg(const interface_cfg_t & cfg){
	interface_base_cfg(cfg);
	return true;
}

bool USBEtheIFaceCFG::interface_cfg(){
	cfg_ptr->bootproto = AUTOIP;
	return interface_cfg(*cfg_ptr);
}

/*==================NetConfiger=========================*/

void* switch_thread_handler(void* args){
	NetConfiger* configer = (NetConfiger*)args;
	while(1){
		pthread_mutex_lock(&configer->config_mutex);
		configer->current_interface();
		if(configer->current_iface_type == ETH && 
			configer->ethernetCFG.isRunning() &&
			!configer->ethernetCFG.running){
			InterfaceBase::RouteItem dflt_route;
			int ret = configer->default_route(dflt_route);
			if(ret){
				InterfaceBase::delRoute(dflt_route);
				configer->current_iface_type = NONE;
				configer->current_iface_ptr = nullptr;
			}
		}
		if(configer->current_iface_type > ETH && configer->ethernetCFG.isRunning() &&
			configer->ethernetCFG.running){
			configer->ethernetCFG.interface_cfg();
		}
		if(configer->current_iface_type > WLAN && configer->wlanCFG.isRunning() &&
			configer->wlanCFG.running){
			LOG_INFO << "wlan is up. Switch to wlan...";
			configer->wlanCFG.interface_cfg();
		}
		else if(configer->current_iface_type == NONE && configer->usbethCFG.isRunning() &&
				configer->usbethCFG.running){
			LOG_DEBUG << "wlan is down. Switch to usbeth..";
			configer->usbethCFG.interface_cfg();
		}
		pthread_mutex_unlock(&configer->config_mutex);
		sleep(5);
	}
}

NetConfiger::NetConfiger()
:ethernetCFG(INTER_ETHERNET), wlanCFG(INTER_WLAN), usbethCFG(INTER_USBETH)
{
	pthread_mutex_init(&config_mutex, NULL);
	pthread_create(&switch_thread_pid, NULL, switch_thread_handler, this);
}

NetConfiger::~NetConfiger(){
	pthread_cancel(switch_thread_pid);
}

bool NetConfiger::default_route(InterfaceBase::RouteItem & route){
	auto & route_table = InterfaceBase::route_table;
	route_table.clear();
	InterfaceBase::parseRouteTable();
	for(auto route_ptr = route_table.begin(); 
		route_ptr != route_table.end(); route_ptr++)
	{
		if(route_ptr->dest_ip == "0.0.0.0"){
			route = *route_ptr;
			return true;
		}
	}
	return false;
			
}

void NetConfiger::current_interface(){
	InterfaceBase::RouteItem dflt_route;
	bool ret = default_route(dflt_route);
	if(ret){
		if(dflt_route.ifaceName == INTER_ETHERNET){
			current_iface_type = ETH;
			current_iface_ptr = &ethernetCFG;
		}
		else if(dflt_route.ifaceName == INTER_WLAN){
			current_iface_type = WLAN;
			current_iface_ptr = &wlanCFG;
		}
		else if(dflt_route.ifaceName == INTER_USBETH){
			current_iface_type = USBETH;
			current_iface_ptr = &usbethCFG;
		}
		else{
			current_iface_type = NONE;
			current_iface_ptr = nullptr;
		}
	}
	else{
		current_iface_type = NONE;
		current_iface_ptr = nullptr;
	}
}

/*==================NetworkManager======================*/

NetworkManager::NetworkManager(const std::string & ifaceName)
:InterfaceBase(ifaceName)
{
	
}

/*===============wpa_supplicant_info===========================*/

bool wpa_supplicant_info::read(const std::string & path){
	std::string filestr;
	std::string ssid_patten = "ssid=\"(.+)\"";
	std::string psk_patten = "psk=\"(.+)\"";
	int ret1, ret2;
	if(!file_read(path, filestr)){
		return false;
	}
	wifiName = regex(filestr.c_str(), ssid_patten.c_str(), ret1, 0);
	wifiPassword = regex(filestr.c_str(), psk_patten.c_str(), ret2, 0);
	return (ret1==REG_NOERROR&&ret2==REG_NOERROR)?true:false;
}

bool wpa_supplicant_info::write(const std::string & path){
	std::string write_patten = "ap_scan=1\n"\
								"network={\n"\
        						"ssid=\"%s\"\n"\
        						"key_mgmt=WPA-PSK\n"\
        						"psk=\"%s\"\n"\
       							"priority=2\n"\
								"}";
	char writebuf[128]{0};
	sprintf(writebuf, write_patten.c_str(), wifiName.c_str(), wifiPassword.c_str());
	std::string writestr = writebuf;
	LOG_DEBUG << "write wpa_supplicant: " << writestr;
	return file_write(path, writestr);
}

/*======================WifiManager=======================*/
void* fetch_network_param_thread(void* args){
	WifiManager* manager = reinterpret_cast<WifiManager*>(args);
	do{
		manager->wifi_scan();
		sleep(30);
	}while(1);
}

void* connection_thread(void* args){
	WifiManager* manager = reinterpret_cast<WifiManager*>(args);
	system("killall wpa_supplicant");
	SYSTEMCMD("wpa_supplicant -B -Dnl80211 -c %s -i %s",
				WPA_CONFIG_PATH, INTER_WLAN);
	if(manager->prepared()){
		LOG_INFO << "wifi connect success! Lunching dhcp...";
		SYSTEMCMD("udhcpc -i %s", INTER_WLAN);
		manager->_s_cb();
	}
	else{
		LOG_INFO << "wifi connect failed!";
		manager->_f_cb();
	}
	
}

WifiManager::WifiManager(const std::string & ifaceName)
:NetworkManager(ifaceName)
{
	this->setUP(true);
	if(_wpa_info.read(WPA_CONFIG_PATH)){
		//connect_async();
	}
	fetch_network_param_daemo();
}

WifiManager::~WifiManager(){
	setUP(false);
	fetch_network_param_stop();
}

bool WifiManager::prepared(){
	uint16_t intervals = 2;
	uint16_t times = 6;
	for(; times > 0; times--){
		if(this->isRunning() && this->running){
			if(system("udhcpc -i wlan0 -n") == 0){
				return true;
			}
		}
		sleep(intervals);
	}
	system("killall wpa_supplicant");
	return false;
}

void WifiManager::wifi_scan(){
	const char* wifiNamePatten = "SSID: (.+)";
	int ret;
	char readbuf[1024]{0};
	memset(readbuf, 0, sizeof(readbuf));
	FILE* popenfp = popen("iw dev wlan0 scan | grep SSID", "r");
	if(fread(readbuf, 1, sizeof(readbuf), popenfp) > 0){
		_wifi_info.clear();
		std::stringstream ss(readbuf);
		std::string readstr;
		while(getline(ss, readstr, '\n')){
			WifiManager::WifiInfo_t wifiinfo;
			wifiinfo.wifiName = regex(readstr.c_str(), wifiNamePatten, ret, 1);
			if(ret == REG_NOERROR){
				printf("scan wifi: %s\n", wifiinfo.wifiName.c_str());
				_wifi_info.push_back(wifiinfo);
			}
		}
	}
}

bool WifiManager::connect_async(){
	pthread_create(&_connect_pid, NULL, connection_thread, this);
	pthread_detach(_connect_pid);
}

bool WifiManager::connect(){
	system("killall wpa_supplicant");
	usleep(50 * 1000);
	SYSTEMCMD("wpa_supplicant -B -Dnl80211 -c %s -i %s",
				WPA_CONFIG_PATH, INTER_WLAN);
	if(this->prepared()){
		LOG_INFO << "wifi connect success! Lunching dhcp...";
		//SYSTEMCMD("udhcpc -i %s", INTER_WLAN);
		_s_cb();
		return true;
	}
	LOG_INFO << "wifi connect failed!";
	_f_cb();
	return false;
}

bool WifiManager::connect_async(const std::string & wifiName, 
							const std::string & wifiPassword, 
							ConnectStateCallback_t s_cb,
							ConnectStateCallback_t f_cb){
	_wpa_info.wifiName = wifiName;
	_wpa_info.wifiPassword = wifiPassword;
	_wpa_info.write(WPA_CONFIG_PATH);
	_s_cb = s_cb;
	_f_cb = f_cb; 
	return this->connect_async();
}

bool WifiManager::connect(const std::string & wifiName, 
							const std::string & wifiPassword,
							ConnectStateCallback_t s_cb,
							ConnectStateCallback_t f_cb){
	_wpa_info.wifiName = wifiName;
	_wpa_info.wifiPassword = wifiPassword;
	_s_cb = s_cb;
	_f_cb = f_cb;
	_wpa_info.write(WPA_CONFIG_PATH);
	return this->connect();
}

void WifiManager::fetch_network_param_daemo(){
	fetch_pid = new pthread_t;
	pthread_create(fetch_pid, NULL, fetch_network_param_thread, this);
}

void WifiManager::fetch_network_param_stop(){
	pthread_cancel(*fetch_pid);
	delete fetch_pid;
}