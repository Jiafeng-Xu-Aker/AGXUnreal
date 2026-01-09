// Copyright Aker Solutions
#pragma once

UENUM(BlueprintType)
enum class EAGX_WindAndWaterParametersCoefficient : uint8
{
	PRESSURE_DRAG,	
	VISCOUS_DRAG,	
	LIFT, 	
	BUOYANCY 
};

/**
 * Enum that specify the tessellation levels of native, non-mesh shapes, e.g, sphere, capsule and cylinder,
 * that are associated with hydro collision in the context of WindAndWaterController.
 */
UENUM(BlueprintType)
enum class EAGX_WindAndWaterShapeTessellation : uint8
{
	LOW,
	MEDIUM,
	HIGH,
	ULTRA_HIGH,
	DEFAULT_TESSELLATION = MEDIUM,
};
