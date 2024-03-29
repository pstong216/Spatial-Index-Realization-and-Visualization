#ifndef RTREE_SRC_H_INCLUDED
#define RTREE_SRC_H_INCLUDED

#include "Geometry.h"
#include "Tree.h"

#include <algorithm>
#include <array>
#include <queue>
#include <string>
#include <vector>

#include "CMakeIn.h"

namespace hw6 {

    /// <summary>
    /// </summary>
    /// <param name="M">Maximum number of children of each nodes.</param>
    template <uint8_t M> class RNode {//RNode 是一个模板类，表示 R 树的一个节点。M 是模板参数，代表节点的最大子节点数
    private:
        RNode<M>* parent = nullptr;
        std::array<RNode<M>*, M> children = []() {
            decltype(children) ret;
            for (decltype(M) i = 0; i < M; ++i)
                ret[i] = nullptr;
            return ret;
            }();
            decltype(M) childrenNum = 0;//子节点数
            Envelope bbox;
            std::vector<Feature> features;

    public:
        // int count;
        RNode() = delete;//禁用默认构造函数
        RNode(const Envelope& box) : bbox(box) {}//构造函数，传入一个MBR

        inline bool isLeafNode() { return childrenNum == 0; }//判断是否是叶子节点

        inline const Envelope& getEnvelope() { return bbox; }//返回节点的MBR

        inline RNode<M>* getParent() { return parent; }//返回父节点
        inline void setEnvelope(const Envelope& box) { bbox = box; }//设置节点的MBR

        inline RNode<M>* getChildNode(size_t i)
        {
            return i < childrenNum ? children[i] : nullptr;
        }//返回第i个子节点

        inline const RNode<M>* getChildNode(size_t i) const
        {
            return i < childrenNum ? children[i] : nullptr;
        }//返回第i个子节点

        inline decltype(M) getChildNum() const { return childrenNum; }//返回子节点数

        inline size_t getFeatureNum() const { return features.size(); }//返回节点中的Feature数

        inline const Feature& getFeature(size_t i) const { return features[i]; }//返回第i个Feature

        inline const std::vector<Feature>& getFeatures() const { return features; }//返回所有Feature

        inline void add(const Feature& f) { features.push_back(f); }//添加一个Feature

        inline void add(RNode<M>* child)
        {//添加一个子节点
            children[childrenNum] = child;
            child->parent = this;
            ++childrenNum;
        }

        inline void remove(const Feature& f)
        {//删除一个Feature
            auto where = [&]() {
                for (auto itr = features.begin(); itr != features.end(); ++itr)
                    if (itr->getName() == f.getName())
                        return itr;
                }();
                features.erase(where);
                if (features.empty())
                    features.shrink_to_fit(); // free memory unused but allocated
        }

        inline void remove(RNode<M>* child)
        {//删除一个子节点
            for (decltype(M) i = 0; i < childrenNum; ++i)
                if (children[i] == child)
                {
                    --childrenNum;
                    std::swap(children[i], children[childrenNum]);
                    children[childrenNum] = nullptr;
                    break;
                }
        }

        inline Feature popBackFeature()
        {//弹出最后一个Feature
            auto ret = features.back();
            features.pop_back();
            return ret;
        }

        inline RNode<M>* popBackChildNode()
        {//弹出最后一个子节点
            --childrenNum;
            auto ret = children[childrenNum];
            children[childrenNum] = nullptr;
            return ret;
        }

        void countNode(int& interiorNum, int& leafNum)
        {//计算节点数
            if (isLeafNode())
            {
                ++leafNum;
            }
            else
            {
                ++interiorNum;
                for (decltype(M) i = 0; i < childrenNum; ++i)
                    children[i]->countNode(interiorNum, leafNum);
            }
        }

        int countHeight(int height)
        {//计算高度
            ++height;
            if (!isLeafNode())
            {
                int cur = height;
                for (decltype(M) i = 0; i < childrenNum; ++i)
                    // height = max(height, children[i]->countHeight(cur));
                    height = height > children[i]->countHeight(cur) ? height : children[i]->countHeight(cur);
            }
            return height;
        }

        inline void draw()
        {//绘制节点
            if (isLeafNode())
            {

                // std::cout << "draw Leaf Node: " << &bbox << std::endl;
                bbox.draw();


            }
            else
                for (decltype(M) i = 0; i < childrenNum; ++i)
                    children[i]->draw();
        }

        void updateEnvelope()
        {//更新节点的MBR

            if (isLeafNode())
            {
                bbox = features[0].getGeom()->getEnvelope();
                for (decltype(M) i = 1; i < features.size(); ++i)
                    bbox = bbox.unionEnvelope(features[i].getGeom()->getEnvelope());

            }
            else
            {
                bbox = children[0]->getEnvelope();
                for (decltype(M) i = 1; i < childrenNum; ++i)
                    bbox = bbox.unionEnvelope(children[i]->getEnvelope());
            }
            if (parent != nullptr)//递归更新父节点的MBR
                parent->updateEnvelope();
        }

        void rangeQuery(const Envelope& rect, std::vector<Feature>& features)
        {//范围查询
// TODO
            if (!bbox.intersect(rect))//如果节点不相交，直接返回
                return;
            if (isLeafNode())
            {//如果是叶子节点
                for (auto& f : this->features)
                {//遍历所有的Feature
                    if (f.getEnvelope().intersect(rect))//如果Feature的包围盒相交
                        features.push_back(f);//将Feature放入结果数组
                }

                return;
            }
            else
            {//如果不是叶子节点
                for (decltype(M) i = 0; i < childrenNum; ++i)
                {//遍历所有的子节点
                    if (getChildNode(i)->getEnvelope().intersect(rect))//如果子节点相交
                        getChildNode(i)->rangeQuery(rect, features);//递归查询
                }
            }
        }

        RNode<M>* pickLeaf(Feature f)
        {//点查询
            if (isLeafNode())
            {//如果是叶子节点
                return this;
            }
            else
            {//如果不是叶子节点,找到相交或者最近的子节点
                for (decltype(M) i = 0; i < childrenNum; ++i)
                {   //如果子节点的MBR包含点，直接递归
                    if (getChildNode(i)->getEnvelope().contain(f.getEnvelope()))
                    {
                        return getChildNode(i)->pickLeaf(f);
                    }
                }
                //如果没有找到相交的子节点，找到最近的子节点
                double distance = INFINITY;
                int index = 0;//记录最近的子节点的索引

                for (int i = 0;i < childrenNum;i++)
                {
                    double s1 = f.getGeom()->getEnvelope().getArea();
                    double s2 = getChildNode(i)->getEnvelope().unionEnvelope(f.getGeom()->getEnvelope()).getArea();
                    double d = s2 - s1;

                    if (d < distance)
                    {
                        distance = d;
                        index = i;
                    }
                }
                return getChildNode(index)->pickLeaf(f);
                // return nullptr;
            }
        }

        RNode<M>* pointNearLeafNode(double x, double y)
        {//点查询
            //todo
            if (isLeafNode())//如果是叶子节点,返回该叶子节点
            {
                return this;
            }

            if (this->bbox.contain(x, y))//如果不是叶子节点，且包围盒包含点，找到包围点的子节点
            {
                for (int i = 0;i < childrenNum;i++)
                {
                    if (getChildNode(i)->getEnvelope().contain(x, y))//如果有包围点的子节点，递归
                    {
                        return getChildNode(i)->pointNearLeafNode(x, y);
                    }
                }

            }

            double distance = INFINITY;
            int index = 0;//记录最近的子节点的索引
            for (int i = 0;i < childrenNum;i++)
            {
                double s1 = getChildNode(i)->getEnvelope().getArea();
                double s2 = getChildNode(i)->getEnvelope().unionEnvelope(Point(x, y).getEnvelope()).getArea();
                double d = s2 - s1;
                // int index = 0;
                if (d < distance)
                {
                    distance = d;
                    index = i;
                }
            }
            return children[index]->pointNearLeafNode(x, y);

            // return nullptr;
        }
    };

