
// CodeViewView.cpp : CCodeViewView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CCodeViewView 생성/소멸

CCodeViewView::CCodeViewView()
	: CFormView(CCodeViewView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CCodeViewView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

    //------------------------------------
    // 그리드 세팅
    //------------------------------------
    m_Grid.SetFixedColumnSelection(TRUE);
    m_Grid.SetFixedRowSelection(TRUE);

    m_Grid.SetHeaderSort(1);
    m_Grid.SetSingleRowSelection(1);
    m_Grid.AllowReorderColumn(TRUE); // implemented now only if m_bCallback
    m_Grid.EnableDragRowMode(TRUE);

    // 그리드 컬럼수 세팅
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(1);

    m_Grid.SetRowCount(300);
    m_Grid.SetColumnCount(5);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

    //------------------------------------
    // 콤보박스 세팅
    //------------------------------------
    // 콤보 ITEM 갯수 만큼 높이 조정
    CRect rect;
    m_combo1.GetDroppedControlRect(rect);
    m_combo1.SetWindowPos(NULL,0,0,rect.Width(),rect.Height()+100,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);

    // 콤보 초기값
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


// CCodeViewView 진단

#ifdef _DEBUG
void CCodeViewView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCodeViewView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCodeViewDoc* CCodeViewView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeViewDoc)));
	return (CCodeViewDoc*)m_pDocument;
}
#endif //_DEBUG



