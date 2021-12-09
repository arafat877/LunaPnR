#pragma once

#include <type_traits>
#include <sstream>
#include <vector>
#include <string>
#include <atomic>
#include <algorithm>
#include <unordered_map>

#ifdef NO_SSIZE_T
#include <type_traits>
typedef std::make_signed<size_t>::type ssize_t;
#endif

namespace ChipDB {

struct INamedStorageListener
{
    enum class NotificationType
    {
        UNSPECIFIED = 0,
        ADD,
        REMOVE
    };

    /** userID: the user ID when at addListener was called to register this listener.
     *  index : the index of the item that was modified, -1 for entire collection.
     *  t     : type of modification that occurred.
    */
    virtual void notify(int32_t userID= -1, ssize_t index = -1, NotificationType t = NotificationType::UNSPECIFIED) = 0;
};

/** container to store object pointers and provides fast named lookup. 
*/
template <class T, bool owning = false>
class NamedStorage
{
public:
    NamedStorage()
    {
        static_assert(std::is_pointer<T>::value, "Stored object type must be a pointer");
    }

    virtual ~NamedStorage()
    {
        if constexpr (owning)
        {
            for(auto ptr : m_objects)
            {
                delete ptr;
            }
        }
    }

    void clear()
    {
        if constexpr (owning)
        {
            for(auto ptr : m_objects)
            {
                delete ptr;
            }
        }

        m_objects.clear();
        m_nameToIndex.clear();
        notifyAll(-1, INamedStorageListener::NotificationType::REMOVE);
    }

    size_t size() const
    {
        return m_objects.size();
    }

    bool add(const std::string &name, T object)
    {
        auto iter = m_nameToIndex.find(name);
        if (iter == m_nameToIndex.end())
        {
            // no such named object, okay to add!
            ssize_t index = m_objects.size();
            m_objects.push_back(object);
            m_nameToIndex[name] = index;
            notifyAll(index, INamedStorageListener::NotificationType::ADD);
            return true;
        }
        return false;   // object already exists
    }

#if 0
    bool add(T object)
    {
        auto iter = m_nameToIndex.find(name);
        if (iter == m_nameToIndex.end())
        {
            // no such named object, okay to add!
            m_objects.push_back(object);
            m_nameToIndex[name] = m_objects.size()-1;
            return true;
        }
    }
#endif

    bool remove(const std::string &name)
    {
        auto iter = m_nameToIndex.find(name);
        if (iter == m_nameToIndex.end())
        {
            return false;   // no such named object
        }
        else
        {
            size_t index = iter->second;
            m_nameToIndex.erase(iter);
            if (index < m_objects.size())
            {
                m_objects[index] = nullptr;
                notifyAll(index, INamedStorageListener::NotificationType::REMOVE);
            }            
            return true;
        }
    }

    T lookup(const std::string &name)
    {
        auto iter = m_nameToIndex.find(name);
        if (iter == m_nameToIndex.end())
        {
            return nullptr;
        }
        else
        {            
            return m_objects.at(iter->second);
        }
    }

    T lookup(const std::string &name) const
    {
        auto iter = m_nameToIndex.find(name);
        if (iter == m_nameToIndex.end())
        {
            return nullptr;
        }
        else
        {
            return m_objects.at(iter->second);
        }
    }

    T at(size_t index)
    {
        #if 0
        if (index >= m_objects.size())
        {
            std::stringstream ss;
            ss << "NamedStorage out of range: max=" << m_objects.size()-1 << " requested=" << index;
            throw std::out_of_range(ss.str());
        }
        #endif

        if (index >= m_objects.size())
        {
            return nullptr;
        }

        return m_objects.at(index);
    }
 
    const T at(size_t index) const
    {
        #if 0
        if (index >= m_objects.size())
        {
            std::stringstream ss;
            ss << "NamedStorage out of range: max=" << m_objects.size()-1 << " requested=" << index;
            throw std::out_of_range(ss.str());
        }
        #endif

        return m_objects.at(index);
    }

    auto begin()
    {
        return m_objects.begin();
    }

    auto begin() const
    {
        return m_objects.begin();
    }

    auto end()
    {
        return m_objects.end();
    }

    auto end() const
    {
        return m_objects.end();
    }

    void addListener(INamedStorageListener *listener, int32_t userID = -1)
    {
        // check if the listener already exists.
        auto iter = std::find_if(m_listeners.begin(), m_listeners.end(), 
            [listener](auto const &listenerData)
            {
                return listenerData.m_listener == listener;
            }
        );

        // if not, add it!
        if (iter == m_listeners.end())
        {            
            m_listeners.emplace_back();
            m_listeners.back().m_listener = listener;
            m_listeners.back().m_userID = userID;
        }
    }

    void removeListener(INamedStorageListener *listener)
    {
        auto iter = std::find_if(m_listeners.begin(), m_listeners.end(), 
            [listener](auto const &listenerData)
            {
                return listenerData.m_listener == listener;
            }
        );

        if (iter != m_listeners.end())
        {
            m_listeners.erase(iter);
        }        
    }

    /** trigger a generic message to all the listeners that the data has changed */
    void contentsChanged() const
    {
        notifyAll();
    }

protected:
    
    void notifyAll(ssize_t index = -1, INamedStorageListener::NotificationType t = 
        INamedStorageListener::NotificationType::UNSPECIFIED) const
    {
        for(auto &listenerData : m_listeners)
        {
            if (listenerData.m_listener != nullptr)
            {
                listenerData.m_listener->notify(listenerData.m_userID, index, t);
            }
        }
    }

    struct ListenerData
    {
        INamedStorageListener *m_listener;
        int32_t                m_userID;
    };

    std::vector<ListenerData> m_listeners;
    std::vector<T> m_objects;
    std::unordered_map<std::string, size_t> m_nameToIndex;
};



};
