#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <string>
#include "Geometry.h"
#include <cmath>

namespace hw6 {


  class Feature {
  private:
    std::string name;
    Geometry* geom;

  public:
    Feature() : geom(NULL) {}
    Feature(const std::string& name, Geometry* geom) : name(name), geom(geom) {}

    const std::string& getName() const { return name; }

    const Geometry* getGeom() const { return geom; }

    const Envelope& getEnvelope() const { return geom->getEnvelope(); }

    double maxDistance2Envelope(double x, double y) const
    {
      const Envelope& e = geom->getEnvelope();
      double x1 = e.getMinX();
      double y1 = e.getMinY();
      double x2 = e.getMaxX();
      double y2 = e.getMaxY();

      double d1 = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      double d2 = sqrt((x - x1) * (x - x1) + (y - y2) * (y - y2));
      double d3 = sqrt((x - x2) * (x - x2) + (y - y1) * (y - y1));
      double d4 = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

      return std::max(std::max(d1, d2), std::max(d3, d4));
    }

    double distance(double x, double y) const
    {
      Point point(x, y);
      return geom->distance(&point);
    }

    void print() const
    {
      std::cout << "Feature: " << name << " ";
      geom->print();
    }

    void draw() const
    {
      if (geom)
        geom->draw();
    }
  };

  class Tree {
  protected:
    size_t capacity;
    Envelope bbox;

  public:
    Tree(size_t cap) : capacity(cap) {}

    int getCapacity() const { return capacity; }

    virtual void setCapacity(int capacity) { this->capacity = capacity; }

    const Envelope& getEnvelope() const { return bbox; }

    virtual bool constructTree(const std::vector<Feature>& features) = 0;

    virtual void countNode(int& interiorNum, int& leafNum) = 0;

    virtual void countHeight(int& height) = 0;

    virtual void rangeQuery(const Envelope& rect,
      std::vector<Feature>& features) = 0;

    virtual bool NNQuery(double x, double y,
      std::vector<Feature>& features) = 0;

    virtual void draw() = 0;
  };

  class Pair {
  private:
    Feature f1;
    Feature f2;
  public:
    Pair() {}
    Pair(const Feature& f1, const Feature& f2) : f1(f1), f2(f2) {}

    const Feature& getFirst() const { return f1; }
    const Feature& getSecond() const { return f2; }

    void print(int i) const
    {
      std::cout << "Pairs" << i << ": " << std::endl;
      f1.print();
      std::cout << std::endl;
      f2.print();
      std::cout << std::endl;
    }


  };

} // namespace hw6

#endif // !TREE_H_INCLUDED

