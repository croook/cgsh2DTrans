
// cgsh2DTransView.cpp : Ccgsh2DTransView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cgsh2DTrans.h"
#endif

#include "cgsh2DTransDoc.h"
#include "cgsh2DTransView.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccgsh2DTransView

IMPLEMENT_DYNCREATE(Ccgsh2DTransView, CView)

BEGIN_MESSAGE_MAP(Ccgsh2DTransView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// Ccgsh2DTransView 构造/析构

Ccgsh2DTransView::Ccgsh2DTransView()
{
	// TODO: 在此处添加构造代码

}

Ccgsh2DTransView::~Ccgsh2DTransView()
{
}

BOOL Ccgsh2DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccgsh2DTransView 绘制

void Ccgsh2DTransView::OnDraw(CDC* /*pDC*/)
{
	Ccgsh2DTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* pDC = GetDC();

	pDC->MoveTo(0, m_wndHeight / 2);
	pDC->LineTo(m_wndWidth, m_wndHeight / 2);
	pDC->MoveTo(m_wndWidth / 2, 0);
	pDC->LineTo(m_wndWidth / 2, m_wndHeight);

	CPen newPen;
	CPen *oldPen;

	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	oldPen = (CPen *)pDC->SelectObject(&newPen);

	pDC->MoveTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndLy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndRy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndRx, m_wndHeight / 2 - pDoc->m_wndRy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndRx, m_wndHeight / 2 - pDoc->m_wndLy);
	pDC->LineTo(m_wndWidth / 2 + pDoc->m_wndLx, m_wndHeight / 2 - pDoc->m_wndLy);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();

	// m_transSelect = 2 control window, has been processed in CCgTransSelect class.
	if (pDoc->m_transSelect == 0) CalculateMatrix(pDoc->m_lineMatrix);
	else if (pDoc->m_transSelect == 1) CalculateMatrix(pDoc->m_polygonMatrix);
 	else if (pDoc->m_transSelect == 3) CalculateMatrix(pDoc->m_LB_LineMatrix);
 	else if (pDoc->m_transSelect == 4) CalculateMatrix(pDoc->m_WA_polygonMatrix);

	// Trans & Display General Line Space Model
	TransLine(pDoc->P1, pDoc->P2, &pDoc->TP1, &pDoc->TP2, pDoc->m_lineMatrix);
	DisplayLine(pDC, pDoc->TP1, pDoc->TP2, RGB(255, 0, 0));

	// Cohn-Sutherland Subdivision Line Clip 
	int cx1, cy1, cx2, cy2;
	pDoc->m_lineVisible = FALSE;
	cx1 = pDoc->TP1.x;    cy1 = pDoc->TP1.y;
	cx2 = pDoc->TP2.x;    cy2 = pDoc->TP2.y;

	if (ClipLine(&cx1, &cy1, &cx2, &cy2)) {

		pDoc->CP1.x = cx1;    pDoc->CP1.y = cy1;
		pDoc->CP2.x = cx2;    pDoc->CP2.y = cy2;

		DisplayLine(pDC, pDoc->CP1, pDoc->CP2, RGB(0, 0, 255));
		pDoc->m_lineVisible = TRUE;
	}

	// Trans & Display General Polygon Space Model
	TransPolygon(pDoc->m_pointNumber, pDoc->m_spPolygon, pDoc->m_transPolygon, pDoc->m_polygonMatrix);
	DisplayPolygon(pDC, pDoc->m_pointNumber, pDoc->m_transPolygon, RGB(255, 0, 0));

	// Sutherland-Hodgman Polygon Clip
	pDoc->m_polygonVisible = FALSE;

	if (ClipPolygon(pDoc->m_pointNumber, pDoc->m_transPolygon, &pDoc->m_clipPointNumber, pDoc->m_clipPolygon))
	{
		DisplayPolygon(pDC, pDoc->m_clipPointNumber, pDoc->m_clipPolygon, RGB(0, 0, 255));
		pDoc->m_polygonVisible = TRUE;
	}

	//LB
	TransLine(pDoc->LB_P1, pDoc->LB_P2, &pDoc->LB_TP1, &pDoc->LB_TP2, pDoc->m_LB_LineMatrix);
	DisplayLine(pDC, pDoc->LB_TP1, pDoc->LB_TP2, RGB(0, 255, 255));

	pDoc -> m_LB_lineVisible = FALSE;
	int LB_cx1, LB_cy1, LB_cx2, LB_cy2;
	LB_cx1 = pDoc->LB_TP1.x;    LB_cy1 = pDoc->LB_TP1.y;
	LB_cx2 = pDoc->LB_TP2.x;    LB_cy2 = pDoc->LB_TP2.y;

	if (LB_ClipLine(&LB_cx1, &LB_cy1, &LB_cx2, &LB_cy2)) {

		pDoc->LB_CP1.x = LB_cx1;    pDoc->LB_CP1.y = LB_cy1;
		pDoc->LB_CP2.x = LB_cx2;    pDoc->LB_CP2.y = LB_cy2;

		DisplayLine(pDC, pDoc->LB_CP1, pDoc->LB_CP2, RGB(0, 0, 255));
		pDoc->m_LB_lineVisible = TRUE;
	}

	//WA
	TransPolygon(pDoc->m_WA_pointNumber, pDoc->m_WA_spPolygon, pDoc->m_WA_transPolygon, pDoc->m_WA_polygonMatrix);
	DisplayPolygon(pDC, pDoc->m_WA_pointNumber, pDoc->m_WA_transPolygon, RGB(255, 0, 0));

	// WA Polygon Clip
	pDoc->m_WA_polygonVisible = FALSE;

	if (WA_ClipPolygon(pDoc->m_WA_pointNumber, pDoc->m_WA_transPolygon, &pDoc->m_WA_clipPointNumber, pDoc->m_WA_clipPolygon , pDoc -> inpoints))
	{
		WA_DisplayPolygon(pDC, pDoc->m_WA_clipPointNumber, pDoc->m_WA_clipPolygon,pDoc -> inpoints, RGB(0, 0, 255));
		pDoc->m_WA_polygonVisible = TRUE;
	}


	pDoc->UpdateAllViews(this);

	// Must remember to release pDC
	ReleaseDC(pDC);
}


