#include "Model/Ext_WindAndWaterParametersBarrier.h"
// AGX Dynamics for Unreal includes.
#include "BarrierOnly/AGXRefs.h"

#include <agxModel/WindAndWaterParameters.h>

#include "TypeConversions.h"
#include "Model/Ext_WindAndWaterControllerBarrier.h"
#include "Model/Ext_WindAndWaterParametersEnums.h"

FExt_WindAndWaterParametersBarrier::FExt_WindAndWaterParametersBarrier()
	: NativeRef{new FExt_WindAndWaterParametersRef}
{
}

FExt_WindAndWaterParametersBarrier::FExt_WindAndWaterParametersBarrier(std::unique_ptr<FExt_WindAndWaterParametersRef> Native)
	: NativeRef{std::move(Native)}
{
	check(NativeRef->Native->is<agxModel::WindAndWaterParameters>());
}

FExt_WindAndWaterParametersBarrier::FExt_WindAndWaterParametersBarrier(FExt_WindAndWaterParametersBarrier&& Other) noexcept
	: NativeRef{std::move(Other.NativeRef)}
{
	Other.NativeRef.reset(new FExt_WindAndWaterParametersRef);
}

FExt_WindAndWaterParametersBarrier::~FExt_WindAndWaterParametersBarrier()
{
	// Must provide a destructor implementation in the .cpp file because the
	// std::unique_ptr NativeRef's destructor must be able to see the definition,
	// not just the forward declaration, of FWindAndWaterParametersRef.
}

bool FExt_WindAndWaterParametersBarrier::HasNative() const
{
	return NativeRef->Native != nullptr;
}

FExt_WindAndWaterParametersRef* FExt_WindAndWaterParametersBarrier::GetNative()
{
	check(HasNative());
	return NativeRef.get();
}

const FExt_WindAndWaterParametersRef* FExt_WindAndWaterParametersBarrier::GetNative() const
{
	check(HasNative());
	return NativeRef.get();
}

uintptr_t FExt_WindAndWaterParametersBarrier::GetNativeAddress() const
{
	if (!HasNative())
	{
		return 0;
	}

	return reinterpret_cast<uintptr_t>(NativeRef->Native.get());
}

void FExt_WindAndWaterParametersBarrier::SetNativeAddress(uintptr_t NativeAddress)
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

void FExt_WindAndWaterParametersBarrier::ReleaseNative()
{
	NativeRef->Native = nullptr;
}

void FExt_WindAndWaterParametersBarrier::SetCoefficient(EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value) const
{
	NativeRef->Native->setCoefficient(Convert(Coefficient), Value);
}

void FExt_WindAndWaterParametersBarrier::SetShapeTessellation(EAGX_Ext_WindAndWaterShapeTessellation ShapeTessellation) const
{
	NativeRef->Native->setShapeTessellationLevel(Convert(ShapeTessellation));
}

void FExt_WindAndWaterParametersBarrier::SetHydrodynamicCoefficient(FExt_WindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value)
{
	agxModel::HydrodynamicsParameters::setHydrodynamicCoefficient(Controller->GetNative()->Native, RigidBody->GetNative()->Native, Convert(Coefficient), Value);
}

void FExt_WindAndWaterParametersBarrier::SetAerodynamicCoefficient(FExt_WindAndWaterControllerBarrier* Controller, FRigidBodyBarrier* RigidBody, EAGX_Ext_WindAndWaterParametersCoefficient Coefficient, double Value)
{
	agxModel::HydrodynamicsParameters::setAerodynamicCoefficient(Controller->GetNative()->Native, RigidBody->GetNative()->Native, Convert(Coefficient), Value);
}
