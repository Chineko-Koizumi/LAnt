#ifndef IPC_HPP
#define IPC_HPP

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif


#include "DrawingAppConstants.hpp"

#include <mutex>
#include <queue>

namespace IPC 
{ 
    PACK(struct Message
    {
        uint8_t dataType;       // indicator for what structure is inside message
        uint8_t message[100];   // table of binary data
    };)

    enum MessageData : uint8_t
    {
        GUI_MESSAGE = 0U
    };


    PACK(struct GUIMessage
    {
        uint8_t dataType;
        uint8_t type; /// type of msg, (0 text update, 1 progressbar float value)
        uint8_t message[99];
    };)

    enum GUIData : uint8_t
    {
        TEXT_UPDATE = 0U,
        PROGRESSBAR_UPDATE,
        COPY_WINDOW_UPDATE
    };

	inline std::queue<Message> _G_MSG_Queue{};
	inline std::mutex _G_MSG_MSGMutex{};
}






#endif