// Ccgsh2DTransView 打印

BOOL Ccgsh2DTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccgsh2DTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccgsh2DTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccgsh2DTransView 诊断

#ifdef _DEBUG
void Ccgsh2DTransView::AssertValid() const
{
	CView::AssertValid();
}

void Ccgsh2DTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccgsh2DTransDoc* Ccgsh2DTransView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccgsh2DTransDoc)));
	return (Ccgsh2DTransDoc*)m_pDocument;
}
#endif //_DEBUG


// Ccgsh2DTransView 消息处理程序


void Ccgsh2DTransView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_wndWidth = cx;
	m_wndHeight = cy;
}

#define DELTAX 10
#define DELTAY 10
#define DELTATHETA 3.1415926/36
#define SCALETAX 2
#define SCALETAY 2

void Ccgsh2DTransView::CalculateMatrix(float transMatrix[3][2])
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	switch (pDoc->m_transDir) {
	case 0: // -X
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(-DELTAX, 0, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(-sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(1.0/SCALETAX, 1.0/SCALETAY, transMatrix);
			break;
		}
		break;
	case 1: // +X
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(DELTAX, 0, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(SCALETAX, SCALETAY, transMatrix);
			break;
		}
		break;
	case 2: // -Y
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(0, -DELTAY, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(-sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(1.0/SCALETAX, 1.0/SCALETAY, transMatrix);
			break;
		}
		break;
	case 3: // +Y
		switch (pDoc->m_transMode) {
		case 0: // Move
			TranslateMatrix(0, DELTAY, transMatrix);
			break;
		case 1: // rotate
			RotateMatrix(sin(DELTATHETA), cos(DELTATHETA), transMatrix);
			break;
		case 2: // Scale
			ScaleMatrix(SCALETAX, SCALETAY, transMatrix);
			break;
		}
		break;
	}
}

void Ccgsh2DTransView::ScaleMatrix(float Sx, float Sy, float m[3][2])
{
	// Please fill in the right code below lines ...

	for (int i = 0; i < 3; i++) {
		m[i][0] *= Sx;
		m[i][1] *= Sy;
	}
}

void Ccgsh2DTransView::RotateMatrix(float S, float C, float m[3][2])
{
	float temp;

	// Please fill in the right code below lines ...

	for (int i = 0; i < 3; i++) {
		temp = m[i][0];
		m[i][0] = temp * C - m[i][1] * S;
		m[i][1] = temp * S + m[i][1] * C;
	}
}

void Ccgsh2DTransView::TranslateMatrix(float Tx, float Ty, float m[3][2])
{
	// Please fill in the right code below lines ...
	m[2][0] += Tx;
	m[2][1] += Ty;
}

void Ccgsh2DTransView::TransLine(CPoint p1, CPoint p2, CPoint *tp1, CPoint *tp2, float transMatrix[3][2])
{
	// Please fill in the right code below lines ...

	tp1->x = transMatrix[0][0] * p1.x + transMatrix[1][0] * p1.y + transMatrix[2][0];
	tp1->y = transMatrix[0][1] * p1.x + transMatrix[1][1] * p1.y + transMatrix[2][1];
	tp2->x = transMatrix[0][0] * p2.x + transMatrix[1][0] * p2.y + transMatrix[2][0];
	tp2->y = transMatrix[0][1] * p2.x + transMatrix[1][1] * p2.y + transMatrix[2][1];

}

void Ccgsh2DTransView::DisplayLine(CDC* pDC, CPoint p1, CPoint p2, COLORREF rgbColor)
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	CPen newPen;
	CPen *oldPen;
	CPoint VP1, VP2;

	newPen.CreatePen(PS_SOLID, 1, rgbColor);
	oldPen = (CPen *)pDC->SelectObject(&newPen);

	VP1.x = m_wndWidth / 2 + p1.x;
	VP1.y = m_wndHeight / 2 - p1.y;
	VP2.x = m_wndWidth / 2 + p2.x;
	VP2.y = m_wndHeight / 2 - p2.y;

	pDC->MoveTo(VP1);
	pDC->LineTo(VP2);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void Ccgsh2DTransView::TransPolygon(int pointNumber, CPoint spPolygon[N], CPoint transPolygon[N], float transMatrix[3][2])
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	// Please fill in the right code below lines ...

	for (int i = 0; i < pointNumber; i++) {

		transPolygon[i].x = transMatrix[0][0] * spPolygon[i].x + transMatrix[1][0] * spPolygon[i].y + transMatrix[2][0];
		transPolygon[i].y = transMatrix[0][1] * spPolygon[i].x + transMatrix[1][1] * spPolygon[i].y + transMatrix[2][1];
	}

}

