/**
  ******************************************************************************
  * @file    lib_NDEF_Vcard.h
  * @author  MMY Application Team
  * @version $Revision: 1329 $
  * @date    $Date: 2015-11-05 10:34:25 +0100 (Thu, 05 Nov 2015) $
  * @brief   This file help to manage Vcard NDEF file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_VCARD_H
#define __LIB_NDEF_VCARD_H
#include "config.h"
#if USE_NFC03A1

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF.h"


#define VCARD_VERSION_2_1             "2.1"
#define VCARD_VERSION_2_1_SIZE        3

#define VCARD_VERSION_3_0             "3.0"
#define VCARD_VERSION_3_0_SIZE        3


#define BEGIN                         "BEGIN:"
#define VCARD                         "VCARD"
#define VERSION                       "VERSION:"
#define FIRSTNAME                     "FN:"
#define HOME_TEL                      "TEL;HOME:"
#define WORK_TEL                      "TEL;WORK:"
#define CELL_TEL                      "TEL;CELL:"
#define HOME_EMAIL                    "EMAIL;HOME:"
#define WORK_EMAIL                    "EMAIL;WORK:"
#define HOME_ADDRESS                  "ADR;HOME:"
#define WORK_ADDRESS                  "ADR;WORK:"
#define TITLE                         "TITLE:"
#define ORG                           "ORG:"
#define END                           "END:"
#define JPEG                          "JPEG"

#define LIMIT                         "\r\n"

#define BEGIN_STRING_SIZE             6
#define VCARD_STRING_SIZE             5
#define VERSION_STRING_SIZE           8
#define FIRSTNAME_STRING_SIZE         3
#define HOME_TEL_STRING_SIZE          9
#define WORK_TEL_STRING_SIZE          9
#define CELL_TEL_STRING_SIZE          9
#define HOME_EMAIL_STRING_SIZE        11
#define WORK_EMAIL_STRING_SIZE        11
#define HOME_ADDRESS_STRING_SIZE      9
#define WORK_ADDRESS_STRING_SIZE      9
#define TITLE_STRING_SIZE             6
#define ORG_STRING_SIZE               4
#define END_STRING_SIZE               4
#define JPEG_STRING_SIZE              4	

#define LIMIT_STRING_SIZE             2

typedef struct
{
  char Version [10];
  char FirstName[80];
  char Title[80];
  char Org[80];
  char HomeAddress[80];
  char WorkAddress[80];
  char HomeTel[40];
  char WorkTel[40];
  char CellTel[40];
  char HomeEmail[80];
  char WorkEmail[80];
}sVcardInfo;

/* Struct defined in lib_NDEF.h */
typedef struct sRecordInfo sRecordInfo_vcard;

uint16_t NDEF_ReadVcard( sRecordInfo_vcard *pRecordStruct, sVcardInfo *pVcardStruct );
uint16_t NDEF_WriteVcard( sVcardInfo *pVcardStruct );
void NDEF_PrepareVcardMessage( sVcardInfo *pVcardStruct, uint8_t *pNDEFMessage, uint16_t *size );

#ifdef __cplusplus
}
#endif

#endif /* __LIB_NDEF_VCARD_H */


#endif
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
