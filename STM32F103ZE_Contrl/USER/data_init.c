#include "data_init.h"




static void Encoder1_data_init(void){
	

	
extern QueueHandle_t Encoder1_Overflow_Queue;
extern QueueHandle_t Encoder1_Status_Queue;		
extern QueueHandle_t Encoder2_Status_Queue;		
extern QueueHandle_t Motor1_PID_Parameter_Queue;
	
M1_PID *M1_speed_PID_wifiset;
extern M1_PID  Motor1_PID_struct_init;
M1_speed_PID_wifiset=&Motor1_PID_struct_init;

	


	
Encoder1_status_t *Encoder1_init;	
extern Encoder1_status_t Encoder1_struct_init;
Encoder1_init = &Encoder1_struct_init;	
	
	
Encoder2_status_t *Encoder2_init;	
extern Encoder2_status_t Encoder2_struct_init;
Encoder2_init = &Encoder2_struct_init;	

int Encoder1_overflow_count_init;	
	
/*PID初始化*/	
M1_speed_PID_wifiset->Kp=22.0;
M1_speed_PID_wifiset->Ki=2.0;
M1_speed_PID_wifiset->Kd=1.0;
xQueueOverwrite(Motor1_PID_Parameter_Queue,(void *)&M1_speed_PID_wifiset);
	
	
	
	
/*编码器溢出计数初始化*/
Encoder1_overflow_count_init=0;
xQueueOverwrite(Encoder1_Overflow_Queue,&Encoder1_overflow_count_init);





/*编码器状态队列初始化*/
#if Car_ctrl_speed_loop	
Encoder1_init->Encoder1_Speed=0.0;

Encoder2_init->Encoder2_Speed=0.0;
#endif

#if	Car_ctrl_length_loop
Encoder1_init->Encoder1_distance=0.0;
Encoder2_init->Encoder2_distance=0.0;
#endif

#if	Car_ctrl_dirc_inquiry
Encoder1_init->Encoder1_Direction=3;
Encoder2_init->Encoder2_Direction=3;
#endif


xQueueOverwrite(Encoder1_Status_Queue,&Encoder1_init);
xQueueOverwrite(Encoder2_Status_Queue,&Encoder2_init);

};







static void Data_init(void){
	
	Encoder1_data_init();
};


static void Set_init(void){
	
	
};




void init_set(){
	
Data_init();
Set_init();
};

