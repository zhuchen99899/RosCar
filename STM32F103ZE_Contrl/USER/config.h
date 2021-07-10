#ifndef __CONFIG_H
#define __CONFIG_H


/*************
Debug�ȼ�
ӵ������ȼ�
����ģʽ��1.ģʽ1 ���������DEBUG �ȼ��Լ�����ʱ��
					2.ģʽ2 level.�ᰴ�յȼ��ߵ����������ȼ��߻Ὣ�͵ȼ�pr�������������ȼ���������ʱ��
					3.ģʽ3.pure ����ȫ���յȼ����������ȼ�����Ӧ���򲻻�������������
***************/

#define NANO 0
#define ERROR 1
#define WARN 2
#define INFO 3
#define ENTRY 4
#define DEBUG 5
#define PRINT_LEVEL WARN

//*   ����ģʽ   */
#define pr(level, ...)                \
    do                                \
    {                                 \
        if (level <= PRINT_LEVEL)     \
            printf("[" #level "]"     \
                   "[" __DATE__       \
                   " " __TIME__       \
                   "] " __VA_ARGS__); \
    } while (0);




//*   ����ģʽ   */
#define pr_pure(level, ...)       \
    do                            \
    {                             \
        if (level <= PRINT_LEVEL) \
            printf("[" #level "]" \
									 __VA_ARGS__);  \
    } while (0);

		
//	*	�ȼ�ģʽ	*/	
#define pr_level(level, ...)       \
    do                            \
    {                             \
        if (level == PRINT_LEVEL) \
            printf(__VA_ARGS__);  \
    } while (0);

		
//*   ����ģʽ   */
#define pr_err(...) pr(ERROR, ##__VA_ARGS__)
#define pr_warn(...) pr(WARN, ##__VA_ARGS__)
#define pr_info(...) pr(INFO, ##__VA_ARGS__)
#define pr_bug(...) pr(DEBUG, ##__VA_ARGS__)
#define pr_entry(inout) pr(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)





//*   ����ģʽ   */
#define pr_err_pure(...) pr_pure(ERROR, ##__VA_ARGS__)
#define pr_warn_pure(...) pr_pure(WARN, ##__VA_ARGS__)
#define pr_info_pure(...) pr_pure(INFO, ##__VA_ARGS__)
#define pr_bug_pure(...) pr_pure(DEBUG, ##__VA_ARGS__)
#define pr_entry_pure(inout) pr_pure(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)



//	*	�ȼ�ģʽ	*/	
#define pr_err_level(...) pr_level(ERROR, ##__VA_ARGS__)
#define pr_warn_level(...) pr_level(WARN, ##__VA_ARGS__)
#define pr_info_level(...) pr_level(INFO, ##__VA_ARGS__)
#define pr_bug_level(...) pr_level(DEBUG, ##__VA_ARGS__)
#define pr_entry_level(inout) pr_level(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)



#endif



