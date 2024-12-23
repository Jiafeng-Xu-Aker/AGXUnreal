// Copyright 2023, Algoryx Simulation AB.

#include "Materials/AGX_Ext_WindAndWaterAwareShapeMaterialAssetTypeActions.h"

#include "Model/AGX_Ext_WindAndWaterAwareShapeMaterial.h"

#define LOCTEXT_NAMESPACE "FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions"

FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions(
	EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::GetName() const
{
	return LOCTEXT("AssetName", "AGX Wind And Water Aware Shape Material");
}

uint32 FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::GetCategories()
{
	return AssetCategory;
}

FColor FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::GetTypeColor() const
{
	return FColor(255, 115, 0);
}

FText FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return LOCTEXT("AssetDescription", "Defines bulk and surface properties of AGX Shapes.");
}

UClass* FAGX_Ext_WindAndWaterAwareShapeMaterialTypeActions::GetSupportedClass() const
{
	return UAGX_Ext_WindAndWaterAwareShapeMaterial::StaticClass();
}

#undef LOCTEXT_NAMESPACE
