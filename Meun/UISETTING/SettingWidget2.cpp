#include "SettingWidget2.h"
#include "GameFramework/GameUserSettings.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Framerate2.h"
#include"SelectionBase.h"


namespace
{
	constexpr EFramerate2 FramerateOptions[] =
	{
		EFramerate2::FPS_30,
		EFramerate2::FPS_48,
		EFramerate2::FPS_60,
		EFramerate2::FPS_90,
		EFramerate2::FPS_120,
		EFramerate2::FPS_Uncapped,
	};

	typedef int32(UGameUserSettings::* GetFunc)()const;
	typedef void(UGameUserSettings::* SetFunc)(int);
	struct FSelectionElement
	{
		USelectionBase* Widget;
		GetFunc GetFunc;
		SetFunc SetFunc;
	};
}

void USettingWidget2::NativeConstruct()
{
	Super::NativeConstruct();
	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	InitializeResolutionComboBox();
	InitializeVSync();
	InitializeFramerate();

	const FSelectionElement SelectionElements[] =
	{
		{ShadingQualitySelection,&UGameUserSettings::GetShadingQuality,&UGameUserSettings::SetShadingQuality},
		{GlobalIlluminationQualitySelection,&UGameUserSettings::GetGlobalIlluminationQuality,&UGameUserSettings::SetGlobalIlluminationQuality},
		{PostProcessQualitySelection,&UGameUserSettings::GetPostProcessingQuality,&UGameUserSettings::SetPostProcessingQuality},
		{VisualEffectQualitySelection,&UGameUserSettings::GetVisualEffectQuality,&UGameUserSettings::SetVisualEffectQuality},
		{ShadowQualitySelection,&UGameUserSettings::GetShadowQuality,&UGameUserSettings::SetShadowQuality}
	};

	for (const auto& [Widget, GetFunc, SetFunc] : SelectionElements) 
	{
		const auto CurrentSelection = std::invoke(GetFunc, GameUserSettings);
		Widget->SetCurrentSelection(CurrentSelection);
		Widget->OnSelectionChange.BindLambda
		(
			[this, SetFunc](int InSelection)
			{
				std::invoke(SetFunc, GameUserSettings, InSelection);
				GameUserSettings->ApplySettings(false);
			}
		);
	}
}

UWidget* USettingWidget2::NativeGetDesiredFocusTarget() const
{
	return ResolutionComboBox;
}

//更新分辨率
void USettingWidget2::InitializeResolutionComboBox()
{
	Resolutions.Reset();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	
	ResolutionComboBox->ClearOptions();
	for (const auto& Resolution : Resolutions)
	{
		const auto ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}

	const auto CurrentResolution = GameUserSettings->GetScreenResolution();
	const auto SelectedIndex = Resolutions.IndexOfByPredicate(
		[&CurrentResolution](const FIntPoint& InResolution)
		{
			return InResolution == CurrentResolution;
		}
	);
	check(SelectedIndex >= 0);

	ResolutionComboBox->OnSelectionChanged.Clear();
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingWidget2::OnResolutionChanged);
}

void USettingWidget2::InitializeVSync()
{
	VSyncCheckBox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
	VSyncCheckBox->OnCheckStateChanged.Clear();
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingWidget2::OnVSyncChanged);
}


//帧数设置
void USettingWidget2::InitializeFramerate()
{
	FramerateSelection->Clear();

	int FrameraOptionIndex = 0;


	const auto CurrentFramerate = GameUserSettings->GetFrameRateLimit();
	for (const auto& Framerate : FramerateOptions) 
	{
		FramerateSelection->AddOption(
			{
				FText::FromString(FFramerateUtils2::EnumToString(Framerate))
			}
		);

		if (CurrentFramerate == FFramerateUtils2::EnumToValue(Framerate))
		{
			FramerateSelection->SetCurrentSelection(FrameraOptionIndex);
		}

		FrameraOptionIndex++;
	}

	FramerateSelection->OnSelectionChange.BindLambda([this](const int InSelection)
		{
			GameUserSettings->SetFrameRateLimit(FFramerateUtils2::EnumToValue(FramerateOptions[InSelection]));
			GameUserSettings->ApplySettings(false);
		}
	);
}

void USettingWidget2::OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	int32 selectedIndex = ResolutionComboBox->GetSelectedIndex();
	if (selectedIndex < 0 || selectedIndex >= Resolutions.Num())
	{
		// Handle the error, e.g., log an error message and return
		UE_LOG(LogTemp, Error, TEXT("OnResolutionChanged: Invalid array index: %d"), selectedIndex);
		return;
	}

	const auto SelectedResolution = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	GameUserSettings->SetScreenResolution(SelectedResolution);
	GameUserSettings->ApplySettings(false);
}

void USettingWidget2::OnVSyncChanged(bool InIsChecked)
{
	GameUserSettings->SetVSyncEnabled(InIsChecked);
	GameUserSettings->ApplySettings(false);
}
