#pragma once

// CScope ÃüÁîÄ¿±ê
#include "stdafx.h"
class CScope : public CObject
{
public:
	CScope(CWnd *pWnd = NULL);
	virtual ~CScope();
	void drawCurrent(double x, double y);
	void updateXY(double x, double y);
	void drawImage(int width = 1 ,COLORREF rgb = RGB(0, 0, 255)) const;
	void drawList(double *x, double *y, int num);
	void clear();
	double getXmin() const;
	double getXmax() const;
	double getYmin() const;
	double getYmax() const;

private:
	void setLogicalMap(CDC *pDC) const;
	void addPoint(double x, double y);
	void drawXY(CDC *pDC, COLORREF rgb = RGB(0, 0, 0)) const;

private:
	CWnd *m_pWnd;
	LONG nHeight, nWidth;
	double lx, rx, by, ty;	
	LONG xRange, yRange;
	double offsetX, offsetY;
	LONG spaceX, spaceY;
	double scaleX, scaleY;
	

private:
	CList<double> m_ListX, m_ListY;
};


