#include <iostream>
#include <guitar/application.hpp>

class Test : public guitar::Application
{
public:
    explicit Test(const int argc, const char** argv)
        : Application(argc, argv)
    {
        Events().Register("on_key", this, [this](guitar::EventPayload* payload)
        {
            const auto& keyPayload = *dynamic_cast<guitar::KeyPayload*>(payload);

            if (keyPayload.Key == GLFW_KEY_ESCAPE && keyPayload.Action == GLFW_RELEASE)
            {
                Close();
                return true;
            }

            if (keyPayload.Key == GLFW_KEY_F11 && keyPayload.Action == GLFW_RELEASE)
            {
                Schedule([this] { ToggleFullscreen(); });
                return true;
            }

            return false;
        });

        Events().Register("test", this, [](guitar::EventPayload*)
        {
            std::cout << "Hello World!" << std::endl;
            return true;
        });

        Events().Register("img_test", this, [](guitar::EventPayload* payload)
        {
            const auto& imagePayload = *dynamic_cast<guitar::ImagePayload*>(payload);
            *imagePayload.PWidth = 100;
            *imagePayload.PHeight = 100;
            *imagePayload.PTextureID = nullptr;
            return true;
        });

        Events().Register("open_demo", this, [this](guitar::EventPayload*)
        {
            Schedule([this] { UseLayout("demo"); });
            return true;
        });
    }
};

int main(const int argc, const char** argv)
{
    Test test(argc, argv);
    test.Launch();
    return 0;
}
