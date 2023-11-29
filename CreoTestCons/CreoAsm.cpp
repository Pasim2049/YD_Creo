#include "CreoAsm.h"
#include <direct.h>
using namespace CADModelTool;
extern std::vector<CADConstraint> vecCadCons;
const int maxLength = 32;
#define LENGTH(array) (sizeof(array)/sizeof(*array))
void COPYPOINT3(double* pntArray, CADPoint& cadPnt)
{
	//if(!pntArray || LENGTH(pntArray)!=3)
	//	return ;
	cadPnt.x = pntArray[0];
	cadPnt.y = pntArray[1];
	cadPnt.z = pntArray[2];
}

static std::string WChar2Ansi(wchar_t* pwszSrc)
{
	char desBuf[maxLength] = {0};
	sprintf_s(desBuf, maxLength, "%ws", pwszSrc);
	return std::string(desBuf);
}

static std::string GetPrtName(std::string& str)
{
	return str + ".prt";
}
static std::string GetAsmName(std::string& str)
{
	return str + ".asm";
}

static std::string GetStepName(std::string& str)
{
	return str + ".step";
}


//static std::string WChar2Ansi(LPCWSTR pwszSrc)
//{
//	CString cs(pwszSrc);
//#ifdef _UNICODE
//	//如果是unicode工程
//	USES_CONVERSION;
//	std::string str(W2A(cs));
//	return str;
//#else
//	//如果是多字节工程 
//	std::string str(cs.GetBuffer());
//	cs.ReleaseBuffer();
//	return str;
//#endif
//
//	//int iLen = csStrData.GetLength() + 1;
//	//	char* pSrc = csStrData.GetBuffer(iLen * sizeof(char));
//	//	std::string strRet = std::string(pSrc);
//	//	csStrData.ReleaseBuffer(iLen);
//	//	return strRet;
//
//	//std::string tmp=CT2A(src);
//	//return tmp;
//	//USES_CONVERSION;
//	//char* p = T2A(src.GetBuffer(0));
//	//std::string dst(p);
//	//src.ReleaseBuffer();
//	//return dst;
//	//return CStringA(tmp);
//	//CString s = _T("abc");
//	//std::string str = CStringA(s);
//
//	//return CT2A(tmp.GetString());
//	//return tmp.GetBuffer();
//	//int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
//	//if (nLen<= 0) return std::string("");
//	//char* pszDst = new char[nLen];
//	//if (NULL == pszDst) return std::string("");
//	//WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
//	//pszDst[nLen -1] = 0;
//	//std::string strTemp(pszDst);
//	//delete [] pszDst;
//	//return strTemp;
//}

static int Loc_CreoGetPlaneData(const ProSurface& plane, CADPlane& cadPlane)
{
	ProGeomitemdata* geomdata;
	ProError err = ProSurfaceDataGet(plane, &geomdata);
	if (err != PRO_TK_NO_ERROR || geomdata == NULL)
		return 1;
	COPYPOINT3(geomdata->data.p_surface_data->srf_shape.plane.e3, cadPlane.normal);
	COPYPOINT3(geomdata->data.p_surface_data->srf_shape.plane.origin, cadPlane.origin);
	return 0;
}

static int Loc_CreoGetAxisData(const ProAxis& axis, CADLine& line)
{
	ProGeomitemdata* geomdata;
	ProError err = ProAxisDataGet(axis, &geomdata);
	if (err != PRO_TK_NO_ERROR || geomdata == NULL)
		return 1;
	//err = ProLinedataGet( a_gitem_data->data.p_curve_data,&end1, &end2);
	COPYPOINT3(geomdata->data.p_curve_data->line.end1, line.startPnt);
	COPYPOINT3(geomdata->data.p_curve_data->line.end2, line.endPnt);
	return 0;
}

static int Loc_CreoGetEdgeData(const ProEdge& edge, CADLine& line)
{
	ProGeomitemdata* geomdata;
	ProError err = ProEdgeDataGet(edge, &geomdata);
	if (err != PRO_TK_NO_ERROR || geomdata == NULL)
		return 1;
	//err = ProLinedataGet( a_gitem_data->data.p_curve_data,&end1, &end2);
	COPYPOINT3(geomdata->data.p_curve_data->line.end1, line.startPnt);
	COPYPOINT3(geomdata->data.p_curve_data->line.end2, line.endPnt);
	return 0;
}

