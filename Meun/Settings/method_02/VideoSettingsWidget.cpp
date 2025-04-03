#include"VideoSettingsWidget.h"
#include "GameFramework/GameUserSettings.h"
#include"Kismet/KismetSystemLibrary.h"
void UVideoSettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (!UserSettings) return;

    // �����ȼ�ת��������������Ŀʵ������ƥ�䣩
    auto ConvertScalability = [](int32 EngineValue) -> EQualityLevel {
        return static_cast<EQualityLevel>(FMath::Clamp(EngineValue, 0, 4));
        };

    // ��ʼ����������
    QuickSetQualitySwitcher->SetCurrentLevel(EQualityLevel::Medium); // Ĭ��ֵ
    QuickSetQualitySwitcher->OnOptionChanged.AddDynamic(this, &UVideoSettingsWidget::OnQuickSetChanged);

    // ��ʼ����ѡ��ؼ�
    ShadowQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetShadowQuality()));//��Ӱ����
    VisualEffectsSwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetVisualEffectQuality()));//�Ӿ�Ч��
    PostProcessingSwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetPostProcessingQuality()));//���ڴ���
    FoliageQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetFoliageQuality()));//ֲ������
    ShadingQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetShadingQuality()));//��ɫ����
    GlobalIlluminationQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetGlobalIlluminationQuality()));//ȫ�ֹ���
    AntiAliasingQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetAntiAliasingQuality()));//�����
    ReflectionQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetReflectionQuality()));//��������
    TextureQualitySwitcher->SetCurrentLevel(ConvertScalability(UserSettings->GetTextureQuality()));//��������

    // ��ʼ������ģʽ
    WindowModeDropdown->InitializeWithEnum<EDisplayWindowMode>();
    const EDisplayWindowMode CurrentWindowMode = static_cast<EDisplayWindowMode>(UserSettings->GetFullscreenMode());
    WindowModeDropdown->SetSelectedOption(StaticEnum<EDisplayWindowMode>()->GetDisplayNameTextByValue((int64)CurrentWindowMode).ToString());

    // ��ʼ���ֱ���
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
    for (const FIntPoint& Res : AvailableResolutions)
    {
        CachedResolutionStrings.Add(FString::Printf(TEXT("%d x %d"), Res.X, Res.Y));
    }
    ResolutionDropdown->InitializeWithCustomOptions(CachedResolutionStrings);

    // ���õ�ǰ�ֱ���
    FIntPoint CurrentRes = UserSettings->GetScreenResolution();
    FString CurrentResStr = FString::Printf(TEXT("%d x %d"), CurrentRes.X, CurrentRes.Y);
    ResolutionDropdown->SetSelectedOption(CurrentResStr);

    // ��ʼ��֡��
    TArray<FString> FrameRateOptions = { "30", "60", "90", "120", "144", "Unlimited" };
    FrameRateDropdown->InitializeWithCustomOptions(FrameRateOptions);

    // ���õ�ǰ֡��
    int32 CurrentFrameRate = UserSettings->GetFrameRateLimit();
    FString CurrentFrameRateStr = CurrentFrameRate == 0 ? "Unlimited" : FString::FromInt(CurrentFrameRate);
    FrameRateDropdown->SetSelectedOption(CurrentFrameRateStr);
}



void UVideoSettingsWidget::SaveSettings()
{
    // Ӧ������
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

    // ���洰��ģʽ
    FString WindowModeStr = WindowModeDropdown->GetSelectedOption();
    EDisplayWindowMode WindowMode = static_cast<EDisplayWindowMode>(StaticEnum<EDisplayWindowMode>()->GetValueByNameString(WindowModeStr));
    UserSettings->SetFullscreenMode(static_cast<EWindowMode::Type>(WindowMode));

    // ����ֱ���
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

    // ����֡��
    FString FrameRateStr = FrameRateDropdown->GetSelectedOption();
    int32 NewFrameRate = FrameRateStr == "Unlimited" ? 0 : FCString::Atoi(*FrameRateStr);
    UserSettings->SetFrameRateLimit(NewFrameRate);

    UserSettings->ApplySettings(false);
}

void UVideoSettingsWidget::RestoreDefaults()
{
    // ����ΪĬ��ֵ
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

    // ���ô���ģʽ
    WindowModeDropdown->SetSelectedOption(
        StaticEnum<EDisplayWindowMode>()->GetDisplayNameTextByValue((int64)EWindowMode::Fullscreen).ToString());

    // ���÷ֱ��ʵ�ϵͳ�Ƽ�
    ResolutionDropdown->SetSelectedOption(FString::Printf(TEXT("%d x %d"),1920, 1080));

    // ����֡��Ϊ60
    FrameRateDropdown->SetSelectedOption("60");
   
    SaveSettings();
 
}

void UVideoSettingsWidget::OnQuickSetChanged(EQualityLevel NewLevel)
{
    // ͬ�����л���ѡ��������õĵȼ�
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