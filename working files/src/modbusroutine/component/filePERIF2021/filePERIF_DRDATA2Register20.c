
#include "header.h"
#include "variables_external_m.h"

int getFileDRDATA2Register20(int recordNumber, int registerNumber, int recordLen);

int openRegistratorFunc20(int number_file);
int superReaderFunc20(int fileNumber, int recordNumber, int registerNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDRDATA2Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_DRDATA2REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_DRDATA2REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileDRDATA2Register20