void CCodeViewView::ProcessRecordset(CString strTable)
{

	// TODO: Add extra initialization here
	
	
    //------------
	// 인자 세팅
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
    // 그리드 컬럼수 세팅
    //-------------------
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(1);

    m_Grid.SetRowCount(300);
    m_Grid.SetColumnCount(5);

    //------------
	// 변수선언
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
    else if (sCombo1 == "11:테이블맵핑정보")
    {
        TRACE("01:테이블정보\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT * FROM TMP_TABLE_INFO_01 WHERE 테이블영문명1 LIKE UPPER('%"+strTableName2+"%') OR  테이블영문명2 LIKE UPPER('%"+strTableName2+"%') OR 테이블한글명2 LIKE UPPER('%"+strTableName2+"%') \n";

        } else if (dblinkYn == 'Y') {
        }
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
    else if (sCombo1 == "12:테이블정보")
    {
        TRACE("01:테이블정보\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT  A.TABLE_NAME                         AS 영문테이블명 \n"
                     "      ,(SELECT MAX(COMMENTS) FROM ALL_TAB_COMMENTS WHERE OWNER = 'INST1' AND TABLE_NAME = A.TABLE_NAME) AS 한글테이블명 \n"
                     "       ,A.COLUMN_ID                          AS 순번 \n"
                     "       ,A.COLUMN_NAME                        AS 필드명 \n"
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
    else if (sCombo1 == "13:테이블맵핑목록")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT * FROM TMP_TABLE_INFO_02 WHERE 테이블영문명1 LIKE UPPER('%"+strTableName2+"%') OR 테이블영문명2 LIKE UPPER('%"+strTableName2+"%') OR 테이블한글명2 = UPPER('%"+strTableName2+"%') \n";
                    
        } else if (dblinkYn == 'Y') {            
        } 
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

	}
    else if (sCombo1 == "14:테이블정보_PKG")
    {
        TRACE("01:테이블정보\r\n");
        //strSql = "SELECT TABLE_NAME FROM SYS.ALL_TAB_COLUMNS WHERE ROWNUM < 2";
        if (dblinkYn == 'N') {
			
            strSql = "SELECT  A.TABLE_NAME                         AS 영문테이블명 \n"
                     "      ,CASE WHEN (SELECT MAX(COMMENTS) FROM SYS.ALL_TAB_COMMENTS WHERE TABLE_NAME = A.TABLE_NAME) IS NOT NULL \n"
                     "            THEN (SELECT MAX(COMMENTS) FROM SYS.ALL_TAB_COMMENTS WHERE TABLE_NAME = A.TABLE_NAME) \n"
                     "            ELSE (SELECT MAX(테이블한글명2) FROM TMP_TABLE_INFO_02 WHERE 테이블영문명2 = '"+m_edit1.Trim()+"') END AS 한글테이블명 \n"
                     "       ,A.COLUMN_ID                          AS 순번 \n"
                     "       ,A.COLUMN_NAME                        AS 영문필드명 \n"
                     "       ,CASE WHEN A.TABLE_NAME LIKE 'TSK%' THEN A.COLUMN_NAME ELSE B.COMMENTS END AS 한글필드명 \n"
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
    else if (sCombo1 == "15:테이블목록INST1")
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
    else if (sCombo1 == "16:신INDEX검색")
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

    else if (sCombo1 == "21:구코드그룹명검색")
    {

        strSql = "SELECT * FROM TMP_CODE_INFO_01 WHERE 코드그룹명 LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
                 "AND   (   코드명 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),코드명)||'%' \n"
                 "       OR 코드명 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 코드명 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 코드ID LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),코드ID)||'%' \n"
                 "       OR 코드ID LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 코드ID LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
				 "UNION SELECT 'ITJC050' 코드그룹ID,'분개모듈_계산식정보'코드그룹명,TGT_J_ACC_GB 코드ID, CALC 코드명,TGT_TR_GB||'_'||CD_GB,CALC_NM FROM ITJC050 WHERE '분개모듈_계산식정보' LIKE UPPER('"+m_edit1.Trim()+"%')  \n";
			//"ORDER BY 코드그룹ID,코드ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

    }
    else if (sCombo1 == "22:구코드ID검색")
    {
        strSql = "SELECT * FROM TMP_CODE_INFO_01 WHERE 코드ID = UPPER('"+m_edit1.Trim()+"')          \n"
		         "UNION SELECT 'ITMC010' 코드그룹ID,'종목정보_공통'코드그룹명,SEC_ID 코드ID, KOR_NM 코드명,'','' FROM ITMC010 \n"
				 "WHERE SEC_ID LIKE UPPER('"+m_edit1.Trim()+"%')  \n"
                 "AND   (   KOR_NM LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),KOR_NM)||'%' \n"
                 "       OR KOR_NM LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR KOR_NM LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
				 "UNION SELECT 'ITJC050' 코드그룹ID,'분개모듈_계산식정보'코드그룹명,TGT_J_ACC_GB 코드ID, CALC 코드명,TGT_TR_GB||'_'||CD_GB,CALC_NM FROM ITJC050 WHERE TGT_J_ACC_GB LIKE UPPER('"+m_edit1.Trim()+"%')  \n";
                 //"ORDER BY 코드그룹ID,코드ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

    }
    else if (sCombo1 == "23:신코드그룹명검색")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE FA공통코드ID LIKE UPPER('%"+m_edit1.Trim()+"%') \n"
                 "AND   (   FA상세코드명 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA상세코드명)||'%' \n"
                 "       OR FA상세코드명 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA상세코드명 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 상세코드ID LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),상세코드ID)||'%' \n"
                 "       OR 상세코드ID LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 상세코드ID LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "ORDER BY FA공통코드ID,상세코드ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "24:신코드ID검색")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE 상세코드ID = UPPER('"+m_edit1.Trim()+"')          \n";

                 //"ORDER BY 코드그룹ID,코드ID                                              \n";
        db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "25:신코드ID명검색")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA21 WHERE FA상세코드명 = UPPER('"+m_edit1.Trim()+"')          \n";

                 //"ORDER BY 코드그룹ID,코드ID                                              \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "26:신코드종목코드")
    {
        strSql = "SELECT FA종목코드,FA운용회사코드,FA종목한글명,종목중분류구분,(SELECT MAX(FA상세코드명) FROM INST1.TSKGHFA21 WHERE FA공통코드ID = '종목중분류구분코드' AND 상세코드ID = A.종목중분류구분) 종목중분류구분명,시스템최종사용자번호,시스템최종처리일시, \n"
      			 "A.국제표준거래소코드,A.FA상장구분,A.FA과세구분,A.FA국가코드,A.통화코드,A.수령통화코드,A.평가통화코드,A.FA원천징수구분,A.해외원천징수구분 \n"			
                 "FROM INST1.TSKGHFB01 A WHERE FA종목코드 LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA종목한글명)||'%' \n"
                 "       OR FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),종목중분류구분)||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
				 "      ) \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "27:신코드종목코드2")
    {
        strSql = "SELECT FA종목코드,FA운용회사코드,FA종목한글명,종목중분류구분,(SELECT MAX(FA상세코드명) FROM INST1.TSKGHFA21 WHERE FA공통코드ID = '종목중분류구분코드' AND 상세코드ID = A.종목중분류구분) 종목중분류구분명,시스템최종사용자번호,시스템최종처리일시, \n"
      			 "A.국제표준거래소코드,A.FA상장구분,A.FA과세구분,A.FA국가코드,A.통화코드,A.수령통화코드,A.평가통화코드,A.FA원천징수구분,A.해외원천징수구분 \n"			
			     "FROM INST1.TSKGHFB01 A WHERE FA종목코드 LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA종목한글명)||'%' \n"
                 "       OR FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA종목한글명 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),종목중분류구분)||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 종목중분류구분 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "AND 종목중분류구분 <> 'ZZZZ' \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "28:신거래코드")
    {
        strSql = "SELECT * FROM INST1.TSKGHFA43 WHERE FA거래코드 LIKE UPPER('"+m_edit1.Trim()+"%')          \n"
                 "AND   (   거래코드한글명 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),거래코드한글명)||'%' \n"
                 "       OR 거래코드한글명 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR 거래코드한글명 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA거래코드 LIKE '%'||NVL(UPPER('"+m_edit2.Trim()+"'),FA거래코드)||'%' \n"
                 "       OR FA거래코드 LIKE '%'||NVL(UPPER('"+m_edit3.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "       OR FA거래코드 LIKE '%'||NVL(UPPER('"+m_edit4.Trim()+"'),UPPER('"+m_edit2.Trim()+"'))||'%' \n"
                 "      ) \n"
                 "ORDER BY FA거래코드 \n"
				 ;
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
	else if (sCombo1 == "31:컬럼영문맵핑검색")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT * FROM TMP_TABLE_INFO_01 WHERE 컬럼명1 = UPPER('"+strTableName2+"') OR 컬럼명2 = UPPER('"+strTableName2+"') \n";
                    
        } else if (dblinkYn == 'Y') {            
        } 
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));

	}

    else if (sCombo1 == "32:신필드검색")
    {
        if (dblinkYn == 'N') {
            strSql = "SELECT  A.TABLE_NAME                         AS 영문테이블명 \n"
                     "      ,(SELECT MAX(COMMENTS) FROM ALL_TAB_COMMENTS WHERE OWNER = 'INST1' AND TABLE_NAME = A.TABLE_NAME) AS 한글테이블명 \n"
                     "       ,A.COLUMN_ID                          AS 순번 \n"
                     "       ,A.COLUMN_NAME                        AS 필드명 \n"
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
    else if (sCombo1 == "41:분개구분")
    {
        strSql = "SELECT A.분개그룹코드,A.종목중분류구분,(SELECT MAX(FA상세코드명) FROM INST1.TSKGHFA21 WHERE FA공통코드ID = '종목중분류구분코드' AND 상세코드ID = A.종목중분류구분) 종목중분류코드명,A.FA거래코드,A.시스템최종사용자번호,A.시스템최종처리일시 \n"
			     "FROM INST1.TSKGHFF09 A \n"
				 "WHERE 종목중분류구분 = '"+m_edit1.Trim()+"' \n"
				 "OR    종목중분류구분 = '"+m_edit2.Trim()+"' \n"
				 "OR    종목중분류구분 = '"+m_edit3.Trim()+"' \n"
				 "OR    종목중분류구분 = '"+m_edit4.Trim()+"' \n"
				 "ORDER BY 1 DESC \n" 
				 ;

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
	}
    else if (sCombo1 == "91:프로시져INST1")
    {
        strSql = "SELECT A.NAME \n"
			     "       ,REPLACE(REPLACE(REPLACE(A.TEXT, chr(13),''),chr(10),''),chr(09),'') TEXT \n"
				 "FROM ALL_SOURCE"+strDblinkName+" A WHERE A.NAME LIKE UPPER('"+m_edit1.Trim()+"') \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGH001;pwd=ybk34910"));
    }
    else if (sCombo1 == "92:프로시져KGHPKG")
    {
        strSql = "SELECT A.NAME \n"
			     "       ,REPLACE(REPLACE(REPLACE(A.TEXT, chr(13),''),chr(10),''),chr(09),'') TEXT \n"
				 "FROM ALL_SOURCE"+strDblinkName+" A WHERE A.NAME LIKE UPPER('"+m_edit1.Trim()+"') \n";
		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
	else if (sCombo1 == "93:프로시져 계층도")
    {
        strSql = "SELECT A.P_NM,A.P_NM_H,LEVEL,LPAD(' ',8*(LEVEL-1))||C_NM||'('||C_NM_H||')' HIERARCHY \n"
			     "FROM   KGH_DEPENDENCIES A \n"
				 "WHERE A.C_NM LIKE 'PC_KGH_CP_JL%'"
			     "START WITH A.P_NM = '"+m_edit1.Trim()+"' \n"
			     "CONNECT BY PRIOR A.C_NM = A.P_NM \n";

		db.Open(_T("ODBC;DSN=DSKGHT01;uid=KGHPKG;pwd=olA89656"));
    }
	else if (sCombo1 == "94:프로시져 역계층도")
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
	// 필드정보
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

    // VIEW 사이즈 조절(스크롤)
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
	// 소멸자
	//-------------------------------
    delete [] m_pFieldinfo;

}


