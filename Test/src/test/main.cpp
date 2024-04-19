#include <guitar/application.hpp>

using namespace std::placeholders;

class Test : public guitar::Application
{
public:
    explicit Test(const std::filesystem::path &root)
            : Application(root)
    {
        Register(std::bind(&Test::OnInput, this, _1, _2, _3, _4));
    }

    void OnInput(const int key, const int scancode, const int action, const int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
            Close();
    }
};

int main(int argc, const char **argv)
{
    Test test(argv[0]);
    return !test.Launch();
}
