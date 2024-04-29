#include <guitar/application.hpp>

class Test : public guitar::Application
{
public:
    explicit Test(const std::filesystem::path& root)
        : Application(root)
    {
        Register([this](const int key, const int scancode, const int action, const int mods)
        {
            (void)scancode;
            (void)mods;

            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                Close();
        });
    }
};

int main(const int argc, const char** argv)
{
    (void)argc;

    Test test(argv[0]);
    return !test.Launch();
}
