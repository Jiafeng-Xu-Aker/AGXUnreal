// Copyright Aker Solutions. All rights reserved.


#include "Interaction/AGX_Ext_AddedMassInteractionComponent.h"

#include "AGX_LogCategory.h"
#include "AGX_RigidBodyComponent.h"
#include "AGX_Simulation.h"
#include "Utilities/AGX_StringUtilities.h"

FExt_AddedMassInteractionBarrier* UAGX_Ext_AddedMassInteractionComponent::GetOrCreateNative()
{
	if (!HasNative())
	{
		if (GIsReconstructingBlueprintInstances)
		{
			// We're in a very bad situation. Someone need this Component's native but if we're in
			// the middle of a RerunConstructionScripts and this Component haven't been given its
			// Native yet then there isn't much we can do. We can't create a new one since we will
			// be given the actual Native soon, but we also can't return the actual Native right now
			// because it hasn't been restored from the Component Instance Data yet.
			//
			// For now we simply die in non-shipping (checkNoEntry is active) so unit tests will
			// detect this situation, and log error and return nullptr otherwise, so that the
			// application can at least keep running. It is unlikely that the simulation will behave
			// as intended.
			checkNoEntry();
			UE_LOG(
				LogAGX, Error,
				TEXT("A request for the AGX Dynamics instance for added mass '%s' in '%s' was made "
					"but we are in the middle of a Blueprint Reconstruction and the requested "
					"instance has not yet been restored. The instance cannot be returned, which "
					"may lead to incorrect scene configuration."),
				*GetName(), *GetLabelSafe(GetOwner()));
			return nullptr;
		}

		InitializeNative();
	}
	check(HasNative()); /// \todo Consider better error handling than 'check'.
	return &NativeBarrier;
}


bool UAGX_Ext_AddedMassInteractionComponent::HasNative() const
{
	return NativeBarrier.HasNative();
}

uint64 UAGX_Ext_AddedMassInteractionComponent::GetNativeAddress() const
{
	return static_cast<uint64>(NativeBarrier.GetNativeAddress());
}

void UAGX_Ext_AddedMassInteractionComponent::SetNativeAddress(uint64 NativeAddress)
{
	check(!HasNative());
	NativeBarrier.SetNativeAddress(static_cast<uintptr_t>(NativeAddress));
}

void UAGX_Ext_AddedMassInteractionComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (GIsReconstructingBlueprintInstances)
	{
		// Another UAGX_Ext_AddedMassInteractionComponent will inherit this one's Native, so don't wreck it.
		// The call to NativeBarrier.ReleaseNative below is safe because the AGX Dynamics Simulation
		// will retain a reference counted pointer to the AGX Dynamics Rigid Body.
		//
		// But what if the this Native isn't currently part of any Simulation? Can we guarantee that
		// something will keep the Native instance alive? Should we do explicit incref/decref
		// on the Native in GetNativeAddress / SetNativeAddress?
	}
	else if (
		HasNative() && Reason != EEndPlayReason::EndPlayInEditor &&
		Reason != EEndPlayReason::Quit && Reason != EEndPlayReason::LevelTransition)
	{
		if (UAGX_Simulation* Sim = UAGX_Simulation::GetFrom(this))
		{
			Sim->Remove(*this);
		}
	}

	if (HasNative())
	{
		NativeBarrier.ReleaseNative();
	}
}

void UAGX_Ext_AddedMassInteractionComponent::SetEnable(bool bFlag)
{
	check(HasNative());
	NativeBarrier.SetEnable(bFlag);
}

void UAGX_Ext_AddedMassInteractionComponent::AssignRigidBody1Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	check(HasNative())
	NativeBarrier.AssignRigidBody1Block(Block6x6);
}

void UAGX_Ext_AddedMassInteractionComponent::AssignRigidBody2Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	check(HasNative())
	NativeBarrier.AssignRigidBody2Block(Block6x6);
}

void UAGX_Ext_AddedMassInteractionComponent::AssignOffDiagonalBlock(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	check(HasNative())
	NativeBarrier.AssignOffDiagonalBlock(Block6x6);
}

void UAGX_Ext_AddedMassInteractionComponent::AssignRigidBody1BlockAt(const TArray<double, TFixedAllocator<36>>& Block6x6, const FVector& At) const
{
	check(HasNative())
	NativeBarrier.AssignRigidBody1BlockAt(Block6x6, At);
}

void UAGX_Ext_AddedMassInteractionComponent::InitializeNative()
{
	check(!GIsReconstructingBlueprintInstances);
	check(!HasNative());
	const FRigidBodyBarrier* RigidBody1Barrier = RigidBody1.IsValid() ? RigidBody1->GetOrCreateNative() : nullptr;
	const FRigidBodyBarrier* RigidBody2Barrier = RigidBody2.IsValid() ? RigidBody2->GetOrCreateNative() : nullptr;
	NativeBarrier.AllocateNative(RigidBody1Barrier, RigidBody2Barrier);
	check(HasNative());
}

FExt_AddedMassInteractionBarrier* UAGX_Ext_AddedMassInteractionComponent::GetNative()
{
	if (!HasNative())
	{
		return nullptr;
	}
	return &NativeBarrier;
}

const FExt_AddedMassInteractionBarrier* UAGX_Ext_AddedMassInteractionComponent::GetNative() const
{
	if (!HasNative())
	{
		return nullptr;
	}
	return &NativeBarrier;
}

// Called when the game starts
void UAGX_Ext_AddedMassInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!HasNative() && !GIsReconstructingBlueprintInstances)
	{
		// Not initializing a new Native if currently reconstructing Blueprint instances because
		// if we should have a Native then one will be assigned to us by our Component Instance
		// Data.
		InitializeNative();

		check(HasNative()); /// \todo Consider better error handling than 'check'.
		SetEnable(bIsEnabled);
		if (AddedMassInteraction)
		{
			AssignRigidBody1Block(TArray<double, TFixedAllocator<36>>(AddedMassInteraction->RigidBody1AddedMassMatrix));
			AssignRigidBody2Block(TArray<double, TFixedAllocator<36>>(AddedMassInteraction->RigidBody2AddedMassMatrix));
			AssignOffDiagonalBlock(TArray<double, TFixedAllocator<36>>(AddedMassInteraction->OffDiagonalAddedMassMatrix));
		}
	}
	// ...
}
