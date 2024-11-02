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
        uint8_t messageType;       // indicator for what structure is inside message
        uint8_t message[100U];  // table of binary data
    };)

    enum messageType : uint8_t
    {
        GUI_MESSAGE_TEXT_UPDATE = 0U,
        GUI_MESSAGE_VALUE_UPDATE
    };

    PACK(struct GUIMessageTextUpdate
    {
        const uint8_t messageType = GUI_MESSAGE_TEXT_UPDATE;
        uint8_t textName; 
        uint8_t message[99U];
    };)

    PACK(struct GUIMessageValueUpdate
    {
        const uint8_t messageType = GUI_MESSAGE_VALUE_UPDATE;
        uint8_t valueName; 
        uint8_t message[8U];
    };)

    enum GUIData : uint8_t
    {
        TEXT_UPDATE = 0U,
        PROGRESSBAR_UPDATE,
        COPY_WINDOW_UPDATE
    };

    inline std::queue<Message> _G_MSG_Queue{};
    inline std::mutex _G_MSG_Mutex{};
    inline uint8_t _G_MSG_Universal_Buffer[100U]{};

    static void SendMessege(Message* pMsg) 
    {
        _G_MSG_Mutex.lock();
            _G_MSG_Queue.push(*pMsg);
        _G_MSG_Mutex.unlock();
    }

    static void SendMessege(messageType type, GUIData dataType, void* pData, uint8_t dataSize)
    {
        Message msg;

        msg.messageType = type;
        msg.message[0] = dataType;
        memcpy_s(&msg.message[1], dataSize, pData, dataSize);

        _G_MSG_Mutex.lock();
            _G_MSG_Queue.push(msg);
        _G_MSG_Mutex.unlock();
    }

}

#endif