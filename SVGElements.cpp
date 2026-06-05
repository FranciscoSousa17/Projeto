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
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    void Ellipse::translate(const Point &t)
    {
        center = center.translate(t);
    }

    void Ellipse::rotate(const Point &origin, int degrees)
    {
    center = center.rotate(origin, degrees);
    }

    void Ellipse::scale(const Point &origin, int v)
    {
    center = center.scale(origin, v);
    radius.x *= v;
    radius.y *= v;
    }

    SVGElement* Ellipse::clone() const
    {
    return new Ellipse(*this);
    }

    Circle::Circle(const Color &fill, const Point &center, int radius)
    : Ellipse(fill, center, Point{radius, radius})
    {
    }

    SVGElement* Circle::clone() const
    {
        return new Circle(*this);
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

    void Polyline::translate(const Point &t)
    {
    for (Point &p : points)
    {
        p = p.translate(t);
    }
    }

    void Polyline::rotate(const Point &origin, int degrees)
    {
    for (Point &p : points)
    {
        p = p.rotate(origin, degrees);
    }
    }

    void Polyline::scale(const Point &origin, int v)
    {
    for (Point &p : points)
    {
        p = p.scale(origin, v);
    }
    }

    SVGElement* Polyline::clone() const
    {
    return new Polyline(*this);
    }

    Line::Line(const Color &stroke, const Point &start, const Point &end)
    : Polyline(stroke, vector<Point>{start, end})
    {
    }

    SVGElement* Line::clone() const
    {
    return new Line(*this);
    }

    Polygon::Polygon(const Color &fill, const std::vector<Point> &points)
    : fill(fill), points(points)
    {
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }

    void Polygon::translate(const Point &t)
    {
    for (Point &p : points)
    {
        p = p.translate(t);
    }
    }

    void Polygon::rotate(const Point &origin, int degrees)
    {
    for (Point &p : points)
    {
        p = p.rotate(origin, degrees);
    }
    }

    void Polygon::scale(const Point &origin, int v)
    {   
    for (Point &p : points)
    {
        p = p.scale(origin, v);
    }
    }

    SVGElement* Polygon::clone() const
    {
    return new Polygon(*this);
    }

    Rect::Rect(const Color &fill, const Point &upper_left, int width, int height) 
    : Polygon(fill, std::vector<Point>{
                        upper_left,
                        Point{upper_left.x + width - 1, upper_left.y},
                        Point{upper_left.x + width - 1, upper_left.y + height - 1},
                        Point{upper_left.x, upper_left.y + height - 1}})
    {
    }

    SVGElement* Rect::clone() const
    {
    return new Rect(*this);
    }
    
    Group::Group(const std::vector<SVGElement *> &elements)
    : elements(elements)
    {
    }

    Group::~Group()
    {
    for (SVGElement *element : elements)
    {
        delete element;
    }
    }

    void Group::draw(PNGImage &img) const
    {
    for (SVGElement *element : elements)
    {
        element->draw(img);
    }
    }

    void Group::translate(const Point &t)
    {
    for (SVGElement *element : elements)
    {
        element->translate(t);
    }
    }

    void Group::rotate(const Point &origin, int degrees)
    {
    for (SVGElement *element : elements)
    {
        element->rotate(origin, degrees);
    }
    }

    void Group::scale(const Point &origin, int v)
    {
    for (SVGElement *element : elements)
    {
        element->scale(origin, v);
    }
    }

    SVGElement* Group::clone() const
    {
    std::vector<SVGElement *> copied_elements;

    for (SVGElement *element : elements)
    {
        copied_elements.push_back(element->clone());
    }

    return new Group(copied_elements);
    }
}