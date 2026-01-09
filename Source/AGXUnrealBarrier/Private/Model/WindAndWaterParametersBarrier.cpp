#include "Model/WindAndWaterParametersBarrier.h"
// AGX Dynamics for Unreal includes.
#include "BarrierOnly/AGXRefs.h"

#include <agxModel/WindAndWaterParameters.h>

#include "BarrierOnly/AGXTypeConversions.h"
#include "Model/WindAndWaterControllerBarrier.h"
#include "Model/WindAndWaterParametersEnums.h"

FWindAndWaterParametersBarrier::FWindAndWaterParametersBarrier()
	: NativeRef{new FExt_WindAndWaterParametersRef}
{
}

FWindAndWaterParametersBarrier::FWindAndWaterParametersBarrier(std::unique_ptr<FExt_WindAndWaterParametersRef> Native)
	: NativeRef{std::move(Native)}
{
	check(NativeRef->Native->is<agxModel::WindAndWaterParameters>());
}

FWindAndWaterParametersBarrier::FWindAndWaterParametersBarrier(FWindAndWaterParametersBarrier&& Other) noexcept
	: NativeRef{std::move(Other.NativeRef)}
{
	Other.NativeRef.reset(new FExt_WindAndWaterParametersRef);
}

FWindAndWaterParametersBarrier::~FWindAndWaterParametersBarrier()
{
	// Must provide a destructor implementation in the .cpp file because the
	// std::unique_ptr NativeRef's destructor must be able to see the definition,
	// not just the forward declaration, of FWindAndWaterParametersRef.
}

bool FWindAndWaterParametersBarrier::HasNative() const
{
	return NativeRef->Native != nullptr;
}

FExt_WindAndWaterParametersRef* FWindAndWaterParametersBarrier::GetNative()
{
	check(HasNative());
	return NativeRef.get();
}

const FExt_WindAndWaterParametersRef* FWindAndWaterParametersBarrier::GetNative() const
{
	check(HasNative());
	return NativeRef.get();
}

uintptr_t FWindAndWaterParametersBarrier::GetNativeAddress() const
{
	if (!HasNative())
	{
		return 0;
	}

	return reinterpret_cast<uintptr_t>(NativeRef->Native.get());
}

void FWindAndWaterParametersBarrier::SetNativeAddress(uintptr_t NativeAddress)
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

	NativeRef->Native = reinterpret_cast<agxModel::WindAndWaterParameters*>(NativeAddress);
}

void FWindAndWaterParametersBarrier::ReleaseNative()
{
	NativeRef->Native = nullptr;
}

void FWindAndWaterParametersBarrier::SetCoefficient(EAGX_WindAndWaterParametersCoefficient Coefficient, double Value) const
{
	NativeRef->Native->setCoefficient(Convert(Coefficient), Value);
}

void FWindAndWaterParametersBarrier::SetShapeTessellation(EAGX_WindAndWaterShapeTessellation ShapeTessellation) const
{
	NativeRef->Native->setShapeTessellationLevel(Convert(ShapeTessellation));
}

void FWindAndWaterParametersBarrier::SetHydrodynamicCoefficient(FWindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_WindAndWaterParametersCoefficient Coefficient, double Value)
{
	agxModel::HydrodynamicsParameters::setHydrodynamicCoefficient(Controller->GetNative()->Native, RigidBody->GetNative()->Native, Convert(Coefficient), Value);
}

void FWindAndWaterParametersBarrier::SetAerodynamicCoefficient(FWindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_WindAndWaterParametersCoefficient Coefficient, double Value)
{
	agxModel::HydrodynamicsParameters::setAerodynamicCoefficient(Controller->GetNative()->Native, RigidBody->GetNative()->Native, Convert(Coefficient), Value);
}
