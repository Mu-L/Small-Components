// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CommonDisplaySettings.h"
#include"Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"



void UCommonDisplaySettings::NativeConstruct()
{
    Super::NativeConstruct();
    GameUserSettings = UGameUserSettings::GetGameUserSettings();
    InitializeResolutionComboBox();//分辨率
    InitializeVSync();//垂直同步
    InitializeFullscreenMode();//窗口模式
    InitializeFramerate();//帧数
    InitializeGlobalIllumination();//全局光照
    InitializeAntiAliasing();//抗锯齿
    InitializeReflectionQuality();//反射质量
    InitializeTextureQuality();//纹理质量
    InitializeShadowQuality();//阴影质量
    InitializeVisualEffects();//视觉效果
    InitializePostProcessing();//后期处理
    InitializeVegetationQuality();//植被质量
    InitializeColorQuality();//着色质量

    // 绑定按钮点击事件
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UCommonDisplaySettings::OnSaveButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SaveButton is not bound!"));
    }

    if (RestoreDefaultsButton)
    {
        RestoreDefaultsButton->OnClicked.AddDynamic(this, &UCommonDisplaySettings::OnRestoreDefaultsButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RestoreDefaultsButton is not bound!"));
    }
}

// 辅助函数，用于初始化ComboBox
void UCommonDisplaySettings::InitializeComboBox(UComboBoxString* ComboBox, TArray<FString>& Options, FString DefaultOption)
{
    if (!ComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ComboBox is not bound!"));
        return;
    }

    ComboBox->ClearOptions();
    Options.Empty();
    Options.Add(TEXT("low"));
    Options.Add(TEXT("medium"));
    Options.Add(TEXT("high"));
    Options.Add(TEXT("epic"));
    Options.Add(TEXT("cinematic"));

    for (const FString& Option : Options)
    {
        ComboBox->AddOption(Option);
    }

    int32 SelectedIndex = ComboBox->FindOptionIndex(DefaultOption);
    if (SelectedIndex != INDEX_NONE)
    {
        ComboBox->SetSelectedIndex(SelectedIndex);
    }
    else
    {
        ComboBox->SetSelectedIndex(0);
    }
}

void UCommonDisplaySettings::SetQualityLevel(UGameUserSettings* UserSettings, FString Option, void(UGameUserSettings::* Setter)(int32), EQualityLevel MinLevel, EQualityLevel MaxLevel)
{

    if (!UserSettings)
    {
        return;
    }

    int32 Quality = INDEX_NONE;
    if (Option == TEXT("low")) Quality = static_cast<int32>(EQualityLevel::Low);
    else if (Option == TEXT("medium")) Quality = static_cast<int32>(EQualityLevel::Medium);
    else if (Option == TEXT("high")) Quality = static_cast<int32>(EQualityLevel::High);
    else if (Option == TEXT("epic")) Quality = static_cast<int32>(EQualityLevel::Epic);
    else if (Option == TEXT("cinematic")) Quality = static_cast<int32>(EQualityLevel::Cinematic);

    // 验证质量等级是否在有效范围内
    if (Quality >= static_cast<int32>(MinLevel) && Quality <= static_cast<int32>(MaxLevel))
    {
        (UserSettings->*Setter)(Quality);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid quality level selected: %s"), *Option);
    }

    UserSettings->ApplySettings(false);
}

FString UCommonDisplaySettings::GetQualityLevel(int32(UGameUserSettings::* Getter)() const)
{
    // 使用函数指针获取质量级别
    int32 QualityLevel = (GameUserSettings->*Getter)();
    FString QualityMode;

    switch (QualityLevel)
    {
    case static_cast<int32>(EQualityLevel::Low):
        QualityMode = TEXT("low");
        break;
    case static_cast<int32>(EQualityLevel::Medium):
        QualityMode = TEXT("medium");
        break;
    case static_cast<int32>(EQualityLevel::High):
        QualityMode = TEXT("high");
        break;
    case static_cast<int32>(EQualityLevel::Epic):
        QualityMode = TEXT("epic");
        break;
    case static_cast<int32>(EQualityLevel::Cinematic): // 如果添加了 Cinematic 选项
        QualityMode = TEXT("cinematic");
        break;
    default:
        // 处理无效的质量级别
        QualityMode = TEXT("unknown"); // 或者选择一个更合适的默认值
        UE_LOG(LogTemp, Warning, TEXT("Invalid quality level: %d. Defaulting to '%s'."), QualityLevel, *QualityMode);
        break;
    }

    return QualityMode;
}

