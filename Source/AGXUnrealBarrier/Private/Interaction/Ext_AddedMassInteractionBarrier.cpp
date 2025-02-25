// Copyright Aker Solutions. All rights reserved.


#include "Interaction/Ext_AddedMassInteractionBarrier.h"

#include "BarrierOnly/AGXRefs.h"
#include "AGX_LogCategory.h"
#include "RigidBodyBarrier.h"

FExt_AddedMassInteractionBarrier::FExt_AddedMassInteractionBarrier()
	: NativeRef{new FExt_AddedMassInteractionRef}
{
}

FExt_AddedMassInteractionBarrier::FExt_AddedMassInteractionBarrier(FExt_AddedMassInteractionBarrier&& Other) noexcept
	: NativeRef{std::move(Other.NativeRef)}
{
}

FExt_AddedMassInteractionBarrier::FExt_AddedMassInteractionBarrier(std::unique_ptr<FExt_AddedMassInteractionRef> Native)
	: NativeRef{std::move(Native)}
{
}

FExt_AddedMassInteractionBarrier::~FExt_AddedMassInteractionBarrier()
{
	// Must provide a destructor implementation in the .cpp file because the
	// std::unique_ptr NativeRef's destructor must be able to see the definition,
	// not just the forward declaration, of FExt_AddedMassInteractionRef.
}

FExt_AddedMassInteractionBarrier& FExt_AddedMassInteractionBarrier::operator=(FExt_AddedMassInteractionBarrier&& Other) noexcept
{
	NativeRef = std::move(Other.NativeRef);
	return *this;
}

bool FExt_AddedMassInteractionBarrier::HasNative() const
{
	return NativeRef->Native != nullptr;
}

void FExt_AddedMassInteractionBarrier::AllocateNative(const FRigidBodyBarrier* RigidBodyBarrier1, const FRigidBodyBarrier* RigidBodyBarrier2)
{
	check(!HasNative());
	const agx::RigidBodyRef RigidBody1 = RigidBodyBarrier1 ? RigidBodyBarrier1->GetNative()->Native.get(): nullptr;
	const agx::RigidBodyRef RigidBody2 = RigidBodyBarrier2 ? RigidBodyBarrier1->GetNative()->Native.get(): nullptr;
	NativeRef->Native = new agx::AddedMassInteraction(RigidBody1, RigidBody2);
}

void FExt_AddedMassInteractionBarrier::ReleaseNative()
{
	check(HasNative());
	NativeRef->Native = nullptr;
}

void FExt_AddedMassInteractionBarrier::SetEnable(bool bIsEnabled)
{
	check(HasNative());
	NativeRef->Native->setEnable(bIsEnabled);
}

void FExt_AddedMassInteractionBarrier::AssignRigidBody1Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	if (NativeRef->Native->getRigidBody1())
	{
		agx::AddedMassInteraction::Matrix6x6& Mat = NativeRef->Native->getRigidBody1Storage()->getBlock();
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 6; ++j)
			{
				Mat[i][j] = Block6x6[i * 6 + j];
			}
		// FString RigidBodyName = NativeRef->Native->getRigidBody1()->getName().c_str(); 
		UE_LOG(LogAGX, Log, TEXT("Assign added mass block to rigid body 1 %hs."),
		       NativeRef->Native->getRigidBody1()->getName().c_str());
	}
	else
	{
		UE_LOG(LogAGX, Warning, TEXT("Failed attempt to assign added mass block to %hs, rigid body 1 is invalid."),
		       NativeRef->Native->getRigidBody1()->getName().c_str());
	}
}

void FExt_AddedMassInteractionBarrier::AssignRigidBody2Block(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	if (NativeRef->Native->getRigidBody2())
	{
		agx::AddedMassInteraction::Matrix6x6& Mat = NativeRef->Native->getRigidBody2Storage()->getBlock();
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 6; ++j)
			{
				Mat[i][j] = Block6x6[i * 6 + j];
			}
		UE_LOG(LogAGX, Log, TEXT("Assign added mass block to rigid body 2 %hs."),
		       NativeRef->Native->getRigidBody2()->getName().c_str());
	}
	else
	{
		UE_LOG(LogAGX, Warning, TEXT("Failed attempt to assign added mass block to %hs, rigid body 2 is invalid."),
		       NativeRef->Native->getRigidBody2()->getName().c_str());
	}
}

void FExt_AddedMassInteractionBarrier::AssignOffDiagonalBlock(const TArray<double, TFixedAllocator<36>>& Block6x6) const
{
	if (NativeRef->Native->getRigidBody1() && NativeRef->Native->getRigidBody2())
	{
		agx::AddedMassInteraction::Matrix6x6& Mat = NativeRef->Native->getOffDiagonalBlock();
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 6; ++j)
			{
				Mat[i][j] = Block6x6[i * 6 + j];
			}
		UE_LOG(LogAGX, Log, TEXT("Assign off-diagonal block added mass interaction between %hs and %hs"),
		       NativeRef->Native->getRigidBody1()->getName().c_str(),
		       NativeRef->Native->getRigidBody2()->getName().c_str());
	}
	else
	{
		UE_LOG(LogAGX, Warning, TEXT("Failed attempt to assign off-diagonal block added mass interaction from %hs,"
			       "on or both rigid bodies are invalid."), NativeRef->Native->getName().c_str());
	}
}

void FExt_AddedMassInteractionBarrier::AssignRigidBody1BlockAt(const TArray<double, TFixedAllocator<36>>& Block6x6, const FVector& At) const
{
	if (agx::RigidBodyRef RigidBody = NativeRef->Native->getRigidBody1())
	{
		auto Mat66 = agx::AddedMassInteraction::Matrix6x6();
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 6; ++j)
			{
				Mat66.set(i, j, Block6x6[i * 6 + j]);
			}
		const agx::Vec3 CmOffset = RigidBody->getCmLocalTranslate() - agx::Vec3(At.X, At.Y, At.Z);
		NativeRef->Native->translateWorldDiagonalBlock(CmOffset, Mat66);
		NativeRef->Native->getRigidBody1Storage()->getBlock() = Mat66;

		UE_LOG(LogAGX, Log, TEXT("Assign added mass block to parent rigid body %hs."),
		       NativeRef->Native->getRigidBody1()->getName().c_str());
	}
	else
	{
		UE_LOG(LogAGX, Warning, TEXT("Failed attempt to assign added mass block to parent rigid body %hs"),
		       NativeRef->Native->getRigidBody2()->getName().c_str());
	}
}

FExt_AddedMassInteractionRef* FExt_AddedMassInteractionBarrier::GetNative()
{
	return NativeRef.get();
}

const FExt_AddedMassInteractionRef* FExt_AddedMassInteractionBarrier::GetNative() const
{
	return NativeRef.get();
}

uintptr_t FExt_AddedMassInteractionBarrier::GetNativeAddress() const
{
	if (!HasNative())
	{
		return 0;
	}

	return reinterpret_cast<uintptr_t>(NativeRef->Native.get());
}

void FExt_AddedMassInteractionBarrier::SetNativeAddress(uintptr_t NativeAddress)
{
	if (NativeAddress == GetNativeAddress())
	{
		return;
	}

	if (HasNative())
	{
		this->ReleaseNative();
	}

	NativeRef->Native = reinterpret_cast<agx::AddedMassInteraction*>(NativeAddress);
}
