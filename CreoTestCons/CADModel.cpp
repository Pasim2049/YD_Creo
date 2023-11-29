#include "CADModel.h"
#include <stack>

//std::unordered_map<std::string, CADModelTool::CADConsType>mapStr2ConsType = {
//{"UNKNOWN",CADModelTool::UNKNOWN},
//{"FIX",CADModelTool::FIX},
//{"MATE",CADModelTool::MATE},
//{"MATE_OFF",CADModelTool::MATE_OFF},
//{"ALIGN",CADModelTool::ALIGN},
//{"ALIGN_OFF",CADModelTool::ALIGN_OFF},
//{"TANGENT",CADModelTool::TANGENT}
//};
//std::unordered_map<CADModelTool::CADConsType, std::string>mapConsType2Str = {
//{CADModelTool::UNKNOWN,"UNKNOWN"},
//{CADModelTool::FIX,"FIX"},
//{CADModelTool::MATE,"MATE"},
//{CADModelTool::MATE_OFF,"MATE_OFF"},
//{CADModelTool::ALIGN,"ALIGN"},
//{CADModelTool::ALIGN_OFF,"ALIGN_OFF"},
//{CADModelTool::TANGENT,"TANGENT"}
//};
//std::unordered_map< CADModelTool::ElemType, std::string>glStrTypeData.mapElemType2Str = {
//		{CADModelTool::UnknownType,"UnknownType"},
//		{CADModelTool::Point,"Point"},
//		{CADModelTool::Axis,"Axis"},
//		{CADModelTool::Line,"Line"},
//		{CADModelTool::Plane,"Plane"},
//		{CADModelTool::CylindricalSrf,"CylindricalSrf"}
//};
//std::unordered_map<std::string, CADModelTool::ElemType>mapStr2ElemType = {
//	{"UnknownType",CADModelTool::UnknownType},
//	{"Point",CADModelTool::Point},
//	{"Axis",CADModelTool::Axis},
//	{"Line",CADModelTool::Line},
//	{"Plane",CADModelTool::Plane},
//	{"CylindricalSrf",CADModelTool::CylindricalSrf}
//};
//std::unordered_map<std::string, CADModelTool::BaseElemType>mapStr2BaseElemType = {
//	{"UnknownType",CADModelTool::BaseElemType::BaseUnknownType},
//	{"Point",CADModelTool::BaseElemType::BasePoint},
//	{"Vector",CADModelTool::BaseElemType::BaseVector}
//};
//std::unordered_map<CADModelTool::BaseElemType, std::string>mapBaseElemType2Str = {
//		{CADModelTool::BaseElemType::BaseUnknownType,"UnknownType"},
//		{CADModelTool::BaseElemType::BasePoint,"Point"},
//		{CADModelTool::BaseElemType::BaseVector,"Vector"},
//};

