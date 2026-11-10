#ifndef __COMPONENTDEF_H
#define __COMPONENTDEF_H


typedef struct
{
  int  operativMarker[2];//оперативный маркер
  int  isActiveActualData;

//  int (*getModbusRegister)(int);//получить содержимое регистра
//  int (*getModbusBit)(int);//получить содержимое бита
//  int (*setModbusRegister)(int, int);//записать регистр
//  int (*setModbusBit)(int, int);//записать бит

//  int (*postWriteAction)(void);//action после записи

} COMPONENT_OBJ;

//typedef struct
//{
//  int (*getModbusRegister20func)(int, int, int, int);//получить содержимое регистра
//  int (*setModbusRegister21func)(int, int, int, unsigned short *);//записать регистр
//
//  int (*postWriteAction21func)(int, int, int, unsigned short *);//action после записи
//
//} COMPONENT2021_OBJ;

#endif



