#ifndef __CONFIG_H
#define __CONFIG_H


/*************
Debug等级
拥有五个等级
三个模式：1.模式1 会完整输出DEBUG 等级以及编译时间
					2.模式2 level.会按照等级高低输出，输出等级高会将低等级pr输出，包含编译等级不带编译时间
					3.模式3.pure 会完全按照等级输出，如果等级不对应，则不会输出。纯净输出
***************/

#define NANO 0
#define ERROR 1
#define WARN 2
#define INFO 3
#define ENTRY 4
#define DEBUG 5
#define PRINT_LEVEL WARN

//*   完整模式   */
#define pr(level, ...)                \
    do                                \
    {                                 \
        if (level <= PRINT_LEVEL)     \
            printf("[" #level "]"     \
                   "[" __DATE__       \
                   " " __TIME__       \
                   "] " __VA_ARGS__); \
    } while (0);




//*   纯净模式   */
#define pr_pure(level, ...)       \
    do                            \
    {                             \
        if (level <= PRINT_LEVEL) \
            printf("[" #level "]" \
									 __VA_ARGS__);  \
    } while (0);

		
//	*	等级模式	*/	
#define pr_level(level, ...)       \
    do                            \
    {                             \
        if (level == PRINT_LEVEL) \
            printf(__VA_ARGS__);  \
    } while (0);

		
//*   完整模式   */
#define pr_err(...) pr(ERROR, ##__VA_ARGS__)
#define pr_warn(...) pr(WARN, ##__VA_ARGS__)
#define pr_info(...) pr(INFO, ##__VA_ARGS__)
#define pr_bug(...) pr(DEBUG, ##__VA_ARGS__)
#define pr_entry(inout) pr(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)





//*   纯净模式   */
#define pr_err_pure(...) pr_pure(ERROR, ##__VA_ARGS__)
#define pr_warn_pure(...) pr_pure(WARN, ##__VA_ARGS__)
#define pr_info_pure(...) pr_pure(INFO, ##__VA_ARGS__)
#define pr_bug_pure(...) pr_pure(DEBUG, ##__VA_ARGS__)
#define pr_entry_pure(inout) pr_pure(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)



//	*	等级模式	*/	
#define pr_err_level(...) pr_level(ERROR, ##__VA_ARGS__)
#define pr_warn_level(...) pr_level(WARN, ##__VA_ARGS__)
#define pr_info_level(...) pr_level(INFO, ##__VA_ARGS__)
#define pr_bug_level(...) pr_level(DEBUG, ##__VA_ARGS__)
#define pr_entry_level(inout) pr_level(ENTRY, "File:%sFunc:%s()Line:%d %s ", __FILE__,__func__, __LINE__, #inout)



#endif



