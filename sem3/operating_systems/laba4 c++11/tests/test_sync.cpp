#include <gtest/gtest.h>
#include "ipc/win_sync.h"

TEST(SyncTest, CreateAndSignalMessage) {
    ipc::SyncObjects sync;
    ASSERT_TRUE(sync.create_for_receiver("sync_test", 1));

    EXPECT_TRUE(sync.signal_message());
}

TEST(SyncTest, OpenForSender) {
    ipc::SyncObjects sync;
    ASSERT_TRUE(sync.create_for_receiver("sync_test2", 1));

    ipc::SyncObjects senderSync;
    EXPECT_TRUE(senderSync.open_for_sender("sync_test2"));
}