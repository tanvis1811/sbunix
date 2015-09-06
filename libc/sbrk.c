/*Copyright (C) 2007 - Julien Lecomte 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 * for more details. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>
#include <sys/defs.h>


 void *sbrk(signed increment)  
{  
    void* newbrk=0;  
    void* oldbrk = 0;  
    //void * curbrk = 0;  
  
    //if (oldbrk == 0)  
       oldbrk = (void*)((uint64_t)brk(0));  
  
    if (increment == 0)  
        return  oldbrk;  
  
    newbrk = oldbrk + increment;  
  
   brk(newbrk);
       // return (void *) -1;  
  
    //oldbrk = curbrk;  
    //curbrk = newbrk;  
  
    return oldbrk;  
}  
