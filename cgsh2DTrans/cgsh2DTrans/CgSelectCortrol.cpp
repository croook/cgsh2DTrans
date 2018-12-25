// CgSelectCortrol.cpp : 实现文件
//

#include "stdafx.h"
#include "cgsh2DTrans.h"
#include "CgSelectCortrol.h"
#include "cgsh2DTransDoc.h"

// CCgSelectCortrol

IMPLEMENT_DYNCREATE(CCgSelectCortrol, CFormView)

CCgSelectCortrol::CCgSelectCortrol()
	: CFormView(IDD_SELECTBOX)
	, m_transSelect(0)
{
	//  m_transSelect = 0;
}

CCgSelectCortrol::~CCgSelectCortrol()
{
}

void CCgSelectCortrol::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_SELECTOBJ, m_transSelect);
}

BEGIN_MESSAGE_MAP(CCgSelectCortrol, CFormView)
	ON_BN_CLICKED(IDC_MOVE, &CCgSelectCortrol::OnBnClickedMove)
	ON_BN_CLICKED(IDC_XLEFT, &CCgSelectCortrol::OnBnClickedXleft)
	ON_BN_CLICKED(IDC_XRIGHT, &CCgSelectCortrol::OnBnClickedXright)
	ON_BN_CLICKED(IDC_YLEFT, &CCgSelectCortrol::OnBnClickedYleft)
	ON_BN_CLICKED(IDC_YUP, &CCgSelectCortrol::OnBnClickedYup)
	ON_CBN_SELCHANGE(IDC_SELECTOBJ, &CCgSelectCortrol::OnCbnSelchangeSelectobj)
END_MESSAGE_MAP()


// CCgSelectCortrol 诊断

#ifdef _DEBUG
void CCgSelectCortrol::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCgSelectCortrol::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCgSelectCortrol 消息处理程序


#define WND_DELTAX 10
#define WND_DELTAY 10

