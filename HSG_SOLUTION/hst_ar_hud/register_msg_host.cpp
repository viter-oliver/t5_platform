//
// Created by v_f_z on 2022/6/30.
//
/*
car0_show
car0_show
 * */
#include "register_msg_host.h"
#include "adas_can_def.h"
#include "af_bind.h"
#include "afg.h"
#include "debug_var_set.h"
using namespace msg_utility;
using namespace auto_future;
using namespace std;
extern void refresh_hsg();
#define HUD_DEBUG
namespace hud {
    ft_hud_projector* pprojector;
    ft_hud_4_time_curve_3d* pleft_lane;
    ft_hud_4_time_curve_3d* pright_lane;
    ft_hud_obj_3d* pmain_car;
    ft_hud_obj_3d* pcar0;
    ft_hud_obj_3d* pcar1;
    ft_hud_obj_3d* pcar2;
    float lc0=0.f,lc1=0.f,lc2=0.f,lc3=0.f;
    float rc0=0.f,rc1=0.f,rc2=0.f,rc3=0.f;
    void set_fovy(float fovy){
        pprojector->set_fovy(fovy);
    }
    void set_clip(float ffar,float fnear){
        pprojector->set_far(ffar);
        pprojector->set_near(fnear);
    }
    void init_controls(){
        pprojector=(ft_hud_projector*)get_aliase_ui_control("hud_far");
        pleft_lane=(ft_hud_4_time_curve_3d*) get_aliase_ui_control("left_lane_show");
        pright_lane=(ft_hud_4_time_curve_3d*) get_aliase_ui_control("right_lane_show");
        pmain_car=(ft_hud_obj_3d*)get_aliase_ui_control("main_car_show");
        pcar0=(ft_hud_obj_3d*)get_aliase_ui_control("car0_show");
        pcar1=(ft_hud_obj_3d*)get_aliase_ui_control("car1_show");
        pcar2=(ft_hud_obj_3d*)get_aliase_ui_control("car2_show");
#ifdef HUD_DEBUG
        pleft_lane->set_visible(true);
        pright_lane->set_visible(true);
        pmain_car->set_visible(true);
        pcar0->set_visible(true);
        pcar1->set_visible(true);
        pcar2->set_visible(true);
#endif
    }
    void calcu_left_lane(){
        pleft_lane->set_coeff(lc0,lc1,lc2,lc3);
    }
    void calcu_right_lane(){
        pright_lane->set_coeff(rc0,rc1,rc2,rc3);
    }
}
void reg_debug(){
    fifo_debuger::reg_command_handle([](char* pcmd_buff){
        string str_cmd(pcmd_buff);
        auto eq_pos=str_cmd.find_last_of('=');
        if(eq_pos != -1) {
            string alias_name=str_cmd.substr(0,eq_pos);
            string str_value=str_cmd.substr(eq_pos+1);
             auto pt_pos=str_value.find('.');
            if(pt_pos!=-1){
                float fvalue=atof(str_value.c_str());
                printf("f alias:%s =%f\n",alias_name.c_str(),fvalue);
                set_property_aliase_value_T(alias_name,fvalue);
            } else {
                int ivalue=atoi(str_value.c_str());
                printf("i alias:%s =%f\n",alias_name.c_str(),ivalue);
                set_property_aliase_value_T(alias_name,ivalue);
            }
        } else {
            

        }
        return true;
    });
    
}
void register_msg_host(msg_utility::msg_host& mh){
    mh.register_batch_cmd_handle([](u8* pbuff,u32 len)->bool{
        u16* pcmd=(u16*)pbuff;
        if(num_in_range(*pcmd,Vehicle_frame_b0,Vehicle_frame_b7)){
            struct GNU_DEF Vehicle_frame_bx{
                u8 Addition_Vehicle_Number:2;
                u8 Vehicle_ID:6;
                u16 Addition_Vehicle_A_PosX:12;//factor 0.0625
                s16 Addition_Vehicle_A_PosY:10;//factor 0.0625 -32.f
                u16 Addition_Vehicle_A_Type:3;
                u16 Vehicle_ID_2:6;
                u16 Addition_Vehicle_B_PosX:12;//factor 0.0625
                s16 Addition_Vehicle_B_PosY:10;// factor 0.0625 -32.f
                u16 Addition_Vehicle_B_Type:3;//no vehicle,car,bus,truck,special car
            };
            Vehicle_frame_bx* pcan=(Vehicle_frame_bx*)pbuff+2;
            float Addition_Vehicle_A_PosX=pcan->Addition_Vehicle_A_PosX*0.0625f;
            float Addition_Vehicle_A_PosY=pcan->Addition_Vehicle_A_PosY*0.0625f-32.f;
            float Addition_Vehicle_B_PosX=pcan->Addition_Vehicle_B_PosX*0.0625f;
            float Addition_Vehicle_B_PosY=pcan->Addition_Vehicle_B_PosY*0.0625f-32.f;
            //printf("car id:%d posx=%f,posy=%f\n",pcan->Vehicle_ID,Addition_Vehicle_A_PosX,Addition_Vehicle_A_PosY);
            //sprintf("car id:%d posx=%f,posy=%f\n",pcan->Vehicle_ID_2,Addition_Vehicle_B_PosX,Addition_Vehicle_B_PosY);
     
            auto vehicle_id=*pcmd-Vehicle_frame_b0;
            if(vehicle_id==0){
                hud::pcar0->set_transz(Addition_Vehicle_A_PosX);
                hud::pcar0->set_transx(Addition_Vehicle_A_PosY);
                hud::pcar1->set_transz(Addition_Vehicle_B_PosX);
                hud::pcar1->set_transx(Addition_Vehicle_B_PosY);
            } else if(vehicle_id==1){
                hud::pcar2->set_transz(Addition_Vehicle_A_PosX);
                hud::pcar2->set_transx(Addition_Vehicle_A_PosY);
            }
        } else if(num_in_range(*pcmd,Pedestrian_frame_bb,Pedestrian_frame_bd)){
            struct GNU_DEF St_Pedestrian_frame_bx{
                u8 Addition_Pedestrian_Number:2;
                u8 Pedestrian_ID:6;
                u16 Addition_Pedestrian_A_PosX:12;//0.0625 0
                u16 Addition_Pedestrian_A_PosY:10;//0.0625 -32
                u16 Target_Pedestrian_A_Type:3;
                u8 Pedestrian_ID_2:6;
                u16 Addition_Pedestrian_B_PosX:12;//0.0625 0
                u16 Addition_Pedestrian_B_PosY:10;//0.0625 -32
                u16 Target_Pedestrian_B_Type:3;
            };
        } else if(num_in_range(*pcmd,TSR_frame_B1,TSR_frame_B3)) {
            struct GNU_DEF st_TSR_frame_B{
                u8 Vision_only_Sign_Type;
                u32 reserved:24;
                u32 CanMsgEnder:1;
                u32 reserved2:7;
            };
        }
        return false;
    });
    mh.register_msg_handle(Vehicle_frame_a1,[&](u8* pbuff,int len){
        struct GNU_DEF St_Vehicle_frame_a1{
            u8 FCW:1;
            u8 reserved_1:1;
            u8 Vehicle_ID:6;
            u16 Target_Vehicle_PosX:12;//factor:0.0625
            u16 reseved_2:12;
            u16 Target_Vehicle_PosY:10;//factor:0.0625 -31.9375
            u16 reseved_3:6;
            u16 Target_Vehicle_Type:3;
            u16 reseved_4:13;
        };
        St_Vehicle_frame_a1* pcan=(St_Vehicle_frame_a1*)pbuff;
        float Target_Vehicle_PosX=pcan->Target_Vehicle_PosX*0.0625f;
        float Target_Vehicle_PosY=pcan->Target_Vehicle_PosY*0.0625f-32.f;//*100 transfer to cm
        u16* pcanid=(u16*)pbuff;
        static int dcnt=0;
        if(dcnt==0){
          printf("main car(%x): posx:%f,posy:%f___",*pcanid,Target_Vehicle_PosX,Target_Vehicle_PosY); 
          for(int ix=0;ix<sizeof(St_Vehicle_frame_a1);ix++){
            printf("%x|",pbuff[ix]);
          } 
          printf("\n");
        }
        dcnt++;
        dcnt%=100;
        hud::pmain_car->set_transz(Target_Vehicle_PosX);
        hud::pmain_car->set_transx(Target_Vehicle_PosY);
        return false;
    });
    mh.register_msg_handle(Vehicle_frame_a2,[&](u8* pbuff,int len){
        struct GNU_DEF St_Vehicle_frame_a2{
            u16 CAN_VIS_OBS_TTC_WITH_ACC:10;//0.01
            u16 reseved_1:6;
            u16 Target_Vehicle_VelX:12;//0.0625 -127.93
            u16 reseved_2:4;
            u16 Target_Vehicle_AccelX:10;//0.03 -14.97
            u16 reseved_3:6;
            u8 Target_Vehicle_Width;//0.05
            u8 Target_Vehicle_Confidence:6;//0.02
            u16 reseved_4:2;
        };
        St_Vehicle_frame_a2* pcan=(St_Vehicle_frame_a2*)pbuff;
        float Target_Vehicle_Width=pcan->Target_Vehicle_Width*0.05f;
        //printf("main car width=%f\n",Target_Vehicle_Width);
        hud::pmain_car->set_size(Target_Vehicle_Width,50.f);
        return false;
    });
    mh.register_msg_handle(Vehicle_frame_c,[&](u8* pbuff,int len){
        struct GNU_DEF St_Vehicle_frame_c{
            u32 frame_id;
            u8 speed;
        };
        St_Vehicle_frame_c* pcan=(St_Vehicle_frame_c*)pbuff;
        return false;
    });
    mh.register_msg_handle(Pedestrian_frame_a,[&](u8* pbuff,int len){
        struct GNU_DEF St_Pedestrian_frame_a{
            u8 Target_Pedestrian_ID:6;
            u8 reserved_1:2;
            u16 Target_Pedestrian_PosX:12;//0.0625
            u16 Target_Pedestrian_PosY:10;//0.0625 -32.0
            u16 ttc:10;//0.01
            u16 Target_Pedestrian_VelX:12;//0.0625 -127.93
            u16 Target_Pedestrian_VelY:8;//0.0625  -8
            u16 Target_Pedestrian_Type:3;
            u16 reserved_2:1;
        };
        St_Pedestrian_frame_a* pcan=(St_Pedestrian_frame_a*)pbuff;
        return false;
    });
    mh.register_msg_handle(lane_frame_1_l,[&](u8* pbuff,int len){
        struct GNU_DEF St_lane_frame_1_l{
            u8 Lane_Type:4;
            u8 Quality:2;
            u8 reserved:2;//factor 0.0625
            s16 Position;//0.00391 -128
            u16 Curvature;//0.000000976563 -0.032
            u16 CurvatureDerivative;//0.00000000372529 -0.0001
            u8 WidthMarking;//0.01
        };
        St_lane_frame_1_l* pcan=(St_lane_frame_1_l*)pbuff;
        hud::lc0=pcan->Position * 0.00391-128;
        hud::lc2=pcan->Curvature * 0.000000976563-0.032;
        hud::lc3=pcan->CurvatureDerivative*0.00000000372529-0.0001;
        hud::calcu_left_lane();
        return false;
    });
    mh.register_msg_handle(lane_frame_1_h,[&](u8* pbuff,int len){
        struct GNU_DEF St_lane_frame_1_h{
            u16 Heading_Angle;//0.0000108949 -0.357
            u8 ViewRangeStart;//0.5
            u8 ViewRangeEnd;//0.5
            u8 LaneCrossing:1;//0-no crossing,1-crossing
            u8 LaneMarkColor:3;//0-white,1-yellow
        };
        St_lane_frame_1_h* pcan=(St_lane_frame_1_h*)pbuff;
        hud::lc1=pcan->Heading_Angle*0.0000108949 - 0.357;
        hud::calcu_left_lane();
        return false;
    });
    mh.register_msg_handle(lane_frame_2_l,[&](u8* pbuff,int len){
        struct GNU_DEF St_lane_frame_2_l{
            u8 Lane_Type:4;
            u8 Quality:2;
            u8 reserved:2;//factor 0.0625
            s16 Position;//0.00390625 -128
            u16 Curvature;//0.000000976563 -0.032
            u16 CurvatureDerivative;//0.00000000372529 -0.0001
            u8 WidthMarking;
        };
        St_lane_frame_2_l* pcan=(St_lane_frame_2_l*)pbuff;
        hud::rc0=pcan->Position * 0.00391-128;
        hud::rc2=pcan->Curvature * 0.000000976563-0.032;
        hud::rc3=pcan->CurvatureDerivative*0.00000000372529-0.0001;
        hud::calcu_right_lane();
        return false;
    });
    mh.register_msg_handle(lane_frame_2_h,[&](u8* pbuff,int len){
        struct GNU_DEF St_lane_frame_2_h{
            u16 Heading_Angle;//0.0000108949
            u8 ViewRangeStart;//0.5
            u8 ViewRangeEnd;//0.5
            u8 LaneCrossing:1;//0-no crossing,1-crossing
            u8 LaneMarkColor:3;//0-white,1-yellow
        };
        St_lane_frame_2_h* pcan=(St_lane_frame_2_h*)pbuff;
        hud::rc1=pcan->Heading_Angle*0.0000108949 - 0.357;
        hud::calcu_right_lane();
        return false;
    });
    mh.register_msg_handle(TSR_frame_A,[&](u8* pbuff,int len){
        struct GNU_DEF St_TSR_frame_A{
            u8 Vision_only_Sign_Type;
            u32 reserved:24;
            u8 Vision_only_Sign_Type2;
            u32 reserved2:24;
        };
        St_TSR_frame_A* pcan=(St_TSR_frame_A*)pbuff;
        return false;
    });
    mh.register_msg_handle(AEB_WARNING,[&](u8* pbuff,int len){
        struct GNU_DEF St_AEB_WARNING{
            u8 SoundType:3;
            u8 TimeIndicator:2;
            u8 reserved:1;
            u8 ZeroSpeed:1;
            u8 HeadwayValid:1;
            u8 HeadvayMeasurement:7;//0.1
            u8 error:1;
            u8 LDW_OFF:1;
            u8 Left_LDW_ON:1;
            u8 Right_LDW_ON:1;
            u8 FCW_ON:1;
            u8 Peds_FCW:1;
            u8 PedsInDZ:1;
            u8 TSR_enabled:1;
        };
        St_AEB_WARNING* pcan=(St_AEB_WARNING*)pbuff;
        return false;
    });
}