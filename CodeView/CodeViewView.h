
// CodeViewView.h : CCodeViewView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "GridCtrl_src/GridCtrl.h"
#include "GridCtrl_src/NewCellTypes/GridCellNumeric.h"

class CCodeViewView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CCodeViewView();
	DECLARE_DYNCREATE(CCodeViewView)

public:
	enum{ IDD = IDD_CODEVIEW_FORM };

// Ư���Դϴ�.
public:
	CCodeViewDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CGridCtrl m_Grid;

    void CCodeViewView::ProcessRecordset(CString strTable);
    CString CCodeViewView::uItoa(int num);

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CCodeViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // CodeViewView.cpp�� ����� ����
inline CCodeViewDoc* CCodeViewView::GetDocument() const
   { return reinterpret_cast<CCodeViewDoc*>(m_pDocument); }
#endif