void UCommonDisplaySettings::InitializeResolutionComboBox()
{
    // 确保 ComboBox 已绑定
    if (!ResolutionComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ResolutionComboBox is not bound!"));
        return;
    }

    // 清空 ComboBox 的现有选项
    ResolutionComboBox->ClearOptions();

    // 获取支持的分辨率列表
    Resolutions.Reset();
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

    // 添加分辨率选项到 ComboBox
    for (const FIntPoint& Resolution : Resolutions)
    {
        const FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
        ResolutionComboBox->AddOption(ResolutionString);
    }

    // 设置当前分辨率为默认选中项
    const FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();
    const int32 SelectedIndex = Resolutions.IndexOfByPredicate(
        [&CurrentResolution](const FIntPoint& InResolution)
        {
            return InResolution == CurrentResolution;
        }
    );

    // 检查是否找到当前分辨率
    if (SelectedIndex >= 0)
    {
        ResolutionComboBox->SetSelectedIndex(SelectedIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Current resolution not found in supported resolutions!"));
        ResolutionComboBox->SetSelectedIndex(0); // 默认选中第一个选项
    }

    // 绑定选择事件
    ResolutionComboBox->OnSelectionChanged.Clear();
    ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnResolutionChanged);
}

void UCommonDisplaySettings::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // 获取当前选中的索引
    const int32 SelectedIndex = ResolutionComboBox->GetSelectedIndex();

    // 检查索引是否有效
    if (SelectedIndex < 0 || SelectedIndex >= Resolutions.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("OnResolutionChanged: Invalid selected index: %d"), SelectedIndex);
        return;
    }

    // 获取选中的分辨率
    const FIntPoint SelectedResolution = Resolutions[SelectedIndex];

    // 更新游戏用户设置
    if (GameUserSettings)
    {
        GameUserSettings->SetScreenResolution(SelectedResolution);
        GameUserSettings->ApplySettings(false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings is null!"));
    }
}

void UCommonDisplaySettings::InitializeFullscreenMode()
{
    // 确保 ComboBox 已绑定
    if (!FullscreenModeComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("FullscreenModeComboBox is not bound!"));
        return;
    }

    // 清空 ComboBox 的现有选项
    FullscreenModeComboBox->ClearOptions();

    // 初始化窗口模式选项列表
    FullscreenMode.Empty(); // 清空列表（如果之前已经填充过的话）
    FullscreenMode.Add(TEXT("Fullscreen"));
    FullscreenMode.Add(TEXT("WindowedFullscreen"));
    FullscreenMode.Add(TEXT("Windowed"));

    // 添加窗口模式选项到 ComboBox
    for (const FString& Mode : FullscreenMode)
    {
        FullscreenModeComboBox->AddOption(Mode);
    }
  

    // 获取当前的全屏模式设置（这里你需要一个方法来获取这个设置，可能是从GameUserSettings或其他地方）
    FString CurrentFullscreenMode;
    int32 CurrentFullscreenModeChanged = GameUserSettings->GetFullscreenMode();
    if (CurrentFullscreenModeChanged == 0)CurrentFullscreenMode = TEXT("Fullscreen");
    else if (CurrentFullscreenModeChanged == 1)CurrentFullscreenMode = TEXT("WindowedFullscreen");
    else if (CurrentFullscreenModeChanged == 2)CurrentFullscreenMode = TEXT("Windowed");
    
    // 查找当前全屏模式在ComboBox中的索引
    int32 SelectedFullscreenModeIndex = FullscreenModeComboBox->FindOptionIndex(CurrentFullscreenMode);

    // 如果找到了当前全屏模式，则设置它为选中项
    if (SelectedFullscreenModeIndex != INDEX_NONE)
    {
        FullscreenModeComboBox->SetSelectedIndex(SelectedFullscreenModeIndex);
    }
    // 如果没有找到（可能是因为设置了一个不在列表中的模式），则可以选择一个默认值，比如第一个选项
    else
    {
        FullscreenModeComboBox->SetSelectedIndex(0); // 默认选中第一个选项：Windowed
    }

    // 绑定选择事件
    FullscreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnFullscreenModeSelected);
}

