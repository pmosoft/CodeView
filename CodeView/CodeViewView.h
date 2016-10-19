
// CodeViewView.h : CCodeViewView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "GridCtrl_src/GridCtrl.h"
#include "GridCtrl_src/NewCellTypes/GridCellNumeric.h"

class CCodeViewView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CCodeViewView();
	DECLARE_DYNCREATE(CCodeViewView)

public:
	enum{ IDD = IDD_CODEVIEW_FORM };

// 특성입니다.
public:
	CCodeViewDoc* GetDocument() const;

// 작업입니다.
public:
	CGridCtrl m_Grid;

    void CCodeViewView::ProcessRecordset(CString strTable);
    CString CCodeViewView::uItoa(int num);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CCodeViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo1;
	CString m_edit1;
	CString m_edit2;
	CString m_edit3;
	CString m_edit4;
	afx_msg void OnBnClickedButton1();

	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnPaint();
	afx_msg void OnEditCopy();
//	afx_msg void OnEditCut();
//	afx_msg void OnEditPaste();
	afx_msg void OnEditPaste();
	CEdit m_ctrlEdit1;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
//	CString m_nCombo1;
	int m_nCombo1;
//	afx_msg void OnEditTabname();
//	afx_msg void OnUpdateEditTabname(CCmdUI *pCmdUI);
	afx_msg void OnEditTabname();
};

#ifndef _DEBUG  // CodeViewView.cpp의 디버그 버전
inline CCodeViewDoc* CCodeViewView::GetDocument() const
   { return reinterpret_cast<CCodeViewDoc*>(m_pDocument); }
#endif

