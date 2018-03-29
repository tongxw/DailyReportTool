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

#define EXCEL_VERSION 	2		//当前支持的EXCEL版本

#define TYPE_WORKSHEET 	0x10	//当写excel文档时，我们要写这个类型

// excel文档的不同部分都有不同的类型，所以写文档时会用到这些类型
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

//表格的对齐方式类型
#define	ALIGN_GENERAL  						0
#define	ALIGN_LEFT    						1
#define	ALIGN_CENTER  						2
#define	ALIGN_RIGHT   						3
#define	ALIGN_FILL    						4
#define	ALIGN_MULTIPLAN_DEFAULT  	7	
				
// 单元格边界类型
#define BORDER_LEFT    0x08
#define BORDER_RIGHT   0x10
#define BORDER_TOP     0x20
#define BORDER_BOTTOM  0x40


//把所有实现内容放到这个命名空间下面
namespace miniexcel
{
	/********************************************************
	 * 首先, 为我们的excel文件定义writer					*
	 ********************************************************/
	class LittleEndianWriter
	{
	private:
		FILE *m_pFile;						//文件操作指针，表明了在什么地方写入
	public:
		LittleEndianWriter(FILE *f);		//构造函数
		~LittleEndianWriter();				//析构函数 释放或者关闭文件操作句柄
	public:
		//info:写如char型（一个字节）数据
		void Write1(char v);

		//info:写入一个Int型（两个字节）数据
		void Write2(int v);

		//info:写入一个long型（四个字节）数据
		void Write4(long v);

		//info:写入一个float（四个字节）数据
		void WriteFloatIEEE(float v);

		//info:写入一个double（八个字节）数据
		void WriteDoubleIEEE(double v);			
	};
	
	
	/********************************************************
	 * 在EXCEL文件里面，所有东西都是一个二进制记录，在这里定*
	 * 义一个这样的记录虚类									*
	 ********************************************************/
	class BIFFRecord
	{
	protected:
		BIFFRecord () {}  				
		virtual ~BIFFRecord () {} 
		
		//写二进制头
		void Write (LittleEndianWriter *pWriter, int nRecNo, int nRecLen);
	public:
		//定义的一个纯虚函数，因为不同的数据类型的写操作不一样
		virtual void Write (LittleEndianWriter *pWriter) = 0; 	
	};	
	
	/********************************************************
	 * EXCEL文件都有同样的属性，类型无关紧要，我们把属性都	*
	 * 定义在这里											*
	 ********************************************************/
	class excelValueAttributes
	{
	private:
		int	m_nRow;			//表示行数
		int m_nColumn;		//表示列数		
		
		char m_nAttr1;		//单元格属性用三个字节包含
		char m_nAttr2;		//
		char m_nAttr3;		//
	public:					

		excelValueAttributes ();
		excelValueAttributes (int nRow,int nColumn);
		~excelValueAttributes (){}
		//info:属性设置
		void CopyAttributes(excelValueAttributes v)
		{
			m_nRow = v.m_nRow;
			m_nColumn = v.m_nColumn;
			m_nAttr1 = v.m_nAttr1;
			m_nAttr2 = v.m_nAttr2;
			m_nAttr3 = v.m_nAttr3;
		}

		//info:属性写入函数
		void Write (LittleEndianWriter *pWriter);
			
		//info:一下是获取和设置当前项目的行和列
		int  getRow();
		void setRow(int v); 
			
		int  getColumn();
		void setColumn(int v);

		//info:其他属性设置和获取隐藏，加锁，对齐方式，字体大小和格式等		
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
	 * EXCEL文件开始										*
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
	 * 定义浮点数类		（八个字节）						*
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
	 * 定义标签类	（256字节的字符串）						*
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
	 * EXCEL文件结尾										*
	 ********************************************************/
	class excelEOF : public BIFFRecord{
		public:
			excelEOF() {}
			~excelEOF() {}
			void Write (LittleEndianWriter *pWriter);		
	};
	/********************************************************
	 * 单元格类。EXCEL的单元格可以是数字或者是字符串，*
	 * 我们可以方便的操作它									*
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
		//info:目前没有包含进来
		int getType()
		{
			return m_nType;
		}
		
		//info:释放资源
		void clear();
		
		ExcelCell& operator=(const ExcelCell &v);

		ExcelCell& operator=(double v);
		operator double();

		ExcelCell& operator=(const char *v);
		operator const char *();
		
		void Write (LittleEndianWriter *pWriter);		
			
	};
	//设置列宽
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
	 * 这个类用来存贮一个EXCEL工作表并把这个工作表写入文件  *
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

		//info:文件写入函数，记得文件打开方式以 "wb"格式，并关闭文件操作句柄.
		void Write (FILE *dest);
		void SetColumnWidth(int colno, float width);
	};

};

#endif  /* #ifndef MINI_EXCEL_H_ALREADY_INCLUDED */
