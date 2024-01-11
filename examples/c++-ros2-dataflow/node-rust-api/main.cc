#include "../build/dora-node-api.h"
#include "../build/messages.h"

#include <iostream>
#include <vector>

int main()
{
    std::cout << "HELLO FROM C++" << std::endl;

    auto ros2_context = init_ros2_context();
    auto node = ros2_context->new_node("/ros2_demo", "turtle_teleop");
    auto vel_topic = node->create_topic_geometry_msgs_Twist("/turtle1", "cmd_vel", 0);
    auto vel_publisher = node->create_publisher(vel_topic, 0);

    geometry_msgs::Twist twist = {
        .linear = {.x = 1, .y = 0, .z = 0},
        .angular = {.x = 0, .y = 0, .z = 0.5}};

    unsigned char counter = 0;

    auto dora_node = init_dora_node();

    for (int i = 0; i < 20; i++)
    {

        auto event = next_event(dora_node.events);
        auto ty = event_type(event);

        if (ty == DoraEventType::AllInputsClosed)
        {
            break;
        }
        else if (ty == DoraEventType::Input)
        {
            auto input = event_as_input(std::move(event));

            counter += 1;

            std::cout << "Received input " << std::string(input.id) << " (counter: " << (unsigned int)counter << ")" << std::endl;

            std::vector<unsigned char> out_vec{counter};
            rust::Slice<const uint8_t> out_slice{out_vec.data(), out_vec.size()};
            auto result = send_output(dora_node.send_output, "counter", out_slice);
            auto error = std::string(result.error);
            if (!error.empty())
            {
                std::cerr << "Error: " << error << std::endl;
                return -1;
            }
        }
        else
        {
            std::cerr << "Unknown event type " << static_cast<int>(ty) << std::endl;
        }
    }

    std::cout << "GOODBYE FROM C++ node (using Rust API)" << std::endl;

    return 0;
}
