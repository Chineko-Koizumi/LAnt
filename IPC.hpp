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
        uint16_t valueName;
        uint8_t message[100U];  // table of binary data
    };)

    enum messageType : uint8_t
    {
        GUI_MESSAGE_TEXT_UPDATE = 0U,
        GUI_MESSAGE_VALUE_UPDATE
    };

    inline std::queue<Message> _G_MSG_Queue{};
    inline std::mutex _G_MSG_Mutex{};

    inline void SendMessege(Message* pMsg)
    {
        _G_MSG_Mutex.lock();
            _G_MSG_Queue.push(*pMsg);
        _G_MSG_Mutex.unlock();
    }

    inline void SendMessege(messageType type, uint16_t valueName, const void* pData, uint8_t dataSize)
    {
        Message msg;

        msg.messageType = type;
        msg.valueName = valueName;
        memcpy_s(msg.message, dataSize, pData, dataSize);

        _G_MSG_Mutex.lock();
            _G_MSG_Queue.push(msg);
        _G_MSG_Mutex.unlock();
    }

    inline void SendMessege(messageType type, uint16_t valueName, const std::string& rMessage)
    {
        Message msg;

        msg.messageType = type;
        msg.valueName = valueName;
        memcpy_s(msg.message, rMessage.size() + 1, rMessage.c_str(), rMessage.size() + 1);

        _G_MSG_Mutex.lock();
            _G_MSG_Queue.push(msg);
        _G_MSG_Mutex.unlock();
    }

    inline bool GetMessage(Message* pMsg)
    {
        if (_G_MSG_Queue.empty()) return false;

        _G_MSG_Mutex.lock();
            *pMsg = _G_MSG_Queue.front();
            _G_MSG_Queue.pop();
        _G_MSG_Mutex.unlock();

        return true;
    }
}

#endif