#ifndef XAIBOT_PERSONALITY_H
#define XAIBOT_PERSONALITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bot.h"

typedef enum {
    XAIBOT_PERSONALITY_TOXIC_RAGER = 0,
    XAIBOT_PERSONALITY_CHILL_VETERAN,
    XAIBOT_PERSONALITY_SILENT_PRO,
    XAIBOT_PERSONALITY_CONFUSED_NOOB,
    XAIBOT_PERSONALITY_FUNNY_TROLL,
    XAIBOT_PERSONALITY_COWARD_SURVIVOR,
    XAIBOT_PERSONALITY_TRYHARD,
    XAIBOT_PERSONALITY_FRIENDLY_HELPER,
    XAIBOT_PERSONALITY_SARCASTIC,
    XAIBOT_PERSONALITY_ROLEPLAY_SOLDIER,
    XAIBOT_PERSONALITY_CHAOTIC_WILDCARD,
    XAIBOT_PERSONALITY_DRAMATIC_CINEMATIC,
    XAIBOT_PERSONALITY_COUNT
} xaibot_personality_t;

void XaibotPersonality_Init(void);
xaibot_personality_t XaibotPersonality_GetForBot(bot_t *pBot);
const char *XaibotPersonality_GetName(xaibot_personality_t p);
void XaibotPersonality_RerollAll(void);

#ifdef __cplusplus
}
#endif

#endif // XAIBOT_PERSONALITY_H