void Ccgsh2DTransView::DisplayPolygon(CDC* pDC, int pointNumber, CPoint transPolygon[N], COLORREF rgbColor)
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	CPen newPen;
	CPen *oldPen;
	CPoint VPolygon[N];

	newPen.CreatePen(PS_SOLID, 1, rgbColor);
	oldPen = (CPen *)pDC->SelectObject(&newPen);

	for (int i = 0; i < pointNumber; i++) {
		VPolygon[i].x = m_wndWidth / 2 + transPolygon[i].x;
		VPolygon[i].y = m_wndHeight / 2 - transPolygon[i].y;
	}

	pDC->MoveTo(VPolygon[0]);
	for (int i = 1; i < pointNumber; i++) pDC->LineTo(VPolygon[i]);

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void Ccgsh2DTransView::WA_DisplayPolygon(CDC* pDC, int pointNumber, CPoint transPolygon[N], std::vector<int> inpoints ,COLORREF rgbColor)
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	CPen newPen;
	CPen *oldPen;
	std::vector<CPoint> VPolygon;
	CPoint po;

	newPen.CreatePen(PS_SOLID, 1, rgbColor);
	oldPen = (CPen *)pDC->SelectObject(&newPen);

	int i = 0;
	for (int m = 0; m < inpoints.size(); m++) {
		i = inpoints[m];
		if (m + 1 < inpoints.size()) {
			for (; i < inpoints[m+1]; i++) {
				po.x = m_wndWidth / 2 + transPolygon[i].x;
				po.y = m_wndHeight / 2 - transPolygon[i].y;
				VPolygon.push_back(po);
			}

			pDC->MoveTo(VPolygon[0]);
			for (int j = 1; j < VPolygon.size(); j++) pDC->LineTo(VPolygon[j]);
		}
		else {
			for (; i < pointNumber; i++) {
				po.x = m_wndWidth / 2 + transPolygon[i].x;
				po.y = m_wndHeight / 2 - transPolygon[i].y;
				VPolygon.push_back(po);
			}

			pDC->MoveTo(VPolygon[0]);
			for (int j = 1; j < VPolygon.size(); j++) pDC->LineTo(VPolygon[j]);
		}
		VPolygon.clear();
	}

	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

