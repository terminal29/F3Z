#include <entity/component/Component.h>

std::unordered_map<MessageType, std::string> MessageTypeNames = {
    {MessageType::MSG_UPDATE, "MSG_UPDATE"},
    {MessageType::MSG_RENDER_FOREGROUND, "MSG_RENDER_FOREGROUND"},
    {MessageType::MSG_RENDER_MID, "MSG_RENDER_MID"},
    {MessageType::MSG_RENDER_BACKGROUND, "MSG_RENDER_BACKGROUND"}};