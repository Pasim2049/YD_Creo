#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "tinyxml2.h"

namespace CADModelTool
{
	struct  CADPoint
	{
		CADPoint():x(0.),y(0.),z(0.){}
		CADPoint(double _x,double _y,double _z):x(_x),y(_y),z(_z){}
		double x;
		double y;
		double z;
	};

	typedef CADPoint CADVector;

	struct  CADMatrix
	{
		CADMatrix() {}
		CADMatrix(const CADVector& _x, const CADVector& _y, const CADVector& _z) :x(_x), y(_y), z(_z) {}
		CADVector x;
		CADVector y;
		CADVector z;
	};

	struct  CADLine
	{
		CADPoint startPnt;
		CADPoint endPnt;
	};

	struct  CADPlane
	{
		CADPlane() {}
		CADPlane(const CADPoint& _origin, const CADVector& _normal) :origin(_origin), normal(_normal) {}
		CADPoint origin;
		CADVector normal;
	};
	
	enum  BaseElemType
	{
		BaseUnknownType,
		BasePoint,
		BaseVector
	};

	enum  ElemType
	{
		UnknownType,
		Point,
		Axis,
		Line,
		Plane,
		CylindricalSrf
	};

	class  BaseAsmElem
	{
	public:
		BaseAsmElem(const BaseAsmElem&other)
		{
			stpFile = other.stpFile;
			type = other.type;
		}
		BaseAsmElem():stpFile(""),type(UnknownType){}
		BaseAsmElem(const std::string _stpFile, ElemType _type) :stpFile(_stpFile), type(_type) {}
		std::string stpFile;
		ElemType type;
	};
	//点
	class  PointElem:public BaseAsmElem
	{
	public :
		PointElem(const PointElem& other) :BaseAsmElem(other), point(other.point) {}
		CADPoint point;
		PointElem(const std::string& _stpFile,const CADPoint& _point):BaseAsmElem(_stpFile,Point),point(_point){}
	};
	//直线
	class  LineElem:public BaseAsmElem
	{
	public :
		LineElem(const LineElem& other) :BaseAsmElem(other), line(other.line) {}
		CADLine line;
		LineElem(const std::string& _stpFile,const CADLine& _line):BaseAsmElem(_stpFile,Line),line(_line){}
	};
	//轴
	class  AxisElem:public BaseAsmElem
	{
	public :
		AxisElem(const AxisElem& other) :BaseAsmElem(other), line(other.line) {}
		CADLine line;
		AxisElem(const std::string& _stpFile,const CADLine& _line):BaseAsmElem(_stpFile,Axis),line(_line){}
	};
	//平面
	class  PlaneElem:public BaseAsmElem
	{
	public :
		PlaneElem() {}
		PlaneElem(const PlaneElem& other):BaseAsmElem(other), plane(other.plane){}
		CADPlane plane;
		PlaneElem(const std::string& _stpFile,const CADPlane& _plane):BaseAsmElem(_stpFile,Plane),plane(_plane){}
	};

	enum  CADConsType
	{
		UNKNOWN,
		FIX,
		MATE,
		MATE_OFF,
		ALIGN,
		ALIGN_OFF,
		TANGENT
	};

	class  CADConstraint
	{
	public :
		CADConstraint(const CADConstraint& other):type(other.type), value(other.value)
		{
			pElem1 = DeepCopyElem(other.pElem1);
			pElem2 = DeepCopyElem(other.pElem2);
		}
		CADConstraint operator =(const CADConstraint& other)
		{
			CADConstraint tmp = other;
			swap(tmp);
			return *this;
		}

		CADConstraint(CADConsType _type=UNKNOWN,BaseAsmElem* _elem1=nullptr,BaseAsmElem* _elem2=nullptr,double _value =0.):
		   type(_type),pElem1(_elem1),pElem2(_elem2),value(_value){}
		
		double value;
		CADConsType type;
		BaseAsmElem* pElem1;
		BaseAsmElem* pElem2;	

		~CADConstraint();
	private:
		BaseAsmElem* DeepCopyElem(BaseAsmElem* other)
		{
			BaseAsmElem* pElem = nullptr;
			switch (other->type)
			{
			case ElemType::Point:
			{
				PointElem* tmp = (PointElem*)other;
				pElem = new PointElem(*tmp);
			}
			break;
			case ElemType::Axis:
			{
				AxisElem* tmp = (AxisElem*)other;
				pElem = new AxisElem(*tmp);
			}
			case ElemType::Line:
			{
				LineElem* tmp = (LineElem*)other;
				pElem = new LineElem(*tmp);
			}
			break;
			case ElemType::Plane:
			{
				PlaneElem* tmp = (PlaneElem*)other;
				pElem = new PlaneElem(*tmp);
			}	
				break;
			default:
				break;
			}
			return pElem;
		}

		void swap(CADConstraint &other)
		{
			std::swap(pElem1, other.pElem1);
			std::swap(pElem2, other.pElem2);
			std::swap(value, other.value);
			std::swap(type, other.type);
		}
	};

	class  CADPartInfo
	{
	public:
		CADPartInfo() {}
		CADPartInfo(const std::string& _fileName,const CADMatrix& _matrix):stepFileName(_fileName),position(_matrix){}
		std::string stepFileName;
		CADMatrix position;
	};

	class  CADComponent
	{
	public:
		//CADComponent(const CADComponent& other) = delete;
		//CADComponent operator =(const CADComponent& other) = delete;
		CADComponent(std::string _name, bool _isAssembly) :name(_name), isAssembly(_isAssembly){}
		CADComponent(std::string _name, const std::vector<CADConstraint>& _vecCons) :name(_name), isAssembly(true), vecCons(_vecCons) {}
		CADComponent(std::string _name,const CADPartInfo& _part) :name(_name), isAssembly(false), part(_part){}
		~CADComponent();

		bool isAssembly;
		std::vector<CADConstraint> vecCons;//empty when isAssembly is false
		CADPartInfo part;//valid when isAssembly is false
		std::string name;//current doc file name

		std::vector<CADComponent*> vecSubNodes;//empty when isAssembly is true
	};

	int WriteModelToXml(const CADComponent* pRootNode,const std::string &xmlFile);
	int ReadModeFromXml(const std::string &xmlFile,CADComponent** ppRootNode);
}
