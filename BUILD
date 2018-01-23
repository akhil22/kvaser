load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "kvaser_can_client",
    srcs = [
        "kvaser_can_client.cc",
    ],
    hdrs = [
        "kvaser_can_client.h",
    ],
    deps = [
        "//modules/canbus/can_client",
        "//third_party/ros:ros_common",
        "//third_party/linuxcan:can_common",
    ],
)


cpplint()
