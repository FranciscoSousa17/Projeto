#include <cstdio>
#include <iostream>
#include <sstream>
#include <map>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void apply_transform(XMLElement *xml_element, SVGElement *element)
{
    const char *transform_str = xml_element->Attribute("transform");

    if (transform_str == nullptr)
    {
        return;
    }

    Point origin;
    origin.x = 0;
    origin.y = 0;

    const char *origin_str = xml_element->Attribute("transform-origin");

    if (origin_str != nullptr)
    {
        stringstream origin_ss(origin_str);
        origin_ss >> origin.x >> origin.y;
    }

    string transform_text = string(transform_str);
    
    if (transform_text.find("translate") == 0)
    {
        int x = 0;
        int y = 0;

        sscanf(transform_text.c_str(), "translate(%d %d)", &x, &y);

        Point t;
        t.x = x;
        t.y = y;

        element->translate(t);
    }
    else if (transform_text.find("rotate") == 0)
    {
    int degrees = 0;

    sscanf(transform_text.c_str(), "rotate(%d)", &degrees);

            element->rotate(origin, degrees);
        }
        else if (transform_text.find("scale") == 0)
        {
            int v = 1;

            sscanf(transform_text.c_str(), "scale(%d)", &v);

            element->scale(origin, v);
        
        }
}
    // TODO complete code --> 
        void save_id(XMLElement *xml_element, SVGElement *element, map<string, SVGElement *> &ids)
        {
            const char *id_str = xml_element->Attribute("id");

            if (id_str != nullptr)
            {
                ids[string(id_str)] = element;
            }
        }

        void read_elements(XMLElement *parent, vector<SVGElement *> &svg_elements, map<string, SVGElement *> &ids)
        {
        for (XMLElement *child = parent->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
{
            string name = child->Name();

            if (name == "ellipse")
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");

                const char *fill_str = child->Attribute("fill");
                Color fill = parse_color(string(fill_str));

                Point center;
                center.x = cx;
                center.y = cy;

                Point radius;
                radius.x = rx;
                radius.y = ry;

                SVGElement *element = new Ellipse(fill, center, radius);
                apply_transform(child, element);
                save_id(child, element, ids);
                svg_elements.push_back(element);
            }
            else if (name == "circle")
            {
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r");

                const char *fill_str = child->Attribute("fill");
                Color fill = parse_color(string(fill_str));

                Point center;
                center.x = cx;
                center.y = cy;

                SVGElement *element = new Circle(fill, center, r);
                apply_transform(child, element);
                save_id(child, element, ids);
                svg_elements.push_back(element);
            }
            else if (name == "polyline")
            {
                const char *points_str = child->Attribute("points");
                const char *stroke_str = child->Attribute("stroke");

                Color stroke = parse_color(string(stroke_str));

                vector<Point> points;

                stringstream ss(points_str);
                string point_token;

                while (ss >> point_token)
            {
                size_t comma_pos = point_token.find(',');

                int x = stoi(point_token.substr(0, comma_pos));
                int y = stoi(point_token.substr(comma_pos + 1));

                Point p;
                p.x = x;
                p.y = y;

                points.push_back(p);
            }
                SVGElement *element = new Polyline(stroke, points);
                apply_transform(child, element);
                save_id(child, element, ids);
                svg_elements.push_back(element);
              
            }
            else if (name == "line")
            {
                int x1 = child->IntAttribute("x1");
                int y1 = child->IntAttribute("y1");
                int x2 = child->IntAttribute("x2");
                int y2 = child->IntAttribute("y2");

                const char *stroke_str = child->Attribute("stroke");
                Color stroke = parse_color(string(stroke_str));

                Point start;
                start.x = x1;
                start.y = y1;

                Point end;
                end.x = x2;
                end.y = y2;

                SVGElement *element = new Line(stroke, start, end);
                apply_transform(child, element);
                save_id(child, element, ids);
                svg_elements.push_back(element);
        
            }
            else if (name == "polygon")
            {
                const char *points_str = child->Attribute("points");
                const char *fill_str = child->Attribute("fill");

                Color fill = parse_color(string(fill_str)); // converter a cor 

                vector<Point> points; // criar vetor vazio onde guardamos os pontos do poligono

                stringstream ss(points_str);
                string point_token;

                while (ss >> point_token)
                {
                    size_t comma_pos = point_token.find(',');

                    int x = stoi(point_token.substr(0, comma_pos));
                    int y = stoi(point_token.substr(comma_pos + 1));

                    Point p;
                    p.x = x;
                    p.y = y;

                    points.push_back(p);
                }

                    SVGElement *element = new Polygon(fill, points);
                    apply_transform(child, element);
                    save_id(child, element, ids);
                    svg_elements.push_back(element);
            }
            else if (name == "rect")
            {
                    int x = child->IntAttribute("x");
                    int y = child->IntAttribute("y");
                    int width = child->IntAttribute("width");
                    int height = child->IntAttribute("height");

                    const char *fill_str = child->Attribute("fill");
                    Color fill = parse_color(string(fill_str));

                    Point upper_left;
                    upper_left.x = x;
                    upper_left.y = y;

                    SVGElement *element = new Rect(fill, upper_left, width, height);
                    apply_transform(child, element);
                    save_id(child, element, ids);
                    svg_elements.push_back(element);
            }
            else if (name == "g")
            {
                    vector<SVGElement *> group_elements;

                    read_elements(child, group_elements, ids);

                    SVGElement *element = new Group(group_elements); //cria um group com esses elementos 
                    apply_transform(child, element); //aplicar as transformações do proprio <g>
                    save_id(child, element, ids);
                    svg_elements.push_back(element); //adicionar o grupo ao fim do vetor principal (SVG)
            }
           else if (name == "use")
            {
                const char *href_str = child->Attribute("href");

                if (href_str != nullptr)
                {
                    string href = string(href_str);

                    if (!href.empty() && href[0] == '#')
                    {
                        string id = href.substr(1);

                        if (ids.find(id) != ids.end())
                        {
                            SVGElement *element = ids[id]->clone();
                            apply_transform(child, element);
                            save_id(child, element, ids);
                            svg_elements.push_back(element);
                        }
                    }
                }
            }
}
        }
        void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
        {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        map<string, SVGElement *> ids;
        read_elements(xml_elem, svg_elements, ids);
        }
}