
#include "header.h"
#include "variables_external_m.h"

int dataRegistratorRDS(int recordNumber, int registerNumber, int recordLen);
int getFileDSREGRegister20(int recordNumber, int registerNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDSREGRegister20(int recordNumber, int registerNumber, int recordLen)
{
  //Данные Регистратор дискретных сигналов
 if(recordNumber > 9999) return MARKER_ERRORPERIMETR;
 if(registerNumber == 0) return 0;
 registerNumber--;
 recordLen--;
 return dataRegistratorRDS(recordNumber, registerNumber, recordLen);
}//getFileDSREGRegister20


