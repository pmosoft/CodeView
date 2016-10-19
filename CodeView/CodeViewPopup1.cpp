// CodeViewPopup1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CodeView.h"
#include "CodeViewPopup1.h"
#include "afxdialogex.h"


// CodeViewPopup1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CodeViewPopup1, CDialogEx)

CodeViewPopup1::CodeViewPopup1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CodeViewPopup1::IDD, pParent)
{

	m_edit1 = _T("");
}

CodeViewPopup1::~CodeViewPopup1()
{
}

void CodeViewPopup1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(CodeViewPopup1, CDialogEx)
END_MESSAGE_MAP()


// CodeViewPopup1 메시지 처리기입니다.
