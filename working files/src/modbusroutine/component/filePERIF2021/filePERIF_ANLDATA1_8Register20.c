
#include "header.h"
#include "variables_external_m.h"

int getFileANLDATA1Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA2Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA3Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA4Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA5Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA6Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA7Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA8Register20(int recordNumber, int registerNumber, int recordLen);

int openRegistratorFunc20(int number_file);
int superReaderFunc20(int fileNumber, int recordNumber, int registerNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileANLDATA1Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA1REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA1REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA1Register20

int getFileANLDATA2Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA2REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA2REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA2Register20

int getFileANLDATA3Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA3REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA3REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA3Register20

int getFileANLDATA4Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA4REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA4REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA4Register20

int getFileANLDATA5Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA5REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA5REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA5Register20

int getFileANLDATA6Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA6REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA6REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA6Register20

int getFileANLDATA7Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA7REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA7REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA7Register20

int getFileANLDATA8Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA8REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(FILENUMBER(FILEPERIF_ANLDATA8REGISTER20), recordNumber, registerNumber, recordLen);

  return result;
}//getFileANLDATA8Register20
