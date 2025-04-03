#include"VideoSettingsWidget.h"
#include "GameFramework/GameUserSettings.h"
#include"Kismet/KismetSystemLibrary.h"
void UVideoSettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (!UserSettings) return;

    // 质量等级转换函数（需与项目实际配置匹配）
    auto ConvertScalability = [](int32 EngineValue) -> EQualityLevel {
        return static_cast<EQualityLevel>(FMath::Clamp(EngineValue, 0, 4));
        };

    // 初始化快速设置
    QuickSetQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium); // 默认值
    QuickSetQualitySwitcher->OnOptionChanged.AddDynamic(this, &UVideoSettingsWidget::OnQuickSetChanged);

    // 初始化各选项控件
    ShadowQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetShadowQuality()));//阴影质量
    VisualEffectsSwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetVisualEffectQuality()));//视觉效果
    PostProcessingSwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetPostProcessingQuality()));//后期处理
    FoliageQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetFoliageQuality()));//植被质量
    ShadingQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetShadingQuality()));//着色质量
    GlobalIlluminationQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetGlobalIlluminationQuality()));//全局光照
    AntiAliasingQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetAntiAliasingQuality()));//抗锯齿
    ReflectionQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetReflectionQuality()));//反射质量
    TextureQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetTextureQuality()));//纹理质量

    // 初始化窗口模式
    WindowModeDropdown->InitializeWithEnum<EDisplayWindowMode>();
    const EDisplayWindowMode CurrentWindowMode = static_cast<EDisplayWindowMode>(UserSettings->GetFullscreenMode());
    WindowModeDropdown->SetSelectedOption(StaticEnum<EDisplayWindowMode>()->GetDisplayNameTextByValue((int64)CurrentWindowMode).ToString());

    // 初始化分辨率
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
    for (const FIntPoint& Res : AvailableResolutions)
    {
        CachedResolutionStrings.Add(FString::Printf(TEXT("%d x %d"), Res.X, Res.Y));
    }
    ResolutionDropdown->InitializeWithCustomOptions(CachedResolutionStrings);

    // 设置当前分辨率
    FIntPoint CurrentRes = UserSettings->GetScreenResolution();
    FString CurrentResStr = FString::Printf(TEXT("%d x %d"), CurrentRes.X, CurrentRes.Y);
    ResolutionDropdown->SetSelectedOption(CurrentResStr);

    // 初始化帧率
    TArray<FString> FrameRateOptions = { "30", "60", "90", "120", "144", "Unlimited" };
    FrameRateDropdown->InitializeWithCustomOptions(FrameRateOptions);

    // 设置当前帧率
    int32 CurrentFrameRate = UserSettings->GetFrameRateLimit();
    FString CurrentFrameRateStr = CurrentFrameRate == 0 ? "Unlimited" : FString::FromInt(CurrentFrameRate);
    FrameRateDropdown->SetSelectedOption(CurrentFrameRateStr);
}



void UVideoSettingsWidget::SaveSettings()
{
    // 应用设置
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    UserSettings->SetShadowQuality(static_cast<int32>(ShadowQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetVisualEffectQuality(static_cast<int32>(VisualEffectsSwitcher->GetSelectedLevel()));
    UserSettings->SetPostProcessingQuality(static_cast<int32>(PostProcessingSwitcher->GetSelectedLevel()));
    UserSettings->SetFoliageQuality(static_cast<int32>(FoliageQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetShadingQuality(static_cast<int32>(ShadingQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetGlobalIlluminationQuality(static_cast<int32>(GlobalIlluminationQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetAntiAliasingQuality(static_cast<int32>(AntiAliasingQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetReflectionQuality(static_cast<int32>(ReflectionQualitySwitcher->GetSelectedLevel()));
    UserSettings->SetTextureQuality(static_cast<int32>(TextureQualitySwitcher->GetSelectedLevel()));

    // 保存窗口模式
    FString WindowModeStr = WindowModeDropdown->GetSelectedOption();
    EDisplayWindowMode WindowMode = static_cast<EDisplayWindowMode>(StaticEnum<EDisplayWindowMode>()->GetValueByNameString(WindowModeStr));
    UserSettings->SetFullscreenMode(static_cast<EWindowMode::Type>(WindowMode));

    // 保存分辨率
    FString ResolutionStr = ResolutionDropdown->GetSelectedOption();
    FIntPoint NewResolution;
    FString LeftStr, RightStr;
    if (ResolutionStr.Split(TEXT("x"), &LeftStr, &RightStr))
    {
        NewResolution.X = FCString::Atoi(*LeftStr.TrimStartAndEnd());
        NewResolution.Y = FCString::Atoi(*RightStr.TrimStartAndEnd());
        UserSettings->SetScreenResolution(NewResolution);
        UserSettings->RequestResolutionChange(NewResolution.X, NewResolution.Y, UserSettings->GetFullscreenMode());
    }

    // 保存帧率
    FString FrameRateStr = FrameRateDropdown->GetSelectedOption();
    int32 NewFrameRate = FrameRateStr == "Unlimited" ? 0 : FCString::Atoi(*FrameRateStr);
    UserSettings->SetFrameRateLimit(NewFrameRate);

    UserSettings->ApplySettings(false);
}

void UVideoSettingsWidget::RestoreDefaults()
{
    // 重置为默认值
    QuickSetQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    ShadowQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    VisualEffectsSwitcher->SetCurrentLevel(EQualityLevel::Medium);
    PostProcessingSwitcher->SetCurrentLevel(EQualityLevel::Medium);
    FoliageQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    ShadingQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    GlobalIlluminationQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    AntiAliasingQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    ReflectionQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);
    TextureQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium);

    // 重置窗口模式
    WindowModeDropdown->SetSelectedOption(
        StaticEnum<EDisplayWindowMode>()->GetDisplayNameTextByValue((int64)EWindowMode::Fullscreen).ToString());

    // 重置分辨率到系统推荐
    ResolutionDropdown->SetSelectedOption(FString::Printf(TEXT("%d x %d"),1920, 1080));

    // 重置帧率为60
    FrameRateDropdown->SetSelectedOption("60");
   
    SaveSettings();
 
}

void UVideoSettingsWidget::OnQuickSetChanged(EQualityLevel NewLevel)
{
    // 同步所有画质选项到快速设置的等级
    ShadowQualitySwitcher->SetCurrentLevel(NewLevel);
    VisualEffectsSwitcher->SetCurrentLevel(NewLevel);
    PostProcessingSwitcher->SetCurrentLevel(NewLevel);
    FoliageQualitySwitcher->SetCurrentLevel(NewLevel);
    ShadingQualitySwitcher->SetCurrentLevel(NewLevel);
    GlobalIlluminationQualitySwitcher->SetCurrentLevel(NewLevel);
    AntiAliasingQualitySwitcher->SetCurrentLevel(NewLevel);
    ReflectionQualitySwitcher->SetCurrentLevel(NewLevel);
    TextureQualitySwitcher->SetCurrentLevel(NewLevel);
}