static int Loc_CreoGetPointData(const ProPoint& pnt, CADPoint& cadPnt)
{
	ProVector xyz;
	ProError err = ProPointCoordGet(pnt, xyz);
	if (err != PRO_TK_NO_ERROR || xyz == NULL)
		return 1;
	//err = ProLinedataGet( a_gitem_data->data.p_curve_data,&end1, &end2);
	COPYPOINT3(xyz, cadPnt);
	return 0;
}

static int Loc_CreoGetConstraintElem(ProModelitem cfeat, BaseAsmElem** ppElem)
{
	ProName ownerName;
	ProError status = ProMdlNameGet(cfeat.owner, ownerName);
	if (status != PRO_TK_NO_ERROR)
		return 1;
	std::string ptrName = WChar2Ansi(ownerName);
	//std::string ptrName = "WChar2Ansi";
	switch (cfeat.type)
	{
	case PRO_AXIS:
	{
		ProAxis axis;
		status = ProAxisInit((ProSolid)cfeat.owner, cfeat.id, &axis); //!<获取轴信息
		if (status != PRO_TK_NO_ERROR)
			return 1;
		CADLine line;
		if (Loc_CreoGetAxisData(axis, line))
			return 1;
		AxisElem* pAxis = new AxisElem(ptrName, line);
		*ppElem = pAxis;
	}
	break;
	case PRO_SURFACE:
	{
		ProSurface surf;
		status = ProSurfaceInit((ProSolid)cfeat.owner, cfeat.id, &surf); //!<获取轴信息
		if (status != PRO_TK_NO_ERROR)
			return 1;
		ProSrftype surface_type;
		status = ProSurfaceTypeGet(surf, &surface_type);
		if (status != PRO_TK_NO_ERROR)
			return 1;
		if (surface_type == PRO_SRF_PLANE)//平面
		{
			CADPlane cadPlane;
			if (Loc_CreoGetPlaneData(surf, cadPlane))
				return 1;
			PlaneElem* pPlane = new PlaneElem(ptrName, cadPlane);
			*ppElem = pPlane;
		}
		else
		{
			//处理柱面等 PRO_SRF_CYL
		}

		//status = ProSurfaceInit((ProSolid)cfeat.owner,cfeat.id, &p_surface);//!<获取平面
		//status = ProSurfaceToGeomitem((ProSolid)cfeat.owner, p_surface, &pgoitem);//!<获得几何信息
		//str +="SURFACE";
		//strname = NORMALSURFACE;
		//strlast = STANDSURFACE;
	}
	break;
	case PRO_EDGE:
	{
		ProEdge edge;
		status = ProEdgeInit((ProSolid)cfeat.owner, cfeat.id, &edge); //!<获取轴信息
		if (status != PRO_TK_NO_ERROR)
			return 1;
		CADLine line;
		if (Loc_CreoGetEdgeData(edge, line))
			return 1;
		LineElem* pEdge = new LineElem(ptrName, line);
		*ppElem = pEdge;
	}
	case PRO_POINT:
	{
		ProPoint pnt;
		status = ProPointInit((ProSolid)cfeat.owner, cfeat.id, &pnt); //!<获取轴信息
		if (status != PRO_TK_NO_ERROR)
			return 1;
		CADPoint cadPnt;
		if (Loc_CreoGetPointData(pnt, cadPnt))
			return 1;
		PointElem* pPnt = new PointElem(ptrName, cadPnt);
		*ppElem = pPnt;
	}
	case PRO_CSYS:
	case PRO_EDGE_START:
	case PRO_EDGE_END:
		//status = ProEdgeInit((ProSolid)cfeat.owner,cfeat.id, &p_edge);//!<获取边
		//status = ProEdgeToGeomitem((ProSolid)cfeat.owner, p_edge, &pgoitem);//!<获得几何信息
		////strname = NORMALPOINT;
		break;
	default:
		return 1;
		break;
	}
	return 0;
}

