
// CodeViewView.cpp : CCodeViewView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "CodeView.h"
#endif

#include "CodeViewDoc.h"
#include "CodeViewView.h"
#include "CodeViewPopup1.h"

#include "afxdb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCodeViewView

IMPLEMENT_DYNCREATE(CCodeViewView, CFormView)

BEGIN_MESSAGE_MAP(CCodeViewView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &CCodeViewView::OnBnClickedButton1)
	ON_WM_SIZE()
//	ON_WM_PAINT()
ON_COMMAND(ID_EDIT_COPY, &CCodeViewView::OnEditCopy)
//ON_COMMAND(ID_EDIT_CUT, &CCodeViewView::OnEditCut)
//ON_COMMAND(ID_EDIT_PASTE, &CCodeViewView::OnEditPaste)
ON_COMMAND(ID_EDIT_PASTE, &CCodeViewView::OnEditPaste)
ON_CBN_SELCHANGE(IDC_COMBO1, &CCodeViewView::OnCbnSelchangeCombo1)
ON_COMMAND(ID_FILE_OPEN, &CCodeViewView::OnFileOpen)
ON_COMMAND(ID_FILE_SAVE, &CCodeViewView::OnFileSave)
ON_COMMAND(ID_FILE_SAVE_AS, &CCodeViewView::OnFileSaveAs)
//ON_COMMAND(ID_EDIT_TABNAME, &CCodeViewView::OnEditTabname)
//ON_UPDATE_COMMAND_UI(ID_EDIT_TABNAME, &CCodeViewView::OnUpdateEditTabname)
ON_COMMAND(ID_EDIT_TABNAME, &CCodeViewView::OnEditTabname)
END_MESSAGE_MAP()

// CCodeViewView ����/�Ҹ�

CCodeViewView::CCodeViewView()
	: CFormView(CCodeViewView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	m_edit1 = _T("");
	//  m_nCombo1 = _T("");
	m_nCombo1 = 0;
}

CCodeViewView::~CCodeViewView()
{
}

void CCodeViewView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid); // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	//  DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit1);
	//  DDX_CBString(pDX, IDC_COMBO1, m_nCombo1);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nCombo1);
}

BOOL CCodeViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CCodeViewView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

    //------------------------------------
    // �׸��� ����
    //------------------------------------
    m_Grid.SetFixedColumnSelection(TRUE);
    m_Grid.SetFixedRowSelection(TRUE);

    m_Grid.SetHeaderSort(1);
    m_Grid.SetSingleRowSelection(1);
    m_Grid.AllowReorderColumn(TRUE); // implemented now only if m_bCallback
    m_Grid.EnableDragRowMode(TRUE);

    // �׸��� �÷��� ����
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(1);

    m_Grid.SetRowCount(300);
    m_Grid.SetColumnCount(5);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

    //------------------------------------
    // �޺��ڽ� ����
    //------------------------------------
    // �޺� ITEM ���� ��ŭ ���� ����
    CRect rect;
    m_combo1.GetDroppedControlRect(rect);
    m_combo1.SetWindowPos(NULL,0,0,rect.Width(),rect.Height()+100,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

    // �޺� �ʱⰪ
    m_combo1.SetCurSel(0);

}

void CCodeViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCodeViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCodeViewView ����

#ifdef _DEBUG
void CCodeViewView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCodeViewView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCodeViewDoc* CCodeViewView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeViewDoc)));
	return (CCodeViewDoc*)m_pDocument;
}
#endif //_DEBUG



