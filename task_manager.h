#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__
#include <string>
#include <list>
#include "plccore.h"

typedef std::function<void(DataPayload* data)> UITaskCallback;
typedef std::function<void()> DataBlockTaskCallback;

typedef struct{
	std::string addrStart;
	size_t size;
	DataPayload payload;
} PLCDataBlock;

typedef struct{
	std::shared_ptr<PLCDataBlock> datablock;
	std::vector<std::shared_ptr<DataBlockTaskCallback>> successCallback;
	std::vector<std::shared_ptr<DataBlockTaskCallback>> failCallback;
} DataBlockTask;

class TaskManagerImpl{
private:
	std::atomic<bool> Stop_;
	std::thread CoreThread_;
	std::list<std::shared_ptr<DeviceTask>> PermenentTasks_;
	std::list<std::shared_ptr<DeviceTask>> TempTasks_;
public:
	TaskManagerImpl(){};
	~TaskManagerImpl();
	void registerPermenentTask(std::shared_ptr<DeviceTask> task);
	void registerTempTask(std::shared_ptr<DeviceTask> task);
	bool run();
	void stop();
private:
	void coreThreadFunc_();
};

class TaskManager : public TaskManagerImpl{
private:
	std::vector<std::shared_ptr<PLCDataBlock>> DataBlocks_;
	std::vector<std::shared_ptr<DataBlockTask>> DataBlockTasks_;
public:
	TaskManager(){};
	~TaskManager();
	void registerReadDatablockTask(const std::string & addrStart,
									size_t size);
	void registerUIFlashTask(const std::string & addrStart,
							size_t offset,
							UITaskCallback cbk);
};



#endif