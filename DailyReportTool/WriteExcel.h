/////////////////////////////////////////////////////////////////////////////////
/** @file     mexcel.h
 *  @brief    codeproject
 *  @author   codeproject
 *  @version  
 *  @date     2008-07-06
 *  @since    
 *  @remarks  modify by wangyaonan
 *
 */
////////////////////////////////////////////////////////////////////////////////////
#ifndef MINI_EXCEL_H_ALREADY_INCLUDED
#define MINI_EXCEL_H_ALREADY_INCLUDED

#include <stdio.h>
#include <vector>
#include <list>
using namespace std;

#define EXCEL_VERSION 	2		//��ǰ֧�ֵ�EXCEL�汾

#define TYPE_WORKSHEET 	0x10	//��дexcel�ĵ�ʱ������Ҫд�������

// excel�ĵ��Ĳ�ͬ���ֶ��в�ͬ�����ͣ�����д�ĵ�ʱ���õ���Щ����
#define OPCODE_BOF                           0x09
#define OPCODE_FILEPASS                      0x2F
#define OPCODE_INDEX                         0x0B
#define OPCODE_CALCCOUNT                     0x0C
#define OPCODE_CALCMODE                      0x0D
#define OPCODE_PRECISION                     0x0E
#define OPCODE_REFMODE                       0x0F
#define OPCODE_DELTA                         0x10
#define OPCODE_ITERATION                     0x11
#define OPCODE_1904                          0x22
#define OPCODE_BACKUP                        0x40
#define OPCODE_PRINT_ROW_HEADERS             0x2A
#define OPCODE_PRINT_GRIDLINES               0x2B
#define OPCODE_HORIZONTAL_PAGE_BREAKS        0x1B
#define OPCODE_VERTICAL_PAGE_BREAKS          0x1A
#define OPCODE_DEFAULT_ROW_HEIGHT            0x25
#define OPCODE_FONT                          0x31
#define OPCODE_FONT2                         0x32
#define OPCODE_HEADER                        0x14
#define OPCODE_FOOTER                        0x15
#define OPCODE_LEFT_MARGIN                   0x26
#define OPCODE_RIGHT_MARGIN                  0x27
#define OPCODE_TOP_MARGIN                    0x28
#define OPCODE_BOTTOM_MARGIN                 0x29
#define OPCODE_COLWIDTH                      0x24
#define OPCODE_EXTERNCOUNT                   0x16
#define OPCODE_EXTERNSHEET                   0x17
#define OPCODE_EXTERNNAME                    0x23
#define OPCODE_FORMATCOUNT                   0x1F
#define OPCODE_FORMAT                        0x1E
#define OPCODE_NAME                          0x18
#define OPCODE_DIMENSIONS                    0x00
#define OPCODE_COLUMN_DEFAULT                0x20
#define OPCODE_ROW                           0x08
#define OPCODE_BLANK                         0x01
#define OPCODE_INTEGER                       0x02
#define OPCODE_NUMBER                        0x03
#define OPCODE_LABEL                         0x04
#define OPCODE_BOOLERR                       0x05
#define OPCODE_FORMULA                       0x06
#define OPCODE_ARRAY                         0x21
#define OPCODE_CONTINUE                      0x3C
#define OPCODE_STRING                        0x07
#define OPCODE_TABLE                         0x36
#define OPCODE_TABLE2                        0x37
#define OPCODE_PROTECT                       0x12
#define OPCODE_WINDOW_PROTECT                0x19
#define OPCODE_PASSWORD                      0x13
#define OPCODE_NOTE                          0x1C
#define OPCODE_WINDOW1                       0x3D
#define OPCODE_WINDOW2                       0x3E
#define OPCODE_PANE                          0x41
#define OPCODE_SELECTION                     0x1D
#define OPCODE_EOF                           0x0A

//���Ķ��뷽ʽ����
#define	ALIGN_GENERAL  						0
#define	ALIGN_LEFT    						1
#define	ALIGN_CENTER  						2
#define	ALIGN_RIGHT   						3
#define	ALIGN_FILL    						4
#define	ALIGN_MULTIPLAN_DEFAULT  	7	
				
