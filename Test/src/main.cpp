#include <iostream>
#include <vector>
#include <guitar/application.hpp>
#include <guitar/image.hpp>

class Test : public guitar::Application
{
public:
    explicit Test(const int argc, const char** argv)
        : Application(argc, argv)
    {
    }

protected:
    void OnStart() override
    {
        m_Image.StorePixels(100, 100, nullptr);

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
            payload.Width = m_Image.Width;
            payload.Height = m_Image.Height;
            payload.TextureID = reinterpret_cast<ImTextureID>(m_Image.Texture);
            return true;
        });

        Events().Register("open_demo", this, [this](const guitar::EventPayload*)
        {
            Schedule([this] { UseLayout("demo"); });
            return true;
        });

        Events().Register("selected", this, [this](const guitar::EventPayload* pPayload)
        {
            const auto& payload = *dynamic_cast<const guitar::StringPayload*>(pPayload);
            payload.Result = m_Items[m_Selected];
            return true;
        });

        Events().Register("test_combo_body", this, [this](const guitar::EventPayload*)
        {
            for (int i = 0; i < m_Items.size(); ++i)
            {
                ImGui::PushID(i);

                const bool selected = i == m_Selected;
                if (ImGui::Selectable(m_Items[i].c_str(), selected))
                    m_Selected = i;
                if (selected)
                    ImGui::SetItemDefaultFocus();

                ImGui::PopID();
            }

            return true;
        });
    }

private:
    guitar::Image m_Image;

    int m_Selected = 0;
    const std::vector<std::string> m_Items{"ABC", "DEF", "GHI", "JKL", "MNO", "PQR", "STU", "VWX", "Y_Z"};
};

int main(const int argc, const char** argv)
{
    Test test(argc, argv);
    test.Launch();
    return 0;
}
