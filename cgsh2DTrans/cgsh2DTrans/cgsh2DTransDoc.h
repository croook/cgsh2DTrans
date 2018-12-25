
// cgsh2DTransDoc.h : Ccgsh2DTransDoc ��Ľӿ�
//
#include<vector>

#pragma once
#define N 64

class Ccgsh2DTransDoc : public CDocument
{
protected: // �������л�����
	Ccgsh2DTransDoc();
	DECLARE_DYNCREATE(Ccgsh2DTransDoc)

// ����
public:
	CPoint P1, P2;                 // Original Space Line
	CPoint TP1, TP2;               // Trans Space Line
	CPoint CP1, CP2;               // Clip Space Line
	float m_lineMatrix[3][2];
	// Liang-Barsky Line Space Model
	CPoint LB_P1, LB_P2;           // Original Space Line
	CPoint LB_TP1, LB_TP2;         // Trans Space Line
	CPoint LB_CP1, LB_CP2;         // Clip Space Line
	float m_LB_LineMatrix[3][2];
	// General Polygon Space Model
	int m_pointNumber;              // Original Space Polygon point number
	int m_clipPointNumber;          // Clip Space Polygon point number
	CPoint m_spPolygon[N];          // Original Space Polygon
	CPoint m_transPolygon[N];       // Trans Space Polygon
	CPoint m_clipPolygon[N];        // Clip Space Polygon
	float m_polygonMatrix[3][2];

	// Weiler-Atherton Polygon Space Model
	int m_WA_pointNumber;            // Original Space Polygon point number
	int m_WA_clipPointNumber;        // Clip Space Polygon point number
	CPoint m_WA_spPolygon[N];        // Original Space Polygon
	CPoint m_WA_transPolygon[N];     // Trans Space Polygon
	CPoint m_WA_clipPolygon[N];      // Clip Space Polygon
	float m_WA_polygonMatrix[3][2];
	std::vector<int>inpoints;

	BOOL m_lineVisible, m_polygonVisible;
	BOOL m_LB_lineVisible, m_WA_polygonVisible;

	int m_wndLx, m_wndLy, m_wndRx, m_wndRy;     // Space Window (Lx, Ly)-(Rx,Ry)
	int  m_transDir, m_transMode, m_transSelect;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~Ccgsh2DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
