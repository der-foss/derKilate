#ifndef __IO_H__
#define __IO_H__

#include <kilate/native.h>
#include <kilate/node.h>

node* std_print(native_fndata*);

node* std_system(native_fndata*);

node* std_sleep(native_fndata*);

#endif