// Cohn-Sutherland Subdivision Line Clip
int  Ccgsh2DTransView::ClipLine(int *x1, int *y1, int *x2, int *y2)
{
	int visible, m_window[4];
	Ccgsh2DTransDoc* pDoc = GetDocument();

	m_window[0] = pDoc->m_wndLx;    m_window[1] = pDoc->m_wndRx;
	m_window[2] = pDoc->m_wndRy;    m_window[3] = pDoc->m_wndLy;
	for (int i = 0; i < 4; i++) { // Along the WIN Border
		visible = LineVisible(x1, y1, x2, y2);
		if (visible == 1) return 1;         // Total Visible
		if (visible == 0) return 0;         // Total Unvisible

		if (LineCross(*x1, *y1, *x2, *y2, i)) {

			// Please fill in the right code below lines ...
			if (i < 2 && *x2 - *x1) {                       // Left , Right
				float m = (float)(*y2 - *y1) / (*x2 - *x1);
				float iy = m * (m_window[i] - *x1) + *y1;

				if (pVisible(*x1, *y1, i) == 0)
				{
					*x1 = m_window[i];
					*y1 = iy;
				}
				else if (pVisible(*x2, *y2, i) == 0)
				{
					*x2 = m_window[i];
					*y2 = iy;
				}


			}
			else if (*y2 - *y1) {                         // Top    Bottom
				float m = (float)(*x2 - *x1) / (*y2 - *y1);
				float ix = m * (m_window[i] - *y1) + *x1;

				if (pVisible(*x1, *y1, i) == 0)
				{
					*x1 = ix;
					*y1 = m_window[i];
				}
				else if (pVisible(*x2, *y2, i) == 0)
				{
					*x2 = ix;
					*y2 = m_window[i];
				}
			}
		}
	}
	return 1;
}

int Ccgsh2DTransView::LineVisible(int *x1, int *y1, int *x2, int *y2)
{
	int pcode1, pcode2;

	pcode1 = pCode(x1, y1);
	pcode2 = pCode(x2, y2);

	if (!pcode1 && !pcode2)    return 1;     // Visible
	if ((pcode1&pcode2) != 0)  return 0;     // Unvisible
	if (pcode1 == 0) {
		float temp;
		temp = *x1;  *x1 = *x2;  *x2 = temp;
		temp = *y1;  *y1 = *y2;  *y2 = temp;
	}
	return 2;
}

int Ccgsh2DTransView::pCode(int *x, int *y)
{
	int code = 0;
	Ccgsh2DTransDoc* pDoc = GetDocument();

	// Please fill in the right code here ...
	if (*x <= pDoc->m_wndLx)  code |= 1;
	if (*x >= pDoc->m_wndRx)  code |= 2;
	if (*y >= pDoc->m_wndRy)  code |= 4;
	if (*y <= pDoc->m_wndLy)  code |= 8;

	return code;
}

int Ccgsh2DTransView::LineCross(int x1, int y1, int x2, int y2, int i)
{
	int visible1, visible2;

	visible1 = pVisible(x1, y1, i);
	visible2 = pVisible(x2, y2, i);

	if (visible1 != visible2) return 1;
	else                      return 0;

}

