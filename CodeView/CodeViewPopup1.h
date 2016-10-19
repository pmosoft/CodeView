#pragma once


// CodeViewPopup1 대화 상자입니다.

class CodeViewPopup1 : public CDialogEx
{
	DECLARE_DYNAMIC(CodeViewPopup1)

public:
	CodeViewPopup1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CodeViewPopup1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CODEVIEW_POPUP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit1;
};
