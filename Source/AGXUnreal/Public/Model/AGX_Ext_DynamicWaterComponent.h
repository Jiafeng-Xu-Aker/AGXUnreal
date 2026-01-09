// Copyright Aker Solutions. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AGX_NativeOwner.h"
#include "Components/SceneComponent.h"
#include "Model/DynamicWaterBarrier.h"
#include "AGX_Ext_DynamicWaterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGXUNREAL_API UAGX_Ext_DynamicWaterComponent : public USceneComponent, public IAGX_NativeOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAGX_Ext_DynamicWaterComponent();

	/**
	 * Get the Native Barrier. Create the native AGX Dynamics object if it does not
	 * already exist.
	 *
	 * @return The Native Barrier for this controller.
	 */
	FDynamicWaterBarrier* GetOrCreateNative();

	/// Return the native AGX Dynamics representation of this controller. May return nullptr.
	FDynamicWaterBarrier* GetNative();

	const FDynamicWaterBarrier* GetNative() const;

	// ~Begin IAGX_NativeOwner interface.
	virtual bool HasNative() const override;
	virtual uint64 GetNativeAddress() const override;
	virtual void SetNativeAddress(uint64 NativeAddress) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	virtual double FindHeightFromSurface(const FVector& WorldPoint, const FVector& UpVector, const double& Time) const;
	virtual double GetDensity() const;
	virtual FVector GetVelocity(const FVector& WorldPoint) const;

private:
	// Create the native AGX Dynamics object.
	void InitializeNative();

	std::unique_ptr<FDynamicWaterBarrier> NativeBarrier;
};
