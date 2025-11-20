#include <gtest/gtest.h>
#include "ipc/win_sync.h"
#include "ipc/constants.h"

TEST(SyncTest, CreateAndSignalMessage) {
    ipc::SyncObjects sync;
    auto res = sync.create_for_receiver("sync_test", 1);
    ASSERT_TRUE(res.has_value());
    auto sig = sync.signal_message();
    EXPECT_TRUE(sig.has_value());
    auto wait = sync.wait_message(ipc::kDefaultWaitMs);
    EXPECT_TRUE(wait.has_value());
}

TEST(SyncTest, OpenForSenderAndReady) {
    ipc::SyncObjects recv;
    auto res = recv.create_for_receiver("sync_test2", 2);
    ASSERT_TRUE(res.has_value());
    ipc::SyncObjects s1;
    ipc::SyncObjects s2;
    auto o1 = s1.open_for_sender("sync_test2");
    auto o2 = s2.open_for_sender("sync_test2");
    ASSERT_TRUE(o1.has_value());
    ASSERT_TRUE(o2.has_value());
    auto r1 = s1.signal_ready(0);
    auto r2 = s2.signal_ready(1);
    ASSERT_TRUE(r1.has_value());
    ASSERT_TRUE(r2.has_value());
    auto wait = recv.wait_all_ready(ipc::kDefaultWaitMs);
    EXPECT_TRUE(wait.has_value());
}

TEST(SyncTest, LockUnlock) {
    ipc::SyncObjects sync;
    auto res = sync.create_for_receiver("sync_lock", 0);
    ASSERT_TRUE(res.has_value());
    auto l1 = sync.lock_file(ipc::kDefaultWaitMs);
    ASSERT_TRUE(l1.has_value());
    sync.unlock_file();
    auto l2 = sync.lock_file(ipc::kDefaultWaitMs);
    ASSERT_TRUE(l2.has_value());
    sync.unlock_file();
}