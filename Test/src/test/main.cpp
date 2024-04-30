#include <guitar/application.hpp>
#include <iostream>

class Test : public guitar::Application
{
public:
    explicit Test(const std::filesystem::path &root)
            : Application(root)
    {
        Events().Register("on_key", [this](const guitar::EventPayload *payload)
        {
            const auto &keyPayload = *dynamic_cast<const guitar::KeyPayload *>(payload);

            if (keyPayload.Key == GLFW_KEY_ESCAPE && keyPayload.Action == GLFW_RELEASE)
            {
                Close();
                return true;
            }

            if (keyPayload.Key == GLFW_KEY_F11 && keyPayload.Action == GLFW_RELEASE)
            {
                ToggleFullscreen();
                return true;
            }

            return false;
        });

        Events().Register("test", [](const guitar::EventPayload *payload)
        {
            std::cout << "Hello World!" << std::endl;
            return true;
        });
    }
};

int main(const int argc, const char **argv)
{
    (void) argc;

    Test test(argv[0]);
    return !test.Launch();
}