static void Loc_CreoConsTypeToCadConsType(ProAsmcompConstrType type, CADConsType& cadConsType)
{
	switch (type)
	{
	case PRO_ASM_MATE:
		cadConsType = CADConsType::MATE;
		break;
	case PRO_ASM_MATE_OFF:
		cadConsType = CADConsType::MATE_OFF;
		break;
	case PRO_ASM_ALIGN:
		cadConsType = CADConsType::ALIGN;
		break;
	case PRO_ASM_ALIGN_OFF:
		cadConsType = CADConsType::ALIGN_OFF;
		break;
	case PRO_ASM_TANGENT:
		cadConsType = CADConsType::TANGENT;
		break;
	case PRO_ASM_FIX:
		cadConsType = CADConsType::FIX;
		break;
	case PRO_ASM_INSERT:
	case PRO_ASM_ORIENT:
	case PRO_ASM_CSYS:
	case PRO_ASM_PNT_ON_SRF:
	case PRO_ASM_EDGE_ON_SRF:
	case PRO_ASM_DEF_PLACEMENT:
	case PRO_ASM_SUBSTITUTE:
	case PRO_ASM_PNT_ON_LINE:
	case PRO_ASM_AUTO:
	default:
		cadConsType = CADConsType::UNKNOWN;
		break;
	}
}

static int Loc_CreoCreateConstraint(const ProAsmcompconstraint& constrant, CADConstraint& CADcons)
{
	ProDatumside dtmside;
	ProSelection comp_constr, asm_constr;
	ProError status = ProAsmcompconstraintAsmreferenceGet(constrant, &asm_constr, &dtmside);//获得装配元件约束的参照
	if (status != PRO_TK_NO_ERROR)
		return 1;
	status = ProAsmcompconstraintCompreferenceGet(constrant, &comp_constr, &dtmside);//获得装配元件约束的参照
	if (status != PRO_TK_NO_ERROR)
		return 1;

	//获得约束设置的特征
	ProModelitem asmfeat, cfeat;
	status = ProSelectionModelitemGet(asm_constr, &asmfeat);
	status = ProSelectionModelitemGet(comp_constr, &cfeat);

	BaseAsmElem* pElem1 = nullptr;
	if (Loc_CreoGetConstraintElem(asmfeat, &pElem1))
		return 1;
	BaseAsmElem* pElem2 = nullptr;
	if (Loc_CreoGetConstraintElem(cfeat, &pElem2))
		return 1;

	ProAsmcompConstrType type;
	status = ProAsmcompconstraintTypeGet(constrant, &type);//获得装配元件约束的类型
	if (status != PRO_TK_NO_ERROR)
		return 1;
	CADConsType cadConsType;
	Loc_CreoConsTypeToCadConsType(type, cadConsType);

	double offset;
	status = ProAsmcompconstraintOffsetGet(constrant, &offset);//获得装配元件约束的偏置值
	if (status != PRO_TK_NO_ERROR)
		return 1;

	CADcons.type = cadConsType;
	CADcons.pElem1 = pElem1;
	CADcons.pElem2 = pElem2;
	CADcons.value = offset;
	return 0;
}

int CreoGetAllAsmConstraints();
int CreoGetAsmcompConstraints(ProAsmcomp* asmcomp, std::vector<CADConstraint>& vecCADcons);

ProError AsmVisitFilterFun(ProFeature* feature, ProAppData appdate)
{
	ProError status;
	ProFeattype ftype;
	status = ProFeatureTypeGet(feature, &ftype);
	if (ftype == PRO_FEAT_COMPONENT)
		return (PRO_TK_NO_ERROR);
	return (PRO_TK_CONTINUE);
}

