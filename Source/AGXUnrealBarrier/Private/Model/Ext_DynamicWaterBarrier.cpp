#include "Model/Ext_DynamicWaterBarrier.h"

#include <agxModel/WindAndWaterController.h>

#include "BarrierOnly/AGXRefs.h"
#include "TypeConversions.h"

namespace
{
	class CustomWaterWrapper : public agxModel::WaterWrapper
	{
	public:
		CustomWaterWrapper(const FExt_DynamicWaterBarrier& Owner): Owner_(Owner)
		{
		}

		virtual agx::Real findHeightFromSurface(const agx::Vec3& worldPoint, const agx::Vec3& upVector, const agx::TimeStamp& t) const override
		{
			return ConvertToAGX(Owner_.FindHeightFromSurface(ConvertDisplacement(worldPoint), ConvertDisplacement(upVector), t));
		}

		virtual agx::Real getDensity() const override
		{
			return ConvertToAGX(Owner_.GetDensity());
		}

		virtual agx::Vec3 getVelocity(const agx::Vec3& worldPoint) const override
		{
			return ConvertVector(Owner_.GetVelocity(ConvertVector(worldPoint)));
		}

		const FExt_DynamicWaterBarrier& Owner_;
	};

	class CustomWaterFlowGenerator : public agxModel::WindAndWaterController::WaterFlowGenerator
	{
	public:
		CustomWaterFlowGenerator(const FExt_DynamicWaterBarrier& Owner): Owner_(Owner)
		{
		}

		virtual agx::Vec3 getVelocity(const agx::Vec3& worldPoint) const override
		{
			return ConvertVector(Owner_.GetVelocity(ConvertVector(worldPoint)));
		}

		const FExt_DynamicWaterBarrier& Owner_;
	};
}


FExt_DynamicWaterBarrier::FExt_DynamicWaterBarrier()
	: NativeRef{new FExt_DynamicWaterRef}
{
}

FExt_DynamicWaterBarrier::FExt_DynamicWaterBarrier(std::unique_ptr<FExt_DynamicWaterRef> Native)
	: NativeRef{std::move(Native)}
{
	check(NativeRef->NativeWaterWrapper->is<agxModel::WaterWrapper>());
	check(NativeRef->NativeWaterFlowGenerator->is<agxModel::WindAndWaterController::WaterFlowGenerator>());
}

FExt_DynamicWaterBarrier::FExt_DynamicWaterBarrier(FExt_DynamicWaterBarrier&& Other) noexcept
	: NativeRef{std::move(Other.NativeRef)}
{
	Other.NativeRef.reset(new FExt_DynamicWaterRef);
}

FExt_DynamicWaterBarrier::~FExt_DynamicWaterBarrier()
{
	// Must provide a destructor implementation in the .cpp file because the
	// std::unique_ptr NativeRef's destructor must be able to see the definition,
	// not just the forward declaration, of FExt_WaterWrapperRef.
}

bool FExt_DynamicWaterBarrier::HasNative() const
{
	return NativeRef->NativeWaterWrapper != nullptr || NativeRef->NativeWaterFlowGenerator != nullptr;
}

void FExt_DynamicWaterBarrier::AllocateNative()
{
	check(!HasNative());
	NativeRef.reset(new FExt_DynamicWaterRef(new CustomWaterWrapper(*this), new CustomWaterFlowGenerator(*this)));
}

FExt_DynamicWaterRef* FExt_DynamicWaterBarrier::GetNative()
{
	check(HasNative());
	return NativeRef.get();
}

const FExt_DynamicWaterRef* FExt_DynamicWaterBarrier::GetNative() const
{
	check(HasNative());
	return NativeRef.get();
}

uintptr_t FExt_DynamicWaterBarrier::GetNativeAddress() const
{
	if (!HasNative())
	{
		return 0;
	}

	return reinterpret_cast<uintptr_t>(NativeRef->NativeWaterWrapper.get());
}

void FExt_DynamicWaterBarrier::SetNativeAddress(uintptr_t NativeAddress)
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
		NativeRef.reset(new FExt_DynamicWaterRef(nullptr, nullptr));
		return;
	}
	
	NativeRef.reset(new FExt_DynamicWaterRef(
		reinterpret_cast<agxModel::WaterWrapper*>(NativeAddress),
		nullptr));
}

void FExt_DynamicWaterBarrier::ReleaseNative()
{
	NativeRef.reset(new FExt_DynamicWaterRef(nullptr, nullptr));
}