int Ccgsh2DTransView::pVisible(int x, int y, int i)
{
	int visible = 0;
	Ccgsh2DTransDoc* pDoc = GetDocument();

	switch (i) {
	case 0: // Left
		if (x >= pDoc->m_wndLx)  visible = 1; break;
	case 1: // Right
		if (x <= pDoc->m_wndRx)  visible = 1; break;
	case 2: // Top
		if (y <= pDoc->m_wndRy)  visible = 1; break;
	case 3: // Bottom
		if (y >= pDoc->m_wndLy)  visible = 1; break;
	}
	return visible;
}

// Sutherland-Hodgman Polygon Clip
int Ccgsh2DTransView::ClipPolygon(int n, CPoint *tPoints, int *cn, CPoint *cPoints)
{
	int Nin, Nout, ix, iy, Sx, Sy;
	Ccgsh2DTransDoc* pDoc = GetDocument();

	Nin = n;
	for (int i = 0; i < 4; i++) {  // Along the window border
		*cn = 0;
		for (int j = 0; j < Nin; j++) {  // Scan polygon every point and line.
			if (j > 0) {
				if (LineCross(Sx, Sy, tPoints[j].x, tPoints[j].y, i)) {
					interSect(Sx, Sy, tPoints[j].x, tPoints[j].y, i, &ix, &iy);
					outPut(ix, iy, cn, cPoints);
				}
			}
			Sx = tPoints[j].x;
			Sy = tPoints[j].y;
			if (pVisible(Sx, Sy, i)) outPut(Sx, Sy, cn, cPoints);
		}

		Nin = *cn;
		if (*cn == 0) return 0;
		for (int j = 0; j < Nin; j++) {
			tPoints[j].x = cPoints[j].x;
			tPoints[j].y = cPoints[j].y;
		}

		if (cPoints[0].x != cPoints[Nin - 1].x ||
			cPoints[0].y != cPoints[Nin - 1].y) {

			tPoints[Nin].x = cPoints[Nin].x = cPoints[0].x;
			tPoints[Nin].y = cPoints[Nin].y = cPoints[0].y;

			Nin++;
			*cn = Nin;
		}
	}
	return 1;
}

void Ccgsh2DTransView::interSect(int Sx, int  Sy, int Px, int Py, int  i, int *ix, int *iy)
{
	Ccgsh2DTransDoc* pDoc = GetDocument();

	// Please fill in the right code below lines ...
	switch (i) {
	case 0: // Left
		*ix = pDoc->m_wndLx;
		*iy = (int)((float)((*ix - Px)*(Sy - Py) / (Sx - Px) + Py + 0.5));
		break;
	case 1: // Right
		*ix = pDoc->m_wndRx;
		*iy = (int)((float)((*ix - Px)*(Sy - Py) / (Sx - Px) + Py + 0.5));
		break;
	case 2: // Top
		*iy = pDoc->m_wndRy;
		*ix = (int)((float)((*iy - Py)*(Sx - Px) / (Sy - Py) + Px + 0.5));
		break;
	case 3: // Bottom
		*iy = pDoc->m_wndLy;
		*ix = (int)((float)((*iy - Py)*(Sx - Px) / (Sy - Py) + Px + 0.5));
		break;
	}
}

void Ccgsh2DTransView::outPut(int x, int y, int *cn, CPoint *cPoints)
{
	cPoints[*cn].x = x;
	cPoints[*cn].y = y;
	(*cn)++;
}

//LB


int  Ccgsh2DTransView::Min(int a, int b, int c = 999) {
	if (a > b) {
		return b < c ? b : c;
	}
	else {
		return a < c ? a : c;
	}
}

int  Ccgsh2DTransView::Max(int a, int b, int c = -999) {
	if (a > b) {
		return a > c ? a : c;
	}
	else {
		return b > c ? b : c;
	}
}

