#include <iostream>
#include "lvgl/lvgl.h"
#include "task_manager.h"

extern pthread_mutex_t lv_lock;

TaskManagerImpl::TaskManagerImpl(){
	this->Stop_.store(true);
}

TaskManagerImpl::~TaskManagerImpl(){
	this->stop();
	deviceManager_.stop();
}

bool TaskManagerImpl::run(){
	deviceManager_.run();
	if(this->Stop_.load() == true){
		this->Stop_.store(false);
		this->CoreThread_ = std::thread(std::bind(&TaskManagerImpl::coreThreadFunc_, this));
		this->CoreThread_.detach();
		return true;
	}
	return false;
}

void TaskManagerImpl::stop(){
	this->Stop_.store(true);
	deviceManager_.stop();
}

bool TaskManagerImpl::registerDevice(DeviceDescribe describe){
	return deviceManager_.registerDevice(describe);
}

void TaskManagerImpl::unregisterDevice(DeviceNameType id){
	return deviceManager_.unregisterDevice(id);
}

void TaskManagerImpl::registerPermenentTask(std::shared_ptr<DeviceTask> task){
	PermenentTasks_.push_back(task);
}

void TaskManagerImpl::registerTempTask(std::shared_ptr<DeviceTask> task){
	TempTasks_.push_back(task);
}

void TaskManagerImpl::coreThreadFunc_(){
	while(1){
		if(this->Stop_.load())break;
		for(auto & task : TempTasks_){
			if(!deviceManager_.isRunning())break;
			deviceManager_.submitePLCTask(task);
		}
		TempTasks_.clear();
		for(auto & task : PermenentTasks_){
			if(!deviceManager_.isRunning())break;
			deviceManager_.submitePLCTask(task);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}

TaskManager::~TaskManager(){
	this->stop();
}

void TaskManager::registerReadDatablockTask(const std::string & deviceID,
											PLCAddress_t addrType,
											const std::string & addrStart,
											size_t size)
{
	auto task = std::make_shared<DataBlockTask>();
	task->pDeviceTask = std::make_shared<DeviceTask>();
	task->pPayload = std::make_shared<DataPayload>();
	task->pDeviceTask->deviceID = deviceID;
	task->pDeviceTask->addrType = addrType;
	task->pDeviceTask->address = addrStart;
	task->pDeviceTask->size = size;
	task->pDeviceTask->payload = task->pPayload;
	task->pDeviceTask->PLCTaskSuccessCallback = std::bind(&TaskManager::readDatablockSuccessFunc_, this, task);
	task->pDeviceTask->PLCTaskFailedCallback = std::bind(&TaskManager::readDatablockFailFunc_, this, task);
	DataBlockTasks_.push_back(task);
	registerPermenentTask(task->pDeviceTask);
}

void TaskManager::readDatablockSuccessFunc_(std::shared_ptr<DataBlockTask> pTask){
	pthread_mutex_lock(&lv_lock);
	for(auto & cb : pTask->arrayTempSuccessCallback){
		cb();
	}
	pTask->arrayTempSuccessCallback.clear();
	for(auto & cb : pTask->arraySuccessCallback){
		cb();	
	}
	pthread_mutex_unlock(&lv_lock);
}

void TaskManager::readDatablockFailFunc_(std::shared_ptr<DataBlockTask> pTask){
	std::cout << "Read datablock [" <<
				pTask->pDeviceTask->address << "] error.";
	if(connectionLostCallback_)
		connectionLostCallback_();
}

void TaskManager::registerUIFlashTask(const std::string & addrStart,
							UITaskCallback cbk)
{
	for(auto & taskIter : DataBlockTasks_){
		if(taskIter->pDeviceTask->address == addrStart){
			taskIter->arraySuccessCallback.push_back(std::bind(cbk, taskIter->pPayload.get()));
			break;
		}
	}
}

void TaskManager::registerOnceUIFlashTask(const std::string & addrStart,
							UITaskCallback cbk)
{
	for(auto & taskIter : DataBlockTasks_){
		if(taskIter->pDeviceTask->address == addrStart){
			taskIter->arrayTempSuccessCallback.push_back(std::bind(cbk, taskIter->pPayload.get()));
			break;
		}
	}
}

void TaskManager::fetchPLCValue(const std::string & addrStart,
								UITaskCallback cbk)
{
	for(auto & taskIter : DataBlockTasks_){
		if(taskIter->pDeviceTask->address == addrStart){
			cbk(taskIter->pPayload.get());
		}
	}
}
