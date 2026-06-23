#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xaibot_config.h"
#include "util.h"

// Simple config implementation for Phase 1

static char g_language[8] = "tr"; // default Turkish
static int g_ai_chat_enabled = 1;
static int g_local_fallback_enabled = 1;
static int g_menu_enabled = 1;
static int g_groq_enabled = 0;
static int g_bot_banter_enabled = 1;
static int g_personality_enabled = 1;
static int g_emotion_enabled = 1;
static int g_atmosphere_enabled = 1;
static int g_dynamic_difficulty_enabled = 0;
static int g_debug = 0;
static int g_global_chat_cooldown = 10; // seconds default
static int g_per_bot_chat_cooldown = 5; // seconds default
static int g_max_messages_per_minute = 10;
static char g_gameplay_profile[32] = "normal";
static char g_groq_key_path[256] = ""; // optional override

static const char * DEFAULT_CFG_PATH = "addons/jk_botti/xaibot.cfg";

static void trim_inplace(char *s)
{
    if (!s) return;
    // trim leading
    char *p = s;
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) p++;
    if (p != s) memmove(s, p, strlen(p)+1);
    // trim trailing
    int len = strlen(s);
    while (len > 0 && (s[len-1] == ' ' || s[len-1] == '\t' || s[len-1] == '\r' || s[len-1] == '\n')) s[--len] = '\0';
}

static void set_default_values(void)
{
    strcpy(g_language, "tr");
    g_ai_chat_enabled = 1;
    g_local_fallback_enabled = 1;
    g_menu_enabled = 1;
    g_groq_enabled = 0;
    g_bot_banter_enabled = 1;
    g_personality_enabled = 1;
    g_emotion_enabled = 1;
    g_atmosphere_enabled = 1;
    g_dynamic_difficulty_enabled = 0;
    g_debug = 0;
    g_global_chat_cooldown = 10;
    g_per_bot_chat_cooldown = 5;
    g_max_messages_per_minute = 10;
    strcpy(g_gameplay_profile, "normal");
    g_groq_key_path[0] = '\0';
}

void XaibotConfig_Init(void)
{
    set_default_values();
    XaibotConfig_Reload();
}

void XaibotConfig_Reload(void)
{
    char path[512];
    const char *cfg = DEFAULT_CFG_PATH;

    // open config file
    UTIL_BuildFileName_N(path, sizeof(path), (char *)cfg, NULL);

    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        // missing config: leave defaults
        if (g_debug)
            UTIL_ConsolePrintf("xaibot: config not found at %s, using defaults\n", path);
        return;
    }

    if (g_debug)
        UTIL_ConsolePrintf("xaibot: loading config %s\n", path);

    char line[512];
    int lineno = 0;

    while (fgets(line, sizeof(line), fp))
    {
        lineno++;
        // strip comments
        char *p = strchr(line, '#');
        if (p) *p = '\0';
        trim_inplace(line);
        if (line[0] == 0) continue;

        // key = value
        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = 0;
        char *k = line;
        char *v = eq + 1;
        trim_inplace(k);
        trim_inplace(v);

        // handle keys
        if (!stricmp(k, "language"))
        {
            if (v[0])
            {
                if (!stricmp(v, "tr") || !stricmp(v, "en"))
                    safe_strcopy(g_language, sizeof(g_language), v);
                else
                    safe_strcopy(g_language, sizeof(g_language), "tr");
            }
        }
        else if (!stricmp(k, "ai_chat_enabled"))
            g_ai_chat_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "local_fallback_enabled"))
            g_local_fallback_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "menu_enabled"))
            g_menu_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "groq_enabled"))
            g_groq_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "bot_banter_enabled"))
            g_bot_banter_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "personality_enabled"))
            g_personality_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "emotion_enabled"))
            g_emotion_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "atmosphere_enabled"))
            g_atmosphere_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "dynamic_difficulty_enabled"))
            g_dynamic_difficulty_enabled = atoi(v) ? 1 : 0;
        else if (!stricmp(k, "global_chat_cooldown"))
            g_global_chat_cooldown = atoi(v);
        else if (!stricmp(k, "per_bot_chat_cooldown"))
            g_per_bot_chat_cooldown = atoi(v);
        else if (!stricmp(k, "max_messages_per_minute"))
            g_max_messages_per_minute = atoi(v);
        else if (!stricmp(k, "gameplay_profile"))
            safe_strcopy(g_gameplay_profile, sizeof(g_gameplay_profile), v);
        else if (!stricmp(k, "groq_key_path"))
            safe_strcopy(g_groq_key_path, sizeof(g_groq_key_path), v);
        else if (!stricmp(k, "debug"))
            g_debug = atoi(v) ? 1 : 0;
        else
        {
            if (g_debug)
                UTIL_ConsolePrintf("xaibot: unknown config key '%s' line %d\n", k, lineno);
        }
    }

    fclose(fp);

    // safety clamps
    if (g_global_chat_cooldown < 0) g_global_chat_cooldown = 0;
    if (g_per_bot_chat_cooldown < 0) g_per_bot_chat_cooldown = 0;
    if (g_max_messages_per_minute < 1) g_max_messages_per_minute = 1;

    // if groq_enabled but no key path specified, leave groq_enabled=0 until key is valid
    if (g_groq_enabled && g_groq_key_path[0] == '\0')
    {
        // don't auto-disable here; groq key loader can validate later. Keep as-is.
    }
}

int XaibotConfig_IsEnabled(void)
{
    return g_ai_chat_enabled;
}

const char *XaibotConfig_GetLanguage(void)
{
    return (const char *)g_language;
}

// Additional getters for other modules (internal use)
int XaibotConfig_GroqEnabled(void) { return g_groq_enabled; }
int XaibotConfig_BotBanterEnabled(void) { return g_bot_banter_enabled; }
int XaibotConfig_PersonalityEnabled(void) { return g_personality_enabled; }
int XaibotConfig_EmotionEnabled(void) { return g_emotion_enabled; }
int XaibotConfig_AtmosphereEnabled(void) { return g_atmosphere_enabled; }
int XaibotConfig_DynamicDifficultyEnabled(void) { return g_dynamic_difficulty_enabled; }
int XaibotConfig_GlobalChatCooldown(void) { return g_global_chat_cooldown; }
int XaibotConfig_PerBotChatCooldown(void) { return g_per_bot_chat_cooldown; }
int XaibotConfig_MaxMessagesPerMinute(void) { return g_max_messages_per_minute; }
const char *XaibotConfig_GameplayProfile(void) { return (const char*)g_gameplay_profile; }
const char *XaibotConfig_GroqKeyPath(void) { return (const char*)g_groq_key_path; }
int XaibotConfig_MenuEnabled(void) { return g_menu_enabled; }
int XaibotConfig_Debug(void) { return g_debug; }

