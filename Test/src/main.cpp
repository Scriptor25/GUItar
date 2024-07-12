#include <iostream>
#include <vector>
#include <guitar/application.hpp>
#include <guitar/image.hpp>
#include <implot.h>

class Test : public guitar::Application
{
public:
    explicit Test(const int argc, const char** argv)
        : Application(argc, argv)
    {
        Events().Register("test", this, [](const guitar::EventBase*)
        {
            std::cout << "Hello World!" << std::endl;
            return true;
        });

        Events().Register("img_test", this, [this](const guitar::EventBase* pEvent)
        {
            auto& event = *dynamic_cast<const guitar::ImmutableEvent<guitar::ImagePayload>*>(pEvent);
            event.Payload.Width = m_Image.Width;
            event.Payload.Height = m_Image.Height;
            event.Payload.TextureID = reinterpret_cast<ImTextureID>(m_Image.Texture);
            return true;
        });

        Events().Register("open_demo", this, [this](const guitar::EventBase*)
        {
            Schedule([this] { UseLayout("demo"); });
            return true;
        });

        Events().Register("selected", this, [this](const guitar::EventBase* pEvent)
        {
            auto& event = *dynamic_cast<const guitar::MutableEvent<std::string>*>(pEvent);
            event.Payload = m_Items[m_Selected];
            return true;
        });

        Events().Register("test_combo_body", this, [this](const guitar::EventBase*)
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

        Events().Register("on_input_text", this, [this](const guitar::EventBase* pEvent)
        {
            auto& event = *dynamic_cast<const guitar::ImmutableEvent<std::string>*>(pEvent);
            if (event.Payload.empty())
                return false;
            std::cout << "Input: " << event.Payload << std::endl;
            return true;
        });

        Events().Register("ABC", this, [](const guitar::EventBase* pEvent)
        {
            auto& event = *dynamic_cast<const guitar::MutableEvent<std::string>*>(pEvent);
            event.Payload = "ABC";
            return true;
        });

        Events().Register("close", this, [this](const guitar::EventBase*)
        {
            Close();
            return true;
        });

        Events().Register("fullscreen", this, [this](const guitar::EventBase*)
        {
            Schedule([this] { ToggleFullscreen(); });
            return true;
        });
    }

protected:
    void OnStart() override
    {
        m_Image.StorePixels(100, 100, nullptr);
    }

    void OnImGui() override
    {
        if (ImGui::Begin("Custom Window"))
        {
            if (ImPlot::BeginPlot("Test Plot", {-1, -1}))
            {
                const auto [buttons, axes] = Input().GetGamepad(0);
                const float xs[]{
                    0.0f,
                    axes[GLFW_GAMEPAD_AXIS_LEFT_X]
                };
                const float ys[]{
                    0.0f,
                    -axes[GLFW_GAMEPAD_AXIS_LEFT_Y]
                };
                ImPlot::PlotLine("", xs, ys, 2);
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
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