void CCgSelectCortrol::OnUpdate(CView* /*pSeneinder*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CStatic *viewPlane = (CStatic *)GetDlgItem(IDC_VIEWPORT);
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	CDC     *prjPlaneCDC;
	CRect    prjPlaneRet;

	prjPlaneCDC = viewPlane->GetDC();
	viewPlane->GetClientRect(&prjPlaneRet);

	// Clear BackGround of DC
	CBrush brBlack(RGB(255, 255, 0));
	prjPlaneCDC->FillRect(prjPlaneRet, &brBlack);

	ViewTransLine(prjPlaneCDC, prjPlaneRet);
	ViewTransPolygon(prjPlaneCDC, prjPlaneRet);

	//if (pDoc->m_lineVisible) ViewTransLine(prjPlaneCDC, prjPlaneRet);
	//if (pDoc->m_polygonVisible) ViewTransPolygon(prjPlaneCDC, prjPlaneRet);

	// Must remember to release prjPlaneCDC
	ReleaseDC(prjPlaneCDC);
}


void CCgSelectCortrol::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	m_transSelect = pDoc->m_transSelect;

	UpdateData(FALSE);
}

void CCgSelectCortrol::ViewTransLine(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen *oldPen;
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	oldPen = (CPen *)pDC->SelectObject(&newPen); // pick up your pen

	int vx1, vy1, vx2, vy2;

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	vx1 = int((float)(pDoc->CP1.x - pDoc->m_wndLx) * Sx);
	vy1 = int((float)(pDoc->CP1.y - pDoc->m_wndLy) * Sy);
	vx2 = int((float)(pDoc->CP2.x - pDoc->m_wndLx) * Sx);
	vy2 = int((float)(pDoc->CP2.y - pDoc->m_wndLy) * Sy);

	pDC->MoveTo(vx1, dcRect.bottom - vy1);
	pDC->LineTo(vx2, dcRect.bottom - vy2);

	vx1 = int((float)(pDoc->LB_CP1.x - pDoc->m_wndLx) * Sx);
	vy1 = int((float)(pDoc->LB_CP1.y - pDoc->m_wndLy) * Sy);
	vx2 = int((float)(pDoc->LB_CP2.x - pDoc->m_wndLx) * Sx);
	vy2 = int((float)(pDoc->LB_CP2.y - pDoc->m_wndLy) * Sy);

	pDC->MoveTo(vx1, dcRect.bottom - vy1);
	pDC->LineTo(vx2, dcRect.bottom - vy2);

	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}

void CCgSelectCortrol::ViewTransPolygon(CDC* pDC, CRect dcRect)
{
	CPen newPen;
	CPen *oldPen;
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	// Create new red-color pen to Draw Clipping Line
	newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	oldPen = (CPen *)pDC->SelectObject(&newPen);

	CPoint m_VPolygon[N];

	int wndWidth = pDoc->m_wndRx - pDoc->m_wndLx;  // Space Window size parameters.
	int wndHeight = pDoc->m_wndRy - pDoc->m_wndLy;

	float Sx = (float)(dcRect.right) / wndWidth;    // View-Space Viewport size parameters.
	float Sy = (float)(dcRect.bottom) / wndHeight;

	// Please fill the right codes below lines ...
	for (int i = 0; i < pDoc->m_clipPointNumber; i++) {
		m_VPolygon[i].x = int((float)(pDoc->m_clipPolygon[i].x - pDoc->m_wndLx) * Sx);
		m_VPolygon[i].y = int((float)(pDoc->m_wndRy - pDoc->m_clipPolygon[i].y) * Sy);
	}

	pDC->MoveTo(m_VPolygon[0]);
	for (int i = 1; i < pDoc->m_clipPointNumber; i++) pDC->LineTo(m_VPolygon[i]);

	//draw WA_Polygon
	//for (int i = 0; i < pDoc->m_WA_clipPointNumber; i++) {
	//	m_VPolygon[i].x = (pDoc->m_WA_clipPolygon[i].x - pDoc->m_wndLx) * Sx;
	//	m_VPolygon[i].y = (pDoc->m_wndRy - pDoc->m_WA_clipPolygon[i].y) * Sy;
	//}

	//pDC->MoveTo(m_VPolygon[0]);
	//for (int i = 1; i < pDoc->m_WA_clipPointNumber; i++) pDC->LineTo(m_VPolygon[i]);
	std::vector<CPoint> VPolygon;
	int i = 0;
	CPoint po;
	for (int m = 0; m < pDoc->inpoints.size(); m++) {
		i = pDoc->inpoints[m];
		if (m + 1 < pDoc->inpoints.size()) {
			for (; i < pDoc->inpoints[m + 1]; i++) {
				po.x = int((float)(pDoc->m_WA_clipPolygon[i].x - pDoc->m_wndLx) * Sx);
				po.y = int((float)(pDoc->m_wndRy - pDoc->m_WA_clipPolygon[i].y) * Sy);
				VPolygon.push_back(po);
			}

			pDC->MoveTo(VPolygon[0]);
			for (int j = 1; j < VPolygon.size(); j++) pDC->LineTo(VPolygon[j]);
		}
		else {
			for (; i < pDoc->m_WA_clipPointNumber; i++) {
				po.x = int((float)(pDoc->m_WA_clipPolygon[i].x - pDoc->m_wndLx) * Sx);
				po.y = int((float)(pDoc->m_wndRy - pDoc->m_WA_clipPolygon[i].y) * Sy);
				VPolygon.push_back(po);
			}

			pDC->MoveTo(VPolygon[0]);
			for (int j = 1; j < VPolygon.size(); j++) pDC->LineTo(VPolygon[j]);
		}
		VPolygon.clear();
	}


	// Remember: must delete newPen every time.	
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();
}


void CCgSelectCortrol::OnBnClickedMove()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();
	CButton *pButton = (CButton *)GetDlgItem(IDC_MOVE);

	//	if (pDoc->rayTraceNow) return;

	pDoc->m_transMode++;
	if (pDoc->m_transMode > 2) pDoc->m_transMode = 0;

	if (pDoc->m_transMode == 0)      pButton->SetWindowText(_T("Move"));
	else if (pDoc->m_transMode == 1) pButton->SetWindowText(_T("Rotate"));
	else                             pButton->SetWindowText(_T("Scale"));
}


void CCgSelectCortrol::OnBnClickedXleft()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	if (pDoc->m_transMode == 0) {

		if (pDoc->m_transSelect == 2) {
			pDoc->m_wndLx -= WND_DELTAX;
			pDoc->m_wndRx -= WND_DELTAX;
		}
	}

	pDoc->m_transDir = 0;

	pDoc->UpdateAllViews(this);
}


void CCgSelectCortrol::OnBnClickedXright()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	if (pDoc->m_transMode == 0) {

		if (pDoc->m_transSelect == 2) {
			pDoc->m_wndLx += WND_DELTAX;
			pDoc->m_wndRx += WND_DELTAX;
		}
	}

	pDoc->m_transDir = 1;

	pDoc->UpdateAllViews(this);
}


void CCgSelectCortrol::OnBnClickedYleft()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	if (pDoc->m_transMode == 0) {

		if (pDoc->m_transSelect == 2) {
			pDoc->m_wndLy -= WND_DELTAY;
			pDoc->m_wndRy -= WND_DELTAY;
		}
	}

	pDoc->m_transDir = 2;

	pDoc->UpdateAllViews(this);
}


void CCgSelectCortrol::OnBnClickedYup()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	if (pDoc->m_transMode == 0) {

		if (pDoc->m_transSelect == 2) {
			pDoc->m_wndLy += WND_DELTAY;
			pDoc->m_wndRy += WND_DELTAY;
		}
	}

	pDoc->m_transDir = 3;

	pDoc->UpdateAllViews(this);
}


void CCgSelectCortrol::OnCbnSelchangeSelectobj()
{
	Ccgsh2DTransDoc* pDoc = (Ccgsh2DTransDoc*)GetDocument();

	UpdateData(TRUE);

	pDoc->m_transSelect = m_transSelect;

	UpdateData(FALSE);
}
