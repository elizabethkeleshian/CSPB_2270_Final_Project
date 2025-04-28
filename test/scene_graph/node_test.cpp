#include <gtest/gtest.h>
#include "scene_graph/node.h"

TEST(NodeTest, BasicCreation) {
    auto node = std::make_shared<scene_graph::Node>("TestNode");
    EXPECT_EQ(node->getName(), "TestNode");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