void UCommonDisplaySettings::OnFullscreenModeSelected(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // 处理选择的窗口模式
    UE_LOG(LogTemp, Log, TEXT("Selected Fullscreen Mode: %s"), *SelectedItem);

    // 根据选择的模式更新设置
    if (SelectedItem == TEXT("Fullscreen"))
    {
        // 设置为全屏模式
        GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
    }
    else if (SelectedItem == TEXT("WindowedFullscreen"))
    {
        // 设置为窗口化全屏模式
        GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
    }
    else if (SelectedItem == TEXT("Windowed"))
    {
        // 设置为窗口模式
        GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
    }

    // 应用设置（如果需要的话）
    GameUserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeFramerate()
{
    // 确保 ComboBox 已绑定
    if (!FrameLimitComboBox)
    {
        UE_LOG(LogTemp, Error, TEXT("GlobalIlluminationComboBox is not bound!"));
        return;
    }

    // 清空 ComboBox 的现有选项
    FrameLimitComboBox->ClearOptions();

    // 初始化全局光照选项列表
    FrameLimit.Empty(); // 清空列表（如果之前已经填充过的话）
    FrameLimit.Add(TEXT("30"));
    FrameLimit.Add(TEXT("60"));
    FrameLimit.Add(TEXT("90"));
    FrameLimit.Add(TEXT("120"));
    FrameLimit.Add(TEXT("165"));
    FrameLimit.Add(TEXT("Uncapped"));

    for (const FString& Mode : FrameLimit)
    {
        FrameLimitComboBox->AddOption(Mode);
    }
  
    // 获取当前模式设置（这里你需要一个方法来获取这个设置，可能是从GameUserSettings或其他地方）
    FString FrameLimitMode ;

    float CurrentFrameLimitModeChanged = GameUserSettings->GetFrameRateLimit();
    if (CurrentFrameLimitModeChanged == 30.0f)FrameLimitMode = TEXT("30");
    else if (CurrentFrameLimitModeChanged == 60.0f)FrameLimitMode = TEXT("60");
    else if (CurrentFrameLimitModeChanged == 90.0f)FrameLimitMode = TEXT("90");
    else if (CurrentFrameLimitModeChanged == 120.0f)FrameLimitMode = TEXT("120");
    else if (CurrentFrameLimitModeChanged == 165.0f)FrameLimitMode = TEXT("165");
    else if (CurrentFrameLimitModeChanged == 0.0f)FrameLimitMode = TEXT("Uncapped");

    // 查找当前全屏模式在ComboBox中的索引
    int32 SelectedFrameModeIndex = FrameLimitComboBox->FindOptionIndex(FrameLimitMode);

    if (SelectedFrameModeIndex != INDEX_NONE)
    {
        FrameLimitComboBox->SetSelectedIndex(SelectedFrameModeIndex);
    }
    else
    {
        FrameLimitComboBox->SetSelectedIndex(0); 
    }

    // 绑定选择事件
    FrameLimitComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnFramerateChanged);
}

void UCommonDisplaySettings::OnFramerateChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    // 获取游戏用户设置
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (!UserSettings)
    {
        return;
    }

    // 根据选择的选项更新全局光照设置
    if (SelectedItem == TEXT("30"))
    {
        UserSettings->SetFrameRateLimit(30.0f); 
    }
    else if (SelectedItem == TEXT("60"))
    {
        UserSettings->SetFrameRateLimit(60.0f);
    }
    else if (SelectedItem == TEXT("90"))
    {
        UserSettings->SetFrameRateLimit(90.0f);
    }
    else if (SelectedItem == TEXT("120"))
    {
        UserSettings->SetFrameRateLimit(120.0f);
    }
    else if (SelectedItem == TEXT("165"))
    {
        UserSettings->SetFrameRateLimit(165.0f);
    }
    else if (SelectedItem == TEXT("Uncapped"))
    {
        UserSettings->SetFrameRateLimit(0.0f);
    }

    // 应用设置
    UserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeVSync()
{
    VSyncCheckBox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
    VSyncCheckBox->OnCheckStateChanged.Clear();
    VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UCommonDisplaySettings::OnVSyncChanged);
}

void UCommonDisplaySettings::OnVSyncChanged(bool InIsChecked)
{
    GameUserSettings->SetVSyncEnabled(InIsChecked);
    UE_LOG(LogTemp, Error, TEXT(" Is VSync!"));
    GameUserSettings->ApplySettings(false);
}

void UCommonDisplaySettings::InitializeGlobalIllumination()
{
    FString CurrentMode=GetQualityLevel(&UGameUserSettings::GetGlobalIlluminationQuality);
    InitializeComboBox(GlobalIlluminationComboBox, GlobalIllumination, CurrentMode);
    GlobalIlluminationComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnGlobalIlluminationChanged);
}

void UCommonDisplaySettings::OnGlobalIlluminationChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetGlobalIlluminationQuality);
}

void UCommonDisplaySettings::InitializeAntiAliasing()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetAntiAliasingQuality);
    InitializeComboBox(AntiAliasingComboBox, AntiAliasing, CurrentMode);
    AntiAliasingComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnAntiAliasingChanged);
}

void UCommonDisplaySettings::OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetAntiAliasingQuality);
}

