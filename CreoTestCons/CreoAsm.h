#pragma once
#include <ProAsmcomppath.h>
#include <ProMenuBar.h>
#include <ProUtil.h>
#include <ProSolid.h>
#include <ProObjects.h>
#include <ProMdl.h>
#include <ProFeatType.h>
#include <ProFeature.h>
#include "ProFeatForm.h"
#include <ProFeatType.h>
#include "ProElemId.h"
#include "ProSection.h"
#include "ProElempath.h"
#include <ProAsmcomp.h>
#include <ProSurface.h>
#include <ProAxis.h>
#include <ProEdge.h>
#include <ProModelitem.h>
#include <vector>
#include <string>
#include "CADModel.h"
using namespace CADModelTool;

typedef struct asm_data
{
    ProMdl mdl;
    int level;
    ProAsmcomppath comppath;
    CADComponent* pCadNode;
} AsmCompData;

int CreoGetAllAsmConstraints();