    template <uint8_t M> class RTree : public Tree {//RTree 是一个模板类，表示 R 树。M 是模板参数，代表节点的最大子节点数
    private:
        // // Vars here MAY be useful, but it's ok to delete them {
        constexpr static auto m = M >> 1;
        // constexpr static auto M_minus_m = M - m;
        // constexpr static double EQ_ERR = 0.0000000005;
        // // }

        RNode<M>* root = nullptr;//根节点

    public:
        RTree() : Tree(M) { static_assert(M >= 4); }//构造函数，传入最大子节点数
        ~RTree()
        {//析构函数 递归删除根节点
            if (root != nullptr)
                delete root;
            root = nullptr;
        }

        virtual void setCapacity(int capacity) override
        {
            // DO NOTHING, since capacity is immutable in R tree
        }
        virtual void adjustTree(RNode<M>* leaf, RNode<M>* newleaf1, RNode<M>* newleaf2)
        {
            RNode<M>* parent = leaf->getParent();//获取父节点
            //递归调整树
            if (parent == nullptr)//如果父节点为空，说明是根节点,新建一个根节点
            {
                parent = new RNode<M>(newleaf1->getEnvelope().unionEnvelope(newleaf2->getEnvelope()));
                //释放旧的叶子节点
                leaf = nullptr;
                parent->add(newleaf1);
                parent->add(newleaf2);
                this->root = parent;
                return;
            }
            else
            {
                //首先判断父节点是否有空间添加新的子节点
                if (parent->getChildNum() < M)//如果父节点有空间添加新的子节点
                {

                    //释放旧的叶子节点
                    parent->remove(leaf);
                    leaf = nullptr;
                    // delete leaf;

                    //将新的两个叶子节点添加到父节点中
                    parent->add(newleaf1);
                    parent->add(newleaf2);

                    parent->updateEnvelope();
                    return;
                }
                else//如果父节点没有空间添加新的子节点，对父节点进行分裂
                {
                    parent->remove(leaf);
                    //delete leaff;
                    uint8_t TempM = M + (uint8_t)1;
                    std::vector<RNode<M>*> Rnodes;//建立一个临时的节点数组
                    for (int i = 0; i < parent->getChildNum(); i++)
                    {
                        Rnodes.push_back(parent->getChildNode(i));//将父节点的所有子节点放入临时节点数组
                    }

                    Rnodes.push_back(newleaf1);//将新的两个叶子节点放入临时节点数组
                    Rnodes.push_back(newleaf2);

                    RNode<M>* newLeaf1 = new RNode<M>(Rnodes[0]->getEnvelope());
                    RNode<M>* newLeaf2 = new RNode<M>(Rnodes[1]->getEnvelope());

                    split(Rnodes, newLeaf1, newLeaf2);

                    adjustTree(parent, newLeaf1, newLeaf2);

                }
            }

        }

        virtual void split(std::vector<RNode<M>*>& Rnodes, RNode<M>* newLeaf1, RNode<M>* newLeaf2)
        {
            int seedindex1 = 0;
            int seedindex2 = 1;
            double maxDistance = 0;
            for (int i = 0; i < Rnodes.size(); i++)
            {
                for (int j = i + 1; j < Rnodes.size(); j++)
                {
                    Envelope e1 = Rnodes[i]->getEnvelope();
                    Envelope e2 = Rnodes[j]->getEnvelope();
                    Envelope e = e1.unionEnvelope(e2);//合并两个MBR
                    double distance = e.getArea() - e1.getArea() - e2.getArea();
                    if (distance > maxDistance)
                    {
                        maxDistance = distance;
                        seedindex1 = i;
                        seedindex2 = j;
                    }
                }
            }
            RNode<M>* seed1 = Rnodes[seedindex1];
            RNode<M>* seed2 = Rnodes[seedindex2];
            //将种子点分别放入两个叶子节点
            // RNode<M>* newLeaf1 = new RNode<M>(seed1->getEnvelope());
            // RNode<M>* newLeaf2 = new RNode<M>(seed2->getEnvelope());
            newLeaf1->setEnvelope(seed1->getEnvelope());
            newLeaf2->setEnvelope(seed2->getEnvelope());
            newLeaf1->add(seed1);
            newLeaf2->add(seed2);
            //将剩余的几何特征分别放入两个叶子节点
            for (int i = 0; i < Rnodes.size(); i++)
            {
                if (i != seedindex1 && i != seedindex2)
                {
                    // int n1 = newLeaf1->getChildNum();
                    // int n2 = newLeaf2->getChildNum();
                    // if (n1 < m && ((int)M - n1 - n2 + 1 <= m - n1))//保证节点的叶子数大于等于m
                    // {
                    //     newLeaf1->add(Rnodes[i]);
                    //     newLeaf1->setEnvelope(newLeaf1->getEnvelope().unionEnvelope(Rnodes[i]->getEnvelope()));
                    // }
                    // else if (n2 < m && ((int)M - n1 - n2 <= m - n2))
                    // {
                    //     newLeaf2->add(Rnodes[i]);
                    //     newLeaf2->setEnvelope(newLeaf2->getEnvelope().unionEnvelope(Rnodes[i]->getEnvelope()));
                    // }
                    // else
                    {
                        Envelope e1 = newLeaf1->getEnvelope().unionEnvelope(Rnodes[i]->getEnvelope());
                        Envelope e2 = newLeaf2->getEnvelope().unionEnvelope(Rnodes[i]->getEnvelope());
                        double d1 = e1.getArea() - newLeaf1->getEnvelope().getArea();
                        double d2 = e2.getArea() - newLeaf2->getEnvelope().getArea();
                        if (d1 < d2)
                        {
                            newLeaf1->add(Rnodes[i]);//添加 node
                            newLeaf1->setEnvelope(e1);//更新包围盒
                        }
                        else
                        {
                            newLeaf2->add(Rnodes[i]);
                            newLeaf2->setEnvelope(e2);
                        }
                    }
                }
            }
        }
        virtual bool constructTree(const std::vector<Feature>& features) override
        {//构造树
            int count = 0;
            // TODO
            if (features.empty())//如果特征为空，返回false
                return false;
            //否则建树
            for (auto& f : features)
            {
                // std::cout << "读入features" << features.size() << std::endl;
                if (count == 0 || root == nullptr)
                {
                    root = new RNode<M>(f.getGeom()->getEnvelope());//用第一个feature的MBR作为根节点的MBR
                    root->add(f);//添加一个feature
                    count++;
                    // std::cout << "root添加一个feature" << std::endl;
                }
                else
                {
                    RNode<M>* leaf = pickLeaf(f);
                    // std::cout << "leaf: " << leaf << std::endl;
                    if (leaf != nullptr)
                    {
                        count++;
                        // std::cout << "获得leaf后，添加feature" << std::endl;
                        if (leaf->getFeatureNum() < M)//如果叶子节点的Feature数小于M，直接添加
                        {
                            int num = leaf->getFeatureNum();
                            // std::cout << "叶子节点的Feature数小于M，直接添加" << std::endl;
                            leaf->add(f);
                            // leaf->setEnvelope(leaf->getEnvelope().unionEnvelope(f.getGeom()->getEnvelope()));//更新包围盒
                            leaf->updateEnvelope();
                        }
                        else//如果叶子节点的Feature数等于M，分裂
                        {
                            // std::cout << "叶子节点的Feature数等于M，分裂" << std::endl;
                            leaf->add(f);//先添加,再分裂

                            //基于二次分裂(quadratic split)算法，选择最左和最右两个几何特征作为种子点，对几何特征进行分组

                            double maxDistance = 0;
                            int seed1Index = 0;
                            int seed2Index = 1;
                            for (int i = 0; i < leaf->getFeatureNum(); i++)
                            {
                                for (int j = i + 1; j < leaf->getFeatureNum(); j++)
                                {
                                    Envelope e1 = leaf->getFeature(i).getGeom()->getEnvelope();
                                    Envelope e2 = leaf->getFeature(j).getGeom()->getEnvelope();
                                    Envelope e = e1.unionEnvelope(e2);//合并两个MBR
                                    double distance = e.getArea() - e1.getArea() - e2.getArea();
                                    if (distance > maxDistance)
                                    {
                                        maxDistance = distance;
                                        seed1Index = i;
                                        seed2Index = j;
                                    }
                                }
                            }
                            Feature seed1 = leaf->getFeature(seed1Index);
                            Feature seed2 = leaf->getFeature(seed2Index);
                            RNode<M>* newLeaf1 = new RNode<M>(seed1.getGeom()->getEnvelope());//新建一个叶子节点
                            RNode<M>* newLeaf2 = new RNode<M>(seed2.getGeom()->getEnvelope());//新建一个叶子节点
                            //将种子点分别放入两个叶子节点
                            newLeaf1->add(seed1);
                            newLeaf2->add(seed2);
                            //将剩余的几何特征分别放入两个叶子节点
                            int* index = new int[m];
                            for (int i = 0; i < leaf->getFeatureNum(); i++)
                            {
                                if (i != seed1Index && i != seed2Index)
                                {
                                    // int n1 = newLeaf1->getFeatureNum();
                                    // int n2 = newLeaf2->getFeatureNum();
                                    // if (n1 < m && (int)M + 1 - n1 - n2 <= m - n1)
                                    // {
                                    //     newLeaf1->add(leaf->getFeature(i));//添加 feature
                                    //     newLeaf1->setEnvelope(newLeaf1->getEnvelope().unionEnvelope(leaf->getFeature(i).getGeom()->getEnvelope()));//更新包围盒
                                    // }
                                    // else if (n2 < m && (int)M + 1 - n1 - n2 <= m - n2)
                                    // {
                                    //     newLeaf2->add(leaf->getFeature(i));
                                    //     newLeaf2->setEnvelope(newLeaf2->getEnvelope().unionEnvelope(leaf->getFeature(i).getGeom()->getEnvelope()));
                                    // }
                                    // else
                                    {


                                        Envelope e1 = newLeaf1->getEnvelope().unionEnvelope(leaf->getFeature(i).getGeom()->getEnvelope());
                                        Envelope e2 = newLeaf2->getEnvelope().unionEnvelope(leaf->getFeature(i).getGeom()->getEnvelope());
                                        double d1 = e1.getArea() - newLeaf1->getEnvelope().getArea();
                                        double d2 = e2.getArea() - newLeaf2->getEnvelope().getArea();
                                        if (d1 < d2)
                                        {
                                            newLeaf1->add(leaf->getFeature(i));//添加 feature
                                            newLeaf1->setEnvelope(e1);//更新包围盒
                                        }
                                        else
                                        {
                                            newLeaf2->add(leaf->getFeature(i));
                                            newLeaf2->setEnvelope(e2);
                                        }
                                    }
                                }
                            }


                            //将原来节点的父节点设置为新建的两个叶子节点的父节点
                            // RNode<M>* parent = leaf->getParent();
                            adjustTree(leaf, newLeaf1, newLeaf2);

                        }
                        // 
                    }
                    else
                    {
                        return false;
                    }

                }
            }

            bbox = root->getEnvelope();
            // std::cout << "边界：" << "x: " << bbox.getMinX() << "-" << bbox.getMaxX() << std::endl << "y: " << bbox.getMinY() << "-" << bbox.getMaxY() << std::endl;

        }
        virtual void countNode(int& interiorNum, int& leafNum) override
        {//计算节点数
            interiorNum = leafNum = 0;
            if (root != nullptr)
                root->countNode(interiorNum, leafNum);
        }

        virtual void countHeight(int& height) override
        {//计算高度
            height = 0;
            if (root != nullptr)
                height = root->countHeight(height);
        }

        virtual void rangeQuery(const Envelope& rect,
            std::vector<Feature>& features) override
        {
            features.clear();
            if (root != nullptr)
                root->rangeQuery(rect, features);
        }

        virtual bool NNQuery(double x, double y,
            std::vector<Feature>& features) override
        { 
            features.clear();
            // TODO

            RNode<M>* leaf = pointNearLeafNode(x, y);
            if (leaf != nullptr)//得到叶子节点
            {
                double distance = INFINITY;
                int index = 0;
                // Point p(x, y);
                for (int i = 0;i < leaf->getFeatureNum();i++)
                {

                    double d = leaf->getFeature(i).maxDistance2Envelope(x, y);
                    if (d < distance)
                    {
                        distance = d;
                        index = i;
                        // features.push_back(leaf->getFeature(index));
                    }

                }
                //rangeQuery()
                double radius = leaf->getFeature(index).maxDistance2Envelope(x, y);
                Envelope rect(x - radius, y - radius, x + radius, y + radius);
                rangeQuery(rect, features);//范围查询，filter

            }


            else return false;
        }

        RNode<M>* pointNearLeafNode(double x, double y)
        {//
            if (root != nullptr)
                return root->pointNearLeafNode(x, y);
            else
                return nullptr;
        }


        RNode<M>* pickLeaf(Feature f)
        {//
            if (root != nullptr)
                return root->pickLeaf(f);
            else
                return nullptr;
        }

        virtual void draw() override
        {
            if (root != nullptr)
                root->draw();
        }

    public:
        static void test(int t);

        static void analyse();
    };

} // namespace hw6

#endif // !RTREE_SRC_H_INCLUDED
