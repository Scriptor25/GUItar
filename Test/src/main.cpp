#include <iostream>
#include <guitar/application.hpp>
#include <guitar/image.hpp>

class Test : public guitar::Application
{
public:
    explicit Test(const int argc, const char** argv)
        : Application(argc, argv)
    {
        Schedule([this]
        {
            m_Image.StorePixels(100, 100, nullptr);
        });

        Events().Register("on_key", this, [this](const guitar::EventPayload* pPayload)
        {
            const auto& payload = *dynamic_cast<const guitar::KeyPayload*>(pPayload);

            if (payload.Key == GLFW_KEY_ESCAPE && payload.Action == GLFW_RELEASE)
            {
                Close();
                return true;
            }

            if (payload.Key == GLFW_KEY_F11 && payload.Action == GLFW_RELEASE)
            {
                Schedule([this] { ToggleFullscreen(); });
                return true;
            }

            return false;
        });

        Events().Register("test", this, [](const guitar::EventPayload*)
        {
            std::cout << "Hello World!" << std::endl;
            return true;
        });

        Events().Register("img_test", this, [this](const guitar::EventPayload* pPayload)
        {
            const auto& payload = *dynamic_cast<const guitar::ImagePayload*>(pPayload);
            *payload.PWidth = m_Image.Width;
            *payload.PHeight = m_Image.Height;
            *payload.PTextureID = reinterpret_cast<ImTextureID>(m_Image.Texture);
            return true;
        });

        Events().Register("open_demo", this, [this](const guitar::EventPayload*)
        {
            Schedule([this] { UseLayout("demo"); });
            return true;
        });
    }

private:
    guitar::Image m_Image{};
};

int main(const int argc, const char** argv)
{
    Test test(argc, argv);
    test.Launch();
    return 0;
}
