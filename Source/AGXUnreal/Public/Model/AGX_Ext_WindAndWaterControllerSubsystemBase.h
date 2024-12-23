// Copyright Aker Solutions. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AGX_NativeOwner.h"
#include "Model/Ext_WindAndWaterControllerBarrier.h"
#include "AGX_Ext_WindAndWaterControllerSubsystemBase.generated.h"

class UAGX_WireComponent;
class UAGX_ShapeComponent;
class UAGX_Ext_DynamicWaterComponent;

/**
 * A world subsystem that holds the agxModel::WindAndWaterController as a singleton.
 * The native agxModel::WindAndWaterController is instanced and added to the agxSDK::Simulation during initialization.
 */
UCLASS(DisplayName="AGX Wind And Water Subsystem (Extension)", Abstract)
class AGXUNREAL_API UAGX_Ext_WindAndWaterControllerSubsystemBase : public UGameInstanceSubsystem, public IAGX_NativeOwner
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	FExt_WindAndWaterControllerBarrier* GetNative();
	
	bool AddWater(UAGX_ShapeComponent* Shape);

	bool SetEnableAerodynamics(UAGX_ShapeComponent* Shape, bool bEnabled);
	bool SetEnableAerodynamics(UAGX_WireComponent* Wire, bool bEnabled);

	void SetHydrodynamicParameters(UAGX_ShapeComponent* Shape, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value);
	void SetHydrodynamicParameters(UAGX_ShapeComponent* Shape, EAGX_Ext_WindAndWaterShapeTessellation ShapeTessellation);
	void SetAerodynamicParameters(UAGX_ShapeComponent* Shape, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value);
	void SetAerodynamicParameters(UAGX_ShapeComponent* Shape, EAGX_Ext_WindAndWaterShapeTessellation ShapeTessellation);

	void SetWaterWrapper(UAGX_ShapeComponent* Shape, UAGX_Ext_DynamicWaterComponent* WaterWrapper);
	void SetWaterFlowGenerator(UAGX_ShapeComponent* ParentShape, UAGX_Ext_DynamicWaterComponent* DynamicWater);
	virtual bool UpdateNativeWindAndWaterParameters(UAGX_ShapeComponent* Shape);
	
	virtual bool HasNative() const override;
	virtual uint64 GetNativeAddress() const override;
	virtual void SetNativeAddress(uint64 NativeAddress) override;

	static UAGX_Ext_WindAndWaterControllerSubsystemBase* GetFrom(const UActorComponent* Component);

	static UAGX_Ext_WindAndWaterControllerSubsystemBase* GetFrom(const AActor* Actor);

private:
	FExt_WindAndWaterControllerBarrier NativeWindAndWaterControllerBarrier;
};
