#pragma once


// CodeViewPopup1 ��ȭ �����Դϴ�.

class CodeViewPopup1 : public CDialogEx
{
	DECLARE_DYNAMIC(CodeViewPopup1)

public:
	CodeViewPopup1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CodeViewPopup1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CODEVIEW_POPUP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit1;
};
