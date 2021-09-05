

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "guess_digtal.h"



//游戏开始
guess_digtal_t*
guess_digtal_start(void)
{

    guess_digtal_t *l = malloc(sizeof(*l));

    l->first = NULL;
    l->last = NULL;
    l->result_file_name = RESULT_FILE_NAME;

    return l;

}
//销毁
void destroy_list(guess_digtal_t* l)
{

    if (l == NULL || l->first == NULL)
    {
        return ;
    }

    digtal_record_t *p = l->first;
    while (p)
    {
        l->first = p->next;
        p->next = NULL;
        p->prev = NULL;

        free(p);

        p = l->first;
    }
    l->last = NULL;

    free(l);
}

//保存记录，
//成功返回0，
//失败返回-1
int  save_record(digtal_record_t  *r, const char* filename)
{
    int i;
    if (r == NULL || filename == NULL)
    {
        return -1;
    }

    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        perror("fopen error");
        return -1;
    }

    fprintf(fp, "%s[%d] %d:", r->name,r->id, r->num );
    for (i = 0; i < r->times; i++)
    {
        fprintf(fp, "%d ", r->record[i]);
    }
    fprintf(fp, "\n");

    fclose(fp);
    return 0;
}

//找到“游戏玩家”
digtal_record_t *
find_game_owner(guess_digtal_t* l,  int id)
{
    if (l == NULL || l->first == NULL)
    {
        return NULL;
    }

    digtal_record_t *p = l->first;
    while (p)
    {
        if (p->id == id)
        {
            return p;
        }

        p = p->next;
    }

    return NULL;
    

}

//返回 玩家ID
int add_node(guess_digtal_t *l, const char *name)
{
    static int game_id = 1000;
    if (l == NULL || name == NULL)
    {
        return -1 ;
    }

    digtal_record_t *p = malloc(sizeof(*p));
    strcpy(p->name, name);
    p->num = rand() % 101;
    p->id = game_id++;
    p->times = 0;
    p->next = NULL;
    p->prev = NULL;

    if (l->last == NULL)
    {
        l->first = p;
        l->last = p;
    }
    else
    {
        l->last->next = p;
        p->prev = l->last;
        
        l->last = p;
    }

    return p->id ;
}


void delete_node(guess_digtal_t *l, digtal_record_t *p)
{
    save_record(p, l->result_file_name);

    if (l->first == p)
    {
        l->first = NULL;
        l->last = NULL;
    }
    else if (l->last == p)
    {
        l->last  = p->prev;
        l->last->next = NULL;
        p->prev = NULL;
    }
    else
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->next = p->prev = NULL;
    }


    free(p);
}

//添加 狂数字记录
//返回大了(2)，还是小了(1)，还是OK(0)
//还是出错了(-1)
int add_record(guess_digtal_t *l, int id, int x)
{
 

    digtal_record_t *p = find_game_owner(l,id);
    if (p == NULL)
    {
        return -1;
    }

    if (p->num == x)
    {
		p->record[ p->times ] = x; //添加最后一个记录
        delete_node(l,p);
        return 0;
    }
    else 
    {
        p->record[p->times++] = x;
        if (x > p->num)
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
}