CString CCodeViewView::uItoa(int num)
{
    char str[30];
   	itoa(num,str,10);
   	return str;
}



// CCodeViewView 메시지 처리기


void CCodeViewView::OnBnClickedButton1()
{
	UpdateData(true);
    TRACE("05:테이블정보");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    ProcessRecordset("Cats_Tmp_Approval");
}



void CCodeViewView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
//}


void CCodeViewView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	//CEdit* pEdit = (CEdit*)GetFocus();
	//if(pEdit) m_Grid.OnEditCopy();
	//else pEdit->Copy();

	//CEdit* pWndEdit = (CEdit*) m_wndTabs.GetActiveWnd ();
	//ASSERT_VALID (pWndEdit);
	//pWndEdit->Copy ();
    // CWnd *wnd;
    // wnd = GetDlgItem( IDC_EDIT2);
    // wnd->SetFocus();
	
    CEdit* pFocusEdit = (CEdit*)GetFocus();  // 현재 포커스를 얻어옴

    CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
    CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
    CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT3);
    CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT4);

    if (pFocusEdit == pEdit1)  // 현재 포커스가 첫번째 에디트 박스에 있는 경우
    {
        pEdit1->SetSel(0,-1); // 전체선택
        pEdit1->Copy();       // 클립보드로 복사
    }
    else if (pFocusEdit == pEdit2)  // 현재 포커스가 두번째 에디트 박스에 있는 경우
    {
	    pEdit2->SetSel(0,-1);
        pEdit2->Copy();
    }
    else if (pFocusEdit == pEdit3)  // 현재 포커스가 두번째 에디트 박스에 있는 경우
    {
	    pEdit3->SetSel(0,-1);
        pEdit3->Copy();
    }   
    else if (pFocusEdit == pEdit4)  // 현재 포커스가 두번째 에디트 박스에 있는 경우
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
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	m_Grid.OnEditCut();
//}

void CCodeViewView::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//m_ctrlEdit1.Paste();
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Paste();
}


void CCodeViewView::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCodeViewView::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnFileSaveAs();
}


void CCodeViewView::OnFileSaveAs()
{
	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
				//strTabInfo.Format("문자열%d", cnt);
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
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}


//void CCodeViewView::OnUpdateEditTabname(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//}



void CCodeViewView::OnEditTabname()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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