void UCommonDisplaySettings::InitializeReflectionQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetReflectionQuality);
    InitializeComboBox(ReflectionQualityComboBox, ReflectionQuality, CurrentMode);
    ReflectionQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnReflectionQualityChanged);
}

void UCommonDisplaySettings::OnReflectionQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetReflectionQuality);
}

void UCommonDisplaySettings::InitializeTextureQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetTextureQuality);
    InitializeComboBox(TextureQualityComboBox, TextureQuality, CurrentMode);
    TextureQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnTextureQualityChanged);
}

void UCommonDisplaySettings::OnTextureQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetTextureQuality);
}

void UCommonDisplaySettings::InitializeShadowQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetShadowQuality);
    InitializeComboBox(ShadowQualityComboBox, ShadowQuality, CurrentMode);
    ShadowQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnShadowQualityChanged);
}

void UCommonDisplaySettings::OnShadowQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetShadowQuality);
}

void UCommonDisplaySettings::InitializeVisualEffects()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetVisualEffectQuality);
    InitializeComboBox(VisualEffectsComboBox, VisualEffects, CurrentMode);
    VisualEffectsComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnVisualEffectsChanged);
}

void UCommonDisplaySettings::OnVisualEffectsChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetVisualEffectQuality);
}

void UCommonDisplaySettings::InitializePostProcessing()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetPostProcessingQuality);
    InitializeComboBox(PostProcessingComboBox, PostProcessing, CurrentMode);
    PostProcessingComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnPostProcessingChanged);
}

void UCommonDisplaySettings::OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetPostProcessingQuality);
}

void UCommonDisplaySettings::InitializeVegetationQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetFoliageQuality);
    InitializeComboBox(VegetationQualityComboBox, VegetationQuality, CurrentMode);
    VegetationQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnVegetationQualityChanged);
}

void UCommonDisplaySettings::OnVegetationQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetFoliageQuality);
}

void UCommonDisplaySettings::InitializeColorQuality()
{
    FString CurrentMode = GetQualityLevel(&UGameUserSettings::GetShadingQuality);
    InitializeComboBox(ColorQualityComboBox, ColorQuality, CurrentMode);
    ColorQualityComboBox->OnSelectionChanged.AddDynamic(this, &UCommonDisplaySettings::OnColorQualityChanged);
}

void UCommonDisplaySettings::OnColorQualityChanged(FString SelectedItem, ESelectInfo::Type InSelectionType)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    SetQualityLevel(UserSettings, SelectedItem, &UGameUserSettings::SetShadingQuality);
}

void UCommonDisplaySettings::OnSaveButtonClicked()
{
    if (GameUserSettings)
    {
        // 保存当前设置到 INI 文件
        GameUserSettings->SaveSettings();
        UE_LOG(LogTemp, Log, TEXT("Settings saved!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings is null!"));
    }
}

void UCommonDisplaySettings::OnRestoreDefaultsButtonClicked()
{
    if (GameUserSettings)
    {
        GameUserSettings = UGameUserSettings::GetGameUserSettings();
        // 应用分辨率
        GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
        //窗口模式
        GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
        //帧数限制
        GameUserSettings->SetFrameRateLimit(60.0f);
       //垂直同步
        GameUserSettings->SetVSyncEnabled(false);
        //全局光照
        GameUserSettings->SetGlobalIlluminationQuality(2);
        //抗锯齿
        GameUserSettings->SetAntiAliasingQuality(2);
        //反射质量
        GameUserSettings->SetReflectionQuality(2);
        //纹理质量
        GameUserSettings->SetTextureQuality(2);
        //阴影质量
        GameUserSettings->SetShadowQuality(2);
        //视觉效果
        GameUserSettings->SetVisualEffectQuality(2);
        //后期处理
        GameUserSettings->SetPostProcessingQuality(2);
        //植被质量
        GameUserSettings->SetFoliageQuality(2);
        //着色质量
        GameUserSettings->SetShadingQuality(2);
        InitializeResolutionComboBox();//分辨率
        InitializeVSync();//垂直同步
        InitializeFullscreenMode();//窗口模式
        InitializeFramerate();//帧数
        InitializeGlobalIllumination();//全局光照
        InitializeAntiAliasing();//抗锯齿
        InitializeReflectionQuality();//反射质量
        InitializeTextureQuality();//纹理质量
        InitializeShadowQuality();//阴影质量
        InitializeVisualEffects();//视觉效果
        InitializePostProcessing();//后期处理
        InitializeVegetationQuality();//植被质量
        InitializeColorQuality();//着色质量
        GameUserSettings->ApplySettings(false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameUserSettings !"));
    }
}


