#include <random>
#include "plccore.h"
#include "limlog.h"

#define TASKMAXTHRESHHOLD               5

uint64_t generateRandomDeviceId() {
    std::random_device rd;  // 用于获取硬件随机数
    std::mt19937_64 gen(rd());  // 使用 Mersenne Twister 引擎生成 64 位随机数
    std::uniform_int_distribution<uint64_t> distrib(0, UINT64_MAX); // 定义 64 位无符号整数的均匀分布

    return distrib(gen);  // 生成随机数
}

DeviceManager::DeviceManager()
:TaskMaxThreshHold_(TASKMAXTHRESHHOLD),
TaskSize_(0)
{

}

DeviceManager::~DeviceManager(){

}

bool DeviceManager::registerDevice(DeviceDescribe describe){
    if(describe.protoType == PROTOCOL_TYPE_MC){
        auto device = std::make_shared<MCProtocol>(describe.ipAddr, 
                                                    describe.port);
        if(!device->connect()){
            return false;
        }
        else{
            deviceList_[describe.deviceName] = device;
            return true;
        }
    }
    else if(describe.protoType == PROTOCOL_TYPE_S7){
        auto device = std::make_shared<S7Protocol>(describe.ipAddr,
                                                describe.port,
                                                describe.siemensPLCType);
        if(!device->connect()){
            return false;
        }
        else{
            deviceList_[describe.deviceName] = device;
            return true;
        }
    }
#ifdef CONFIG_PROTOCOL_MODBUS
    else if(describe.protoType == PROTOCOL_TYPE_MODBUS_TCP){
        auto device = std::make_shared<ModBusTCPProtocol>(describe.ipAddr, 
                                                describe.port,
                                                describe.modbus_slave);
        if(!device->connect()){
            return false;
        }
        else{
            deviceList_[describe.deviceName] = device;
            return true;
        }
    }
    else if(describe.protoType == PROTOCOL_TYPE_MODBUS_RTU){
        auto device = std::make_shared<ModBusRTUProtocol>(describe.serialDeviceName,
                                                describe.baut,
                                                describe.parity,
                                                describe.dataBits,
                                                describe.stopBits,
                                                describe.modbus_slave);
        if(!device->connect()){
            return false;
        }
        else{
            deviceList_[describe.deviceName] = device;
            return true;
        }
    }
#endif
    return false;
}

bool DeviceManager::reconnectDevice(DeviceNameType id){
    auto deviceItem = deviceList_.find(id);
    if(deviceItem != deviceList_.end()){
        return deviceItem->second->reconnect();
    }
    return false;
}

void DeviceManager::unregisterDevice(DeviceNameType id){
    auto devicePtr = deviceList_.find(id);
    if(devicePtr == deviceList_.end())
        return;
    deviceList_.erase(id);
}

bool DeviceManager::submitePLCTask(std::shared_ptr<DeviceTask> task){
    std::unique_lock<std::mutex> lock(TaskMutex_);
    TaskFull_.wait(lock, [&](){return TaskQueue_.size()<TaskMaxThreshHold_;});
    TaskQueue_.push(task);
    TaskEmpty_.notify_all();
    return true;
}

void DeviceManager::run(){
    this->Stop_.store(false);
    this->CoreThread_ = std::thread(std::bind(&DeviceManager::coreThreadFunc_, this));
    this->CoreThread_.detach();
}

void DeviceManager::stop(){
    this->Stop_.store(true);
    TaskFull_.notify_all();
    TaskEmpty_.notify_all();
}

void DeviceManager::coreThreadFunc_(){
    while(1){
        std::unique_lock<std::mutex> lock(TaskMutex_);
        TaskEmpty_.wait(lock, [&](){return !TaskQueue_.empty() || Stop_.load();});
        if(Stop_.load()) return;
        while(!TaskQueue_.empty()){
            auto & TaskIter = TaskQueue_.front();
            auto devicePair = deviceList_.find(TaskIter->deviceID);
            err_code_t ret = -2;
            if(devicePair != deviceList_.end() && devicePair->second->is_connected()){
                auto devicePtr = devicePair->second;
                if(TaskIter->rw == PLC_ACCESS_READ){
                    switch(TaskIter->addrType){
                        case PLC_ADDRESS_BOOL:
                            ret = devicePtr->batch_read_bool(TaskIter->address, 
                                        TaskIter->payload->dbool,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_BYTE:
                            ret = devicePtr->batch_read_byte(TaskIter->address,
                                        TaskIter->payload->dbyte,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_SHORT:
                            ret = devicePtr->batch_read_short(TaskIter->address,
                                        TaskIter->payload->dshort,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_USHORT:
                            ret = devicePtr->batch_read_ushort(TaskIter->address,
                                        TaskIter->payload->dushort,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_INT32:
                            ret = devicePtr->batch_read_int32(TaskIter->address,
                                        TaskIter->payload->dint32,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_UINT32:
                            ret = devicePtr->batch_read_uint32(TaskIter->address,
                                        TaskIter->payload->duint32,
                                        TaskIter->size);
                            break;
                        case PLC_ADDRESS_FLOAT:
                            ret = devicePtr->batch_read_float(TaskIter->address,
                                        TaskIter->payload->dfloat,
                                        TaskIter->size);
                            break;
                        default:
                            break;
                    }
                }
                else if(TaskIter->rw == PLC_ACCESS_WRITE){
                    switch (TaskIter->addrType){
                        case PLC_ADDRESS_BOOL:
                            ret = devicePtr->batch_write_bool(TaskIter->address,
                                        TaskIter->payload->dbool);
                            break;
                        case PLC_ADDRESS_BYTE:
                            ret = devicePtr->batch_write_byte(TaskIter->address,
                                        TaskIter->payload->dbyte);
                            break;
                        case PLC_ADDRESS_SHORT:
                            ret = devicePtr->batch_write_short(TaskIter->address,
                                        TaskIter->payload->dshort);
                            break;
                        case PLC_ADDRESS_USHORT:
                            ret = devicePtr->batch_write_ushort(TaskIter->address,
                                        TaskIter->payload->dushort);
                            break;
                        case PLC_ADDRESS_INT32:
                            ret = devicePtr->batch_write_int32(TaskIter->address,
                                        TaskIter->payload->dint32);
                            break;
                        case PLC_ADDRESS_UINT32:
                            ret = devicePtr->batch_write_uint32(TaskIter->address,
                                        TaskIter->payload->duint32);
                            break;
                        case PLC_ADDRESS_FLOAT:
                            ret = devicePtr->batch_write_float(TaskIter->address,
                                        TaskIter->payload->dfloat);
                            break;
                        default:
                            break;
                    }
                }

            }
            else if(devicePair == deviceList_.end()){
                LOG_ERROR << "Device " << TaskIter->deviceID <<
                        " not found!";
            }
            else if(devicePair->second->is_connected()){
                LOG_ERROR << "Device " << TaskIter->deviceID << 
                        " net connected!";
            }

            //PLC Task success
            if(ret == MC_ERROR_CODE_SUCCESS || 
                ret == S7_ERROR_CODE_SUCCESS){
                TaskIter->PLCTaskSuccessCallback();
            }
            else{
                LOG_ERROR << "Operate error. Ret=" << (int)ret;
                TaskIter->PLCTaskFailedCallback();
            }
            TaskQueue_.pop();
            TaskFull_.notify_all();
        }
    }
}