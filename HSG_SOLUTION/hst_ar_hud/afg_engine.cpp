#include <thread>
#include "afg_engine.h"
#include "main.h"
#include "uart.h"
#include "af_timer.h"
#include "af_bind.h"
#include "msg_host.h"
#include "adas_can_def.h"
#include "register_msg_host.h"
#include "debug_var_set.h"
extern void refresh_hsg();
using namespace msg_utility;
msg_host g_msg_host;
afg_engine::afg_engine(int argc, char **argv)
:application(argc,argv){
    _screen_width = 1024;
	_screen_height = 768;
	_win_width = 1024;
	_win_height = 768;

	_wposx = 0.f;
	_wposy = 0.f;
}
void afg_engine::resLoaded()
{
    printf("resloaded");
    fifo_debuger::init_var_set_fifo();
    hud::init_controls();
    reg_debug();
    register_msg_host(g_msg_host);

    auto is_valid=[](u16 cmd_tag)->bool{
        if(num_in_range(cmd_tag,Vehicle_frame_a1,Vehicle_frame_c)
        || num_in_range(cmd_tag,Vehicle_frame_b0,Vehicle_frame_b7)
        || num_in_range(cmd_tag,Pedestrian_frame_a,Pedestrian_frame_c)
        || num_in_range(cmd_tag,lane_frame_1_h,lane_frame_c)
        || num_in_range(cmd_tag,TSR_frame_A,TSR_frame_C)
        || cmd_tag==AEB_WARNING){
            return true;
        }
        return false;
    };
    thread thd_uart_com([&]{
        const int read_buff_len=0x200;
        const int param_buff_len=read_buff_len*2;
        const u8 valid_cmd_len=10;
        u8 read_buff[read_buff_len];
        u8 param_buff[param_buff_len];
        u16 rear_id{0},front_id{0};
        union{
            u8 cmd_value[valid_cmd_len];
            u16 cmd_tag;
        } cmd_head;
        int uart_fd=openport("/dev/tty0");
        if(uart_fd>0){
            setport(uart_fd,57600,8,1,'n');
            printf("enter loop of receive uart message!\n");
            while (true){
                wait_fd_read_eable(uart_fd);
                int n=read(uart_fd,read_buff,read_buff_len);
                printf("got %d bytes\n",n);
                if(n==0)
                    continue;
                auto nid=front_id+n;
                if(nid<param_buff_len){
                    memcpy(param_buff+front_id,read_buff,n);
                    front_id=nid;
                } else {
                    auto remain_len=nid-param_buff_len;
                    auto leave_len=n-remain_len;
                    memcpy(param_buff+front_id,read_buff,remain_len);
                    memcpy(param_buff,read_buff+remain_len,leave_len);
                    front_id=leave_len;
                }
                for (; rear_id!=front_id;) {
                    static bool picked_valid_data=false;
                    static u8 valid_data_idx=2;
                    u8 cur_data=param_buff[rear_id];
                    auto n_id= next_id(rear_id,param_buff_len);
                    if (picked_valid_data){
                        cmd_head.cmd_value[valid_data_idx++]=cur_data;
                        if (valid_data_idx==valid_cmd_len)
                        {
                            valid_data_idx=2;
                            picked_valid_data=false;
                            g_msg_host.pick_valid_data(cmd_head.cmd_value,valid_cmd_len);
                        }
                    } else {
                        cmd_head.cmd_value[0]=cur_data;
                        cmd_head.cmd_value[1]=param_buff[nid];
                        if(is_valid(cmd_head.cmd_tag)){
                            picked_valid_data=true;
                            n_id= next_id(n_id,param_buff_len);
                        }
                    }
                    rear_id= n_id;
                }
            }
        } else {
            printf("fail to open serial port:/dev/tty0\n");
        }
    });
    thd_uart_com.detach();

}
void afg_engine::onUpdate()
{
    g_msg_host.execute_cmd();
    fifo_debuger::cmd_update();
}
AFGUI_APP(afg_engine)