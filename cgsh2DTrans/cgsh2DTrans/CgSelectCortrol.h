#pragma once



// CCgSelectCortrol ������ͼ

class CCgSelectCortrol : public CFormView
{
	DECLARE_DYNCREATE(CCgSelectCortrol)

protected:
	CCgSelectCortrol();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCgSelectCortrol();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECTBOX };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	int m_transSelect;
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate();
	void ViewTransLine(CDC * pDC, CRect dcRect);
	void ViewTransPolygon(CDC * pDC, CRect dcRect);
	afx_msg void OnBnClickedMove();
	afx_msg void OnBnClickedXleft();
	afx_msg void OnBnClickedXright();
	afx_msg void OnBnClickedYleft();
	afx_msg void OnBnClickedYup();
	afx_msg void OnCbnSelchangeSelectobj();
	int m_transSelect;
};


