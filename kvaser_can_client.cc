#include "modules/canbus/can_client/kvaser/kvaser_can_client.h"
#include <iostream>
using namespace AS::CAN;
using apollo::common::ErrorCode;
namespace apollo{
  namespace canbus{
    namespace can{
      using apollo::common::ErrorCode;
      bool KvaserCanClient::Init(const CANCardParameter &parameters){
	std::cout<<"calling init"<<std::endl;
/*	if(!parameters.has_circuit_id()){
	  AERROR << "No channel id is provided in paremeters. Plese provide these parameters"<<parameters.DebugString();
	  return false;
	}
	if(!parameters.has_bit_rate()){
	  AERROR << "No bit rate is provided in paremeters. Plese provide these parameters"<<parameters.DebugString();
	  return false;
	}
	if(!parameters.has_hardware_id()){
	  AERROR << "No hardware id is provided in paremeters. Plese provide these parameters" <<parameters.DebugString();
	  return false;
	}
	circuit_id_ = parameters.circuit_id(); 
	bit_rate_ = parameters.bit_rate();
	hardware_id_ = parameters.hardware_id();*/
	circuit_id_ = 0;
	bit_rate_ = 500000;
	hardware_id_ = 29949;
	return true;
      }
      ErrorCode KvaserCanClient::Start(){
	if(is_started_){
	  return ErrorCode::OK;
	}
	return_statuses ret_read, ret_write;
	ret_read = can_reader_.open(hardware_id_, circuit_id_, bit_rate_, false);
	if(ret_read != OK){
	  AERROR <<" cannot open can to read "<< ret_read<< GetErrorString(ret_read);
	  return ErrorCode::CAN_CLIENT_ERROR_BASE;
	}
	ret_write = can_reader_.open(hardware_id_, circuit_id_, bit_rate_, false);

	if(ret_write != OK){
	  AERROR <<" cannot open can to write "<< ret_write<< GetErrorString(ret_write);
	  return ErrorCode::CAN_CLIENT_ERROR_BASE;
	}
	is_started_ = true;
	return ErrorCode::OK;
      }


      void KvaserCanClient::Stop(){
	if(is_started_){
	  is_started_ = false;
	  return_statuses ret;
	  ret = can_reader_.close();
	  if(ret != OK){
	    AERROR << "error closing reader"<< ret <<" , "<<GetErrorString(ret);
	  }
	  ret = can_writer_.close();
	  if(ret != OK){
	    AERROR << "error closing writer" << ret <<" , "<<GetErrorString(ret);
	  }
	  else{
	    AINFO << "close Kvaser can successfull ";
	  }
	}
      } 
      ErrorCode KvaserCanClient::Send(const std::vector<CanFrame> &frames,
	  int32_t *const frame_num) {
	if (!is_started_) {
	  AERROR << "Esd can client has not been initiated! Please init first!";
	  return ErrorCode::CAN_CLIENT_ERROR_SEND_FAILED;
	}
	for (size_t i = 0; i < frames.size() && i < MAX_CAN_SEND_FRAME_LEN; ++i) {
	  return_statuses ret;
	  unsigned char ch[1];
	  memcpy(ch, (char*)(frames[i].data), 1);
	  ret = can_writer_.write(frames[i].id, ch , frames[i].len, false);
	  if(!ret){
	    AERROR << "Write failed Error: "<<GetErrorString(ret) ;
	    return ErrorCode::CAN_CLIENT_ERROR_BASE;
	  }
	}
	return ErrorCode::OK;
      }
      ErrorCode KvaserCanClient::Receive(std::vector<CanFrame> *const frames,
	  int32_t *const frame_num) {
	long id;
	uint8_t msg[8];
	unsigned int size;
	bool extended;
	unsigned long t;
	return_statuses ret;
	if (!is_started_) {
	  AERROR << "Esd can client is not init! Please init first!";
	  return ErrorCode::CAN_CLIENT_ERROR_RECV_FAILED;
	}

	if (*frame_num > MAX_CAN_RECV_FRAME_LEN || *frame_num < 0) {
	  AERROR << "recv can frame num not in range[0, " << MAX_CAN_RECV_FRAME_LEN
	    << "], frame_num:" << *frame_num;
	  // TODO(Authors): check the difference of returning frame_num/error_code
	  return ErrorCode::CAN_CLIENT_ERROR_FRAME_NUM;
	}
	for(int32_t i=0; i< *frame_num && i < MAX_CAN_RECV_FRAME_LEN; ++i)        {
	  ret = can_reader_.read(&id, msg, &size, &extended, &t);
	  if(ret == OK){
	  CanFrame cf;
	  cf.id = id;
	  cf.len = size;
	  std::memcpy(cf.data, msg, size);
	  frames->push_back(cf);
	  }
	  else if(ret == NO_MESSAGES_RECEIVED){
	    i--;
	  }
	  else{
	    AERROR <<" error reading can";
	    return ErrorCode::CAN_CLIENT_ERROR_FRAME_NUM;
	  }    
	}
	return ErrorCode::OK;
      }

      std::string KvaserCanClient::GetErrorString(const int32_t ret) {
	return "called";
      }
    }
  }
}



