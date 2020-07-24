//
// Created by matt on 2020-07-12.
//

#ifndef DOGBONE_UT_EVENTDISPATCHER_H
#define DOGBONE_UT_EVENTDISPATCHER_H

#include "UT_API.h"

#include <unordered_map>
#include <list>
#include <functional>
#include <memory>
#include <optional>

namespace dogb::UT
{
template <typename EventT, typename Prototype>
class EventDispatcher;

template <typename EventT, typename ReturnT, typename ... Args>
class EventDispatcher<EventT, ReturnT (Args...)>
{
private:
    using CallbackT_ = std::function<ReturnT (Args...)>;

    struct Node
    {
        explicit Node(CallbackT_ callback) :
            m_callback(callback)
        {}

        ReturnT operator()(Args&&... args)
        {
            return m_callback(std::forward<Args>(args)...);
        }

        CallbackT_ m_callback;
    };
public:
    using CallbackT = CallbackT_ ;
    using NodePtr = std::shared_ptr<Node>;
    using Handle = std::weak_ptr<Node>;
    using CallbackListT = std::list<NodePtr >;

    Handle appendListener(const EventT& e, const CallbackT& callback)
    {
        return m_callbackList[e].emplace_back(std::make_shared<Node>(callback));
    }
    Handle prependListener(const EventT& e, const CallbackT& callback)
    {
        return m_callbackList[e].prepend(std::make_shared<Node>(callback));
    }
    bool removeListener(const EventT& e, const Handle handle)
    {
        CallbackListT *list = findCallbackList_(e);
        if (!list)
            return false;

        NodePtr node = handle.lock();
        if (!node)
            return false;

        bool was_deleted = false;
        CallbackListT & clist = (*list);
        for (auto it = clist.begin(); it != clist.end();)
        {
            if ((*it) == node)
            {
                clist.erase(it);
                was_deleted = true;
                break;
            }
            ++it;
        }
        return was_deleted;
    }

    void dispatch(const EventT& type, Args&&... args) const
    {
        const CallbackListT *list = findCallbackList_(type);
        if (!list)
            return;

        CallbackListT _list = (*list);
        for (auto&& callback : _list)
            (*callback)(std::forward<Args>(args)...);
    }

private:
    const CallbackListT * findCallbackList_(const EventT& e) const
    {
        return findCallbackListHelper(this, e);
    }
    CallbackListT * findCallbackList_(const EventT & e)
    {
        return findCallbackListHelper(this, e);
    }

    template <typename T>
    static auto findCallbackListHelper(T* _this, const EventT& e) ->
        typename std::conditional_t<std::is_const_v<T>, const CallbackListT *, CallbackListT *>
    {
        auto it = _this->m_callbackList.find(e);
        if (it == _this->m_callbackList.end())
        {
            return nullptr;
        }
        return &((*it).second);
    }

    std::unordered_map<EventT, CallbackListT> m_callbackList;
};

}

#endif // DOGBONE_UT_EVENTDISPATCHER_H
