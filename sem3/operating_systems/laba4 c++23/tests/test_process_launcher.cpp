#include <gtest/gtest.h>
#include "ipc/process_launcher.h"

TEST(ProcessLauncherTest, LaunchZeroSenders) {
    auto res = ipc::ProcessLauncher::launch_senders(0, "sender.exe", "file.dat", "prefix");
    EXPECT_TRUE(res.has_value());
    EXPECT_TRUE(res->empty());
}

TEST(ProcessLauncherTest, InvalidExePath) {
    auto res = ipc::ProcessLauncher::launch_senders(1, "nonexistent.exe", "file.dat", "prefix");
    EXPECT_FALSE(res.has_value());
}

TEST(ProcessLauncherTest, CommandLineFormatFailureWithoutExe) {
    auto res = ipc::ProcessLauncher::launch_senders(2, "fake_sender.exe", "queue.dat", "myprefix");
    EXPECT_FALSE(res.has_value());
}