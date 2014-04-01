

#pragma once

#ifndef __STACK_H__
#define __STACK_H__


#undef  __in
#undef  __out
#define __in
#define __out

typedef enum stack_ret
{
        STACK_SUCCESS = 0,              // success.
        STACK_OUTOFMEMORY,              // when malloc() returns NULL
        STACK_DATAISNULL,               // passing a NULL data pointer or a NULL length pointer
        STACK_ERRORSTACKVAL,            // passing an invalidate stack value
        STACK_EMPTY,                    // stack is empty.
        
}stack_ret;

typedef void *  stack_val;

/*****************************************************************************
// create a stack or a queue. data_bytes shows the length of the data field of
// each node.
// */
stack_val       stack_create(int data_bytes);
stack_ret       stack_push(stack_val stack, __in void * data);
stack_ret       stack_pop(stack_val stack, __out void * data);
stack_ret       stack_pop_front(stack_val stack, __out void * data);
stack_ret       stack_read_front(stack_val stack, __out void * data);
stack_ret       stack_read_rear(stack_val stack, __out void * data);
stack_ret       stack_destroy(stack_val stack);
stack_ret       stack_size(stack_val stack, __out unsigned int * len);

#endif