static class StringTypeData
{
public:
	std::unordered_map<std::string, CADModelTool::CADConsType>mapStr2ConsType;
	std::unordered_map<CADModelTool::CADConsType, std::string>mapConsType2Str;
	std::unordered_map< CADModelTool::ElemType, std::string>mapElemType2Str;
	std::unordered_map<std::string, CADModelTool::ElemType>mapStr2ElemType;
	std::unordered_map<std::string, CADModelTool::BaseElemType>mapStr2BaseElemType;
	std::unordered_map<CADModelTool::BaseElemType, std::string>mapBaseElemType2Str;
	StringTypeData()
	{
		mapStr2ConsType["FIX"] = CADModelTool::FIX;
		mapStr2ConsType["MATE"] = CADModelTool::MATE;
		mapStr2ConsType["MATE_OFF"] = CADModelTool::MATE_OFF;
		mapStr2ConsType["ALIGN"] = CADModelTool::ALIGN;
		mapStr2ConsType["ALIGN_OFF"] = CADModelTool::ALIGN_OFF;
		mapStr2ConsType["TANGENT"] = CADModelTool::TANGENT;

		mapConsType2Str[CADModelTool::UNKNOWN] = "UNKNOWN";
		mapConsType2Str[CADModelTool::FIX] = "FIX";
		mapConsType2Str[CADModelTool::MATE] = "MATE";
		mapConsType2Str[CADModelTool::MATE_OFF] = "MATE_OFF";
		mapConsType2Str[CADModelTool::ALIGN] = "ALIGN";
		mapConsType2Str[CADModelTool::ALIGN_OFF] = "ALIGN_OFF";
		mapConsType2Str[CADModelTool::TANGENT] = "TANGENT";

		mapElemType2Str[CADModelTool::UnknownType] = "UnknownType";
		mapElemType2Str[CADModelTool::Point] = "Point";
		mapElemType2Str[CADModelTool::Axis] = "Axis";
		mapElemType2Str[CADModelTool::Line] = "Line";
		mapElemType2Str[CADModelTool::Plane] = "Plane";
		mapElemType2Str[CADModelTool::CylindricalSrf] = "CylindricalSrf";

		mapStr2ElemType["UnknownType"] = CADModelTool::UnknownType;
		mapStr2ElemType["Point"] = CADModelTool::Point;
		mapStr2ElemType["Axis"] = CADModelTool::Axis;
		mapStr2ElemType["Line"] = CADModelTool::Line;
		mapStr2ElemType["Plane"] = CADModelTool::Plane;
		mapStr2ElemType["CylindricalSrf"] = CADModelTool::CylindricalSrf;

		mapStr2BaseElemType["UnknownType"] = CADModelTool::BaseElemType::BaseUnknownType;
		mapStr2BaseElemType["Point"] = CADModelTool::BaseElemType::BasePoint;
		mapStr2BaseElemType["Vector"] = CADModelTool::BaseElemType::BaseVector;

		mapBaseElemType2Str[CADModelTool::BaseElemType::BaseUnknownType] = "UnknownType";
		mapBaseElemType2Str[CADModelTool::BaseElemType::BasePoint] = "Point";
		mapBaseElemType2Str[CADModelTool::BaseElemType::BaseVector] = "Vector";
	}
	~StringTypeData() {}
};
static StringTypeData glStrTypeData;
static int Loc_CreatePointElem(tinyxml2::XMLElement * elemNode, const CADModelTool::CADPoint & pnt)
{
	auto iter = glStrTypeData.mapBaseElemType2Str.find(CADModelTool::BaseElemType::BasePoint);
	if (iter == glStrTypeData.mapBaseElemType2Str.cend())
		return 1;
	elemNode->SetAttribute("type", iter->second.c_str());
	tinyxml2::XMLElement* pNode = elemNode->InsertNewChildElement("x");
	pNode->SetText(pnt.x);

	pNode = elemNode->InsertNewChildElement("y");
	pNode->SetText(pnt.y);

	pNode = elemNode->InsertNewChildElement("z");
	pNode->SetText(pnt.z);
	return 0;
}
static int Loc_CreateVectorElem(tinyxml2::XMLElement * elemNode, const CADModelTool::CADVector & vec)
{
	auto iter = glStrTypeData.mapBaseElemType2Str.find(CADModelTool::BaseElemType::BaseVector);
	if (iter == glStrTypeData.mapBaseElemType2Str.cend())
		return 1;
	elemNode->SetAttribute("type", iter->second.c_str());
	tinyxml2::XMLElement* pNode = elemNode->InsertNewChildElement("x");
	pNode->SetText(vec.x);

	pNode = elemNode->InsertNewChildElement("y");
	pNode->SetText(vec.y);

	pNode = elemNode->InsertNewChildElement("z");
	pNode->SetText(vec.z);
	return 0;
}
static int Loc_CreateReConstructedElem(tinyxml2::XMLElement * pReconstructed_element, const CADModelTool::BaseAsmElem * pElem)
{
	if (!pElem)
		return 0;

	if (!pReconstructed_element)
		return 1;
	switch (pElem->type)
	{
	case CADModelTool::ElemType::Plane:
	{
		CADModelTool::PlaneElem* plane = (CADModelTool::PlaneElem*)pElem;

		tinyxml2::XMLElement* pOriginPntElem = pReconstructed_element->InsertNewChildElement("base_elem");
		pOriginPntElem->SetAttribute("name", "origin");
		Loc_CreatePointElem(pOriginPntElem, plane->plane.origin);

		tinyxml2::XMLElement* pNormalVecElem = pReconstructed_element->InsertNewChildElement("base_elem");
		pNormalVecElem->SetAttribute("name", "normal");
		Loc_CreateVectorElem(pNormalVecElem, plane->plane.normal);
	}
	break;
	default:
		break;
	}
	return 0;
}
static int Loc_CreateConstrantNode(tinyxml2::XMLElement * pConstrants1, const CADModelTool::CADConstraint cons)
{
	//约束类型
	auto iterStrType = glStrTypeData.mapConsType2Str.find(cons.type);
	if (iterStrType == glStrTypeData.mapConsType2Str.cend())
		return 1;
	pConstrants1->SetAttribute("type", iterStrType->second.c_str());

	//约束参考的第一个体素
	if (cons.pElem1)
	{
		tinyxml2::XMLElement* pConstrants1Comp1 = pConstrants1->InsertNewChildElement("constrant_component1");
		tinyxml2::XMLElement* pAuxiliary_file1 = pConstrants1Comp1->InsertNewChildElement("auxiliary_file");
		pAuxiliary_file1->SetText(cons.pElem1->stpFile.c_str());
		tinyxml2::XMLElement* pReconstructed_element1 = pConstrants1Comp1->InsertNewChildElement("reconstructed_element");

		auto iterStrType = glStrTypeData.mapElemType2Str.find(cons.pElem1->type);
		if (iterStrType == glStrTypeData.mapElemType2Str.cend())
			return 1;
		pReconstructed_element1->SetAttribute("type", iterStrType->second.c_str());
		if (Loc_CreateReConstructedElem(pReconstructed_element1, cons.pElem1))
			return 1;
	}

	//约束参考的第二个体素
	if (cons.pElem2)
	{
		tinyxml2::XMLElement* pConstrants1Comp2 = pConstrants1->InsertNewChildElement("constrant_component2");
		tinyxml2::XMLElement* pAuxiliary_file2 = pConstrants1Comp2->InsertNewChildElement("auxiliary_file");
		pAuxiliary_file2->SetText(cons.pElem2->stpFile.c_str());
		tinyxml2::XMLElement* pReconstructed_element2 = pConstrants1Comp2->InsertNewChildElement("reconstructed_element");

		auto iterStrType = glStrTypeData.mapElemType2Str.find(cons.pElem2->type);
		if (iterStrType == glStrTypeData.mapElemType2Str.cend())
			return 1;
		pReconstructed_element2->SetAttribute("type", iterStrType->second.c_str());
		if (Loc_CreateReConstructedElem(pReconstructed_element2, cons.pElem2))
			return 1;
	}

	return 0;
}
static void Loc_CreateInitCompPosNode(tinyxml2::XMLElement * pSubNode, const CADModelTool::CADPartInfo & partInfo)
{
	tinyxml2::XMLElement* pPositionNode = pSubNode->InsertNewChildElement("Init_Component_Position");
	tinyxml2::XMLElement* pAuxiNode = pPositionNode->InsertNewChildElement("auxiliary_file");
	pAuxiNode->SetText(partInfo.stepFileName.c_str());
	tinyxml2::XMLElement* pSpaceMatrix = pPositionNode->InsertNewChildElement("space_matrix");

	tinyxml2::XMLElement* pMatrixNode = pSpaceMatrix->InsertNewChildElement("x1");
	pMatrixNode->SetText(partInfo.position.x.x);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("x2");
	pMatrixNode->SetText(partInfo.position.x.y);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("x3");
	pMatrixNode->SetText(partInfo.position.x.z);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("y1");
	pMatrixNode->SetText(partInfo.position.y.x);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("y2");
	pMatrixNode->SetText(partInfo.position.y.y);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("y3");
	pMatrixNode->SetText(partInfo.position.y.z);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("z1");
	pMatrixNode->SetText(partInfo.position.z.x);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("z2");
	pMatrixNode->SetText(partInfo.position.z.y);

	pMatrixNode = pSpaceMatrix->InsertNewChildElement("z3");
	pMatrixNode->SetText(partInfo.position.z.z);
}
static int Loc_CreateNode(tinyxml2::XMLElement * pParentNode, const CADModelTool::CADComponent * pCompNode, tinyxml2::XMLElement * *ppSubNode)
{
	auto pNode = pParentNode->InsertNewChildElement("sub_node");
	pNode->SetAttribute("name", pCompNode->name.c_str());
	pNode->SetAttribute("asm", pCompNode->isAssembly);
	if (pCompNode->isAssembly)
	{
		tinyxml2::XMLElement* pConstrantsGroup = pNode->InsertNewChildElement("constrants_group");
		auto iterCons = pCompNode->vecCons.cbegin();
		if (iterCons != pCompNode->vecCons.cend())
		{
			tinyxml2::XMLElement* pConstrants1 = pConstrantsGroup->InsertNewChildElement("constrant1");
			if (Loc_CreateConstrantNode(pConstrants1, *iterCons))
				return 1;
			++iterCons;
			if (iterCons != pCompNode->vecCons.cend())
			{
				tinyxml2::XMLElement* pConstrants2 = pConstrantsGroup->InsertNewChildElement("constrant2");
				if (Loc_CreateConstrantNode(pConstrants2, *iterCons))
					return 1;
			}
		}
	}
	else
		Loc_CreateInitCompPosNode(pNode, pCompNode->part);
	*ppSubNode = pNode;
	return 0;
}
static int Loc_GetDoubleValue(const tinyxml2::XMLElement * pMatrixNode, std::string name, double& value)
{
	auto x1Attr = pMatrixNode->FirstChildElement(name.c_str());
	if (!x1Attr)
		return 1;
	if (x1Attr->QueryDoubleText(&value) != tinyxml2::XML_SUCCESS)
		return 1;
	return 0;
}
static int Loc_GetPoint(const tinyxml2::XMLElement * pNode, CADModelTool::CADPoint & point)
{
	if (Loc_GetDoubleValue(pNode, "x", point.x))
		return 1;
	if (Loc_GetDoubleValue(pNode, "y", point.y))
		return 1;
	if (Loc_GetDoubleValue(pNode, "z", point.z))
		return 1;
	return 0;
}
static int Loc_GetBasePointElem(const tinyxml2::XMLElement * pBaseElem, std::pair<std::string, CADModelTool::CADPoint>&pairPnt)
{
	auto elemNameStr = pBaseElem->Attribute("name");
	if (!elemNameStr)
		return 1;

	auto elemTypeStr = pBaseElem->Attribute("type");
	if (!elemTypeStr)
		return 1;

	auto baseElemType = glStrTypeData.mapStr2BaseElemType.find(elemTypeStr);
	if (baseElemType == glStrTypeData.mapStr2BaseElemType.cend())
		return 1;

	if (baseElemType->second == CADModelTool::BaseElemType::BasePoint)
	{
		CADModelTool::CADPoint point;
		if (Loc_GetPoint(pBaseElem, point))
			return 1;
		pairPnt.first = elemNameStr;
		pairPnt.second = point;
	}
	else if (baseElemType->second == CADModelTool::BaseElemType::BaseVector)
	{
		CADModelTool::CADVector vector;
		if (Loc_GetPoint(pBaseElem, vector))
			return 1;
		pairPnt.first = elemNameStr;
		pairPnt.second = vector;
	}
	else
		return 1;
	return 0;
}
static int Loc_SetPlaneData(const std::pair<std::string, CADModelTool::CADPoint>&pairPnt, CADModelTool::CADPlane & plane)
{
	if (pairPnt.first == "origin")
		plane.origin = pairPnt.second;
	else if (pairPnt.first == "normal")
		plane.normal = pairPnt.second;
	else
		return 1;
	return 0;
}
static int Loc_GetPlaneElem(const std::string & strAxiliary, const tinyxml2::XMLElement * pReConsElemNode, CADModelTool::PlaneElem * *ppPlaneElem)
{
	auto pBaseElem = pReConsElemNode->FirstChildElement("base_elem");
	if (!pBaseElem)
		return 1;
	std::pair<std::string, CADModelTool::CADPoint> pairPnt;
	if (Loc_GetBasePointElem(pBaseElem, pairPnt))
		return 1;

	auto pAnotherBaseElem = pBaseElem->NextSiblingElement();
	if (!pAnotherBaseElem)
		return 1;
	std::pair<std::string, CADModelTool::CADPoint> anthorPairPnt;
	if (Loc_GetBasePointElem(pAnotherBaseElem, anthorPairPnt))
		return 1;

	auto init = (pairPnt.first == "origin" && anthorPairPnt.first == "normal") ||
		(pairPnt.first == "normal" && anthorPairPnt.first == "origin");
	if (!init)
		return 1;

	CADModelTool::CADPlane plane;
	if (Loc_SetPlaneData(pairPnt, plane) || Loc_SetPlaneData(anthorPairPnt, plane))
		return 1;
	*ppPlaneElem = new CADModelTool::PlaneElem(strAxiliary, plane);
	return 0;
}
static int Loc_GetConsElem(const tinyxml2::XMLElement * pConsCompNode, CADModelTool::BaseAsmElem * *ppBaseElem)
{
	/*1 auxiliary_file*/
	auto pAuxiliaryNode = pConsCompNode->FirstChildElement("auxiliary_file");
	if (!pAuxiliaryNode)
		return 1;
	std::string strAxiliary = pAuxiliaryNode->GetText();

	/*2 reconstructed_element*/
	auto pReConsElemNode = pConsCompNode->FirstChildElement("reconstructed_element");
	if (!pReConsElemNode)
		return 1;

	/*2.1 reconstructed_element type*/
	auto consElemTypeStr = pReConsElemNode->Attribute("type");
	if (!consElemTypeStr)
		return 1;
	auto consElemTypeIter = glStrTypeData.mapStr2ElemType.find(consElemTypeStr);
	if (consElemTypeIter == glStrTypeData.mapStr2ElemType.cend())
		return 1;
	auto elemType = consElemTypeIter->second;

	/*reconstructed_element base elem*/

	if (elemType == CADModelTool::ElemType::Plane)
	{
		CADModelTool::PlaneElem* pPlaneElem = nullptr;
		if (Loc_GetPlaneElem(strAxiliary, pReConsElemNode, &pPlaneElem))
			return 1;
		*ppBaseElem = pPlaneElem;
	}
	else
	{
		return 1;
	}

	return 0;
}
static int Loc_GetConstrant(const tinyxml2::XMLElement * pConsNode, CADModelTool::CADConstraint & constraint)
{
	/*1 type*/
	auto consTypeStr = pConsNode->Attribute("type");
	if (!consTypeStr)
		return 1;

	auto consTypeIter = glStrTypeData.mapStr2ConsType.find(consTypeStr);
	if (consTypeIter == glStrTypeData.mapStr2ConsType.cend())
		return 1;
	constraint.type = consTypeIter->second;

	/*2 constrant_component*/
	auto pConsCompNode = pConsNode->FirstChildElement();
	if (!pConsCompNode)
		return 1;
	CADModelTool::BaseAsmElem* pBaseElem = nullptr;
	if (Loc_GetConsElem(pConsCompNode, &pBaseElem))
		return 1;
	constraint.pElem1 = pBaseElem;

	if (constraint.type != CADModelTool::CADConsType::FIX)
	{
		auto pAnotherConsCompNode = pConsCompNode->NextSiblingElement();
		if (!pAnotherConsCompNode)
			return 1;

		CADModelTool::BaseAsmElem* pAnthorBaseElem = nullptr;
		if (Loc_GetConsElem(pAnotherConsCompNode, &pAnthorBaseElem))
			return 1;
		constraint.pElem2 = pAnthorBaseElem;
	}
	return 0;
}
static int Loc_GetConstrantsGroup(const tinyxml2::XMLElement * pConsGroupNode, std::vector<CADModelTool::CADConstraint> &vecConstraint)
{
	auto pConsNode = pConsGroupNode->FirstChildElement();

	while (pConsNode)
	{
		CADModelTool::CADConstraint constraint;
		if (Loc_GetConstrant(pConsNode, constraint))
			return 1;
		vecConstraint.push_back(constraint);
		pConsNode = pConsNode->NextSiblingElement();
	}
	return 0;
}
static int Loc_GetMatrix(const tinyxml2::XMLElement * pMatrixNode, CADModelTool::CADMatrix & matrix)
{
	double x1;
	if (Loc_GetDoubleValue(pMatrixNode, "x1", x1))
		return 1;
	double x2;
	if (Loc_GetDoubleValue(pMatrixNode, "x2", x2))
		return 1;
	double x3;
	if (Loc_GetDoubleValue(pMatrixNode, "x3", x3))
		return 1;
	double y1;
	if (Loc_GetDoubleValue(pMatrixNode, "y1", y1))
		return 1;
	double y2;
	if (Loc_GetDoubleValue(pMatrixNode, "y2", y2))
		return 1;
	double y3;
	if (Loc_GetDoubleValue(pMatrixNode, "y3", y3))
		return 1;
	double z1;
	if (Loc_GetDoubleValue(pMatrixNode, "z1", z1))
		return 1;
	double z2;
	if (Loc_GetDoubleValue(pMatrixNode, "z2", z2))
		return 1;
	double z3;
	if (Loc_GetDoubleValue(pMatrixNode, "z3", z3))
		return 1;
	matrix.x.x = x1;
	matrix.x.y = x2;
	matrix.x.z = x3;

	matrix.y.x = y1;
	matrix.y.y = y2;
	matrix.y.z = y3;

	matrix.z.x = z1;
	matrix.z.y = z2;
	matrix.z.z = z3;
	return 0;
}
static int Loc_GetPartInfo(const tinyxml2::XMLElement * pPartNode, CADModelTool::CADPartInfo & partInfo)
{
	auto pInitNode = pPartNode->FirstChildElement("Init_Component_Position");
	if (!pInitNode)
		return 1;

	auto pAuxiliaryNode = pInitNode->FirstChildElement("auxiliary_file");
	if (!pAuxiliaryNode)
		return 1;

	std::string strAxiliary = pAuxiliaryNode->GetText();

	auto pMatrixNode = pInitNode->FirstChildElement("space_matrix");
	if (!pMatrixNode)
		return 1;
	CADModelTool::CADMatrix matrix;
	if (Loc_GetMatrix(pMatrixNode, matrix))
		return 1;

	partInfo.stepFileName = strAxiliary;
	partInfo.position = matrix;
	return 0;
}
static int Loc_GetSubNode(CADModelTool::CADComponent * *ppParentNode, tinyxml2::XMLElement * pCurElem)
{
	/*get name*/
	auto nameStr = pCurElem->Attribute("name");
	if (!nameStr)
		return 1;

	/*get assembly*/
	auto asmAttr = pCurElem->FindAttribute("asm");
	if (asmAttr == nullptr)
		return 1;
	bool isAsm = false;
	if (asmAttr->QueryBoolValue(&isAsm) != tinyxml2::XML_SUCCESS)
		return 1;

	if (isAsm)/*asm node*/
	{
		auto pConsGroupNode = pCurElem->FirstChildElement("constrants_group");
		if (!pConsGroupNode)
			return 1;
		std::vector<CADModelTool::CADConstraint> vecConstraint;
		if (Loc_GetConstrantsGroup(pConsGroupNode, vecConstraint))
			return 1;
		CADModelTool::CADComponent* pAsmNode = new CADModelTool::CADComponent(nameStr, vecConstraint);
		if (*ppParentNode)
			(*ppParentNode)->vecSubNodes.push_back(pAsmNode);
		else
			*ppParentNode = pAsmNode;

		auto pChildElemNode = pCurElem->FirstChildElement("sub_node");
		while (pChildElemNode)
		{
			if (Loc_GetSubNode(&pAsmNode, pChildElemNode))
				return 1;
			pChildElemNode = pChildElemNode->NextSiblingElement();
		}
	}
	else/*part node*/
	{
		CADModelTool::CADPartInfo partInfo;
		if (Loc_GetPartInfo(pCurElem, partInfo))
			return 1;
		CADModelTool::CADComponent* pPrtNode = new CADModelTool::CADComponent(nameStr, partInfo);
		(*ppParentNode)->vecSubNodes.push_back(pPrtNode);
	}
	return 0;
}

