// Scope.cpp : 实现文件
//

#include "stdafx.h"
#include "Oscilloscope.h"
#include "Scope.h"
#include <math.h>

// CScope

CScope::CScope(CWnd *pWnd)
{
	m_pWnd = pWnd;

	if (m_pWnd != NULL)
	{
		CRect rect;
		m_pWnd->GetClientRect(&rect);
		yRange = nHeight = rect.Height();
		xRange = nWidth = rect.Width();
		lx = rx = ty = by = 0;
		
		spaceX = 0;
		spaceY = 0;
	}
}

CScope::~CScope()
{
	m_pWnd = NULL;
}

void CScope::setLogicalMap(CDC *pDC) const
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(xRange, -yRange);
	pDC->SetViewportExt(nWidth, nHeight);
	CRect rect;
	m_pWnd->GetClientRect(&rect);
	pDC->SetViewportOrg(rect.left, rect.bottom);
}

void CScope::drawCurrent(double x, double y)
{
	addPoint(x, y);
	drawImage();
}

void CScope::addPoint(double x, double y)
{
	updateXY(x, y);
	m_ListX.AddTail(x);
	m_ListY.AddTail(y);
}

void CScope::drawList(double *x, double *y, int num)
{
	for (int i = 0; i < num; i++)
	{
		addPoint(x[i], y[i]);
	}
	drawImage();
}

void CScope::drawXY(CDC *pDC, COLORREF rgb) const
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, rgb); 
	CPen* pOldPen=pDC->SelectObject(&pen);
	pDC->MoveTo((lx + offsetX) * scaleX, offsetY * scaleY);
	pDC->LineTo((rx + offsetX) * scaleX, offsetY * scaleY);
	pDC->LineTo((rx + offsetX) * scaleX - 8, (offsetY) * scaleY + 4);
	pDC->MoveTo((rx + offsetX) * scaleX, offsetY * scaleY);
	pDC->LineTo((rx + offsetX) * scaleX - 8, (offsetY) * scaleY - 4);
	
	pDC->MoveTo(offsetX * scaleX, (by + offsetY) * scaleY);
	pDC->LineTo(offsetX * scaleX, (ty + offsetY)* scaleY);
	pDC->LineTo(offsetX * scaleX - 4, (ty + offsetY)* scaleY - 8);
	pDC->MoveTo(offsetX * scaleX, (ty + offsetY)* scaleY);
	pDC->LineTo(offsetX * scaleX + 4, (ty + offsetY)* scaleY - 8);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}
void CScope::drawImage(int width, COLORREF rgb) const
{
	CDC *pDC = m_pWnd->GetDC();
	setLogicalMap(pDC);
	CDC memDC;
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, width, rgb); 
	CBitmap bmp, *pOldBmp;
	memDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, xRange, yRange);
	pOldBmp = memDC.SelectObject(&bmp);
	pOldPen = memDC.SelectObject(&pen);
	memDC.FillSolidRect(0, 0, xRange, yRange, RGB(255, 255, 255));
	drawXY(&memDC);

	bool first = true;
	for (POSITION posx = m_ListX.GetHeadPosition(), posy = m_ListY.GetHeadPosition(); posx != NULL && posy != NULL ;)
	{
	   double cx = m_ListX.GetNext(posx);
	   double cy = m_ListY.GetNext(posy);
	   LONG x, y;
	   x = (cx + offsetX) * scaleX;
	   y = (cy + offsetY) * scaleY;

	   if (first)
	   {
		   memDC.MoveTo(x, y);
		   first = false;
	   }else{
		   memDC.LineTo(x, y);
	   }
	}
	pDC->BitBlt(0, 0, xRange, yRange, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
	memDC.SelectObject(pOldPen);
	bmp.DeleteObject();
	pen.DeleteObject();
	memDC.DeleteDC();
	m_pWnd->ReleaseDC(pDC);
}

void CScope::updateXY(double x, double y)
{
	while (x > rx)
	{
		rx += 2 * abs(x - rx);
	}

	while (x < lx)
	{
		lx -= 2 * abs(x - lx);
	}

	while (y < by)
	{
		by -= 2 * abs(y - by);
	}

	while (y > ty)
	{
		ty += 2 * abs(y - ty);
	}

	offsetX = -lx;
	offsetY = -by;
	scaleX = xRange / (rx - lx);
	scaleY = yRange / (ty - by);
}

void CScope::clear()
{
	m_ListX.RemoveAll();
	m_ListY.RemoveAll();
	lx = rx = ty = by = 0;		
	spaceX = 0;
	spaceY = 0;
}

double CScope::getXmin() const
{
	return lx;
}

double CScope::getXmax() const
{
	return rx;
}

double CScope::getYmin() const
{
	return by;
}

double CScope::getYmax() const
{
	return ty;
}