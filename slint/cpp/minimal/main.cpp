#include "main_ui.h"

int main(int argc, char **argv)
{
    auto hello_world = HelloWorld::create();
    hello_world->set_my_label("Hello from C++");
    hello_world->run();
    return 0;
}