// ��Ԫ��߽�����
#define BORDER_LEFT    0x08
#define BORDER_RIGHT   0x10
#define BORDER_TOP     0x20
#define BORDER_BOTTOM  0x40


//������ʵ�����ݷŵ���������ռ�����
namespace miniexcel
{
	/********************************************************
	 * ����, Ϊ���ǵ�excel�ļ�����writer					*
	 ********************************************************/
	class LittleEndianWriter
	{
	private:
		FILE *m_pFile;						//�ļ�����ָ�룬��������ʲô�ط�д��
	public:
		LittleEndianWriter(FILE *f);		//���캯��
		~LittleEndianWriter();				//�������� �ͷŻ��߹ر��ļ��������
	public:
		//info:д��char�ͣ�һ���ֽڣ�����
		void Write1(char v);

		//info:д��һ��Int�ͣ������ֽڣ�����
		void Write2(int v);

		//info:д��һ��long�ͣ��ĸ��ֽڣ�����
		void Write4(long v);

		//info:д��һ��float���ĸ��ֽڣ�����
		void WriteFloatIEEE(float v);

		//info:д��һ��double���˸��ֽڣ�����
		void WriteDoubleIEEE(double v);			
	};
	
	
	/********************************************************
	 * ��EXCEL�ļ����棬���ж�������һ�������Ƽ�¼�������ﶨ*
	 * ��һ�������ļ�¼����									*
	 ********************************************************/
	class BIFFRecord
	{
	protected:
		BIFFRecord () {}  				
		virtual ~BIFFRecord () {} 
		
		//д������ͷ
		void Write (LittleEndianWriter *pWriter, int nRecNo, int nRecLen);
	public:
		//�����һ�����麯������Ϊ��ͬ���������͵�д������һ��
		virtual void Write (LittleEndianWriter *pWriter) = 0; 	
	};	
	
	/********************************************************
	 * EXCEL�ļ�����ͬ�������ԣ������޹ؽ�Ҫ�����ǰ����Զ�	*
	 * ����������											*
	 ********************************************************/
	class excelValueAttributes
	{
	private:
		int	m_nRow;			//��ʾ����
		int m_nColumn;		//��ʾ����		
		
		char m_nAttr1;		//��Ԫ�������������ֽڰ���
		char m_nAttr2;		//
		char m_nAttr3;		//
	public:					

		excelValueAttributes ();
		excelValueAttributes (int nRow,int nColumn);
		~excelValueAttributes (){}
		//info:��������
		void CopyAttributes(excelValueAttributes v)
		{
			m_nRow = v.m_nRow;
			m_nColumn = v.m_nColumn;
			m_nAttr1 = v.m_nAttr1;
			m_nAttr2 = v.m_nAttr2;
			m_nAttr3 = v.m_nAttr3;
		}

		//info:����д�뺯��
		void Write (LittleEndianWriter *pWriter);
			
		//info:һ���ǻ�ȡ�����õ�ǰ��Ŀ���к���
		int  getRow();
		void setRow(int v); 
			
		int  getColumn();
		void setColumn(int v);

		//info:�����������úͻ�ȡ���أ����������뷽ʽ�������С�͸�ʽ��		
		void setHidden (bool v);
		bool getHidden ();
			
		void setLocked (bool v);
		bool getLocked ();
			
		void setShaded (bool v);
		bool getShaded ();
			
		void setBorder (int type);
		int  getBorder ();
			
		void setAlignament (int type);
		int  getAlignament ();
														
		void setFontNum (int v);
		int  getFontNum ();

		void setFormatNum (int v);
		int  getFormatNum ();
				
	};
	
