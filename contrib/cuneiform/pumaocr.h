#ifndef PUMAOCR
#define PUMAOCR

/*
Copyright (c) 2008, 2009 Jussi Pakkanen

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


/* This is a simple command line program for the Puma library. */

#include "ctiimage.h" // Must be first, or else you get compile errors.

#include<iostream>
#include<sstream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>

#include"cttypes.h"
#include"puma.h"
#include "lang_def.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"
#include <QString>
#include <QFile>

using namespace std;
using namespace cv;
static int langcode = PUMA_LANG_ENGLISH;
static Bool32 dotmatrix = FALSE;
static Bool32 fax = FALSE;
static Bool32 onecolumn = FALSE;
static int32_t outputformat = PUMA_TOTEXT;

/**
 * Read file and return it as a BMP DIB entity. On failure write an error
 * and return NULL. Caller delete[]'s the returned result.
 */
static char* read_file(const char *fname) {
	char bmpheader[2];
	char *dib;
	FILE *f;
	int32_t dibsize, offset;

	f = fopen(fname, "rb");
	if (!f) {
		cerr << "Could not open file " << fname << ".\n";
		return NULL;
		}
	fread(bmpheader, 1, 2, f);
	if (bmpheader[0] != 'B' || bmpheader[1] != 'M') {
		cerr << fname << " is not a BMP file.\n";
		return NULL;
		}
	fread(&dibsize, sizeof(int32_t), 1, f);
	fread(bmpheader, 1, 2, f);
	fread(bmpheader, 1, 2, f);
	fread(&offset, sizeof(int32_t), 1, f);

	dibsize -= ftell(f);
	dib = new char[dibsize];
	fread(dib, dibsize, 1, f);
	fclose(f);

	if (*((int32_t*)dib) != 40) {
		cerr << "BMP is not of type \"Windows V3\", which is the only supported format.\n";
		cerr << "Please convert your BMP to uncompressed V3 format and try again.\n";
		delete[] dib;
		return NULL;
		}

	if (*((int32_t*) (dib+16)) != 0) {
		cerr << fname << "is a compressed BMP. Only uncompressed BMP files are supported.\n";
		cerr << "Please convert your BMP to uncompressed V3 format and try again.";
		delete[] dib;
		return NULL;
		}
	return dib;
	}
static void pumaDone()
	{
	if(!PUMA_Done()) {
		cerr << "PUMA_Done failed.\n";
		return;
		}
	}

static void pumaInit()
	{
	// Set the language.
	PUMA_SetImportData(PUMA_Word32_Language, &langcode);
	PUMA_SetImportData(PUMA_Bool32_DotMatrix, &dotmatrix);
	PUMA_SetImportData(PUMA_Bool32_Fax100, &fax);
	PUMA_SetImportData(PUMA_Bool32_OneColumn, &onecolumn);

	if(!PUMA_Init(0, 0)) {
		cerr << "PUMA_Init failed.\n";
		return;
		}
	}

static void getTextFromImage(const Mat& src,
							 QString& dst)
	{
	const char *infilename = "resim.bmp";
	string outfilename = "out.txt";

	// Controlling Image
	if (src.empty())
		{
		std::cout << "Cannot open source image!" << std::endl;
		return;
		}
	imwrite(infilename,src);

	/** Puma was started **/
	char *dib;
	dib = read_file(infilename);
	if(!dib) // Error msg is already printed so just get out.
		return;
	if(!PUMA_XOpen(dib, infilename)) {
		cerr << "PUMA_Xopen failed.\n";
		return;
		}

	if(!PUMA_XFinalRecognition()) {
		cerr << "PUMA_XFinalrecognition failed.\n";
		return;
		}

	if(!PUMA_XSave(outfilename.c_str(), outputformat, PUMA_CODE_UTF8)) {
		cerr << "PUMA_XSave failed.\n";
		return;
		}

	if(!PUMA_XClose()) {
		cerr << "PUMA_XClose failed.\n";
		return;
		}

	/** Puma END **/

	QFile* fileTxt = new QFile;
	fileTxt->setFileName(outfilename.c_str());
	if (!fileTxt->open(QIODevice::ReadWrite)){
		cerr << "File couldn't open";
		return;
		}
	dst=fileTxt->readAll();
	fileTxt->remove();
	fileTxt->close();
	QFile::remove(infilename);

	dst.remove("\n");
	dst.remove(".");
	dst.remove(",");
	dst.remove("\"");
	dst.remove("'");
	dst.remove("!");
	dst.remove("*");
	dst.remove("?");
	dst.remove("-");
	dst.remove(";");
	dst.remove("`");
	dst.remove(":");
	dst.remove("\\");
	dst.remove("/");
	dst.remove("+");
	dst.remove("_");
	dst.remove("}");
	dst.remove("{");
	dst.remove("[");
	dst.remove("]");
	dst.remove("(");
	dst.remove(")");
	dst.remove("=");
	dst.remove("&");
	dst.remove("%");
	dst.remove("$");
	dst.remove("#");
	dst.remove("^");
	dst.remove("£");
	dst.remove("é");
	dst.remove("<");
	dst.remove("°");
	dst.remove(">");
	dst.remove("\0");
	dst.remove("|");
	dst.remove("¾");
	dst.remove("½");
	dst.remove("@");
	dst.remove("€");
	dst.remove("ḯ");
	dst.remove("~");
	dst.remove("´");
	dst.remove("“");
	dst.remove("”");
	}

#endif // PUMA

