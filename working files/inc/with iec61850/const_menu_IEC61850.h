#ifndef __CONSTANTS_IEC61850__
#define __CONSTANTS_IEC61850__

#define EKRAN_LIST_TYPE_IEC61850_NODES          (EKRAN_CONTROL_UP + 1)

#define EKRAN_LIST_IN_GOOSE                     (EKRAN_LIST_TYPE_IEC61850_NODES + 1)
#define EKRAN_LIST_IN_MMS                       (EKRAN_LIST_IN_GOOSE + 1)
#define EKRAN_LIST_OUT_LAN                      (EKRAN_LIST_IN_MMS + 1)

#define EKRAN_IN_GOOSE1                         (EKRAN_LIST_OUT_LAN + 1)
#define EKRAN_IN_GOOSE2                         (EKRAN_IN_GOOSE1 + 1)
#define EKRAN_IN_GOOSE3                         (EKRAN_IN_GOOSE2 + 1)
#define EKRAN_IN_GOOSE4                         (EKRAN_IN_GOOSE3 + 1)
#define EKRAN_IN_GOOSE5                         (EKRAN_IN_GOOSE4 + 1)
#define EKRAN_IN_GOOSE6                         (EKRAN_IN_GOOSE5 + 1)
#define EKRAN_IN_GOOSE7                         (EKRAN_IN_GOOSE6 + 1)
#define EKRAN_IN_GOOSE8                         (EKRAN_IN_GOOSE7 + 1)
#define EKRAN_IN_GOOSE9                         (EKRAN_IN_GOOSE8 + 1)
#define EKRAN_IN_GOOSE10                        (EKRAN_IN_GOOSE9 + 1)
#define EKRAN_IN_GOOSE11                        (EKRAN_IN_GOOSE10 + 1)
#define EKRAN_IN_GOOSE12                        (EKRAN_IN_GOOSE11 + 1)
#define EKRAN_IN_GOOSE13                        (EKRAN_IN_GOOSE12 + 1)
#define EKRAN_IN_GOOSE14                        (EKRAN_IN_GOOSE13 + 1)
#define EKRAN_IN_GOOSE15                        (EKRAN_IN_GOOSE14 + 1)
#define EKRAN_IN_GOOSE16                        (EKRAN_IN_GOOSE15 + 1)

#define EKRAN_IN_MMS1                           (EKRAN_IN_GOOSE16 + 1)
#define EKRAN_IN_MMS2                           (EKRAN_IN_MMS1 + 1)
#define EKRAN_IN_MMS3                           (EKRAN_IN_MMS2 + 1)
#define EKRAN_IN_MMS4                           (EKRAN_IN_MMS3 + 1)

#define EKRAN_OUT_LAN1                          (EKRAN_IN_MMS4 + 1)
#define EKRAN_OUT_LAN2                          (EKRAN_OUT_LAN1 + 1)
#define EKRAN_OUT_LAN3                          (EKRAN_OUT_LAN2 + 1)
#define EKRAN_OUT_LAN4                          (EKRAN_OUT_LAN3 + 1)

#define EKRAN_LN_FOR_IEC61850_RANG_SMALL        (EKRAN_OUT_LAN4 + 1)
#define EKRAN_LN_FOR_IEC61850_RANG              (EKRAN_LN_FOR_IEC61850_RANG_SMALL + 1)

enum _index_ml_type_iec61850_notes
{
  INDEX_TYPE_IEC61850_IN_GOOSE = 0,
  INDEX_TYPE_IEC61850_IN_MMS,
  INDEX_TYPE_IEC61850_OUT_LAN,
  
  MAX_ROW_FOR_TYPE_IEC61850_NODES
};


#endif 
