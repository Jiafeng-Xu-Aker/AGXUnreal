// Copyright 2023, Algoryx Simulation AB.

#include "Materials/AGX_Ext_WindAndWaterAwareShapeMaterialAssetFactory.h"

// AGX Dynamics for Unreal includes.

#include "Model/AGX_Ext_WindAndWaterAwareShapeMaterial.h"

UAGX_Ext_WindAndWaterAwareShapeMaterialFactory::UAGX_Ext_WindAndWaterAwareShapeMaterialFactory(const class FObjectInitializer& OBJ)
	: Super(OBJ)
{
	SupportedClass = UAGX_ShapeMaterial::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

UObject* UAGX_Ext_WindAndWaterAwareShapeMaterialFactory::FactoryCreateNew(
	UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context,
	FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UAGX_Ext_WindAndWaterAwareShapeMaterial::StaticClass()));
	return NewObject<UAGX_Ext_WindAndWaterAwareShapeMaterial>(
		InParent, Class, Name, Flags | RF_Transactional, Context);
}
