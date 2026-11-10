#ifndef __HEADER_M_H
#define __HEADER_M_H

#define FILENUMBER(fileNumber)     (fileNumber    <<16)
#define RECORDNUMBER(recordNumber) (recordNumber  &0xFFFF)
#define RECORDNUMBER_recordNumber_ (recordNumber  &0xFFFF)

#define GET_UPR_REGISTER000FOR20(fileNum)       getUprRegister000for20      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber)

#define GET_GROUPUPR_REGISTER000FOR20(fileNum)  getGroupUprRegister000for20 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, countGroupBit)

#define GET_UST_REGISTER000FOR20(fileNum)       getUstRegister000for20      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber)

#define SET_UPR_REGISTER000FOR21(fileNum)       setUprRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define SET_GROUPUPR_REGISTER000FOR21(fileNum)  setGroupUprRegister000for21 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, countGroupBit, value)

#define SET_UST_REGISTER000FOR21(fileNum)       setUstRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define POST_UPR_REGISTER000FOR21(fileNum)      postUprRegister000for21     \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define POST_GROUPUPR_REGISTER000FOR21(fileNum) postGroupUprRegister000for21 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                     \
        registerNumber, countGroupBit, value)

#define POST_UST_REGISTER000FOR21(fileNum)      postUstRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                     \
        registerNumber, value)

#include "componentdef.h"
#include "componentenum.h"
#include "prototyps.h"

#endif
