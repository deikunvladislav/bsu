#ifndef OS_LAB5_RW_LOCK_HPP
#define OS_LAB5_RW_LOCK_HPP

#include <shared_mutex>
#include <unordered_map>
#include <mutex>
#include <cstdint>

class PerRecordRWLock {
public:
    void lock_read(int32_t id) { get_mutex(id).lock_shared(); }
    void unlock_read(int32_t id) { get_mutex(id).unlock_shared(); }
    void lock_write(int32_t id) { get_mutex(id).lock(); }
    void unlock_write(int32_t id) { get_mutex(id).unlock(); }

    bool try_lock_read(int32_t id) { return get_mutex(id).try_lock_shared(); }
    bool try_lock_write(int32_t id) { return get_mutex(id).try_lock(); }

private:
    struct Entry { std::shared_mutex m; };
    Entry& get_entry(int32_t id) {
        std::lock_guard<std::mutex> g(map_mutex_);
        return entries_.try_emplace(id).first->second;
    }
    std::shared_mutex& get_mutex(int32_t id) { return get_entry(id).m; }

    std::unordered_map<int32_t, Entry> entries_;
    std::mutex map_mutex_;
};

#endif