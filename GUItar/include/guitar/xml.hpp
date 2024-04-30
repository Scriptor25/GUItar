#pragma once

#include <string>
#include <guitar/guitar.hpp>
#include <tinyxml2/tinyxml2.h>

namespace guitar
{
    std::string GetStringAttrib(const tinyxml2::XMLElement *xml, const std::string &name, const std::string &alt);

    int GetIntAttrib(const tinyxml2::XMLElement *xml, const std::string &name, int alt);

    void FromXML(const tinyxml2::XMLElement *xml, AppConfig &ref);

    void FromXML(const tinyxml2::XMLElement *xml, Layout &ref);

    void FromXML(ResourceManager *resources, const tinyxml2::XMLElement *xml, Element *&ref);

    void FromXML(const tinyxml2::XMLElement *xml, DockSpaceElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, DemoElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, WindowElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, TextElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, ButtonElement &ref);
}
