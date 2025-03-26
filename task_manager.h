#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__
#include <string>
#include <list>
#include "plccore.h"

typedef std::function<void(DataPayload* data)> UITaskCallback;
typedef std::function<void()> DataBlockTaskCallback;

typedef struct{
	std::shared_ptr<DeviceTask> pDeviceTask;
	std::shared_ptr<DataPayload> pPayload;
	std::vector<DataBlockTaskCallback> arraySuccessCallback;
	std::vector<DataBlockTaskCallback> arrayTempSuccessCallback;
} DataBlockTask;


class ActionListener{
public:
	virtual void on_success() = 0;
	virtual void on_fail() = 0;
};

class ConnectionActionListener: public ActionListener{
public:
	virtual void connect_lost() = 0;
};


class TaskManagerImpl{
protected:
	std::atomic<bool> Stop_;
	std::atomic<bool> IsConnected_;
	std::thread CoreThread_;
	std::list<std::shared_ptr<DeviceTask>> PermenentTasks_;
	std::list<std::shared_ptr<DeviceTask>> TempTasks_;

	DeviceManager deviceManager_;
	DeviceDescribe deviceDescribe_;

	ConnectionActionListener* pDeviceStatusCallback_;
public:
	TaskManagerImpl();
	~TaskManagerImpl();
    bool connectDevice(DeviceDescribe describe, ConnectionActionListener & callback);
    void disconnectDevice();
	bool isConnected(){return IsConnected_.load();};
	void registerPermenentTask(std::shared_ptr<DeviceTask> task);
	void registerTempTask(std::shared_ptr<DeviceTask> task);
	bool run();
	void stop();
private:
	void coreThreadFunc_();
};

class TaskManager : public TaskManagerImpl{
private:
	std::function<void()> connectionLostCallback_;
	std::vector<std::shared_ptr<DataBlockTask>> DataBlockTasks_;
public:
	TaskManager(){};
	~TaskManager();
	void setCallback(std::function<void()> callback){connectionLostCallback_ = callback;};
	void registerReadDatablockTask(const std::string & deviceID,
									PLCAddress_t addrType,
									const std::string & addrStart,
									size_t size);
	void registerDataArriveAction(const std::string & addrStart,
							UITaskCallback cbk);
	void registerDataArriveOnce(const std::string & addrStart,
							UITaskCallback cbk);
	void fetchPLCValue(const std::string & addrStart,
							UITaskCallback cbk);
	
	friend class TaskListenerAction;
private:
	void readDatablockSuccessFunc_(std::shared_ptr<DataBlockTask> pTask);
	void readDatablockFailFunc_(std::shared_ptr<DataBlockTask> pTask);
};

#endif