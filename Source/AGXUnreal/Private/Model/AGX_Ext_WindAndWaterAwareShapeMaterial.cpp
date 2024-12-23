// Copyright Aker Solutions. All Rights Reserved.

#include "Model/AGX_Ext_WindAndWaterAwareShapeMaterial.h"

void UAGX_Ext_WindAndWaterAwareShapeMaterial::CopyShapeMaterialProperties(const UAGX_ShapeMaterial* Source)
{
	Super::CopyShapeMaterialProperties(Source);
	if (auto SubSource = Cast<UAGX_Ext_WindAndWaterAwareShapeMaterial>(Source))
	{
		bIsWaterGeometry = SubSource->bIsWaterGeometry;
		HydroParameters = SubSource->HydroParameters;
		AeroParameters = SubSource->AeroParameters;
	}
}