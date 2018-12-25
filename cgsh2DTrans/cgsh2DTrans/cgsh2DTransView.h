
// cgsh2DTransView.h : Ccgsh2DTransView 类的接口
//

#pragma once
#include<vector>

class Ccgsh2DTransView : public CView
{
protected: // 仅从序列化创建
	Ccgsh2DTransView();
	DECLARE_DYNCREATE(Ccgsh2DTransView)

// 特性
public:
	Ccgsh2DTransDoc* GetDocument() const;
	int m_wndWidth, m_wndHeight;
// 操作
public:
	void CalculateMatrix(float transMatrix[3][2]);

	void ScaleMatrix(float Sx, float Sy, float m[3][2]);
	void RotateMatrix(float S, float C, float m[3][2]);
	void TranslateMatrix(float Tx, float Ty, float m[3][2]);

	void TransLine(CPoint p1, CPoint p2, CPoint *tp1, CPoint *tp2, float transMatrix[3][2]);
	void TransPolygon(int pointNumber, CPoint spPolygon[N], CPoint transPolygon[N], float transMatrix[3][2]);

	void DisplayLine(CDC* pDC, CPoint p1, CPoint p2, COLORREF rgbColor);
	void DisplayPolygon(CDC* pDC, int pointNumber, CPoint transPolygon[N], COLORREF rgbColor);

	void WA_DisplayPolygon(CDC * pDC, int pointNumber, CPoint transPolygon[N], std::vector<int> inpoints, COLORREF rgbColor);

	void DrawClipLine(CDC* pDC);
	void DrawClipPolygon(CDC* pDC);

	// Cohn-Sutherland Subdivision Line Clip
	int  ClipLine(int *x1, int *y1, int *x2, int *y2);
	int  pCode(int *x, int *y);
	int  LineVisible(int *x1, int *y1, int *x2, int *y2);

	// Sutherland-Hodgman Polygon Clip
	int  ClipPolygon(int n, CPoint *tPoints, int *cn, CPoint *cPoints);
	int  pVisible(int x, int y, int i);
	int  LineCross(int x1, int y1, int x2, int y2, int i);
	void interSect(int Sx, int  Sy, int Px, int Py, int  i, int *ix, int *iy);
	void outPut(int x, int y, int *cn, CPoint *cPoints);
	int Min(int a, int b, int c);
	int Max(int a, int b, int c);
	int LB_ClipLine(int * x1, int * y1, int * x2, int * y2);
	bool Judge_in_out(int x1, int y1, int x2, int y2, int ix, int iy);
	int WA_ClipPolygon(int n, CPoint * tPoints, int * cn, CPoint * cPoints, std::vector<int> &inpoint);
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Ccgsh2DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // cgsh2DTransView.cpp 中的调试版本
inline Ccgsh2DTransDoc* Ccgsh2DTransView::GetDocument() const
   { return reinterpret_cast<Ccgsh2DTransDoc*>(m_pDocument); }
#endif

class Point {

private:
	CPoint point;
	int flag; // 0 in 1 out
	int number;
public:
	Point(int x, int y ,int f,int num);
	Point(CPoint a, int f,int num);
	int Getpointx();
	int Getpointy();
	int Getflag();
	int Getnumber();
	Point();
};
