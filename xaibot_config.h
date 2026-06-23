#ifndef XAIBOT_CONFIG_H
#define XAIBOT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Simple XAIBot config API for Phase 1

void XaibotConfig_Init(void);
void XaibotConfig_Reload(void);
int  XaibotConfig_IsEnabled(void); // AI chat overall
const char *XaibotConfig_GetLanguage(void);

// Additional getters
int XaibotConfig_GroqEnabled(void);
int XaibotConfig_BotBanterEnabled(void);
int XaibotConfig_PersonalityEnabled(void);
int XaibotConfig_EmotionEnabled(void);
int XaibotConfig_AtmosphereEnabled(void);
int XaibotConfig_DynamicDifficultyEnabled(void);
int XaibotConfig_GlobalChatCooldown(void);
int XaibotConfig_PerBotChatCooldown(void);
int XaibotConfig_MaxMessagesPerMinute(void);
const char *XaibotConfig_GameplayProfile(void);
const char *XaibotConfig_GroqKeyPath(void);
int XaibotConfig_MenuEnabled(void);
int XaibotConfig_Debug(void);

#ifdef __cplusplus
}
#endif

#endif // XAIBOT_CONFIG_H