void CCodeViewView::ProcessRecordset(CString strTable)
{

	// TODO: Add extra initialization here
	
	
    //------------
	// ���� ����
	//------------	
	CString strTableName1 = m_edit1.Right(m_edit1.GetLength()-m_edit1.Find('.')-1).TrimRight('\n');
    char dblinkYn = strTableName1.Find('@') > 0 ? 'Y' : 'N';

	CString strDblinkName = "";
	CString strTableName2 = "";
	
    if (dblinkYn == 'Y') {
        strDblinkName = strTableName1.Right(strTableName1.GetLength()-strTableName1.Find('@'));
	    strTableName2 = strTableName1.Left(strTableName1.Find('@'));
    } else {
	    strTableName2 = strTableName1.Trim();
    } 
	

    //-------------------
    // �׸��� �÷��� ����
    //-------------------
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(1);

    m_Grid.SetRowCount(300);
    m_Grid.SetColumnCount(5);

    //------------
	// ��������
	//------------
    CDatabase db;
    CRecordset rs;

	CString strSql;
	CString strSql02 = "";
	CString strSql03 = "";
	CString strSql04 = "";

    CODBCFieldInfo *m_pFieldinfo;
	CString strField;
	CString strField2;

	int rowCnt;

	CString sCombo1;

    TRACE("start");





    //------------
	// db open
	//------------
    //db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    //db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));


	m_combo1.GetWindowText(sCombo1);
    TRACE("01 sCombo1=%s\r\n",sCombo1);

	//TRACE("01 sCombo1=%i\r\n",_ttoi(sCombo1.Mid(0,sCombo1.Find(":"))));
    //TRACE("01 m_edit1.TrimRight=%s\r\n",m_edit1.TrimRight);


    if      (m_edit1 == _T(""))
    {
        strSql = "SELECT ' ' col FROM FA_CM90200 WHERE 1=2";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
    else if (sCombo1 == "11:���̺��������")
    {
        TRACE("01:���̺�����\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT * FROM TMP_TABLE_INFO_01 WHERE ���̺�����1 LIKE UPPER('%"+strTableName2+"%') OR  ���̺�����2 LIKE UPPER('%"+strTableName2+"%') OR ���̺��ѱ۸�2 LIKE UPPER('%"+strTableName2+"%') \n";

        } else if (dblinkYn == 'Y') {
        }
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
    else if (sCombo1 == "12:���̺�����")
    {
        TRACE("01:���̺�����\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT  A.TABLE_NAME                         AS �������̺�� \n"
                     "      ,(SELECT MAX(COMMENTS) FROM ALL_TAB_COMMENTS WHERE OWNER = 'INST1' AND TABLE_NAME = A.TABLE_NAME) AS �ѱ����̺�� \n"
                     "       ,A.COLUMN_ID                          AS ���� \n"
                     "       ,A.COLUMN_NAME                        AS �ʵ�� \n"
                     "       ,CASE WHEN A.DATA_TYPE IN ('CHAR','VARCHAR','VARCHAR2') THEN A.DATA_TYPE||'('||A.DATA_LENGTH||')' \n"
                     "             WHEN A.DATA_TYPE IN ('NUMBER') THEN A.DATA_TYPE||'('||A.DATA_PRECISION||','||A.DATA_SCALE||')' \n"
                     "             WHEN A.DATA_TYPE IN ('DATE','TIMESTAMP') THEN A.DATA_TYPE \n"
                     "             ELSE A.DATA_TYPE \n"
                     "        END                                  AS DATA_TYPE \n"
                     "       ,A.NULLABLE                           AS N \n"
                     "       ,DECODE(C.COLUMN_NAME,NULL,NULL,'PK') AS PK \n";

            strSql02="FROM   ALL_TAB_COLUMNS  A, \n"
                     "       ALL_COL_COMMENTS B, \n"
                     "      (SELECT COLUMN_NAME \n"
                     "       FROM ALL_IND_COLUMNS \n"
                     "       WHERE TABLE_NAME LIKE UPPER('"+m_edit1.Trim()+"') AND TABLE_OWNER = 'INST1' \n"
                     "       AND   INDEX_NAME = (SELECT INDEX_NAME FROM ALL_INDEXES \n"
                     "                           WHERE OWNER = 'INST1' AND TABLE_NAME = UPPER('"+m_edit1.Trim()+"') AND UNIQUENESS = 'UNIQUE')) C \n"
                     "WHERE  A.OWNER = B.OWNER AND A.OWNER = 'INST1' \n"
                     "AND    A.TABLE_NAME  = B.TABLE_NAME \n"
                     "AND    A.COLUMN_NAME = B.COLUMN_NAME \n"
                     "AND    A.COLUMN_NAME = C.COLUMN_NAME(+) \n"
                     "AND    A.TABLE_NAME  LIKE UPPER('"+m_edit1.Trim()+"') \n"
                     "AND   (   A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),A.COLUMN_NAME)||'%' \n"
                     "       OR A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                     "       OR A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                     "      ) \n"
					 "ORDER BY A.TABLE_NAME,A.COLUMN_ID \n"; 

        } else if (dblinkYn == 'Y') {
        }
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
    else if (sCombo1 == "13:���̺���θ��")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT * FROM TMP_TABLE_INFO_02 WHERE ���̺�����1 LIKE UPPER('%"+strTableName2+"%') OR ���̺�����2 LIKE UPPER('%"+strTableName2+"%') OR ���̺��ѱ۸�2 = UPPER('%"+strTableName2+"%') \n";
                    
        } else if (dblinkYn == 'Y') {            
        } 
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

	}
    else if (sCombo1 == "14:���̺�����_PKG")
    {
        TRACE("01:���̺�����\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT  A.TABLE_NAME                         AS �������̺�� \n"
                     "      ,CASE WHEN (SELECT MAX(COMMENTS) FROM SYS.ALL_TAB_COMMENTS WHERE TABLE_NAME = A.TABLE_NAME) IS NOT NULL \n"
                     "            THEN (SELECT MAX(COMMENTS) FROM SYS.ALL_TAB_COMMENTS WHERE TABLE_NAME = A.TABLE_NAME) \n"
                     "            ELSE (SELECT MAX(���̺��ѱ۸�2) FROM TMP_TABLE_INFO_02 WHERE ���̺�����2 = '"+m_edit1.Trim()+"') END AS �ѱ����̺�� \n"
                     "       ,A.COLUMN_ID                          AS ���� \n"
                     "       ,A.COLUMN_NAME                        AS �����ʵ�� \n"
                     "       ,CASE WHEN A.TABLE_NAME LIKE 'TSK%' THEN A.COLUMN_NAME ELSE B.COMMENTS END AS �ѱ��ʵ�� \n"
                     "       ,CASE WHEN A.DATA_TYPE IN ('CHAR','VARCHAR','VARCHAR2') THEN A.DATA_TYPE||'('||A.DATA_LENGTH||')' \n"
                     "             WHEN A.DATA_TYPE IN ('NUMBER') THEN A.DATA_TYPE||'('||A.DATA_PRECISION||','||A.DATA_SCALE||')' \n"
                     "             WHEN A.DATA_TYPE IN ('DATE','TIMESTAMP') THEN A.DATA_TYPE \n"
                     "             ELSE A.DATA_TYPE \n"
                     "        END                                  AS DATA_TYPE \n"
                     "       ,A.NULLABLE                           AS N \n"
                     "       ,DECODE(C.COLUMN_NAME,NULL,NULL,'PK') AS PK \n";

            strSql02="FROM   ALL_TAB_COLUMNS  A, \n"
                     "       ALL_COL_COMMENTS B, \n"
                     "      (SELECT COLUMN_NAME \n"
                     "       FROM ALL_IND_COLUMNS \n"
                     "       WHERE TABLE_NAME LIKE UPPER('"+m_edit1.Trim()+"') \n"
                     "       AND   INDEX_NAME = (SELECT INDEX_NAME FROM ALL_INDEXES \n"
                     "                           WHERE TABLE_NAME = UPPER('"+m_edit1.Trim()+"') AND UNIQUENESS = 'UNIQUE')) C \n"
                     "WHERE  A.OWNER = B.OWNER \n"
                     "AND    A.TABLE_NAME  = B.TABLE_NAME \n"
                     "AND    A.COLUMN_NAME = B.COLUMN_NAME \n"
                     "AND    A.COLUMN_NAME = C.COLUMN_NAME(+) \n"
                     "AND    A.TABLE_NAME  LIKE UPPER('"+m_edit1.Trim()+"') \n"
                     "AND   (   A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),A.COLUMN_NAME)||'%' \n"
                     "       OR A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                     "       OR A.COLUMN_NAME LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                     "      ) \n"
					 "ORDER BY A.TABLE_NAME,A.COLUMN_ID \n"; 

        } else if (dblinkYn == 'Y') {
        }
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

    }
    else if (sCombo1 == "15:���̺���INST1")
    {
        strSql = "SELECT A.TABLE_NAME,B.COMMENTS \n"
                 "FROM  ALL_TABLES A, ALL_TAB_COMMENTS B  \n"
                 "WHERE A.TABLE_NAME = B.TABLE_NAME  \n"
                 "AND A.OWNER = 'INST1' AND B.OWNER = 'INST1' AND A.TABLE_NAME  LIKE UPPER('%"+m_edit1.Trim()+"%') \n"    
                 "AND LENGTH(TRIM(A.TABLE_NAME)) = 9 \n"
                 "AND   (   B.COMMENTS LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),B.COMMENTS)||'%' \n"
                 "       OR B.COMMENTS LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR B.COMMENTS LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR A.TABLE_NAME LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),A.TABLE_NAME)||'%' \n"
                 "       OR A.TABLE_NAME LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR A.TABLE_NAME LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n";

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));

	}
    else if (sCombo1 == "16:��INDEX�˻�")
    {
        strSql = "SELECT TABLE_NAME,INDEX_NAME,REPLACE(SUBSTR(I2,INSTR(I2,'(')+1,LENGTH(I2)),');',''),I2 \n"
                 "FROM  ( \n"
                 "       SELECT   REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(I1,',)',')'),',)',')'),',)',')'),',)',')'),',)',')'),',)',')'),',)',')'),',)',')'),',)',')'),',)',')') I2 \n"
                 "               ,TABLE_NAME,INDEX_NAME \n"
                 "       FROM   ( \n"
                 "               SELECT 'CREATE '||DECODE(MAX(A.UNIQUENESS),'NONUNIQUE','',MAX(A.UNIQUENESS))||' INDEX '||' '||INDEX_NAME||' ON '||MAX(A.TABLE_OWNER)||'.'||MAX(A.TABLE_NAME)||'(' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  1 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  2 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  3 THEN COLUMN_NAME ELSE NULL END)||',' \n";
        strSql02="                      ||MAX(CASE A.COLUMN_POSITION WHEN  4 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  5 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  6 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  7 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  8 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN  9 THEN COLUMN_NAME ELSE NULL END)||',' \n"
                 "                      ||MAX(CASE A.COLUMN_POSITION WHEN 10 THEN COLUMN_NAME ELSE NULL END)||',);' I1 \n";
        strSql03="                      ,MAX(A.TABLE_NAME) TABLE_NAME \n"
                 "                      ,A.INDEX_NAME  INDEX_NAME \n"
                 "                      --,MAX(COLUMN_POSITION) \n"
                 "               FROM  (SELECT A.TABLE_NAME,A.COLUMN_NAME,A.INDEX_NAME,A.COLUMN_POSITION,B.UNIQUENESS,A.TABLE_OWNER \n"
                 "                      FROM ALL_IND_COLUMNS A, \n"
                 "                                 ALL_INDEXES B \n"
                 "                      WHERE A.TABLE_OWNER = 'INST1' AND B.TABLE_OWNER = 'INST1' \n"
                 "                      AND   A.TABLE_NAME LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
                 "                      AND   A.INDEX_NAME = B.INDEX_NAME \n"
                 "                      ORDER BY A.INDEX_NAME,A.COLUMN_POSITION) A \n"
                 "               GROUP BY A.INDEX_NAME \n"
                 "               ) \n"
                 "       ) \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }

    else if (sCombo1 == "21:���ڵ�׷��˻�")
    {

        strSql = "SELECT * FROM TMP_CODE_INFO_01 WHERE �ڵ�׷�� LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
                 "AND   (   �ڵ�� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),�ڵ��)||'%' \n"
                 "       OR �ڵ�� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �ڵ�� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),�ڵ�ID)||'%' \n"
                 "       OR �ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
				 "UNION SELECT 'ITJC050' �ڵ�׷�ID,'�а����_��������'�ڵ�׷��,TGT_J_ACC_GB �ڵ�ID, CALC �ڵ��,TGT_TR_GB||'_'||CD_GB,CALC_NM FROM ITJC050 WHERE '�а����_��������' LIKE UPPER('"+m_edit1.Trim()+"%')  \n";
			//"ORDER BY �ڵ�׷�ID,�ڵ�ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

    }
    else if (sCombo1 == "22:���ڵ�ID�˻�")
    {
        strSql = "SELECT * FROM TMP_CODE_INFO_01 WHERE �ڵ�ID = UPPER('"+m_edit1.Trim()+"')          \n"
		         "UNION SELECT 'ITMC010' �ڵ�׷�ID,'��������_����'�ڵ�׷��,SEC_ID �ڵ�ID, KOR_NM �ڵ��,'','' FROM ITMC010 \n"
				 "WHERE SEC_ID LIKE UPPER('"+m_edit1.Trim()+"%')  \n"
                 "AND   (   KOR_NM LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),KOR_NM)||'%' \n"
                 "       OR KOR_NM LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR KOR_NM LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
				 "UNION SELECT 'ITJC050' �ڵ�׷�ID,'�а����_��������'�ڵ�׷��,TGT_J_ACC_GB �ڵ�ID, CALC �ڵ��,TGT_TR_GB||'_'||CD_GB,CALC_NM FROM ITJC050 WHERE TGT_J_ACC_GB LIKE UPPER('"+m_edit1.Trim()+"%')  \n";
                 //"ORDER BY �ڵ�׷�ID,�ڵ�ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

    }
    else if (sCombo1 == "23:���ڵ�׷��˻�")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE FA�����ڵ�ID LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
                 "AND   (   FA���ڵ�� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA���ڵ��)||'%' \n"
                 "       OR FA���ڵ�� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA���ڵ�� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR ���ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),���ڵ�ID)||'%' \n"
                 "       OR ���ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR ���ڵ�ID LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "ORDER BY FA�����ڵ�ID,���ڵ�ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "24:���ڵ�ID�˻�")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE ���ڵ�ID = UPPER('"+m_edit1.Trim()+"')          \n";

                 //"ORDER BY �ڵ�׷�ID,�ڵ�ID                                              \n";
        db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "25:���ڵ�ID��˻�")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE FA���ڵ�� = UPPER('"+m_edit1.Trim()+"')          \n";

                 //"ORDER BY �ڵ�׷�ID,�ڵ�ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "26:���ڵ������ڵ�")
    {
        strSql = "SELECT FA�����ڵ�,FA���ȸ���ڵ�,FA�����ѱ۸�,�����ߺз�����,(SELECT MAX(FA���ڵ��) FROM INST1.TSKGHFA21 WHERE FA�����ڵ�ID = '�����ߺз������ڵ�' AND ���ڵ�ID = A.�����ߺз�����) �����ߺз����и�,�ý�����������ڹ�ȣ,�ý�������ó���Ͻ�, \n"
      			 "A.����ǥ�ذŷ����ڵ�,A.FA���屸��,A.FA��������,A.FA�����ڵ�,A.��ȭ�ڵ�,A.������ȭ�ڵ�,A.����ȭ�ڵ�,A.FA��õ¡������,A.�ؿܿ�õ¡������ \n"			
                 "FROM INST1.TSKGHFB01 A WHERE FA�����ڵ� LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA�����ѱ۸�)||'%' \n"
                 "       OR FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),�����ߺз�����)||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
				 "      ) \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "27:���ڵ������ڵ�2")
    {
        strSql = "SELECT FA�����ڵ�,FA���ȸ���ڵ�,FA�����ѱ۸�,�����ߺз�����,(SELECT MAX(FA���ڵ��) FROM INST1.TSKGHFA21 WHERE FA�����ڵ�ID = '�����ߺз������ڵ�' AND ���ڵ�ID = A.�����ߺз�����) �����ߺз����и�,�ý�����������ڹ�ȣ,�ý�������ó���Ͻ�, \n"
      			 "A.����ǥ�ذŷ����ڵ�,A.FA���屸��,A.FA��������,A.FA�����ڵ�,A.��ȭ�ڵ�,A.������ȭ�ڵ�,A.����ȭ�ڵ�,A.FA��õ¡������,A.�ؿܿ�õ¡������ \n"			
			     "FROM INST1.TSKGHFB01 A WHERE FA�����ڵ� LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA�����ѱ۸�)||'%' \n"
                 "       OR FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA�����ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),�����ߺз�����)||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �����ߺз����� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "AND �����ߺз����� <> 'ZZZZ' \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "28:�Űŷ��ڵ�")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA43 WHERE FA�ŷ��ڵ� LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   �ŷ��ڵ��ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),�ŷ��ڵ��ѱ۸�)||'%' \n"
                 "       OR �ŷ��ڵ��ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR �ŷ��ڵ��ѱ۸� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA�ŷ��ڵ� LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA�ŷ��ڵ�)||'%' \n"
                 "       OR FA�ŷ��ڵ� LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA�ŷ��ڵ� LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "ORDER BY FA�ŷ��ڵ� \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "31:�÷��������ΰ˻�")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT * FROM TMP_TABLE_INFO_01 WHERE �÷���1 = UPPER('"+strTableName2+"') OR �÷���2 = UPPER('"+strTableName2+"') \n";
                    
        } else if (dblinkYn == 'Y') {            
        } 
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

	}

    else if (sCombo1 == "32:���ʵ�˻�")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT  A.TABLE_NAME                         AS �������̺�� \n"
                     "      ,(SELECT MAX(COMMENTS) FROM ALL_TAB_COMMENTS WHERE OWNER = 'INST1' AND TABLE_NAME = A.TABLE_NAME) AS �ѱ����̺�� \n"
                     "       ,A.COLUMN_ID                          AS ���� \n"
                     "       ,A.COLUMN_NAME                        AS �ʵ�� \n"
                     "       ,CASE WHEN A.DATA_TYPE IN ('CHAR','VARCHAR','VARCHAR2') THEN A.DATA_TYPE||'('||A.DATA_LENGTH||')' \n"
                     "             WHEN A.DATA_TYPE IN ('NUMBER') THEN A.DATA_TYPE||'('||A.DATA_PRECISION||','||A.DATA_SCALE||')' \n"
                     "             WHEN A.DATA_TYPE IN ('DATE','TIMESTAMP') THEN A.DATA_TYPE \n"
                     "             ELSE A.DATA_TYPE \n"
                     "        END                                  AS DATA_TYPE \n"
                     "FROM   ALL_TAB_COLUMNS  A \n"
                     "WHERE  OWNER = 'INST1' AND A.COLUMN_NAME LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
					 "AND (LENGTH(TRIM(A.TABLE_NAME)) IN (9,7)) \n"
                     "ORDER BY A.COLUMN_NAME,A.TABLE_NAME \n"
                     ;

        } else if (dblinkYn == 'Y') {            
        } 
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
	}
    else if (sCombo1 == "41:�а�����")
    {
        strSql = "SELECT A.�а��׷��ڵ�,A.�����ߺз�����,(SELECT MAX(FA���ڵ��) FROM INST1.TSKGHFA21 WHERE FA�����ڵ�ID = '�����ߺз������ڵ�' AND ���ڵ�ID = A.�����ߺз�����) �����ߺз��ڵ��,A.FA�ŷ��ڵ�,A.�ý�����������ڹ�ȣ,A.�ý�������ó���Ͻ� \n"
			     "FROM INST1.TSKGHFF09 A \n"
				 "WHERE �����ߺз����� = '"+m_edit1.Trim()+"' \n"
				 "OR    �����ߺз����� = '"+m_edit2.Trim()+"' \n"
				 "OR    �����ߺз����� = '"+m_edit3.Trim()+"' \n"
				 "OR    �����ߺз����� = '"+m_edit4.Trim()+"' \n"
				 "ORDER BY 1 DESC \n" 
				 ;

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
	}
    else if (sCombo1 == "91:���ν���INST1")
    {
        strSql = "SELECT A.NAME \n"
			     "       ,REPLACE(REPLACE(REPLACE(A.TEXT, chr(13),''),chr(10),''),chr(09),'') TEXT \n"
				 "FROM ALL_SOURCE"+strDblinkName+" A WHERE A.NAME LIKE UPPER('"+m_edit1.Trim()+"') \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
    else if (sCombo1 == "92:���ν���KGHPKG")
    {
        strSql = "SELECT A.NAME \n"
			     "       ,REPLACE(REPLACE(REPLACE(A.TEXT, chr(13),''),chr(10),''),chr(09),'') TEXT \n"
				 "FROM ALL_SOURCE"+strDblinkName+" A WHERE A.NAME LIKE UPPER('"+m_edit1.Trim()+"') \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
	else if (sCombo1 == "93:���ν��� ������")
    {
        strSql = "SELECT A.P_NM,A.P_NM_H,LEVEL,LPAD(' ',8*(LEVEL-1))||C_NM||'('||C_NM_H||')' HIERARCHY \n"
			     "FROM   KGH_DEPENDENCIES A \n"
				 "WHERE A.C_NM LIKE 'PC_KGH_CP_JL%'"
			     "START WITH A.P_NM = '"+m_edit1.Trim()+"' \n"
			     "CONNECT BY PRIOR A.C_NM = A.P_NM \n";

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
	else if (sCombo1 == "94:���ν��� ��������")
    {
        strSql = "SELECT * FROM KGH_DEPENDENCIES WHERE C_NM = '"+m_edit1.Trim()+"' \n";

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
	else if (sCombo1 == "95:PLSQLDEV USING")
    {
        strSql = "SELECT A.SID,A.SERIAL# \n"
                 "      ,A.USERNAME \n"
                 "      ,A.OSUSER \n"
                 "      ,A.MACHINE \n"
                 "      ,A.PROGRAM \n"
                 "      ,A.ACTION \n"
                 "FROM  V$SESSION A \n"
                 "WHERE A.STATUS = 'ACTIVE' \n" 
                 "AND   A.PROGRAM = 'plsqldev.exe' \n"
                 "ORDER BY 1,2 \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	//TRACE("01 strSql=%s\r\n",strSql);
	TRACE("01 strSql=\r\n\r\n%s",strSql);
	TRACE("%s",strSql02);
	TRACE("%s",strSql03);
	TRACE("%s\r\n\r\n",strSql04);
	//TRACE("01 strSql=\r\n%s\r\n",strSql+strSql02+strSql03);

	rs.m_pDatabase = &db;	
	rs.Open(CRecordset::forwardOnly,strSql+strSql02+strSql03+strSql04);


    //------------
	// �ʵ�����
	//------------
    m_pFieldinfo = new CODBCFieldInfo[rs.GetODBCFieldCount()];

    for (int i=0; i<rs.GetODBCFieldCount(); i++) {
        rs.GetODBCFieldInfo((short) i , m_pFieldinfo[i]);
            //TRACE("02 i=%d\r\n",i);
        //TRACE("02 m_nSQLType=%d\r\n",m_pFieldinfo[i].m_nSQLType);
		if(m_pFieldinfo[i].m_nSQLType == 8) 
		{
			//TRACE("021 i=%d\r\n",i);
		    m_Grid.SetCellType(0,i+1,RUNTIME_CLASS(CGridCellNumeric));
		}
    }

    m_Grid.SetColumnCount(rs.GetODBCFieldCount()+1);


	for (int j=0; j<rs.GetODBCFieldCount(); j++) {
	    m_Grid.SetItemText(0,j+1,m_pFieldinfo[j].m_strName);
	}

    rowCnt=1;

    char str[30];

	while(!rs.IsEOF())
    {
		itoa(rowCnt,str,10);
        m_Grid.SetItemText(rowCnt,0,uItoa(rowCnt));
		for (int colCnt=0; colCnt<rs.GetODBCFieldCount(); colCnt++) {
            rs.GetFieldValue(short(colCnt),strField);

            
          
			if(m_pFieldinfo[colCnt].m_nSQLType == 8) 
			{
				//TRACE("03 m_nSQLType=%d\r\n",m_pFieldinfo[colCnt].m_nSQLType);
	            //TRACE("03 colCnt=%d\r\n",colCnt);
				//TRACE("03 strField=%s\r\n",strField);
				m_Grid.SetCellType(rowCnt,colCnt+1,RUNTIME_CLASS(CGridCellNumeric));
	        }			
			
			m_Grid.SetItemText(rowCnt,colCnt+1,strField);

            //TRACE("strField=%s",strField);
		}
        rs.MoveNext();
        rowCnt++;

        switch(rowCnt) {
			case   50 : m_Grid.SetRowCount(200);
			case  150 : m_Grid.SetRowCount(400);
			case  350 : m_Grid.SetRowCount(600);
			case  550 : m_Grid.SetRowCount(1000);
			case  900 : m_Grid.SetRowCount(2000);
			case 1900 : m_Grid.SetRowCount(3000);
			case 2900 : m_Grid.SetRowCount(4000);
			case 3900 : m_Grid.SetRowCount(5000);
			case 4900 : m_Grid.SetRowCount(6000);
			case 5900 : m_Grid.SetRowCount(7000);
			case 6900 : m_Grid.SetRowCount(8000);
			case 7900 : m_Grid.SetRowCount(9000);
			case 8900 : m_Grid.SetRowCount(10000);
		}

    }

    m_Grid.SetRowCount(rowCnt);
    m_Grid.SetRowResize(TRUE);
    m_Grid.SetColumnResize(TRUE);
	m_Grid.AutoSize(GVS_DEFAULT);

    // VIEW ������ ����(��ũ��)
    //CSize sizeTotal;
    //2,0,402,43
    //sizeTotal.cx = m_Grid.GetVirtualWidth(); 
	//sizeTotal.cy = m_Grid.GetVirtualHeight();
    //sizeTotal.cx = m_Grid.GetVirtualWidth() > 1500 ? 1500 : m_Grid.GetVirtualWidth();
	//sizeTotal.cy = m_Grid.GetVirtualHeight() > 800 ? 800  : m_Grid.GetVirtualHeight();   
	
	//m_Grid.MoveWindow(12,50,(sizeTotal.cx > 1200 ? 1200 :sizeTotal.cx+12),(sizeTotal.cy > 500 ? 500 :sizeTotal.cy+50), FALSE);
    //m_Grid.MoveWindow(12,50,sizeTotal.cx+100,sizeTotal.cy+20, FALSE);
    //SetScrollSizes(MM_TEXT,sizeTotal);
	//SendMessage(WM_SIZE,0,0);
	
	//SetWindowPos();
	//WM_SIZE->OnSize();
	//onSize();
	//SendMessage(WM_SIZE,(WPARAM)SIZE_RESTORED,MAKELPARAM(m_mWidth,m_mHeight));
	Invalidate();



    //Invalidate();
	//MoveWindow(0,0, (sizeTotal.cx > 400 ? 400 :sizeTotal.cx),(sizeTotal.cy > 1000 ? 1000 :sizeTotal.cy), FALSE);
    //Invalidate();

	rs.Close();
    db.Close();


    //-------------------------------
	// �Ҹ���
	//-------------------------------
    delete [] m_pFieldinfo;

}


CString CCodeViewView::uItoa(int num)
{
    char str[30];
   	itoa(num,str,10);
   	return str;
}



// CCodeViewView �޽��� ó����


void CCodeViewView::OnBnClickedButton1()
{
	UpdateData(true);
    TRACE("05:���̺�����");
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessRecordset("Cats_Tmp_Approval");
}



void CCodeViewView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (cx <= 1 || cy <= 1 ) 
        return;	
	//TRACE("01 OnSize cx = %d\r\n",cx);
	//TRACE("01 OnSize cy = %d\r\n",cy);
	if(m_Grid.GetSafeHwnd())
        //m_Grid.MoveWindow(0,0, cx,cy, FALSE);
	    m_Grid.MoveWindow(12,50,cx-12,cy-50, FALSE);

}


//void CCodeViewView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	// �׸��� �޽����� ���ؼ��� CFormView::OnPaint()��(��) ȣ������ ���ʽÿ�.
//}


void CCodeViewView::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	//CEdit* pEdit = (CEdit*)GetFocus();
	//if(pEdit) m_Grid.OnEditCopy();
	//else pEdit->Copy();

	//CEdit* pWndEdit = (CEdit*) m_wndTabs.GetActiveWnd ();
	//ASSERT_VALID (pWndEdit);
	//pWndEdit->Copy ();
    // CWnd *wnd;
    // wnd = GetDlgItem( IDC_EDIT2);
    // wnd->SetFocus();
	
    CEdit* pFocusEdit = (CEdit*)GetFocus();  // ���� ��Ŀ���� ����

    CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
    CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
    CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT3);
    CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT4);

    if (pFocusEdit == pEdit1)  // ���� ��Ŀ���� ù��° ����Ʈ �ڽ��� �ִ� ���
    {
        pEdit1->SetSel(0,-1); // ��ü����
        pEdit1->Copy();       // Ŭ������� ����
    }
    else if (pFocusEdit == pEdit2)  // ���� ��Ŀ���� �ι�° ����Ʈ �ڽ��� �ִ� ���
    {
	    pEdit2->SetSel(0,-1);
        pEdit2->Copy();
    }
    else if (pFocusEdit == pEdit3)  // ���� ��Ŀ���� �ι�° ����Ʈ �ڽ��� �ִ� ���
    {
	    pEdit3->SetSel(0,-1);
        pEdit3->Copy();
    }   
    else if (pFocusEdit == pEdit4)  // ���� ��Ŀ���� �ι�° ����Ʈ �ڽ��� �ִ� ���
    {
	    pEdit4->SetSel(0,-1);
        pEdit4->Copy();
    }
    else
    {
	    m_Grid.OnEditCopy();
    }

}     

