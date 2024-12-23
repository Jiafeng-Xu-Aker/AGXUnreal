#include "Model/Ext_WindAndWaterControllerBarrier.h"

// AGX Dynamics for Unreal includes.
#include "BarrierOnly/AGXRefs.h"

#include <agxModel/WindAndWaterController.h>

#include "AGXBarrierFactories.h"
#include "Model/Ext_DynamicWaterBarrier.h"

FExt_WindAndWaterControllerBarrier::FExt_WindAndWaterControllerBarrier()
	: NativeRef{new FExt_WindAndWaterControllerRef}
{
}

FExt_WindAndWaterControllerBarrier::FExt_WindAndWaterControllerBarrier(std::unique_ptr<FExt_WindAndWaterControllerRef> Native)
	: NativeRef{std::move(Native)}
{
	check(NativeRef->Native->is<agxModel::WindAndWaterController>());
}

FExt_WindAndWaterControllerBarrier::FExt_WindAndWaterControllerBarrier(FExt_WindAndWaterControllerBarrier&& Other) noexcept
	: NativeRef{std::move(Other.NativeRef)}
{
	Other.NativeRef.reset(new FExt_WindAndWaterControllerRef);
}

FExt_WindAndWaterControllerBarrier::~FExt_WindAndWaterControllerBarrier()
{
	// Must provide a destructor implementation in the .cpp file because the
	// std::unique_ptr NativeRef's destructor must be able to see the definition,
	// not just the forward declaration, of FWindAndWaterControllerRef.
}

bool FExt_WindAndWaterControllerBarrier::HasNative() const
{
	return NativeRef->Native != nullptr;
}

void FExt_WindAndWaterControllerBarrier::AllocateNative()
{
	check(!HasNative());
	NativeRef->Native = new agxModel::WindAndWaterController();
}

FExt_WindAndWaterControllerRef* FExt_WindAndWaterControllerBarrier::GetNative()
{
	check(HasNative());
	return NativeRef.get();
}

const FExt_WindAndWaterControllerRef* FExt_WindAndWaterControllerBarrier::GetNative() const
{
	check(HasNative());
	return NativeRef.get();
}

uintptr_t FExt_WindAndWaterControllerBarrier::GetNativeAddress() const
{
	if (!HasNative())
	{
		return 0;
	}

	return reinterpret_cast<uintptr_t>(NativeRef->Native.get());
}

void FExt_WindAndWaterControllerBarrier::SetNativeAddress(uintptr_t NativeAddress)
{
	if (NativeAddress == GetNativeAddress())
	{
		return;
	}

	if (HasNative())
	{
		this->ReleaseNative();
	}

	if (NativeAddress == 0)
	{
		NativeRef->Native = nullptr;
		return;
	}

	NativeRef->Native = reinterpret_cast<agxModel::WindAndWaterController*>(NativeAddress);
}

void FExt_WindAndWaterControllerBarrier::ReleaseNative()
{
	NativeRef->Native = nullptr;
}

FExt_WindAndWaterParametersBarrier FExt_WindAndWaterControllerBarrier::GetOrCreateHydrodynamicsParameters(FShapeBarrier* Shape) const
{
	check(HasNative());
	return AGXBarrierFactories::CreateWindAndWaterParametersBarrier(NativeRef->Native->getOrCreateHydrodynamicsParameters(Shape->GetNative()->NativeShape));
}

FExt_WindAndWaterParametersBarrier FExt_WindAndWaterControllerBarrier::GetOrCreateHydrodynamicsParameters(FWireBarrier* Wire) const
{
	check(HasNative());
	return AGXBarrierFactories::CreateWindAndWaterParametersBarrier(NativeRef->Native->getOrCreateAerodynamicsParameters(Wire->GetNative()->Native));
}

FExt_WindAndWaterParametersBarrier FExt_WindAndWaterControllerBarrier::GetOrCreateAerodynamicsParameters(FShapeBarrier* Shape) const
{
	check(HasNative());
	return AGXBarrierFactories::CreateWindAndWaterParametersBarrier(NativeRef->Native->getOrCreateAerodynamicsParameters(Shape->GetNative()->NativeShape));
}

FExt_WindAndWaterParametersBarrier FExt_WindAndWaterControllerBarrier::GetOrCreateAerodynamicsParameters(FWireBarrier* Wire) const
{
	check(HasNative());
	return AGXBarrierFactories::CreateWindAndWaterParametersBarrier(NativeRef->Native->getOrCreateAerodynamicsParameters(Wire->GetNative()->Native));
}

bool FExt_WindAndWaterControllerBarrier::SetEnableAerodynamics(FShapeBarrier* Shape, bool bEnabled)
{
	check(HasNative());
	return NativeRef->Native->setEnableAerodynamics(Shape->GetNative()->NativeGeometry, bEnabled);
}

bool FExt_WindAndWaterControllerBarrier::SetEnableAerodynamics(FWireBarrier* Wire, bool bEnabled)
{
	check(HasNative());
	return NativeRef->Native->setEnableAerodynamics(Wire->GetNative()->Native, bEnabled);
}

bool FExt_WindAndWaterControllerBarrier::AddWater(const FShapeBarrier* Shape)
{
	check(HasNative());
	return GetNative()->Native->addWater(Shape->GetNative()->NativeGeometry);
}

bool FExt_WindAndWaterControllerBarrier::SetWaterWrapper(FShapeBarrier* Shape, FExt_DynamicWaterBarrier* DynamicWater)
{
	check(HasNative());
	return GetNative()->Native->setWaterWrapper(Shape->GetNative()->NativeGeometry, DynamicWater->GetNative()->NativeWaterWrapper);
}

bool FExt_WindAndWaterControllerBarrier::SetWaterFlowGenerator(FShapeBarrier* Shape, FExt_DynamicWaterBarrier* DynamicWater)
{
	check(HasNative());
	return GetNative()->Native->setWaterFlowGenerator(Shape->GetNative()->NativeGeometry, DynamicWater->GetNative()->NativeWaterFlowGenerator);
}