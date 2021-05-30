#include "data_init.h"




static void Encoder1_data_init(void){
	

	
extern QueueHandle_t Encoder1_Overflow_Queue;
extern QueueHandle_t Encoder1_last_count_Queue;
extern QueueHandle_t Encoder1_Status_Queue;		
	
int Encoder1_overflow_count_init;	
int Encoder1_last_count_init;
	
Encoder1_status_t *Encoder_init;
Encoder1_status_t Encoder_struct_init;
Encoder_init = &Encoder_struct_init;	
	

	
/*编码器溢出计数初始化*/
Encoder1_overflow_count_init=0;
xQueueOverwrite(Encoder1_Overflow_Queue,&Encoder1_overflow_count_init);

/*编码器历史总计数队列初始化*/
Encoder1_last_count_init=0;
//xQueueOverwrite(Encoder1_last_count_Queue,&Encoder1_last_count_init);
xQueueSend(Encoder1_last_count_Queue,&Encoder1_last_count_init,portMAX_DELAY);	

/*编码器状态队列初始化*/
Encoder_init->Encoder1_Direction=3;
Encoder_init->Encoder1_distance=0.0;
Encoder_init->Encoder1_Speed=0.0;

xQueueOverwrite(Encoder1_Status_Queue,&Encoder_init);
	

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

