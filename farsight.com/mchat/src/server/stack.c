/*
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include "stack.h"

struct _struct_link
{
        void    *               data;
        struct _struct_link *   prev;
        struct _struct_link *   next;
};

struct _stack_val
{
        int                     size;
        struct _struct_link *   head;
        struct _struct_link *   tail;
};


typedef struct _struct_link     _struct_link;
typedef struct _stack_val       _stack_val;


/******************************************************************************
 *
 *
 *
 */
static stack_ret _check_val(stack_val stack)
{
        _stack_val *    val;
        _struct_link *  link;
        
        val = (_stack_val *)stack;
        if(val == NULL)
        {
                return STACK_ERRORSTACKVAL;
        }

        if(val->head == NULL && val->tail == NULL)
        {
                return STACK_SUCCESS;
        }

        if(val->head == NULL || val->tail == NULL)
        {
                return STACK_ERRORSTACKVAL;
        }

        link = val->head;

        while(link != NULL)
        {
                if(link->next == NULL && link != val->tail)
                {
                        return STACK_ERRORSTACKVAL;
                }
                link = link->next;
        }
        
        return STACK_SUCCESS;

}


/******************************************************************************
 *
 *
 */

stack_val stack_create(int data_bytes)
{
        _stack_val *   val;

        val = (_stack_val*)malloc(sizeof(_stack_val));
        val->size = data_bytes;
        val->head = NULL;
        val->tail = NULL;

        return (stack_val)val;

}

stack_ret stack_push(stack_val stack, void * data)
{
        _struct_link *  temp;
        _stack_val   *  val;
        stack_ret       ret;

        if(data == NULL)
        {
                return STACK_DATAISNULL;
        }

        ret = _check_val(stack);
        if(ret != STACK_SUCCESS)
        {
                return ret;
        }

        val = (_stack_val *)stack;
        temp = (_struct_link *)malloc(sizeof(_struct_link));
        temp->data = malloc(val->size);
        temp->prev = NULL;
        temp->next = NULL;
        
        if(temp->data == NULL)
        {
                return STACK_OUTOFMEMORY;
        }

        memcpy(temp->data, data, val->size);

        if(val->head == NULL)
        {
                val->head = temp;
        }
        else
        {
                val->tail->next = temp;
                temp->prev = val->tail;
        }

        val->tail = temp;

        return STACK_SUCCESS;

}

stack_ret stack_pop(stack_val stack, void * data)
{
        _stack_val *    val;
        _struct_link *  prev;
        stack_ret       ret;
        
        val = (_stack_val *)stack;
        ret = _check_val(stack);
        if(ret != STACK_SUCCESS)
        {
                return ret;
        }

        if(data == NULL)
        {
                return STACK_DATAISNULL;
        }

        if(val->tail == NULL)
        {
                return STACK_EMPTY;
        }

        memcpy(data, val->tail->data, val->size);
        
        prev = val->tail->prev;

        free(val->tail->data);
        free(val->tail);

        if(prev == NULL)
        {
                val->head = NULL;
        }
        else
        {
                prev->next = NULL;
        }
        val->tail = prev;

        return STACK_SUCCESS;
}

stack_ret stack_pop_front(stack_val stack, void * data)
{
        _stack_val *    val;
        _struct_link *  next;
        stack_ret       ret;
        
        val = (_stack_val *)stack;
        ret = _check_val(stack);
        if(ret != STACK_SUCCESS)
        {
                return ret;
        }

        if(data == NULL)
        {
                return STACK_DATAISNULL;
        }

        if(val->head == NULL)
        {
                return STACK_EMPTY;
        }

        memcpy(data, val->head->data, val->size);
        
        next = val->head->next;

        free(val->head);

        if(next == NULL)
        {
                val->tail = NULL;
        }
        else
        {
                next->prev = NULL;
        }

        val->head = next;

        return STACK_SUCCESS;
}

stack_ret       stack_read_rear(stack_val stack, __out void * data)
{
        _stack_val *    val;
        stack_ret       ret;
        
        val = (_stack_val *)stack;
        ret = _check_val(stack);
        if(ret != STACK_SUCCESS)
        {
                return ret;
        }

        if(data == NULL)
        {
                return STACK_DATAISNULL;
        }

        if(val->tail == NULL)
        {
                return STACK_EMPTY;
        }

        memcpy(data, val->tail->data, val->size);
        
        return STACK_SUCCESS;
}

stack_ret       stack_read_front(stack_val stack, __out void * data)
{
        _stack_val *    val;
        stack_ret       ret;
        
        val = (_stack_val *)stack;
        ret = _check_val(stack);
        if(ret != STACK_SUCCESS)
        {
                return ret;
        }

        if(data == NULL)
        {
                return STACK_DATAISNULL;
        }

        if(val->head == NULL)
        {
                return STACK_EMPTY;
        }

        memcpy(data, val->head->data, val->size);
        
        return STACK_SUCCESS;

}

stack_ret stack_destroy(stack_val stack)
{
        _stack_val *    val;
        stack_ret       ret;
        _struct_link *  link;


        ret = _check_val(stack);

        if(ret != STACK_SUCCESS || ret != STACK_EMPTY)
        {
                return ret;
        }

        val = (_stack_val *)stack;
        link = val->head;

        while(link != NULL)
        {
                val->head = link->next;
                free(link->data);
                free(link);
                link = val->head;
        }

        val->head = NULL;
        val->tail = NULL;

        free(val);
        
        return STACK_SUCCESS;

}

stack_ret stack_size(stack_val stack, __out unsigned int * len)
{

        _stack_val *    val;
        stack_ret       ret;
        _struct_link *  link;
        unsigned int    count;

        ret = _check_val(stack);

        if(ret != STACK_SUCCESS)
        {
                return ret;
        }
        
        if(len == NULL)
        {
                return STACK_DATAISNULL;
        }

        val = (_stack_val *)stack;
        count = 0;
        link = val->head;
        while(link != NULL)
        {
                count ++;
                link = link->next;
        }
        
        *len = count;

        return STACK_SUCCESS;
}