//y = (yB - yA)/(xB - xA) * (x - xA) + yA  or  x = (xB - xA)/(yB - yA) * (y - yA) + xA
int  Ccgsh2DTransView::LB_ClipLine(int *x1, int *y1, int *x2, int *y2) {
	CPoint R, S, T, U, A, B;
	Ccgsh2DTransDoc* pDoc = GetDocument();
	int visible = LineVisible(x1, y1, x2, y2);
	if (visible == 0) return 0;
	else if (visible == 1) return 1;

	A.x = *x1;
	A.y = *y1;
	B.x = *x2;
	B.y = *y2;

	if (A.x == B.x || A.y == B.y) {
		if (A.x == B.x) {
			if (A.y > pDoc->m_wndRy) *y1 = pDoc->m_wndRy;
			else if (A.y < pDoc->m_wndLy) *y1 = pDoc->m_wndLy;
			if (B.y > pDoc->m_wndRy) *y2 = pDoc->m_wndRy;
			else if (B.y < pDoc->m_wndLy) *y2 = pDoc->m_wndLy;
		}
		else {
			if (A.x > pDoc->m_wndRx) *x1 = pDoc->m_wndRx;
			else if (A.x < pDoc->m_wndLx) *x1 = pDoc->m_wndLx;
			if (B.x > pDoc->m_wndRx) *x2 = pDoc->m_wndRx;
			else if (B.x < pDoc->m_wndLx) *x2 = pDoc->m_wndLx;
		}
		return 1;
	}


	int l = Max(pDoc->m_wndLx, Min(A.x, B.x));
	int r = Min(pDoc->m_wndRx, Max(A.x, B.x));

	if (l > r) return 0;

	if ((float)(B.y - A.y) / (B.x - A.x) > 0) {
		U.y = pDoc->m_wndRy;
		U.x = int((float)(B.x - A.x) / (B.y - A.y) * (pDoc->m_wndRy - A.y) + A.x + 0.5);
		if (l > U.x) return 0;
		T.y = pDoc->m_wndLy;
		T.x = int((float)(B.x - A.x) / (B.y - A.y) * (pDoc->m_wndLy - A.y) + A.x + 0.5);
		if (T.x > r) return 0;

		*x1 = Max(l, T.x);
		*x2 = Min(r, U.x);
	}
	else if((float)(B.y - A.y) / (B.x - A.x) < 0){
		T.y = pDoc->m_wndLy;
		T.x = int((float)(B.x - A.x) / (B.y - A.y) * (pDoc->m_wndLy - A.y) + A.x + 0.5);
		if (l > T.x) return 0;
		U.y = pDoc->m_wndRy;
		U.x = int((float)(B.x - A.x) / (B.y - A.y) * (pDoc->m_wndRy - A.y) + A.x + 0.5);
		if (U.x > r) return 0;

		*x1 = Max(l, U.x);
		*x2 = Min(r, T.x);
	}
	*y1 = int((float)(B.y - A.y) / (B.x - A.x)*(*x1 - A.x) + A.y + 0.5);
	*y2 = int((float)(B.y - A.y) / (B.x - A.x)*(*x2 - A.x) + A.y + 0.5);

	return 1;
}



Point::Point(int x , int y ,int f,int num) {
	point.x = x;
	point.y = y;
	flag = f;
	number = num;
}

Point::Point(CPoint a, int f, int num) {
	point.x = a.x;
	point.y = a.y;
	number = num;
	flag = f;
}

int Point::Getpointx() {
	return point.x;
}
int Point::Getpointy() {
	return point.y;
}
int Point::Getflag() {
	return flag;
}
int Point::Getnumber() {
	return number;
}
Point::Point(){}


struct point_order {
	Point p;
	int order;
	point_order() {}
	int distodes;
}po;

bool cmp(struct point_order a, struct point_order b) {
	if( a.order > b.order ) return true;
	else if (a.order == b.order) {
		if (a.distodes < b.distodes) return true;
	}
	return false;
}

