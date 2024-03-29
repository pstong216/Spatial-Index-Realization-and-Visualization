#include "Geometry.h"
#include <cmath>
#include <gl/freeglut.h>

#define NOT_IMPLEMENT -1.0

namespace hw6
{

    /*
     * Envelope functions
     */
    bool Envelope::contain(double x, double y) const
    {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }

    bool Envelope::contain(const Envelope& envelope) const
    {
        // Task ????Envelope?????????
        // TODO
        if (envelope.minX >= minX && envelope.maxX <= maxX && envelope.minY >= minY && envelope.maxY <= maxY)
            return true;
        return false;
    }

    bool Envelope::intersect(const Envelope& envelope) const
    {
        // Task ????Envelope?????
        // TODO
        if (envelope.minX <= maxX && envelope.maxX >= minX && envelope.minY <= maxY && envelope.maxY >= minY)
            return true;
        return false;
    }

    Envelope Envelope::unionEnvelope(const Envelope& envelope) const
    {
        // Task ???????Envelope??????????Envelope
        // TODO
        double minX = std::min(this->minX, envelope.minX);
        double maxX = std::max(this->maxX, envelope.maxX);
        double minY = std::min(this->minY, envelope.minY);
        double maxY = std::max(this->maxY, envelope.maxY);
        return Envelope(minX, maxX, minY, maxY);
    }

    void Envelope::draw() const
    {
        glBegin(GL_LINE_STRIP);

        glVertex2d(minX, minY);
        glVertex2d(minX, maxY);
        glVertex2d(maxX, maxY);
        glVertex2d(maxX, minY);
        glVertex2d(minX, minY);

        glEnd();
    }

    /*
     * Points functions
     */
    double Point::distance(const Point* point) const
    {
        return sqrt((x - point->x) * (x - point->x) +
            (y - point->y) * (y - point->y));
    }

