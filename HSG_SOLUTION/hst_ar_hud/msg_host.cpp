//
// Created by v_f_z on 2022/6/29.
//
#include <thread>
#include <string.h>
#include "msg_host.h"
using namespace std;
namespace msg_utility{
    int next_id(int cur_id,int id_range,int steps){
        cur_id+= steps;
        return cur_id%id_range;
    }
    void msg_host::pick_valid_data(u8 *pbuffer, int len) {

        auto& cur_cmd=_cmd_queque[_front_id];
        memcpy(cur_cmd,pbuffer,len);
        auto nid= next_id(_front_id,que_length);
        if(nid==_rear_id){
            printf("too fast.slow down please!!!!\n");
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        _front_id=nid;
    }
    int msg_host::execute_cmd() {
        int need_refresh_cnt=0;
        for (; _rear_id!=_front_id; _rear_id= next_id(_rear_id,que_length)) {
            u8* exe_cmd=_cmd_queque[_rear_id];
            u16* pcmd_tag=(u16*)exe_cmd;
            bool need_refresh=false;
            auto cmd_id=_dic_msg_handle.find(*pcmd_tag);
            if (cmd_id!=_dic_msg_handle.end()){
                need_refresh=cmd_id->second(exe_cmd+2,cmd_length-2);
            } else {
                need_refresh=_batch_cmd_handle(exe_cmd,cmd_length);
            }
            if (need_refresh){
                need_refresh_cnt++;
            }
        }
        return need_refresh_cnt;
    }
}