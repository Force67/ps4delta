#pragma once

// Copyright (C) Force67

// This file was generated on 08/12/2019

#include "../../vmodlinker.h"

int PS4ABI sceAppMessagingClearEventFlag();
int PS4ABI sceAppMessagingReceiveMsg();
int PS4ABI sceAppMessagingSendMsg();
int PS4ABI sceAppMessagingSendMsgToShellCore();
int PS4ABI sceAppMessagingSendMsgToShellUI();
int PS4ABI sceAppMessagingSetEventFlag();
int PS4ABI sceAppMessagingTryGetEventFlag();
int PS4ABI sceAppMessagingTryReceiveMsg();
int PS4ABI unk_yOiZq_9_ZMQ();
int PS4ABI unk_90unWbnI0qE();
int PS4ABI sceLncUtilAcquireCpuBudgetOfExtraAudioDevices();
int PS4ABI sceLncUtilAcquireCpuBudgetOfImeForBigApp();
int PS4ABI sceLncUtilAcquireCpuBudgetOfInGameStore();
int PS4ABI sceLncUtilActivateCdlg();
int PS4ABI sceLncUtilAddLocalProcess();
int PS4ABI sceLncUtilBlockAppSuspend();
int PS4ABI sceLncUtilBlockingGetEventForDaemon();
int PS4ABI sceLncUtilContinueApp();
int PS4ABI sceLncUtilCrashSyscore();
int PS4ABI sceLncUtilDeactivateCdlg();
int PS4ABI sceLncUtilDeclareReadyForSuspend();
int PS4ABI sceLncUtilDisableSuspendNotification();
int PS4ABI sceLncUtilEnableSuspendNotification();
int PS4ABI sceLncUtilFinishSpecialResume();
int PS4ABI sceLncUtilForceKillApp();
int PS4ABI sceLncUtilForceKillLocalProcess();
int PS4ABI sceLncUtilGetApp0DirPath();
int PS4ABI sceLncUtilGetAppCategory();
int PS4ABI sceLncUtilGetAppFocusedAppStatus();
int PS4ABI sceLncUtilGetAppId();
int PS4ABI sceLncUtilGetAppIdOfBigApp();
int PS4ABI sceLncUtilGetAppIdOfMiniApp();
int PS4ABI sceLncUtilGetAppLaunchedUser();
int PS4ABI sceLncUtilGetAppStatus();
int PS4ABI sceLncUtilGetAppStatusListForShellUIReboot();
int PS4ABI sceLncUtilGetAppTitleId();
int PS4ABI sceLncUtilGetAppType();
int PS4ABI sceLncUtilGetCdlgExec();
int PS4ABI sceLncUtilGetCoredumpState();
int PS4ABI sceLncUtilGetDbgExecutablePath();
int PS4ABI sceLncUtilGetEventForDaemon();
int PS4ABI sceLncUtilGetEventForShellUI();
int PS4ABI sceLncUtilGetLocalProcessStatusList();
int PS4ABI sceLncUtilGetResultKillApp();
int PS4ABI sceLncUtilGetResultLaunchAppByTitleId();
int PS4ABI sceLncUtilInitialize();
int PS4ABI sceLncUtilIsActiveCdlg();
int PS4ABI sceLncUtilIsAppSuspended();
int PS4ABI sceLncUtilIsCpuBudgetOfExtraAudioDevicesAvailable();
int PS4ABI sceLncUtilIsPs2Emu();
int PS4ABI sceLncUtilIsShellUiFgAndGameBgCpuMode();
int PS4ABI sceLncUtilKickCoredumpOnlyProcMem();
int PS4ABI sceLncUtilKillApp();
int PS4ABI sceLncUtilKillAppWithReason();
int PS4ABI sceLncUtilKillLocalProcess();
int PS4ABI sceLncUtilLaunchApp();
int PS4ABI sceLncUtilLoadExec();
int PS4ABI sceLncUtilNotifyCoredumpRequestEnd();
int PS4ABI sceLncUtilNotifyCoredumpRequestProgress();
int PS4ABI sceLncUtilNotifyVshReady();
int PS4ABI sceLncUtilRaiseException();
int PS4ABI sceLncUtilRaiseExceptionLocalProcess();
int PS4ABI sceLncUtilRegisterCdlgSharedMemoryName();
int PS4ABI sceLncUtilRegisterDaemon();
int PS4ABI sceLncUtilRegisterShellUI();
int PS4ABI sceLncUtilReleaseCpuBudgetOfExtraAudioDevices();
int PS4ABI sceLncUtilReleaseCpuBudgetOfImeForBigApp();
int PS4ABI sceLncUtilReleaseCpuBudgetOfInGameStore();
int PS4ABI sceLncUtilResumeApp();
int PS4ABI sceLncUtilResumeLocalProcess();
int PS4ABI sceLncUtilSetAppFocus();
int PS4ABI sceLncUtilSetCdlgExec();
int PS4ABI sceLncUtilSetControllerFocus();
int PS4ABI sceLncUtilSetControllerFocusPermission();
int PS4ABI sceLncUtilStartKillApp();
int PS4ABI sceLncUtilStartLaunchAppByTitleId();
int PS4ABI sceLncUtilSuspendApp();
int PS4ABI sceLncUtilSuspendBackgroundApp();
int PS4ABI sceLncUtilSuspendLocalProcess();
int PS4ABI sceLncUtilSystemSuspend();
int PS4ABI sceLncUtilTerminate();
int PS4ABI sceLncUtilTryBlockAppSuspend();
int PS4ABI sceLncUtilUnblockAppSuspend();
int PS4ABI sceLncUtilUnregisterCdlgSharedMemoryName();
int PS4ABI sceLncUtilUnregisterDaemon();
int PS4ABI sceLncUtilGetParentSocket();
int PS4ABI sceLncUtilUnregisterShellUI();
int PS4ABI sceShellCoreUtilAcquireBgmCpuBudget();
int PS4ABI sceShellCoreUtilActivateAbort();
int PS4ABI sceShellCoreUtilActivateGetStatus();
int PS4ABI sceShellCoreUtilActivateInit();
int PS4ABI sceShellCoreUtilActivateIsActivated();
int PS4ABI sceShellCoreUtilActivateStart();
int PS4ABI sceShellCoreUtilActivateStartAsync();
int PS4ABI sceShellCoreUtilActivateTerm();
int PS4ABI sceShellCoreUtilChangeRunLevel();
int PS4ABI sceShellCoreUtilCheckerAbort();
int PS4ABI sceShellCoreUtilCleanupCrashReport();
int PS4ABI sceShellCoreUtilClearAppData();
int PS4ABI sceShellCoreUtilCrashReportRequestCancel();
int PS4ABI sceShellCoreUtilDeclareBeginOfExternalStorageAppMove();
int PS4ABI sceShellCoreUtilDeclareEndOfExternalStorageAppMove();
int PS4ABI sceShellCoreUtilDeleteDiscInstalledTitleWorkaroundFile();
int PS4ABI sceShellCoreUtilDeleteDownloadedHidConfigFile();
int PS4ABI sceShellCoreUtilDeleteDownloadedNetEvConfigFile();
int PS4ABI sceShellCoreUtilDeleteDownloadedTitleWorkaroundFile();
int PS4ABI sceShellCoreUtilDeleteSmrHddDummyData();
int PS4ABI sceShellCoreUtilDoFsck();
int PS4ABI sceShellCoreUtilDownloadHidConfigFileFromServer();
int PS4ABI sceShellCoreUtilDownloadNetEvConfigFileFromServer();
int PS4ABI sceShellCoreUtilDownloadTitleWorkaroundFileFromServer();
int PS4ABI sceShellCoreUtilEnterPowerLockSection();
int PS4ABI sceShellCoreUtilExecuteCrashReport();
int PS4ABI sceShellCoreUtilExfatFormatExternalHdd();
int PS4ABI sceShellCoreUtilExitMiniApp();
int PS4ABI sceShellCoreUtilExitMiniAppWithValue();
int PS4ABI sceShellCoreUtilFillUpSpaceOnSmrHdd();
int PS4ABI sceShellCoreUtilFireCrashReport();
int PS4ABI sceShellCoreUtilFormatExternalHdd();
int PS4ABI sceShellCoreUtilFormatHddForRestore();
int PS4ABI sceShellCoreUtilFreeUpSpaceOnSmrHdd();
int PS4ABI sceShellCoreUtilGetAppData();
int PS4ABI sceShellCoreUtilGetAppEnableTTS();
int PS4ABI sceShellCoreUtilGetAppEnterButtonAssign();
int PS4ABI sceShellCoreUtilGetAppLaunchedParamInt();
int PS4ABI sceShellCoreUtilGetAppLaunchedParamIntByBudgetType();
int PS4ABI sceShellCoreUtilGetAppLaunchedParamString();
int PS4ABI sceShellCoreUtilGetAppLaunchedParamStringByBudgetType();
int PS4ABI sceShellCoreUtilGetAutoPowerDownRemainingSeconds();
int PS4ABI sceShellCoreUtilGetBasicProductShape();
int PS4ABI sceShellCoreUtilGetCheckerString();
int PS4ABI sceShellCoreUtilGetCheckerStringEx();
int PS4ABI sceShellCoreUtilGetCrashReportCoreFileSetSize();
int PS4ABI sceShellCoreUtilGetCrashReportFilterInfoStart();
int PS4ABI sceShellCoreUtilGetCrashReportInfoForBoot();
int PS4ABI sceShellCoreUtilGetCrashReportInfoForBootStart();
int PS4ABI sceShellCoreUtilGetCrashReportInfoStart();
int PS4ABI sceShellCoreutilGetCrashReportProcessInformation();
int PS4ABI sceShellCoreUtilGetCrashReportResult();
int PS4ABI sceShellCoreUtilGetCrashReportStatus();
int PS4ABI sceShellCoreUtilGetCrashReportUploadStatus();
int PS4ABI sceShellCoreUtilGetDeviceIndexBehavior();
int PS4ABI sceShellCoreUtilGetEffectiveTotalSizeOfUserPartition();
int PS4ABI sceShellCoreUtilGetFreeSizeOfAvContentsTmp();
int PS4ABI sceShellCoreUtilGetFreeSizeOfUserPartition();
int PS4ABI sceShellCoreUtilGetFsckProgress();
int PS4ABI sceShellCoreUtilGetGameLiveStreamingStatus();
int PS4ABI sceShellCoreUtilGetGnmCompositorOnScreenProfilerFlag();
int PS4ABI sceShellCoreUtilGetHidConfigFileInfoString();
int PS4ABI sceShellCoreUtilGetHidConfigFileString();
int PS4ABI sceShellCoreUtilGetHidConfigName();
int PS4ABI sceShellCoreUtilGetHidConfigNum();
int PS4ABI sceShellCoreUtilGetImposeMenuFlagForPs2Emu();
int PS4ABI sceShellCoreUtilGetManifestFileStatus();
int PS4ABI sceShellCoreUtilGetNeedSizeOfAppContent();
int PS4ABI sceShellCoreUtilGetNetEvConfigFileInfoString();
int PS4ABI sceShellCoreUtilGetOptimizationStatus();
int PS4ABI sceShellCoreUtilGetOutOfVrPlayZoneWarning();
int PS4ABI sceShellCoreUtilGetPlatformPrivacySetting();
int PS4ABI sceShellCoreUtilGetProgressOfFormatExternalHdd();
int PS4ABI sceShellCoreUtilGetProgressOfFsck();
int PS4ABI sceShellCoreUtilGetPsStoreIconLayout();
int PS4ABI sceShellCoreUtilGetPsStoreIconState();
int PS4ABI sceShellCoreUtilGetRemotePlayStatus();
int PS4ABI sceShellCoreUtilGetRunLevel();
int PS4ABI sceShellCoreUtilGetSharePlayStatus();
int PS4ABI sceShellCoreUtilGetSmrHddInfoString();
int PS4ABI sceShellCoreUtilGetSocialScreenStatus();
int PS4ABI sceShellCoreUtilGetSplashScreenState();
int PS4ABI sceShellCoreUtilGetSupportSiteURL();
int PS4ABI sceShellCoreUtilGetSuspendConfirmationDialogFlag();
int PS4ABI sceShellCoreUtilGetSystemBGState();
int PS4ABI sceShellCoreUtilGetSystemBGWaveColor();
int PS4ABI sceShellCoreUtilGetSystemBGWaveState();
int PS4ABI sceShellCoreUtilGetTitleWorkaroundFileInfoString();
int PS4ABI sceShellCoreUtilGetTitleWorkaroundFileString();
int PS4ABI sceShellCoreUtilGetUIStatus();
int PS4ABI sceShellCoreUtilGetUserFocus();
int PS4ABI sceShellCoreUtilGetUserIdOfMorpheusUser();
int PS4ABI sceShellCoreUtilGoBackToKratosCurrentSessionGame();
int PS4ABI sceShellCoreUtilHideBlocksFromUser();
int PS4ABI sceShellCoreUtilIncrementVersionNumberOfCameraCalibrationData();
int PS4ABI sceShellCoreUtilIsAppLaunched();
int PS4ABI sceShellCoreUtilIsBgmCpuBudgetAcquired();
int PS4ABI sceShellCoreUtilIsBgmPlaying();
int PS4ABI sceShellCoreUtilIsExternalStorageAppMoveInProgress();
int PS4ABI sceShellCoreUtilIsImposeScreenOverlaid();
int PS4ABI sceShellCoreUtilIsInSystemSuspendBlackList();
int PS4ABI sceShellCoreUtilIsInternalKratosUser();
int PS4ABI sceShellCoreUtilIsNeededCrashReport();
int PS4ABI sceShellCoreUtilIsPowerSaveAlertRequested();
int PS4ABI sceShellCoreUtilIsScreenSaverOn();
int PS4ABI sceShellCoreUtilIsShowCrashReport();
int PS4ABI sceShellCoreUtilIsTemperatureDanger();
int PS4ABI sceShellCoreUtilIsTitleWorkaroundEnabled();
int PS4ABI sceShellCoreUtilIsUsbMassStorageMounted();
int PS4ABI sceShellCoreUtilLaunchByUri();
int PS4ABI sceShellCoreUtilLeavePowerLockSection();
int PS4ABI sceShellCoreUtilLog();
int PS4ABI sceShellCoreUtilMakeManifestFile();
int PS4ABI sceShellCoreUtilMountAppRight();
int PS4ABI sceShellCoreUtilMountDownloadDataForShellUI();
int PS4ABI sceShellCoreUtilMountHddForBackup();
int PS4ABI sceShellCoreUtilMountHddForRestore();
int PS4ABI sceShellCoreUtilNavigateToAnotherApp();
int PS4ABI sceShellCoreUtilNavigateToGoHome();
int PS4ABI sceShellCoreUtilNavigateToLaunchedApp();
int PS4ABI sceShellCoreUtilNotifyFsReadError();
int PS4ABI sceShellCoreUtilNotifyYouTubeAccountLinkStatusChanged();
int PS4ABI sceShellCoreUtilPfAuthClientConsoleTokenClearCache();
int PS4ABI sceShellCoreUtilPostActivityForPsNow();
int PS4ABI sceShellCoreUtilPostErrorLog();
int PS4ABI sceShellCoreUtilPostLaunchConfirmResult();
int PS4ABI sceShellCoreUtilPostPsmEventToShellUI();
int PS4ABI sceShellCoreUtilPreNotifyOfGameLiveStreaming();
int PS4ABI sceShellCoreUtilPreNotifyOfRemotePlay();
int PS4ABI sceShellCoreUtilPreNotifyOfSharePlay();
int PS4ABI sceShellCoreUtilReleaseBgmCpuBudget();
int PS4ABI sceShellCoreUtilReportSessionErrorToGaikaiController();
int PS4ABI sceShellCoreUtilReportUnexpectedFatalErrorToSystemTelemetry();
int PS4ABI sceShellCoreUtilRequestEjectDevice();
int PS4ABI sceShellCoreUtilRequestRebootApp();
int PS4ABI sceShellCoreUtilRequestShutdown();
int PS4ABI sceShellCoreUtilResetAutoPowerDownTimer();
int PS4ABI sceShellCoreUtilResetBgdcConfig();
int PS4ABI sceShellCoreUtilSetAppData();
int PS4ABI sceShellCoreUtilSetDeviceIndexBehavior();
int PS4ABI sceShellCoreUtilSetGameLiveStreamingStatus();
int PS4ABI sceShellCoreUtilSetGnmCompositorOnScreenProfilerFlag();
int PS4ABI sceShellCoreUtilSetImposeStatusFlag();
int PS4ABI sceShellCoreUtilSetPsStoreIconLayout();
int PS4ABI sceShellCoreUtilSetPsStoreIconState();
int PS4ABI sceShellCoreUtilSetRemotePlayStatus();
int PS4ABI sceShellCoreUtilSetSharePlayStatus();
int PS4ABI sceShellCoreUtilSetSkipUpdateCheck();
int PS4ABI sceShellCoreUtilSetSocialScreenStatus();
int PS4ABI sceShellCoreUtilSetSplashScreenState();
int PS4ABI sceShellCoreUtilSetSystemBGState();
int PS4ABI sceShellCoreUtilSetSystemBGWaveColor();
int PS4ABI sceShellCoreUtilSetSystemBGWaveState();
int PS4ABI sceShellCoreUtilSetUIStatus();
int PS4ABI sceShellCoreUtilSetUserFocus();
int PS4ABI sceShellCoreUtilShowErrorDialog();
int PS4ABI sceShellCoreUtilShowPsUnderLockIndicator();
int PS4ABI sceShellCoreUtilSignalUserInput();
int PS4ABI sceShellCoreUtilStartOptimization();
int PS4ABI sceShellCoreUtilStartPsNowGame();
int PS4ABI sceShellCoreUtilStopOptimization();
int PS4ABI sceShellCoreUtilStopPsNowGame();
int PS4ABI sceShellCoreUtilTestBusTransferSpeed();
int PS4ABI sceShellCoreUtilTurnOffScreenSaver();
int PS4ABI sceShellCoreUtilUnmountAppRight();
int PS4ABI sceShellCoreUtilUnmountDownloadDataForShellUI();
int PS4ABI sceShellCoreUtilUnmountHddForBackup();
int PS4ABI sceShellCoreUtilUnmountHddForRestore();
int PS4ABI sceShellCoreUtilWriteSmrHddDummyData();
int PS4ABI sceShellCoreUtilIsRemotePlayCpuBudgetAcquired();
int PS4ABI sceShellCoreUtilTickHeartBeat();
int PS4ABI unk_Hlylpx_n8Cg();
int PS4ABI sceShellCoreUtilIsGameLiveStreamingOnAir();
int PS4ABI sceShellCoreUtilSetGpuLoadEmulationMode();
int PS4ABI sceShellCoreUtilGetCloudClientStatus();
int PS4ABI sceShellCoreUtilIsEyeToEyeDistanceAdjusted();
int PS4ABI sceShellCoreUtilClearPsnAccountInfo();
int PS4ABI sceShellCoreUtilAcquireRemotePlayCpuBudget();
int PS4ABI sceShellCoreUtilAccessibilityZoomUnlock();
int PS4ABI sceShellCoreUtilNotificationRequestedForIDU();
int PS4ABI sceShellCoreUtilGetVersionNumberOfCameraCalibrationData();
int PS4ABI sceShellCoreUtilTriggerPapcRecalculation();
int PS4ABI sceShellCoreUtilNotificationCancelForIDU();
int PS4ABI sceShellCoreUtilGetAppLaunchTypeInfo();
int PS4ABI sceShellCoreUtilSetGameLiveStreamingOnAirFlag();
int PS4ABI unk_bUNkT3XDg0Y();
int PS4ABI sceShellCoreUtilGetGpuLoadEmulationMode();
int PS4ABI sceShellCoreUtilReleaseRemotePlayCpuBudget();
int PS4ABI sceShellCoreUtilShowErrorDialogWithFormatArgs();
int PS4ABI unk_c5_4Scso9EU();
int PS4ABI sceShellCoreUtilNotifyPsnAccountInfoReceived();
int PS4ABI sceShellCoreUtilAcquireSharePlayCpuBudget();
int PS4ABI sceShellCoreUtilIsSharePlayCpuBudgetAcquired();
int PS4ABI sceShellCoreUtilReleaseSharePlayCpuBudget();
int PS4ABI sceShellCoreUtilShowCriticalErrorDialog();
int PS4ABI sceShellCoreUtilGetPapcGamePcl();
int PS4ABI sceShellCoreUtilGetGpuLoadEmulationModeByAppId();
int PS4ABI sceShellCoreUtilNotifyBgmCoreTermination();
int PS4ABI unk_sgYo_zXHQRE();
int PS4ABI sceShellCoreUtilGetDeviceIndexBehaviorWithTimeout();
int PS4ABI sceShellCoreUtilIsBgmCpuBudgetAvailable();
int PS4ABI sceShellCoreUtilChangeToStaffModeForIDU();
int PS4ABI sceShellCoreUtilActivateRecordActivation();
int PS4ABI sceShellCoreUtilGetPsnAccountInfo();
int PS4ABI sceShellCoreUtilShowErrorDialogWithParam();
int PS4ABI sceShellCoreUtilSetGpuLoadEmulationModeByAppId();
int PS4ABI sceShellCoreUtilIsAccessibilityZoomLocked();
int PS4ABI sceShellCoreUtilSetIDUMode();
int PS4ABI sceShellCoreUtilIsKilledOrSuspendedByLogout();
int PS4ABI sceShellCoreUtilAccessibilityZoomLock();
int PS4ABI sceShellCoreUtilGetRegion();
int PS4ABI sceShellCoreUtilGetIDUMode();
int PS4ABI sceShellCoreUtilRequestCameraCalibration();
int PS4ABI sceSystemServiceAddLocalProcess();
int PS4ABI sceSystemServiceAddLocalProcessForPsmKit();
int PS4ABI sceSystemServiceChangeCpuClock();
int PS4ABI sceSystemServiceChangeGpuClock();
int PS4ABI sceSystemServiceChangeMemoryClockToDefault();
int PS4ABI sceSystemServiceChangeMemoryClockToMultiMediaMode();
int PS4ABI sceSystemServiceChangeNumberOfGpuCu();
int PS4ABI sceSystemServiceDisableMusicPlayer();
int PS4ABI sceSystemServiceDisablePersonalEyeToEyeDistanceSetting();
int PS4ABI sceSystemServiceDisableSuspendConfirmationDialog();
int PS4ABI sceSystemServiceEnablePersonalEyeToEyeDistanceSetting();
int PS4ABI sceSystemServiceEnableSuspendConfirmationDialog();
int PS4ABI sceSystemServiceGetAppFocusedAppStatus();
int PS4ABI sceSystemServiceGetAppIdOfBigApp();
int PS4ABI sceSystemServiceGetAppIdOfMiniApp();
int PS4ABI sceSystemServiceGetAppStatus();
int PS4ABI sceSystemServiceGetAppType();
int PS4ABI sceSystemServiceGetDisplaySafeAreaInfo();
int PS4ABI sceSystemServiceGetEventForDaemon();
int PS4ABI sceSystemServiceGetGpuLoadEmulationMode();
int PS4ABI sceSystemServiceGetLocalProcessStatusList();
int PS4ABI sceSystemServiceGetPSButtonEvent();
int PS4ABI sceSystemServiceGetStatus();
int PS4ABI sceSystemServiceGetTitleWorkaroundInfo();
int PS4ABI sceSystemServiceHideSplashScreen();
int PS4ABI sceSystemServiceIsAppSuspended();
int PS4ABI sceSystemServiceIsScreenSaverOn();
int PS4ABI sceSystemServiceIsShellUiFgAndGameBgCpuMode();
int PS4ABI sceSystemServiceKillApp();
int PS4ABI sceSystemServiceKillLocalProcess();
int PS4ABI sceSystemServiceKillLocalProcessForPsmKit();
int PS4ABI sceSystemServiceLaunchApp();
int PS4ABI sceSystemServiceLaunchEventDetails();
int PS4ABI sceSystemServiceLaunchWebBrowser();
int PS4ABI sceSystemServiceLoadExec();
int PS4ABI sceSystemServiceNavigateToAnotherApp();
int PS4ABI sceSystemServiceNavigateToGoHome();
int PS4ABI sceSystemServiceParamGetInt();
int PS4ABI sceSystemServiceParamGetString();
int PS4ABI sceSystemServicePowerTick();
int PS4ABI sceSystemServiceRaiseExceptionLocalProcess();
int PS4ABI sceSystemServiceReceiveEvent();
int PS4ABI sceSystemServiceReenableMusicPlayer();
int PS4ABI sceSystemServiceRegisterDaemon();
int PS4ABI sceSystemServiceReportAbnormalTermination();
int PS4ABI sceSystemServiceResumeLocalProcess();
int PS4ABI sceSystemServiceSetControllerFocusPermission();
int PS4ABI sceSystemServiceSetGpuLoadEmulationMode();
int PS4ABI sceSystemServiceShowDisplaySafeAreaSettings();
int PS4ABI sceSystemServiceSuspendBackgroundApp();
int PS4ABI sceSystemServiceSuspendLocalProcess();
int PS4ABI sceSystemServiceTurnOffScreenSaver();
int PS4ABI sceSystemServiceChangeUvdClock();
int PS4ABI sceSystemServiceChangeAcpClock();
int PS4ABI sceSystemServiceChangeMemoryClock();
int PS4ABI sceSystemServiceChangeMemoryClockToBaseMode();
int PS4ABI sceSystemServiceReleaseFb0();
int PS4ABI sceSystemServiceIsEyeToEyeDistanceAdjusted();
int PS4ABI sceSystemServiceRequestToChangeRenderingMode();
int PS4ABI sceSystemServiceIsBgmPlaying();
int PS4ABI sceSystemServiceGetParentSocket();
int PS4ABI sceSystemServiceSetOutOfVrPlayAreaFlag();
int PS4ABI sceSystemServiceNavigateToGoBackWithValue();
int PS4ABI sceSystemServiceNavigateToGoBack();
int PS4ABI sceSystemServiceChangeVceClock();
int PS4ABI sceSystemServiceShowEyeToEyeDistanceSetting();
int PS4ABI sceSystemServiceGetRenderingMode();
int PS4ABI unk_kDGjRMtUDxo();
int PS4ABI sceSystemServiceChangeSamuClock();
int PS4ABI unk_qdTPJWjquDc();
int PS4ABI sceSystemServiceSetOutOfVrPlayZoneWarning();
int PS4ABI sceSystemServiceGetVersionNumberOfCameraCalibrationData();
int PS4ABI sceSystemServiceLaunchTournamentsTeamProfile();
int PS4ABI sceSystemServiceShowControllerSettings();
int PS4ABI sceSystemServiceTickVideoPlayback();
int PS4ABI sceSystemServiceAcquireFb0();
int PS4ABI sceSystemServiceRequestCameraCalibration();
int PS4ABI sceSystemServiceGetParentSocketForPsmKit();
int PS4ABI sceSystemServiceAddLocalProcessForJvm();
int PS4ABI sceSystemServiceGetParentSocketForJvm();
int PS4ABI sceSystemServiceKillLocalProcessForJvm();
int PS4ABI sceSystemServiceActivateHevcGetStatus();
int PS4ABI sceSystemServiceActivateHevcInit();
int PS4ABI sceSystemServiceActivateHevcStart();
int PS4ABI sceSystemServiceActivateHevcTerm();
int PS4ABI sceSystemServiceActivateHevcAbort();
int PS4ABI sceSystemServiceActivateHevcIsActivated();
int PS4ABI sceSystemServiceActivateHevc();
int PS4ABI sceSystemServiceActivateHevcSoftGetStatus();
int PS4ABI sceSystemServiceActivateHevcSoftStart();
int PS4ABI sceSystemServiceActivateHevcSoftTerm();
int PS4ABI sceSystemServiceActivateHevcSoftIsActivated();
int PS4ABI sceSystemServiceActivateHevcSoft();
int PS4ABI sceSystemServiceActivateHevcSoftAbort();
int PS4ABI sceSystemServiceActivateHevcSoftInit();
int PS4ABI sceSystemServiceActivateMpeg2Abort();
int PS4ABI sceSystemServiceActivateMpeg2Term();
int PS4ABI sceSystemServiceActivateMpeg2Init();
int PS4ABI sceSystemServiceActivateMpeg2GetStatus();
int PS4ABI sceSystemServiceActivateMpeg2IsActivated();
int PS4ABI sceSystemServiceActivateMpeg2Start();
int PS4ABI sceSystemServiceInvokeAppLaunchLink();
int PS4ABI sceSystemServiceShowClosedCaptionAdvancedSettings();
int PS4ABI sceSystemServiceShowClosedCaptionSettings();
int PS4ABI sceSystemServiceGetDbgExecutablePath();
int PS4ABI sceSystemServiceDisablePersonalEyeToEyeDistanceSetting();
int PS4ABI sceSystemServiceEnablePersonalEyeToEyeDistanceSetting();
int PS4ABI sceSystemServiceInitializeForShellCore();
int PS4ABI unk_fBGD_HNimSk();
int PS4ABI sceSystemServiceReenablePartyVoice();
int PS4ABI sceSystemServiceDisablePartyVoice();
int PS4ABI sceSystemServiceGetPlatformPrivacySetting();
int PS4ABI sceSystemServiceGetPlatformPrivacyDefinitionVersion();
int PS4ABI sceSystemServiceGetPlatformPrivacyDefinitionData();
int PS4ABI sceSystemServiceRequestPowerOff();
int PS4ABI sceSystemServiceRequestReboot();
int PS4ABI sceSystemServiceSetPowerSaveLevel();
int PS4ABI sceSystemServiceAddLocalProcessForPs2Emu();
int PS4ABI sceSystemServiceKillLocalProcessForPs2Emu();
int PS4ABI sceSystemServiceShowImposeMenuForPs2Emu();
int PS4ABI sceSystemServiceGetParentSocketForPs2Emu();
int PS4ABI sceSystemServiceAddLocalProcessForPsmKit();
int PS4ABI sceSystemServiceKillLocalProcessForPsmKit();
int PS4ABI sceSystemServiceGetParentSocketForPsmKit();
int PS4ABI sceSystemServiceLaunchStore();
int PS4ABI sceSystemServiceDeclareReadyForSuspend();
int PS4ABI sceSystemServiceDisableSuspendNotification();
int PS4ABI sceSystemServiceEnableSuspendNotification();
int PS4ABI sceSystemServiceTelemetrySetData();
int PS4ABI sceSystemServiceLaunchTournamentList();
int PS4ABI unk_xn_HgPW29x4();
int PS4ABI sceSystemServiceLaunchUdsApp();
int PS4ABI sceSystemServiceLoadExecVideoServiceWebApp();
int PS4ABI sceSystemServiceReenableVoiceRecognition();
int PS4ABI sceSystemServiceDisableVoiceRecognition();
int PS4ABI sceSystemServiceLaunchWebApp();
int PS4ABI unk_uElcdmhh_c8();
int PS4ABI sceSystemServiceLaunchWebBrowser();
int PS4ABI unk_axzblV8OvWU();
int PS4ABI unk_y16IXiJfafA();
int PS4ABI sceSystemStateMgrCancelShutdownTimer();
int PS4ABI sceSystemStateMgrEnterMediaPlaybackMode();
int PS4ABI sceSystemStateMgrEnterStandby();
int PS4ABI sceSystemStateMgrExtendShutdownTimer();
int PS4ABI sceSystemStateMgrExtendShutdownTimerForPostAutoUpdateProcess();
int PS4ABI sceSystemStateMgrGetCurrentState();
int PS4ABI sceSystemStateMgrGetTriggerCode();
int PS4ABI sceSystemStateMgrIsBdDriveReady();
int PS4ABI sceSystemStateMgrIsShellUIShutdownInProgress();
int PS4ABI sceSystemStateMgrIsStandbyModeEnabled();
int PS4ABI sceSystemStateMgrLeaveMediaPlaybackMode();
int PS4ABI sceSystemStateMgrNotifySystemSuspendResumeProgress();
int PS4ABI sceSystemStateMgrReboot();
int PS4ABI sceSystemStateMgrSendCecOneTouchPlayCommand();
int PS4ABI sceSystemStateMgrStartRebootTimer();
int PS4ABI sceSystemStateMgrStartShutdownTimer();
int PS4ABI sceSystemStateMgrStartStadbyTimer();
int PS4ABI sceSystemStateMgrStartVshAutoUpdateTimer();
int PS4ABI sceSystemStateMgrTickPartyChat();
int PS4ABI sceSystemStateMgrTurnOff();
int PS4ABI sceSystemStateMgrVshAutoUpdate();
int PS4ABI sceSystemStateMgrWaitVshAutoUpdateVerifyDone();
int PS4ABI sceSystemStateMgrWakeUp();
int PS4ABI unk_ifJiF5witJ4();
int PS4ABI unk_rIqPq0oWlrg();
int PS4ABI sceSystemStateMgrIsGpuPerformanceNormal();
int PS4ABI sceSystemStateMgrTickMusicPlayback();