ProError AsmVisitFun(ProFeature* feature, ProError status, ProAppData appdate)
{
	ProMdl mdl;	
	status = ProAsmcompMdlGet(feature, &mdl);  //为组件引用的零件或装配体提供ProMdl句柄
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_CONTINUE;

	AsmCompData* appd = (AsmCompData*)appdate;
	CADComponent* pCadNode = appd->pCadNode;
	int level = appd->level;
	ProAsmcomppath comppath = appd->comppath;

	ProAsmcomp asmcomp11;
	status = ProMdlToModelitem(mdl, &asmcomp11);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_CONTINUE;

	ProMdldata mdldata;
	status = ProMdlDataGet(mdl, &mdldata);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_CONTINUE;

	char type[PRO_TYPE_SIZE];
	ProWstringToString(type, mdldata.type);
	
	if (strncmp(type, "PRT", 3) == 0)//如果组件类型是零件
	{
		AsmCompData appd1;
		appd1.level = level;
		appd1.mdl = mdl;
		appd1.comppath = comppath;

		//先执行赋值：comppath.comp_id_table[0]=feature.id;再自增：table_num+1;
		appd1.comppath.comp_id_table[appd1.comppath.table_num++] = feature->id;
		appd1.comppath.comp_id_table[appd1.comppath.table_num] = -1;

		ProAsmcomppath path = appd1.comppath;
		asmcomp11.type = PRO_FEATURE;
		asmcomp11.id = path.comp_id_table[path.table_num - 1];///根据路径ID表获得所选择的零件或子装配体的ID			      
		path.table_num--;
		status = ProAsmcomppathMdlGet(&path, &asmcomp11.owner);  ///根据零件或子装配体的路径获得模型  

		std::vector<CADConstraint> vecCons;
		if (CreoGetAsmcompConstraints(&asmcomp11, vecCons))
			return PRO_TK_CONTINUE;

		/*获取位置矩阵和名称*/
		ProName prtName;
		ProError status = ProMdlNameGet(mdl, prtName);
		if (status != PRO_TK_NO_ERROR)
			return PRO_TK_CONTINUE;
		std::string strName = WChar2Ansi(prtName);
		CADMatrix matrix;
		CADPartInfo partInfo(GetStepName(strName), matrix);
		CADComponent* pPartNode = new CADComponent(GetPrtName(strName), partInfo);
		if (pCadNode)
		{
			pCadNode->vecSubNodes.push_back(pPartNode);
			pCadNode->vecCons.insert(pCadNode->vecCons.cend(), vecCons.cbegin(), vecCons.cend());
		}
	}
	else if (strncmp(type, "ASM", 3) == 0)//如果组件是装配体，则递归调用ProSolidFeatVisit来遍历每个组件
	{
		AsmCompData appd1;
		appd1.level = appd->level + 1;

		appd1.comppath.comp_id_table[appd1.comppath.table_num++] = feature->id;
		appd1.comppath.comp_id_table[appd1.comppath.table_num] = -1;


		ProAsmcomppath path = appd1.comppath;
		asmcomp11.type = PRO_FEATURE;
		asmcomp11.id = path.comp_id_table[path.table_num - 1];///根据路径ID表获得所选择的零件或子装配体的ID			      
		path.table_num--;
		status = ProAsmcomppathMdlGet(&path, &asmcomp11.owner);  ///根据零件或子装配体的路径获得模型 
		std::vector<CADConstraint> vecCons;
		if (CreoGetAsmcompConstraints(&asmcomp11, vecCons))
			return PRO_TK_CONTINUE;


		/*获取位置矩阵和名称*/
		ProName prtName;
		ProError status = ProMdlNameGet(mdl, prtName);
		if (status != PRO_TK_NO_ERROR)
			return PRO_TK_CONTINUE;
		std::string strName = WChar2Ansi(prtName);
		if (pCadNode)
		{
			CADComponent* pAsmNode = new CADComponent(GetAsmName(strName), vecCons);
			pCadNode->vecSubNodes.push_back(pAsmNode);
			pCadNode->vecCons.insert(vecCons.cend(), vecCons.cbegin(), vecCons.cend());
			appd1.pCadNode = pAsmNode;
		}
		status = ProSolidFeatVisit((ProSolid)mdl, AsmVisitFun, AsmVisitFilterFun, &appd1);
	}
	if (feature != nullptr)
		return (PRO_TK_NO_ERROR);
	return (PRO_TK_CONTINUE);
}

