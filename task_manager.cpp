#include "task_manager.h"


TaskManagerImpl::~TaskManagerImpl(){
	this->stop();
}

bool TaskManagerImpl::run(){
	if(this->Stop_.load() == false){
		this->Stop_.store(true);
		this->CoreThread_ = std::thread(std::bind(TaskManagerImpl::coreThreadFunc_, this));
		this->CoreThread_.detach();
		return true;
	}
	return false;
}

void TaskManagerImpl::stop(){
	this->Stop_.store(true);
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
		
	}
}