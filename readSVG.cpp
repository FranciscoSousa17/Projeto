
#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
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
        
        // TODO complete code --> 
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
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

                svg_elements.push_back(new Ellipse(fill, center, radius));
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

                svg_elements.push_back(new Circle(fill, center, r));
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

                svg_elements.push_back(new Polyline(stroke, points));
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

                svg_elements.push_back(new Line(stroke, start, end));
        
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

                    svg_elements.push_back(new Polygon(fill, points));
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

                    svg_elements.push_back(new Rect(fill, upper_left, width, height));
                    }
}
    }
}