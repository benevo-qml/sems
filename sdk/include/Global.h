#pragma once

#include <qmath.h>

//ע����ֹ�ڱ�ͷ�ļ��ж����κα�����

const double g_dPI = 3.1415926536;
const float g_fKilo = 1000.f;
const float g_fPercent = 100.f;
const float g_fSquare3 = sqrt(3.f);
const long g_nSecOfDay = 86400;
const long g_nHourOfYear = 8760;

#define EPSILON		1E-10f	//�������Ƚ���ֵ

#define DELETE_POINTER(p)	{if (NULL != p) {delete p; p = NULL;}}
#define DELETE_POINTERS(p)	{if (NULL != p) {delete[] p; p = NULL;}}

#define G_APP_DIR_BIN QCoreApplication::applicationDirPath()
#define G_APP_DIR_ROOT G_APP_DIR_BIN.left(G_APP_DIR_BIN.lastIndexOf('/'))
#define G_APP_DIR_DAA G_APP_DIR_ROOT.append("/data")