	/********************************************************
	 * EXCEL�ļ���ʼ										*
	 ********************************************************/
	class excelBOF : public BIFFRecord{
		private:
			int m_nVersion; /* Version. in our case EXCEL_VERSION    */
			int	m_nType;    /* The type, in our case TYPE_WORKSHEET  */
		public:
			excelBOF (int nVer,int nType):m_nVersion(nVer),m_nType(nType){}
			~excelBOF(){}
			void Write (LittleEndianWriter *pWriter);
		
	};
	
	/********************************************************
	 * ���帡������		���˸��ֽڣ�						*
	 ********************************************************/
	class excelNUMBER : public BIFFRecord,public excelValueAttributes
	{
	private:
		double m_nValue;
	public:

		excelNUMBER (){	m_nValue = 0;}

		excelNUMBER (double val){m_nValue = val;}

		void setValue (double v){m_nValue = v;}

		double getValue (){	return m_nValue;}

		void Write (LittleEndianWriter *pWriter);
	};	
	
	/* A label: up to 256 character string
	 */
	/********************************************************
	 * �����ǩ��	��256�ֽڵ��ַ�����						*
	 ********************************************************/
	class excelLABEL : public BIFFRecord,public excelValueAttributes{
		private:
			char		m_pchValue[257];
		public:			 
		
			excelLABEL (){
				m_pchValue[0] = '\0';
			}
			
			excelLABEL (const char *v){
				strncpy (m_pchValue, v, sizeof (m_pchValue));
				m_pchValue[sizeof(m_pchValue) - 1] = '\0';
			}
		
			void setValue (const char *v){
				strncpy (m_pchValue, v, sizeof (m_pchValue));
				m_pchValue[sizeof(m_pchValue) - 1] = '\0';
			}
			
			const char *getValue (){
				return m_pchValue;
			}
		
			void Write (LittleEndianWriter *pWriter);
	};
	
	/********************************************************
	 * EXCEL�ļ���β										*
	 ********************************************************/
	class excelEOF : public BIFFRecord{
		public:
			excelEOF() {}
			~excelEOF() {}
			void Write (LittleEndianWriter *pWriter);		
	};
	/********************************************************
	 * ��Ԫ���ࡣEXCEL�ĵ�Ԫ����������ֻ������ַ�����*
	 * ���ǿ��Է���Ĳ�����									*
	 ********************************************************/
	class ExcelCell:public excelValueAttributes
	{
	private:
		char 	*m_pchValue;
		double	m_nValue;
		int		m_nType;
	public:
		ExcelCell ();
		ExcelCell (const ExcelCell &v);
		~ExcelCell ();

		enum
		{
			TYPE_NONE	= 0,
			TYPE_NUMBER = 1,
			TYPE_STRING = 2
		};
		//info:Ŀǰû�а�������
		int getType()
		{
			return m_nType;
		}
		
		//info:�ͷ���Դ
		void clear();
		
		ExcelCell& operator=(const ExcelCell &v);

		ExcelCell& operator=(double v);
		operator double();

		ExcelCell& operator=(const char *v);
		operator const char *();
		
		void Write (LittleEndianWriter *pWriter);		
			
	};
	//�����п�
	class ExcelColumnWidth:public BIFFRecord
	{
	public:
		ExcelColumnWidth (int colno, int width);  				
		~ExcelColumnWidth ();
		
	private:
		int nColStart;
		int nWidth;
	public:
		void Write (LittleEndianWriter *pWriter);
	
	};

	/********************************************************
	 * �������������һ��EXCEL�����������������д���ļ�  *
	 ********************************************************/
	class CMiniExcel
	{
	private:
		vector<vector<ExcelCell> > m_vvTableValues;
		list<ExcelColumnWidth> lstColumnWidths;
	public:
		CMiniExcel ();
		~CMiniExcel ();

		ExcelCell &operator() (unsigned row,unsigned column);

		//info:�ļ�д�뺯�����ǵ��ļ��򿪷�ʽ�� "wb"��ʽ�����ر��ļ��������.
		void Write (FILE *dest);
		void SetColumnWidth(int colno, float width);
	};

};

#endif  /* #ifndef MINI_EXCEL_H_ALREADY_INCLUDED */