bool Ccgsh2DTransView::Judge_in_out(int x1,int y1,int x2,int y2,int ix,int iy) {
	Ccgsh2DTransDoc* pDoc = GetDocument();
	if (x2 > x1) {
		int m = int(iy + (y2 - y1) / (x2 - x1) + ((iy + (y2 - y1) / (x2 - x1)) > 0 ? 0.5 : -0.5));
		if (ix + 1 > pDoc->m_wndLx && ix + 1 <= pDoc->m_wndRx && m >= pDoc->m_wndLy && m <= pDoc->m_wndRy) {
			return true; //inpoint
		}
		else if ((x2 == pDoc->m_wndLx && y2 == pDoc->m_wndRy) || (x2 == pDoc->m_wndLx && y2 == pDoc->m_wndLy)) { //LeftUp and LeftDown
			return true;
		}
	}
	else {
		int m = int(iy - (y2 - y1) / (x2 - x1) + ((iy - (y2 - y1) / (x2 - x1)) > 0 ? 0.5 : -0.5));
		if (ix - 1 >= pDoc->m_wndLx && ix - 1 < pDoc->m_wndRx && m >= pDoc->m_wndLy && m <= pDoc->m_wndRy) {
			return true;
		}
		else if ((x2 == pDoc->m_wndRx && y2 == pDoc->m_wndRy) || (x2 == pDoc->m_wndRx && y2 == pDoc->m_wndLy)) { //LeftUp and LeftDown
			return true;
		}
	}
	return false; //outpoint
}


