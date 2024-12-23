// Copyright Aker Solutions
#pragma once

// AGX Dynamics for Unreal includes.

// Unreal Engine includes.

// System includes.
#include <memory>

enum class EAGX_Ext_WindAndWaterParametersCoefficient : uint8;
enum class EAGX_Ext_WindAndWaterShapeTessellation : uint8;
class FRigidBodyBarrier;
class FExt_WindAndWaterControllerBarrier;
struct FExt_WindAndWaterParametersRef;

/**
 * Barrier between UAGX_WindAndWaterParameters and agxModel::WindAndWaterParameters. UAGX_RigidBody holds an
 * instance of WindAndWaterParametersBarrier and hidden behind the WindAndWaterParametersBarrier is a
 * agxModel::WindAndWaterParameters. This allows UAGX_WindAndWaterParameters to interact with
 * agxModel::WindAndWaterParameters without including agxModel/WindAndWaterParameters.h
 *
 * This class handles all translation between Unreal Engine types and
 * AGX Dynamics types, such as back and forth between FVector and agx::Vec3.
 */
class AGXUNREALBARRIER_API FExt_WindAndWaterParametersBarrier
{
public:
	FExt_WindAndWaterParametersBarrier();
	FExt_WindAndWaterParametersBarrier(std::unique_ptr<FExt_WindAndWaterParametersRef> Native);
	FExt_WindAndWaterParametersBarrier(FExt_WindAndWaterParametersBarrier&& Other) noexcept;
	~FExt_WindAndWaterParametersBarrier();
	
	bool HasNative() const;
	FExt_WindAndWaterParametersRef* GetNative();
	const FExt_WindAndWaterParametersRef* GetNative() const;

	/// @return The address of the underlying AGX Dynamics object.
	uintptr_t GetNativeAddress() const;

	/// Re-assign this Barrier to the given native address. The address must be an existing AGX
	/// Dynamics object of the correct type.
	void SetNativeAddress(uintptr_t NativeAddress);
	void ReleaseNative();

	void SetCoefficient(EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value) const;
	void SetShapeTessellation(EAGX_Ext_WindAndWaterShapeTessellation ShapeTessellation) const;
	static void SetHydrodynamicCoefficient(FExt_WindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value);
	static void SetAerodynamicCoefficient(FExt_WindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value);

protected:
	std::unique_ptr<FExt_WindAndWaterParametersRef> NativeRef;
};
