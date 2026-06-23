#include <stdlib.h>
#include <string.h>

#include "xaibot_personality.h"
#include "util.h"

// Simple personality assignment and utilities
static xaibot_personality_t g_bot_personality[32];

static const char *g_personality_names[XAIBOT_PERSONALITY_COUNT] = {
    "Toxic Rager",
    "Chill Veteran",
    "Silent Pro",
    "Confused Noob",
    "Funny Troll",
    "Coward Survivor",
    "Tryhard Competitive",
    "Friendly Helper",
    "Sarcastic Bot",
    "Roleplay Soldier",
    "Chaotic Wildcard",
    "Dramatic Cinematic Bot"
};

void XaibotPersonality_Init(void)
{
    // Default deterministic assignment based on index to keep predictable behavior
    for (int i = 0; i < 32; ++i)
    {
        g_bot_personality[i] = (xaibot_personality_t)(i % XAIBOT_PERSONALITY_COUNT);
    }
}

static int get_bot_index(bot_t *pBot)
{
    if (!pBot || !pBot->pEdict) return -1;
    int idx = UTIL_GetBotIndex(pBot->pEdict);
    if (idx < 0 || idx >= 32) return -1;
    return idx;
}

xaibot_personality_t XaibotPersonality_GetForBot(bot_t *pBot)
{
    int idx = get_bot_index(pBot);
    if (idx < 0) return XAIBOT_PERSONALITY_CHILL_VETERAN;
    return g_bot_personality[idx];
}

const char *XaibotPersonality_GetName(xaibot_personality_t p)
{
    if (p < 0 || p >= XAIBOT_PERSONALITY_COUNT) return "Unknown";
    return g_personality_names[p];
}

void XaibotPersonality_RerollAll(void)
{
    for (int i = 0; i < 32; ++i)
    {
        g_bot_personality[i] = (xaibot_personality_t)RANDOM_LONG2(0, XAIBOT_PERSONALITY_COUNT - 1);
    }
}

float XaibotPersonality_GetChatFrequencyModifier(xaibot_personality_t p)
{
    switch (p)
    {
        case XAIBOT_PERSONALITY_TOXIC_RAGER: return 1.4f;
        case XAIBOT_PERSONALITY_CHILL_VETERAN: return 0.6f;
        case XAIBOT_PERSONALITY_SILENT_PRO: return 0.3f;
        case XAIBOT_PERSONALITY_CONFUSED_NOOB: return 1.6f;
        case XAIBOT_PERSONALITY_FUNNY_TROLL: return 1.3f;
        case XAIBOT_PERSONALITY_COWARD_SURVIVOR: return 0.8f;
        case XAIBOT_PERSONALITY_TRYHARD: return 1.0f;
        default: return 1.0f;
    }
}

float XaibotPersonality_GetTauntModifier(xaibot_personality_t p)
{
    switch (p)
    {
        case XAIBOT_PERSONALITY_TOXIC_RAGER: return 1.5f;
        case XAIBOT_PERSONALITY_SILENT_PRO: return 0.3f;
        case XAIBOT_PERSONALITY_CONFUSED_NOOB: return 1.2f;
        case XAIBOT_PERSONALITY_FUNNY_TROLL: return 1.4f;
        default: return 1.0f;
    }
}

// Expose additional helpers by static linkage; prototypes can be added to header if needed in future
