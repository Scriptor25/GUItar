#include <guitar/application.hpp>
#include <imgui/imgui.h>

class Test
    : public guitar::Application
{
public:
    explicit Test(const std::filesystem::path& root)
        : Application(root)
    {
    }

protected:
    void OnInit(guitar::AppConfig& config) override
    {
        config.Width = 800;
        config.Height = 600;
        config.Title = "Test";
    }

    void OnStart() override
    {
        Register(
            [this](const int key, const int scancode, const int action, const int mods)
            {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                    Close();
            });

        UseLayout("main");
    }
};

int main(int argc, const char** argv)
{
    Test test(argv[0]);
    return test.Launch();
}