CADModelTool::CADConstraint::~CADConstraint()
{
	if (pElem1)
		delete pElem1;
	if (pElem2)
		delete pElem2;
	pElem1 = nullptr;
	pElem2 = nullptr;
}
CADModelTool::CADComponent::~CADComponent()
{
	for (size_t i = 0; i < vecSubNodes.size(); ++i)
	{
		if (vecSubNodes[i])
		{
			delete vecSubNodes[i];
			vecSubNodes[i] = nullptr;
		}
	}
}
int CADModelTool::ReadModeFromXml(const std::string & xmlFile, CADComponent * *ppRootNode)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(xmlFile.c_str());
	if (err != tinyxml2::XML_SUCCESS)
		return 1;
	tinyxml2::XMLElement* root = doc.RootElement();

	//CADComponent* pRootNode = nullptr;
	auto pSubNode = root->FirstChildElement("sub_node");
	if (Loc_GetSubNode(ppRootNode, pSubNode))
		return 1;
	return 0;
}
int CADModelTool::WriteModelToXml(const CADComponent* pRootNode, const std::string& xmlFile)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);

	tinyxml2::XMLElement* root = doc.NewElement("root");
	doc.InsertEndChild(root);

	std::stack<const CADComponent*>stackNodes;
	stackNodes.push(pRootNode);

	std::stack<tinyxml2::XMLElement*>stackParentXmlNodes;
	stackParentXmlNodes.push(root);

	while (!stackNodes.empty())
	{
		auto pTopNode = stackNodes.top();
		stackNodes.pop();

		auto pXmlParentNode = stackParentXmlNodes.top();
		stackParentXmlNodes.pop();

		tinyxml2::XMLElement* pSubNode = nullptr;
		if (Loc_CreateNode(pXmlParentNode, pTopNode, &pSubNode))
			return 1;

		for (size_t i = 0; i < pTopNode->vecSubNodes.size(); ++i)
		{
			stackNodes.push(pTopNode->vecSubNodes[i]);
			stackParentXmlNodes.push(pSubNode);
		}
	}
	tinyxml2::XMLError err = doc.SaveFile(xmlFile.c_str());
	if (err != tinyxml2::XML_SUCCESS)
		return 1;
	return 0;
}