//void CCodeViewView::OnEditCut()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	m_Grid.OnEditCut();
//}

void CCodeViewView::OnEditPaste()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//m_ctrlEdit1.Paste();
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Paste();
}


void CCodeViewView::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CCodeViewView::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW, NULL);
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW, NULL);
	int cnt = 0;
	CString strTabInfo = "";
	CString token;
	CString token2;

	int iPos = 0;

	CDocument* pDoc = GetDocument();
    CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	CDocument* pCurDoc = NULL;
    POSITION pos = pTemplate->GetFirstDocPosition();

	

	
	char szFilter[] = "Tab File (*.tab)|*.tab|All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, "tab", NULL, OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		
		TRY
		{
			CStdioFile file;

			while( (pCurDoc = pTemplate->GetNextDoc(pos)) != NULL)
			{
				cnt++;
				if(pos == NULL) break;

			}
			for(cnt > 0;cnt--;) AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_CLOSE, NULL);

			file.Open(dlg.GetPathName(), CFile::modeRead); 
   		    while(file.ReadString(strTabInfo))
    		{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW, NULL);
			}
			file.Close();

			file.Open(dlg.GetPathName(), CFile::modeRead);
			pos = pTemplate->GetFirstDocPosition();
    		while( (pCurDoc = pTemplate->GetNextDoc(pos)) != NULL)
    		{
				file.ReadString(strTabInfo);
				TRACE("strTabInfo=%s \r\n",strTabInfo);
				iPos = 0;
				token = strTabInfo.Tokenize("|",iPos);
				TRACE("tokenize1=%s \r\n",token);
				pCurDoc->SetTitle(token);

				POSITION posView = pCurDoc->GetFirstViewPosition();
				CCodeViewView* pView = (CCodeViewView*) pCurDoc->GetNextView(posView);

				token = strTabInfo.Tokenize("|",iPos);
				TRACE("tokenize2=%s \r\n",token);
				pView->m_nCombo1 = atoi(token);

				token = strTabInfo.Tokenize("|",iPos);
				TRACE("tokenize3=%s \r\n",token);
				pView->m_edit1 = token;

				//TRACE("m_nCombo1=%d \r\n",pView->m_nCombo1);
			    //TRACE("m_edit1=%s \r\n",pView->m_edit1.Trim());
			    
				pView->UpdateData(FALSE);
							   
				cnt++;
				if(pos == NULL) break;
    		}
	
			file.Close();
			

		}
		CATCH(CFileException, e)
		{
			e->ReportError();
		}
		END_CATCH
	}
	
}


