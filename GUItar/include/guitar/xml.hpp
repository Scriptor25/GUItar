#pragma once

#include <guitar/guitar.hpp>
#include <tinyxml2/tinyxml2.h>

namespace guitar
{
    void FromXML(const tinyxml2::XMLElement *xml, AppConfig &ref);

    void FromXML(const tinyxml2::XMLElement *xml, Layout &ref);

    void FromXML(const tinyxml2::XMLElement *xml, Element *&ref);

    void FromXML(const tinyxml2::XMLElement *xml, DockSpaceElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, DemoElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, WindowElement &ref);

    void FromXML(const tinyxml2::XMLElement *xml, TextElement &ref);
}
