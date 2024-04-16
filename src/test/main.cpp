#include <guitar/application.hpp>
#include <imgui/imgui.h>

class Test
    : public guitar::Application
{
public:
    Test(const std::filesystem::path &root)
        : Application(root)
    {
    }

protected:
    void OnInit(guitar::AppConfig &config) override
    {
        config.Width = 800;
        config.Height = 600;
        config.Title = "Test";
    }

    void OnStart() override
    {
        Register(
            [this](int key, int scancode, int action, int mods)
            {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                    Close();
            });
    }

    void OnImGui() override
    {
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();
    }

    void OnStop() override
    {
    }

    void OnDestroy() override
    {
    }
};

int main(int argc, const char **argv)
{
    Test test(argv[0]);
    return test.Launch();
}