    double Point::distance(const LineString* line) const
    {
        double mindist = line->getPointN(0).distance(this);
        for (size_t i = 0; i < line->numPoints() - 1; ++i)
        {
            double dist = 0;
            double x1 = line->getPointN(i).getX();
            double y1 = line->getPointN(i).getY();
            double x2 = line->getPointN(i + 1).getX();
            double y2 = line->getPointN(i + 1).getY();
            // Task calculate the distance between Point P(x, y) and Line [P1(x1,
            // y1), P2(x2, y2)] (less than 10 lines)
            // TODO
            double vx = x2 - x1;
            double vy = y2 - y1;

            double wx = x - x1;
            double wy = y - y1;

            double dotProduct = wx * vx + wy * vy;
            double squaredLength = vx * vx + vy * vy;

            double t = dotProduct / squaredLength;

            dist = (t < 0.0) ? std::sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1)) : (t > 1.0) ? std::sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2))
                : std::sqrt((x - (x1 + t * vx)) * (x - (x1 + t * vx)) + (y - (y1 + t * vy)) * (y - (y1 + t * vy)));

            if (dist < mindist)
                mindist = dist;
        }
        return mindist;
    }

    double Point::distance(const Polygon* polygon) const
    {
        LineString line = polygon->getExteriorRing();
        size_t n = line.numPoints();

        bool inPolygon = line.containsPoint(*this);
        // Task whether Point P(x, y) is within Polygon (less than 15 lines)
        // TODO

        double mindist = 0;
        if (!inPolygon)
        {
            mindist = this->distance(&line);
            return mindist;
        }
        else if (polygon->hasInteriorRing)
        {
            if (!polygon->hasInteriorRingList)
            {
                LineString line1 = polygon->getInteriorRing();
                if (line1.containsPoint(*this))
                {
                    mindist = this->distance(&line1);
                    return mindist;
                }
            }
            else
            {
                std::vector<LineString> list = polygon->getInteriorRingList();
                for (int i = 0; i < polygon->getInteriorRingList().size(); i++)
                {
                    LineString line1 = list[i];
                    if (line1.containsPoint(*this))
                    {
                        mindist = this->distance(&line1);
                        return mindist;
                    }
                }
            }
        }
        return mindist;
    }

    bool Point::intersects(const Envelope& rect) const
    {
        return (x >= rect.getMinX()) && (x <= rect.getMaxX()) &&
            (y >= rect.getMinY()) && (y <= rect.getMaxY());
    }

    void Point::draw() const
    {
        glBegin(GL_POINTS);
        glVertex2d(x, y);
        glEnd();
    }

    /*
     * LineString functions
     */
    void LineString::constructEnvelope()
    {
        double minX, minY, maxX, maxY;
        maxX = minX = points[0].getX();
        maxY = minY = points[0].getY();
        for (size_t i = 1; i < points.size(); ++i)
        {
            maxX = std::max(maxX, points[i].getX());
            maxY = std::max(maxY, points[i].getY());
            minX = std::min(minX, points[i].getX());
            minY = std::min(minY, points[i].getY());
        }
        envelope = Envelope(minX, maxX, minY, maxY);
    }

    bool LineString::lintersects(const LineString* line) const
    {
        if (!line->intersects(this->envelope))
            return false;
        else
        {
            for (int i = 0; i < line->numPoints() - 1; i++)
            {
                Point p1 = line->getPointN(i);
                Point p2 = line->getPointN(i + 1);
                for (int j = 0; j < this->numPoints() - 1; j++)
                {
                    Point q1 = this->getPointN(j);
                    Point q2 = this->getPointN(j + 1);

                    // ����ʵ��
                    double d1 = (q1.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (q1.getY() - p1.getY()) * (p2.getX() - p1.getX());
                    double d2 = (q2.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (q2.getY() - p1.getY()) * (p2.getX() - p1.getX());
                    double d3 = (p1.getX() - q1.getX()) * (q2.getY() - q1.getY()) - (p1.getY() - q1.getY()) * (q2.getX() - q1.getX());
                    double d4 = (p2.getX() - q1.getX()) * (q2.getY() - q1.getY()) - (p2.getY() - q1.getY()) * (q2.getX() - q1.getX());

                    if ((d1 * d2 <= 0) && (d3 * d4 <= 0))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool LineString::containsPoint(const Point& point) const
    {
        bool inPolygon = false;
        size_t n = this->numPoints();
        for (int i = 0; i < n; i++)
        {

            double x1 = this->getPointN(i).getX();
            double y1 = this->getPointN(i).getY();
            double x2 = this->getPointN((i + 1) % n).getX();
            double y2 = this->getPointN((i + 1) % n).getY();

            if (((y1 <= point.getY()) && (point.getY() < y2)) || ((y1 > point.getY()) && (point.getY() >= y2)) && ((y1 - point.getY()) * (y2 - point.getY()) <= 0))
            {
                if ((point.getY() - y1) * (x2 - x1) / (y2 - y1) + x1 > point.getX())
                {
                    inPolygon = !inPolygon;
                }
            }
        }
        return inPolygon;
    }

    double LineString::distance(const LineString* line) const
    {
        // TODO
        double mindist;
        if (line->lintersects(this))
            return 0;
        else
        {
            mindist = this->getPointN(0).distance(line);
            for (int i = 0; i < this->numPoints(); i++)
            {
                double dist = this->getPointN(i).distance(line);
                if (dist < mindist)
                    mindist = dist;
            }
            for (int i = 0; i < line->numPoints(); i++)
            {
                double dist = line->getPointN(i).distance(this);
                if (dist < mindist)
                    mindist = dist;
            }
        }
        return mindist;
    }

    double LineString::distance(const Polygon* polygon) const
    {
        // TODO
        LineString eline = polygon->getExteriorRing();
        int incount = 0;
        // �ж��Ƿ���ȫ���⻷��
        for (int j = 0; j < this->numPoints(); j++)
        {
            if (eline.containsPoint(this->getPointN(j)))
                incount++;
        }

        if (incount == this->numPoints()) // ȫ���⻷��
        {
            if (!polygon->hasInteriorRing) // û���ڻ�
            {
                return 0; // ��line string��polygon�ڣ�����Ϊ0
            }
            else if (!polygon->hasInteriorRingList) // ֻ��һ���ڻ�
            {
                LineString iline = polygon->getInteriorRing();
                int ininteriorcount = 0;
                for (int j = 0; j < this->numPoints(); j++)
                {
                    if (iline.containsPoint(this->getPointN(j)))
                    {
                        ininteriorcount++;
                    }
                }
                if (incount == this->numPoints()) // ������ڻ���
                {
                    return this->distance(&iline); // ����line string������ڻ��ľ���
                }
                else
                {
                    return 0; // ��polygon�ཻ������Ϊ0
                }
            }
            else // �ж���ڻ�
            {
                std::vector<LineString> list = polygon->getInteriorRingList();
                for (int k = 0; k < list.size(); k++) // �ж�ÿ���ڻ��Ƿ����line string
                {
                    LineString iline1 = list[k];
                    int ininteriorcount = 0;
                    for (int j = 0; j < this->numPoints(); j++)
                    {
                        if (iline1.containsPoint(this->getPointN(j)))
                        {
                            ininteriorcount++;
                        }
                    }
                    if (incount == this->numPoints())
                    {
                        return this->distance(&iline1);
                    }
                }
                // line string����ȫ���κ�һ���ڻ���
                return 0;
            }
        }
        // ����ȫ���⻷��
        return this->distance(&eline);
    }

    typedef int OutCode;

    const int INSIDE = 0; // 0000
    const int LEFT = 1;   // 0001
    const int RIGHT = 2;  // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8;    // 1000

    // Compute the bit code for a point (x, y) using the clip rectangle
    // bounded diagonally by (xmin, ymin), and (xmax, ymax)
    // ASSUME THAT xmax, xmin, ymax and ymin are global constants.
    OutCode ComputeOutCode(double x, double y, double xmin, double xmax,
        double ymin, double ymax)
    {
        OutCode code;

        code = INSIDE; // initialised as being inside of [[clip window]]

        if (x < xmin) // to the left of clip window
            code |= LEFT;
        else if (x > xmax) // to the right of clip window
            code |= RIGHT;
        if (y < ymin) // below the clip window
            code |= BOTTOM;
        else if (y > ymax) // above the clip window
            code |= TOP;

        return code;
    }

    // Cohen?CSutherland clipping algorithm clips a line from
    // P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with
    // diagonal from (xmin, ymin) to (xmax, ymax).
    bool intersectTest(double x0, double y0, double x1, double y1, double xmin,
        double xmax, double ymin, double ymax)
    {
        // compute outcodes for P0, P1, and whatever point lies outside the clip
        // rectangle
        OutCode outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
        OutCode outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);
        bool accept = false;

        while (true)
        {
            if (!(outcode0 | outcode1))
            {
                // bitwise OR is 0: both points inside window; trivially accept and
                // exit loop
                accept = true;
                break;
            }
            else if (outcode0 & outcode1)
            {
                // bitwise AND is not 0: both points share an outside zone (LEFT,
                // RIGHT, TOP, or BOTTOM), so both must be outside window; exit loop
                // (accept is false)
                break;
            }
            else
            {
                // failed both tests, so calculate the line segment to clip
                // from an outside point to an intersection with clip edge
                double x, y;

                // At least one endpoint is outside the clip rectangle; pick it.
                OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

                // Now find the intersection point;
                // use formulas:
                //   slope = (y1 - y0) / (x1 - x0)
                //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
                //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
                // No need to worry about divide-by-zero because, in each case, the
                // outcode bit being tested guarantees the denominator is non-zero
                if (outcodeOut & TOP)
                { // point is above the clip window
                    x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                    y = ymax;
                }
                else if (outcodeOut & BOTTOM)
                { // point is below the clip window
                    x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                    y = ymin;
                }
                else if (outcodeOut &
                    RIGHT)
                { // point is to the right of clip window
                    y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                    x = xmax;
                }
                else if (outcodeOut &
                    LEFT)
                { // point is to the left of clip window
                    y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                    x = xmin;
                }

                // Now we move outside point to intersection point to clip
                // and get ready for next pass.
                if (outcodeOut == outcode0)
                {
                    x0 = x;
                    y0 = y;
                    outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
                }
                else
                {
                    x1 = x;
                    y1 = y;
                    outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);
                }
            }
        }
        return accept;
    }

    bool LineString::intersects(const Envelope& rect) const
    {
        double xmin = rect.getMinX();
        double xmax = rect.getMaxX();
        double ymin = rect.getMinY();
        double ymax = rect.getMaxY();

        for (size_t i = 1; i < points.size(); ++i)
            if (intersectTest(points[i - 1].getX(), points[i - 1].getY(),
                points[i].getX(), points[i].getY(), xmin, xmax, ymin,
                ymax))
                return true;
        return false;
    }

    void LineString::draw() const
    {
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); ++i)
            glVertex2d(points[i].getX(), points[i].getY());
        glEnd();
    }

    void LineString::print() const
    {
        std::cout << "LineString(";
        for (size_t i = 0; i < points.size(); ++i)
        {
            if (i != 0)
                std::cout << ", ";
            std::cout << points[i].getX() << " " << points[i].getY();
        }
        std::cout << ")";
    }

    /*
     * Polygon
     */
    double Polygon::distance(const Polygon* polygon) const
    {
        return std::min(exteriorRing.distance(polygon),
            polygon->getExteriorRing().distance(this));
    }
    bool Polygon::intersects(const Envelope& rect) const
    {
        //如果包围盒不相交则快速判断为false
        if (!this->envelope.intersect(rect))
            return false;
        //记录包围盒的四个顶点
        Point p1 = Point(rect.getMinX(), rect.getMinY());
        Point p2 = Point(rect.getMinX(), rect.getMaxY());
        Point p3 = Point(rect.getMaxX(), rect.getMaxY());
        Point p4 = Point(rect.getMaxX(), rect.getMinY());
        //存入points
        std::vector<Point> points;
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        points.push_back(p1);
        //得到包围盒的边界线
        LineString enveline = LineString(points);

        // 处理有内环的情况
        if (this->hasInteriorRing)
        {
            if (!this->hasInteriorRingList)
            {
                LineString iline = this->getInteriorRing();
                // 如果内环完全包含了rect，那么这个rect一定不与polygon相交
                if (iline.containsPoint(p1) && iline.containsPoint(p2) && iline.containsPoint(p3) && iline.containsPoint(p4))
                    return false;
            }
            else
            {
                std::vector<LineString> list = this->getInteriorRingList();
                for (int i = 0; i < list.size(); i++)
                {
                    LineString iline = list[i];
                    // 如果某个内环完全包含了rect，那么这个rect一定不与polygon相交
                    if (iline.containsPoint(p1) && iline.containsPoint(p2) && iline.containsPoint(p3) && iline.containsPoint(p4))
                        return false;
                }
            }
        }
        // 检查多边形的顶点是否有在包围盒内的，若有则相交
        for (int i = 0; i < exteriorRing.numPoints(); i++)
        {
            if (rect.contain(exteriorRing.getPointN(i).getEnvelope()))
                return true;
        }
        // 检查包围盒的顶点是否有在多边形内的，若有则相交
        if (exteriorRing.containsPoint(p1))
            return true;
        if (exteriorRing.containsPoint(p2))
            return true;
        if (exteriorRing.containsPoint(p3))
            return true;
        if (exteriorRing.containsPoint(p4))
            return true;
        // 检查包围盒的边界线是否与多边形的边界线相交，若有则相交
        if (exteriorRing.lintersects(&enveline))
            return true;
        return false;
    }
    // bool Polygon::intersects(const Envelope &rect) const
    // {
    //     // TODO
    //     if (exteriorRing.intersects(rect))
    //     {
    //         return true;
    //     }
    //     if (this->hasInteriorRing)
    //     {
    //         if (!this->hasInteriorRingList)
    //         {
    //             LineString iline = this->getInteriorRing();
    //             if (iline.intersects(rect))
    //             {
    //                 return true;
    //             }
    //         }
    //         else
    //         {
    //             std::vector<LineString> list = this->getInteriorRingList();
    //             for (int i = 0; i < list.size(); i++)
    //             {
    //                 LineString iline = list[i];
    //                 if (iline.intersects(rect))
    //                 {
    //                     return true;
    //                 }
    //             }
    //         }
    //     }
    //     return false;
    // }

    void Polygon::draw() const { exteriorRing.draw(); }

    void Polygon::print() const
    {
        std::cout << "Polygon(";
        for (size_t i = 0; i < exteriorRing.numPoints(); ++i)
        {
            if (i != 0)
                std::cout << ", ";
            Point p = exteriorRing.getPointN(i);
            std::cout << p.getX() << " " << p.getY();
        }
        std::cout << ")";
    }

} // namespace hw6
