#include <gtest/gtest.h>
#include "ipc/process_launcher.h"

TEST(ProcessLauncherTest, LaunchZeroSenders) {
    ipc::ProcessLauncher launcher;
    std::vector<ipc::ChildProcess> children;
    std::string err;

    EXPECT_TRUE(launcher.launch_senders(0, "dummy.exe", "file.dat", "prefix", children, err));
    EXPECT_TRUE(children.empty());
}

TEST(ProcessLauncherTest, InvalidExePath) {
    ipc::ProcessLauncher launcher;
    std::vector<ipc::ChildProcess> children;
    std::string err;

    EXPECT_FALSE(launcher.launch_senders(1, "nonexistent.exe", "file.dat", "prefix", children, err));
    EXPECT_FALSE(err.empty());
}

TEST(ProcessLauncherTest, CommandLineFormat) {
    ipc::ProcessLauncher launcher;
    std::vector<ipc::ChildProcess> children;
    std::string err;

    launcher.launch_senders(2, "fake_sender.exe", "queue.dat", "myprefix", children, err);

    EXPECT_FALSE(err.empty());
}