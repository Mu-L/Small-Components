#pragma once
#include "CoreMinimal.h"
#include"UObject/ObjectMacros.h"
#include"Framerate2.generated.h"

UENUM(BlueprintType)
enum class EFramerate2:uint8
{
	FPS_30 UMETA(DisplayName="30 FPS"),
	FPS_48 UMETA(DisplayName = "48 FPS"),
	FPS_60 UMETA(DisplayName = "60 FPS"),
	FPS_90 UMETA(DisplayName = "90 FPS"),
	FPS_120 UMETA(DisplayName = "120 FPS"),
	FPS_Uncapped UMETA(DisplayName = "Uncapped"),
};

class FFramerateUtils2
{
public:
	static int EnumToValue(const EFramerate2& InFramerate)
	{
		switch (InFramerate)
		{
			case EFramerate2::FPS_30:return 30;
			case EFramerate2::FPS_48:return 48;
			case EFramerate2::FPS_60:return 60;
			case EFramerate2::FPS_90:return 90;
			case EFramerate2::FPS_120:return 120;
			default:return 0;
		}
	}

	static FString EnumToString(const EFramerate2& InFramerate)
	{
		const auto Value = EnumToValue(InFramerate);
		return Value > 0 
			? FString::Printf(TEXT("%d FPS"), Value) 
			: FString::Printf(TEXT("Uncapped"));
	}
};