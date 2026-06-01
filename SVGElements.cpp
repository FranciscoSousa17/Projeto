#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,const Point &center,const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }

    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

    Circle::Circle(const Color &fill, const Point &center, int radius)
    : Ellipse(fill, center, Point{radius, radius})
    {
    }

    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
    : stroke(stroke), points(points)
    {
    }


    void Polyline::draw(PNGImage &img) const
    {
    for (size_t i = 1; i < points.size(); i++)
    {
        img.draw_line(points[i - 1], points[i], stroke);
    }
    }

    Line::Line(const Color &stroke, const Point &start, const Point &end)
    : Polyline(stroke, vector<Point>{start, end})
    {
    }

    Polygon::Polygon(const Color &fill, const std::vector<Point> &points)
    : fill(fill), points(points)
    {
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }

    Rect::Rect(const Color &fill, const Point &upper_left, int width, int height) 
    : Polygon (fill, std::vector<Point>{
                        upper_left,
                        Point{upper_left.x + width, upper_left.y},
                        Point{upper_left.x + width, upper_left.y + height},
                        Point{upper_left.x, upper_left.y + height}})
    {
    }       
}