void CCodeViewView::OnFileSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OnFileSaveAs();
}


void CCodeViewView::OnFileSaveAs()
{
	
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int cnt = 1;
	CString strTabInfo = "";
	
	CDocument* pDoc = GetDocument();
    CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	CDocument* pCurDoc = NULL;
    POSITION pos = pTemplate->GetFirstDocPosition();

	char szFilter[] = "Tab File (*.tab)|*.tab|All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, "tab", NULL, OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
		TRY
		{
			CFile file;
			file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite); 

    		while( (pCurDoc = pTemplate->GetNextDoc(pos)) != NULL)
    		{
				//strTabInfo.Format("���ڿ�%d", cnt);
				//pCurDoc->SetTitle(strTabInfo);

				POSITION posView = pCurDoc->GetFirstViewPosition();
				CCodeViewView* pView = (CCodeViewView*) pCurDoc->GetNextView(posView); 
				pView->UpdateData(TRUE);
				//TRACE("m_nCombo1=%d \r\n",pView->m_nCombo1);
			    //TRACE("m_edit1=%s \r\n",pView->m_edit1.Trim());
			    strTabInfo.Format("%s|%d|%s|\r\n",pCurDoc->GetTitle(),pView->m_nCombo1,pView->m_edit1.Trim());

			    TRACE("strTabInfo=%s \r\n",strTabInfo);
			    file.Write(strTabInfo, strTabInfo.GetLength());

				cnt++;
    		    if(pos == NULL) break;
    		}

			file.Close();

		}
		CATCH(CFileException, e)
		{
			e->ReportError();
		}
		END_CATCH
	}

}

