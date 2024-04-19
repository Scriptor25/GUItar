#pragma once

#include <tinyxml2/tinyxml2.h>
#include <guitar/guitar.hpp>

namespace guitar
{
    void FromXML(const tinyxml2::XMLElement* xml, Layout& ref);
    void FromXML(const tinyxml2::XMLElement* xml, Element*& ref);
    void FromXML(const tinyxml2::XMLElement* xml, DockSpaceElement& ref);
    void FromXML(const tinyxml2::XMLElement* xml, DemoElement& ref);
}
