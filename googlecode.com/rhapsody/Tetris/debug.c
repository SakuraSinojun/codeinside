

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "debug.h"

#ifdef __GAME_DEBUG_MESSAGE__

typedef struct memory_link
{
        unsigned int    size;
        int              line;
        char             file[MAX_PATH];
        char             func[100];
        void *           pointer;
        
        struct memory_link * prev;
        struct memory_link * next;
}memory_link;

static memory_link * link_head = NULL;
static memory_link * link_tail = NULL;



static memory_link * _find_link(void * ptr)
{
        memory_link * temp = link_head;
        while(temp != NULL)
        {
                if(temp->pointer == ptr)
                {
                        return temp;
                }
                temp = temp->next;
        }
        return NULL;
}

void __out_debug_(const char * string, ...)
{
        char buffer[1024];
        va_list arglist;

        if (NULL == string)
        {
                return;
        }
                                        
        va_start(arglist, string);
        vsprintf(buffer, string, arglist);
        va_end(arglist);	
        (printf)(buffer); 
}

void * malloc_debug(    unsigned int num_bytes,
                        const char * file,
                        const int line,
                        const char * function
                   )
{
        memory_link *   temp;
        void *          result;
        
        if(num_bytes == 0)
        {
                __out_debug_("[%s]函数分配0字节空间！\n", function);
                return NULL;
        }
        
        result = (malloc)(num_bytes);
        if(result == NULL)
        {
                __out_debug_("内存分配失败！");
                return NULL;
        }
        
        temp = (memory_link *)(malloc)(sizeof(memory_link));
        temp->size = num_bytes;
        temp->line = line;
        strncpy(temp->file, file, MAX_PATH-1);
        strncpy(temp->func, function, 99);
        temp->file[MAX_PATH-1] = '\0';
        temp->func[99] = '\0';
        temp->prev = NULL;
        temp->next = NULL;
        temp->pointer = result;
        
        
        if(link_head == NULL)
        {
                link_head = temp;
        }
        else
        {
                link_tail->next = temp;
                temp->prev = link_tail;
        }
        link_tail = temp;
        
        __out_debug_("分配内存：文件[%s]，行数[%d]，大小[%u]\n", file, line, num_bytes);
        
        return result;
        
}

void free_debug(void * ptr)
{
        memory_link * prev;
        memory_link * next;
        memory_link * temp;
        
        
        if(ptr == NULL)
        {
                __out_debug_("释放空指针！\n");
                return;
        }
        
        temp = _find_link(ptr);
        if(temp == NULL)
        {
                __out_debug_("释放非法指针。\n");
                return;
        }
        
        __out_debug_("释放内存：文件[%s]，行数[%d]，大小[%u]\n", temp->file, temp->line, temp->size);
        
        prev = temp->prev;
        next = temp->next;
        
        if(prev != NULL)
        {
                prev->next = next;
        }else{
                link_head = next;
        }
        if(next != NULL)
        {
                next->prev = prev;
        }else{
                link_tail = prev;
        }
        (free)(ptr);
        
}
#else

void __out_debug_(const char * string, ...)
{

}

#endif






































