//void CCodeViewView::OnEditTabname()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//}


//void CCodeViewView::OnUpdateEditTabname(CCmdUI *pCmdUI)
//{
//	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
//}



void CCodeViewView::OnEditTabname()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CodeViewPopup1 dlg;
	//dlg.SetFocus
	if(dlg.DoModal() == IDOK)
	{
		CString str;
		GetParent()->GetWindowText(str);
		
		//tParent()->SetWindowText(dlg.m_edit1);
		//CCodeViewView* pView = (CCodeViewView*)((CFrameWnd*) AfxGetMainWnd ())->GetActiveView ();
		//pView->SetWindowText(dlg.m_edit1);
        //CMainFrm* pFrame = (CMainFrm*)AfxGetMainWnd();
        //CNEView* pView = (CNEView*)pFrame->GetActiveView();

	    CDocument* pDoc = GetDocument();
        CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	    CDocument* pCurDoc = NULL;
        POSITION pos = pTemplate->GetFirstDocPosition();

   		while( (pCurDoc = pTemplate->GetNextDoc(pos)) != NULL)
   		{
			GetParent()->GetWindowText(str);

			if(str == pCurDoc->GetTitle() )
			{
			    pCurDoc->SetTitle(dlg.m_edit1);
			}   
   		    if(pos == NULL) break;
   		}
	}
}