//ProError AsmVisitFun2(ProFeature* feature, ProError status, ProAppData appdate)
//{
//	ProMdl mdl;
//	status = ProAsmcompMdlGet(feature, &mdl);  //为组件引用的零件或装配体提供ProMdl句柄
//
//	//AsmCompData* appd = (AsmCompData*)appdate;
//	//int level = appd->level;
//	//ProAsmcomppath comppath = appd->comppath;
//
//	ProAsmcomp asmcomp11;
//	status = ProMdlToModelitem(mdl, &asmcomp11);
//
//	ProMdldata mdldata;
//	status = ProMdlDataGet(mdl, &mdldata);
//	char type[PRO_TYPE_SIZE];
//	ProWstringToString(type, mdldata.type);
//
//	if (strncmp(type, "PRT", 3) == 0)//如果组件类型是零件
//	{
//		//AsmCompData appd1;
//		//appd1.level = level;
//		//appd1.mdl = mdl;
//		//appd1.comppath = comppath;
//		//先执行赋值：comppath.comp_id_table[0]=feature.id;再自增：table_num+1;
//		//appd1.comppath.comp_id_table[appd1.comppath.table_num++] = feature->id;
//		//appd1.comppath.comp_id_table[appd1.comppath.table_num] = -1;
//		//ProAsmcomppath path = appd1.comppath;
//		//asmcomp11.type = PRO_FEATURE;
//		//asmcomp11.id = path.comp_id_table[path.table_num - 1];///根据路径ID表获得所选择的零件或子装配体的ID			      
//		//path.table_num--;
//		//status = ProAsmcomppathMdlGet(&path, &asmcomp11.owner);  ///根据零件或子装配体的路径获得模型  
//
//		std::vector<CADConstraint> vecCons;
//		if (CreoGetAsmcompConstraints(&asmcomp11, vecCons))
//			return PRO_TK_CONTINUE;
//	}
//	else if (strncmp(type, "ASM", 3) == 0)//如果组件是装配体，则递归调用ProSolidFeatVisit来遍历每个组件
//	{
//		//AsmCompData appd1;
//		//appd1.level = appd->level + 1;
//		//appd1.comppath.comp_id_table[appd1.comppath.table_num++] = feature->id;
//		//appd1.comppath.comp_id_table[appd1.comppath.table_num] = -1;
//		//ProAsmcomppath path = appd1.comppath;
//		//asmcomp11.type = PRO_FEATURE;
//		//asmcomp11.id = path.comp_id_table[path.table_num - 1];///根据路径ID表获得所选择的零件或子装配体的ID			      
//		//path.table_num--;
//		//status = ProAsmcomppathMdlGet(&path, &asmcomp11.owner);  ///根据零件或子装配体的路径获得模型 
//
//		std::vector<CADConstraint> vecCons;
//		if (CreoGetAsmcompConstraints(&asmcomp11, vecCons))
//			return PRO_TK_CONTINUE;
//		status = ProSolidFeatVisit((ProSolid)mdl, AsmVisitFun, AsmVisitFilterFun, NULL);
//	}
//	if (feature != nullptr)
//		return (PRO_TK_NO_ERROR);
//	return (PRO_TK_CONTINUE);
//}

int CreoGetAllAsmConstraints()
{
	ProMdl p_asm;
	ProError status = ProMdlCurrentGet(&p_asm);
	if (status != PRO_TK_NO_ERROR)
		return 1;

	/*root cad node*/
	ProName prtName;
	status = ProMdlNameGet(p_asm, prtName);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_CONTINUE;
	std::string strName = WChar2Ansi(prtName);
	CADComponent* pRootCadNode = new CADComponent(GetAsmName(strName),true);

	/*root path*/
    ProAsmcomppath rootPath;
    ProIdTable rootTable; 
	rootTable[0] = -1;
	int rootTblSize = 0;
    status = ProAsmcomppathInit((ProSolid)p_asm, rootTable, rootTblSize, &rootPath);
	if (status != PRO_TK_NO_ERROR)
		return 1;

	/*root data*/
    AsmCompData rootAppDate;
	rootAppDate.level = 1;
	rootAppDate.mdl = p_asm;
	rootAppDate.comppath = rootPath;
	rootAppDate.pCadNode = pRootCadNode;

	/*visit root*/
    status = ProSolidFeatVisit((ProSolid)p_asm, AsmVisitFun, AsmVisitFilterFun, &rootAppDate);
	if (status != PRO_TK_NO_ERROR)
		return 1;

	char tmp[256];
	auto err = _getcwd(tmp, 256);
	std::string xmlPath = std::string(tmp) + "\\xxx.xml";
	if (WriteModelToXml(pRootCadNode, xmlPath))
		return 1;
    return 0;
}

int CreoGetAsmcompConstraints(ProAsmcomp* asmcomp, std::vector<CADConstraint>& vecCADcons)
{
    ProError status, aref_status, cref_status;
    ProSelection comp_constr, asm_constr;
    ProAsmcompconstraint* constr_array;
    status = ProAsmcompConstraintsGet(asmcomp, &constr_array);//获得装配元件的约束
    if (status != PRO_TK_NO_ERROR)
        return 1;

    int size = 0;
    ProArraySizeGet(constr_array, &size);
    for (int i = 0; i < size; i++)//遍历装配元件的约束
    {
        CADConstraint CADcons;
        if (Loc_CreoCreateConstraint(constr_array[i], CADcons))
            return 1;
		vecCADcons.push_back(CADcons);
    }
    ProArrayFree((ProArray*)&constr_array);
    return 0;
}

