// Copyright Aker Solutions. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FExt_AddedMassInteractionRef;
class FRigidBodyBarrier;

/**
 * TODO: add description
 */
class AGXUNREALBARRIER_API FExt_AddedMassInteractionBarrier
{
public:
	FExt_AddedMassInteractionBarrier();
	FExt_AddedMassInteractionBarrier(std::unique_ptr<FExt_AddedMassInteractionRef> Native);
	FExt_AddedMassInteractionBarrier(FExt_AddedMassInteractionBarrier&& Other) noexcept;
	~FExt_AddedMassInteractionBarrier();

	FExt_AddedMassInteractionBarrier& operator=(FExt_AddedMassInteractionBarrier&& Other) noexcept;

	bool HasNative() const;
	void AllocateNative(const FRigidBodyBarrier* RigidBodyBarrier1, const FRigidBodyBarrier* RigidBodyBarrier2);
	FExt_AddedMassInteractionRef* GetNative();
	const FExt_AddedMassInteractionRef* GetNative() const;

	/// @return The address of the underlying AGX Dynamics object.
	uintptr_t GetNativeAddress() const;

	/// Re-assign this Barrier to the given native address. The address must be an existing AGX
	/// Dynamics object of the correct type.
	void SetNativeAddress(uintptr_t NativeAddress);
	void ReleaseNative();

	void SetEnable(bool bIsEnabled);
	void AssignRigidBody1Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignRigidBody2Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignOffDiagonalBlock(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignRigidBody1BlockAt(const TArray<double, TFixedAllocator<36>>& Block6x6, const FVector& At) const;
private:
	std::unique_ptr<FExt_AddedMassInteractionRef> NativeRef;
};
