// Copyright Aker Solutions. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AGX_NativeOwner.h"
#include "Components/SceneComponent.h"
#include "Interaction/Ext_AddedMassInteractionBarrier.h"
#include "AGX_Ext_AddedMassInteractionComponent.generated.h"

class UAGX_RigidBodyComponent;

UCLASS(BlueprintType)
class AGXUNREAL_API UAGX_Ext_AddedMassInteraction : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<double> RigidBody1AddedMassMatrix;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<double> RigidBody2AddedMassMatrix;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<double> OffDiagonalAddedMassMatrix;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGXUNREAL_API UAGX_Ext_AddedMassInteractionComponent : public USceneComponent, public IAGX_NativeOwner
{
	GENERATED_BODY()

public:
	/// Get the native AGX Dynamics representation of this added mass. Create it if necessary.
	FExt_AddedMassInteractionBarrier* GetOrCreateNative();

	/// Return the native AGX Dynamics representation of this added mass. May return nullptr.
	FExt_AddedMassInteractionBarrier* GetNative();

	const FExt_AddedMassInteractionBarrier* GetNative() const;

	// ~Begin IAGX_NativeOwner interface.
	virtual bool HasNative() const override;
	virtual uint64 GetNativeAddress() const override;
	virtual void SetNativeAddress(uint64 NativeAddress) override;
	// ~End IAGX_NativeOwner interface.
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	void SetEnable(bool bFlag);
	void AssignRigidBody1Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignRigidBody2Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignOffDiagonalBlock(const TArray<double, TFixedAllocator<36>>& Block6x6) const;
	void AssignRigidBody1BlockAt(const TArray<double, TFixedAllocator<36>>& Block6x6, const FVector& At) const;

protected:
	void InitializeNative();
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAGX_Ext_AddedMassInteraction> AddedMassInteraction;

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UAGX_RigidBodyComponent> RigidBody1;

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UAGX_RigidBodyComponent> RigidBody2;

	UPROPERTY(EditAnywhere)
	bool bIsEnabled;

protected:
	FExt_AddedMassInteractionBarrier NativeBarrier;
};
