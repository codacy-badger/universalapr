/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**********  Заголовок  *******************************************************/
/*  Автор      : Глеб Корольков					                                          */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :							                                  */
/*  Редакция   :  16.10.2000                                                  */
/*  Файл       :  'RMARKER.H'                                                 */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
//Almi 16.06.00 //Last edit ........
#ifndef __RMARKER_H
#define __RMARKER_H

#ifndef __GLOBUS_H
 #include "globus.h"
#endif

#ifdef __RMARKER__
  #define RMARKER_FUNC  FUN_EXPO
#else
  #define RMARKER_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
        extern "C" {
#endif

#pragma pack (push,8)

#ifndef Handle
        #define Handle void *
#endif


///////////////////////////////////////////////////////////////////////////////////////////
RMARKER_FUNC(Bool32) RMARKER_Init(uint16_t wHeightCode,Handle hStorage);
RMARKER_FUNC(Bool32) RMARKER_Done();
RMARKER_FUNC(uint32_t) RMARKER_GetReturnCode();
RMARKER_FUNC(char *) RMARKER_GetReturnString(uint32_t dwError);
RMARKER_FUNC(Bool32) RMARKER_GetExportData(uint32_t dwType, void * pData);
RMARKER_FUNC(Bool32) RMARKER_SetImportData(uint32_t dwType, void * pData);
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagRMPreProcessImage
{
	puchar	*pgpRecogDIB;
	Bool32	gbAutoRotate;
	Bool32  gbDotMatrix;
	Bool32  gbFax100;
	Bool32  gbOneColumn;
	Bool32  gKillVSLComponents;
	uint32_t  gnLanguage;
	uint32_t  gnTables;
	Handle	hCPAGE;
	Handle  hCCOM;
	Handle  hCLINE;
	Handle  hDebugCancelSearchPictures;
	Handle  hDebugCancelComponent;
	Handle  hDebugCancelTurn;
	Handle  hDebugCancelSearchLines;
	Handle  hDebugCancelVerifyLines;
	Handle  hDebugCancelSearchDotLines;
	Handle  hDebugCancelRemoveLines;
	Handle  hDebugCancelSearchTables;
	Handle  hDebugLayoutFromFile;
	Handle  hDebugCancelExtractBlocks;
	Handle  hDebugHandLayout;
	Handle  hDebugPrintBlocksCPAGE;
	Handle  hDebugSVLines;
	Handle  hDebugSVLinesStep;
	Handle  hDebugSVLinesData;
	Handle  hDebugEnableSearchSegment;
	char *szLayoutFileName;
	const char ** pglpRecogName;
	void *  pinfo;
	Handle* phLinesCCOM;
	PBool32 pgneed_clean_line;
	int32_t *  pgnNumberTables;
	uint32_t  gnPictures;
	Bool32* pgrc_line;
} RMPreProcessImage, *PRMPreProcessImage;

typedef struct tagRMCBProgressPoints
{
	void *	pProgressStart;
	void *	pProgressStepSearchTables;
	void *	pProgressStepAutoLayout;
	void *	pProgressFinish;
	void *  pInitPRGTIME;
	void *  pStorePRGTIME;
	void *  pRestorePRGTIME;
	void *  pDonePRGTIME;
	void *  prexcProgressStep;
	void *  pDPumaSkipComponent;
	void *  pDPumaSkipTurn;
	void *  pSetReturnCode;
	void *  pGetModulePath;
	void *  pSetUpdate;
} RMCBProgressPoints, *PRMCBProgressPoints;
////////////////////////////////////////////////////////////////////////////////////////////////

#define PUMA_SVL_FIRST_STEP                         0x1
#define PUMA_SVL_SECOND_STEP                        0x2
#define PUMA_SVL_THRID_STEP                         0x3
#define PUMAMaxNumLines                             2000

////////////////////////////////////////////////////////////////////////////////////////////////
#define DEC_FUN(a,b,c) typedef a (*FNRMARKER##b)c; RMARKER_FUNC(a) RMARKER_##b c
DEC_FUN(Bool32, PageMarkup,          (PRMPreProcessImage,void*,int,void*,int));
DEC_FUN(Bool32, SearchTableInZone,    (Handle hPage,Handle hCCOM,uint32_t perc,Rect32 rect));
#undef DEC_FUN

///////////////

#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif //
