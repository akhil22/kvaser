Kvaser can client interface for Apollo.

REQUIREMENTS:

Install linuxcan library and build kvaser_interface(https://github.com/astuff/kvaser_interface) as Apollo Ros package.

Installation:
Put the repo inside APOLLO_ROOT_DIR/modules/canbus/can_client then follow "Registering the new CAN card in CanClientFactory" and "Update the config file" from https://github.com/ApolloAuto/apollo/blob/master/docs/howto/how_to_add_a_new_can_card.md, replacing "EXAMPLE_CAN" with "KVASER_CAN" in both of them. Go to APOLLO_ROOT_DIR and build using Bazel (bazel build).






