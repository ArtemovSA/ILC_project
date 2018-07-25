/*
# This file is Copyright 2010 Dean Hall.
#
# This file is part of the Python-on-a-Chip program.
# Python-on-a-Chip is free software: you can redistribute it and/or modify
# it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.
#
# Python-on-a-Chip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# A copy of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1
# is seen in the file COPYING up one directory from this.
*/

#ifndef _PLAT_H_
#define _PLAT_H_

#define PM_HEAP_SIZE 0x5000 //20k
#define PM_STRING_SIZE 512
#define PM_FLOAT_LITTLE_ENDIAN
#define PM_PLAT_HEAP_ATTR __attribute__((aligned (4)))

//Описание ошибки
typedef struct {
   uint8_t error_code; //Код ошибки
   char description[50]; //Описание    
}PM_error_codes_t;


//Структура ошибки выполнения
typedef struct{
  uint8_t type_dev;     //Тип устройства
  uint8_t adr_sw;       //Адрес
  uint8_t adr_port;     //Порт
  uint8_t error_code;   //Код ошибки
  char thread_name[30]; //Имя потока
}PM_error_t;

extern char PM_str_buf[PM_STRING_SIZE]; //Global string buffer
extern PM_error_t PM_error; //Return error value
extern const PM_error_codes_t PM_error_codes[]; //Error description

#endif /* _PLAT_H_ */