int Ccgsh2DTransView::WA_ClipPolygon(int n, CPoint *tPoints, int *cn, CPoint *cPoints,std::vector<int> &inpoint) {
	std::vector <Point>SP;
	std::vector <Point>CP;
	inpoint.clear();
	int Nin = n;
	int Sx, Sy , ix ,iy;
	int edgeorder[4] = {0,2,1,3};
	Ccgsh2DTransDoc* pDoc = GetDocument();

	for (int i = 0; i < n; i++) {
		SP.push_back(Point(tPoints[i],2,-1));
	}

	int number = 1;
	std::vector<struct point_order>SPtemp;
	for (int i = 0; i < 4; i++) {  // Along the window border
		for (int j = 0; j < Nin; j++) {  // Scan polygon every point and line.
			if (j > 0) {
				if (LineCross(Sx, Sy, tPoints[j].x, tPoints[j].y, edgeorder[i])) {
					interSect(Sx, Sy, tPoints[j].x, tPoints[j].y, edgeorder[i], &ix, &iy);
					if (!(ix < pDoc->m_wndLx || ix > pDoc->m_wndRx || iy < pDoc->m_wndLy || iy > pDoc->m_wndRy)) {
						po.order = j;
						po.distodes = (tPoints[j].x - ix)*(tPoints[j].x - ix) + (tPoints[j].y - iy)*(tPoints[j].y - iy);

						if (Judge_in_out(Sx, Sy, tPoints[j].x, tPoints[j].y, ix, iy)) {
							po.p = Point(ix, iy, 0, number);
							CP.push_back(Point(ix, iy, 0, number));
						}
						else {
							po.p = Point(ix, iy, 1, number);
							CP.push_back(Point(ix, iy, 1, number));
						}

						SPtemp.push_back(po);
						number++;
					}
				}
			}
			Sx = tPoints[j].x;
			Sy = tPoints[j].y;
		}
	}

	if (number == 1) return 0;

	sort(SPtemp.begin(),SPtemp.end(),cmp);

	for (int i = 0; i < SPtemp.size(); i++) {
		if (SPtemp[i].p.Getpointx() == pDoc->m_wndLx || SPtemp[i].p.Getpointx() == pDoc->m_wndRx) {
			if (SPtemp[i].p.Getpointy() == pDoc->m_wndLy || SPtemp[i].p.Getpointy() == pDoc->m_wndRy) {
				for (int j = i+1; j < SPtemp.size(); j++)
				{
					if (SPtemp[j].p.Getpointx() == SPtemp[i].p.Getpointx() && SPtemp[j].p.Getpointy() == SPtemp[i].p.Getpointy() && SPtemp[j].p.Getflag() == SPtemp[i].p.Getflag()) {
						SPtemp.erase(SPtemp.begin() + j);
					}
				}
			}
		}
	}

	for (int i = 0; i < SPtemp.size(); i++) {
		SP.insert(SP.begin()+SPtemp[i].order , SPtemp[i].p);
	}

	std::vector<Point> edge[4];
	edge[0].push_back(Point(pDoc->m_wndLx, pDoc->m_wndLy, 2,-1));
	edge[1].push_back(Point(pDoc->m_wndLx, pDoc->m_wndRy, 2,-1));
	edge[2].push_back(Point(pDoc->m_wndRx, pDoc->m_wndRy, 2,-1));
	edge[3].push_back(Point(pDoc->m_wndRx, pDoc->m_wndLy, 2,-1));
	for (int i = 0; i < CP.size(); i++) {
		if (CP[i].Getpointx() == pDoc->m_wndLx) { //Left
			int j = edge[0].size()-1;
			while (j >= 0 && edge[0][j].Getpointy() > CP[i].Getpointy()) {
				j--;
			}
			edge[0].insert(edge[0].begin() + j + 1, CP[i]);
		}
		else if (CP[i].Getpointy() == pDoc->m_wndRy) { //Up
			int j = edge[1].size() - 1;
			while (j >= 0 && edge[1][j].Getpointx() > CP[i].Getpointx()) {
				j--;
			}
			edge[1].insert(edge[1].begin() + j + 1, CP[i]);
		}
		else if (CP[i].Getpointx() == pDoc->m_wndRx) { //Right
			int j = edge[2].size() - 1;
			while (j >= 0 && edge[2][j].Getpointy() < CP[i].Getpointy()) {
				j--;
			}
			edge[2].insert(edge[2].begin() + j + 1, CP[i]);
		}
		else{ //Down
			int j = edge[3].size() - 1;
			while (j >= 0 && edge[3][j].Getpointx() < CP[i].Getpointx()) {
				j--;
			}
			edge[3].insert(edge[3].begin() + j + 1, CP[i]);
		}
	}
	CP.clear();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < edge[i].size(); j++) {
			CP.push_back(edge[i][j]);
		}
	}
	CP.push_back(Point(pDoc->m_wndLx, pDoc->m_wndLy, 2,-1));

	for (int i = 0; i < SP.size() - 1; i++) {
		if (SP[i].Getflag() != 2) {
			for (int j = i + 1; j < SP.size(); j++) {
				if (SP[j].Getflag() != 2) {
					if (SP[j].Getflag() == SP[i].Getflag()) {
						for (int m = 0; m < CP.size(); m++) {
							if (CP[m].Getnumber() == SP[j].Getnumber()) {
								CP.erase(CP.begin() + m);
							}
						}
						SP.erase(SP.begin() + j);
					}
					else {
						break;
					}
				}
			}
		}
	}

	std::vector<int> inpoints;
	for (int i = 0; i < SP.size(); i++) {
		if (SP[i].Getflag() == 0) {
			inpoints.push_back(i); //record all the inpoint
		}
	}
	(*cn) = 0;

	for (int m = 0; m < inpoints.size(); m++) {
		int j = 0;
		int i = inpoints[m];
		int num = 0;
		int flag = 0;
		num = SP[i].Getnumber();
		inpoint.push_back(*cn);
		while (SP[i].Getnumber() != num || flag == 0) {

			flag = 1;
			while (SP[i].Getflag() != 1) {
				cPoints[*cn].x = SP[i].Getpointx();
				cPoints[*cn].y = SP[i].Getpointy();
				(*cn)++;
				i = (i + 1) % SP.size();
			}

			while (CP[j].Getnumber() != SP[i].Getnumber()) {
				j = (j + 1) % CP.size();
			}

			while (CP[j].Getflag() != 0) {
				cPoints[*cn].x = CP[j].Getpointx();
				cPoints[*cn].y = CP[j].Getpointy();
				(*cn)++;
				j = (j + 1) % CP.size();
			}
			while (SP[i].Getnumber() != CP[j].Getnumber()) {
				i = (i + 1) % SP.size();
			}
		}
		cPoints[*cn].x = SP[inpoints[m]].Getpointx();
		cPoints[*cn].y = SP[inpoints[m]].Getpointy();
		(*cn)++;
	}

	return 1;
}
