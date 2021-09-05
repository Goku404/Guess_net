
#ifndef __GUESS_DIGTAL_H__
#define __GUESS_DIGTAL_H__


#define RESULT_FILE_NAME "guess_digtal.txt"

//“猜数字记录”
typedef struct guess_digtal_record
{
    char name[32]; //猜数字者姓名
    int id; //猜数字者 ID, “玩家ID”

    int num;  //正确的数字，[0-100]
    int record[100];//猜数字的记录
    int times;//猜数字的次数

    struct guess_digtal_record *next;
    struct guess_digtal_record *prev;
}digtal_record_t;


//“头结点”
typedef struct guess_digtal
{
    digtal_record_t *first;
    digtal_record_t *last;
    

    const char * result_file_name;
}guess_digtal_t;


//游戏开始
guess_digtal_t*
guess_digtal_start(void);


//销毁
void destroy_list(guess_digtal_t*);


//保存记录，
//成功返回0，
//失败返回-1
int  save_record(digtal_record_t  *r, const char* filename);

digtal_record_t *
find_game_owner(guess_digtal_t* l,  int id);


//添加 狂数字记录
//返回大了(2)，还是小了(1)，还是OK(0)
int add_record(guess_digtal_t *l, int id, int x);

//返回 玩家ID
int add_node(guess_digtal_t *l, const char *name);
#endif