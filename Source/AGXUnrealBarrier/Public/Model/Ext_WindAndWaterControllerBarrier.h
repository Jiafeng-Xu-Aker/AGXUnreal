// Copyright Aker Solutions
#pragma once

// AGX Dynamics for Unreal includes.

// Unreal Engine includes.

// System includes.
#include <memory>

#include "Ext_WindAndWaterParametersBarrier.h"


class FExt_WaterFlowGeneratorBarrier;
class FExt_DynamicWaterBarrier;
class FWireBarrier;
struct FShapeBarrier;
struct FExt_WindAndWaterControllerRef;
struct FGeometryRef;

/**
 * Barrier between UAGX_WindAndWaterController and agxModel::WindAndWaterController.
 * UAGX_WindAndWaterController holds an instance of WindAndWaterControllerBarrier
 * and hidden behind the WindAndWaterControllerBarrier is a agxModel::WindAndWaterController.
 * This allows UAGX_WindAndWaterController to interact with
 * agxModel::WindAndWaterController without including agxModel/WindAndWaterController.h.
 * This class handles all translation between Unreal Engine types and
 * AGX Dynamics types, such as back and forth between FVector and agx::Vec3.
 */
class AGXUNREALBARRIER_API FExt_WindAndWaterControllerBarrier
{
public:
	FExt_WindAndWaterControllerBarrier();
	FExt_WindAndWaterControllerBarrier(std::unique_ptr<FExt_WindAndWaterControllerRef> Native);
	FExt_WindAndWaterControllerBarrier(FExt_WindAndWaterControllerBarrier&& Other) noexcept;
	~FExt_WindAndWaterControllerBarrier();
	
	bool HasNative() const;
	void AllocateNative();
	FExt_WindAndWaterControllerRef* GetNative();
	const FExt_WindAndWaterControllerRef* GetNative() const;

	/// @return The address of the underlying AGX Dynamics object.
	uintptr_t GetNativeAddress() const;

	/// Re-assign this Barrier to the given native address. The address must be an existing AGX
	/// Dynamics object of the correct type.
	void SetNativeAddress(uintptr_t NativeAddress);
	void ReleaseNative();
	
	FExt_WindAndWaterParametersBarrier GetOrCreateHydrodynamicsParameters(FShapeBarrier* Shape) const;
	FExt_WindAndWaterParametersBarrier GetOrCreateHydrodynamicsParameters(FWireBarrier* Wire) const;
	
	FExt_WindAndWaterParametersBarrier GetOrCreateAerodynamicsParameters(FShapeBarrier* Shape) const;
	FExt_WindAndWaterParametersBarrier GetOrCreateAerodynamicsParameters(FWireBarrier* Wire) const;

	bool SetEnableAerodynamics(FShapeBarrier* Shape, bool bEnabled);
	bool SetEnableAerodynamics(FWireBarrier* Wire, bool bEnabled);
	bool SetEnableHydrodynamics(FShapeBarrier* Shape, bool bEnabled);
	bool SetEnableHydrodynamics(FWireBarrier* Wire, bool bEnabled);
	bool AddWater(const FShapeBarrier* Shape);
	bool SetWaterWrapper(FShapeBarrier* Shape, FExt_DynamicWaterBarrier* DynamicWater);
	bool SetWaterFlowGenerator(FShapeBarrier* Shape, FExt_DynamicWaterBarrier* WaterWrapper);

private:
	std::unique_ptr<FExt_WindAndWaterControllerRef> NativeRef;
};
