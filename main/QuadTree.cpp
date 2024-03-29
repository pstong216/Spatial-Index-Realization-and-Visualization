#include "QuadTree.h"
#include <set>

namespace hw6 {

    /*
     * QuadNode
     */
    void QuadNode::split(size_t capacity)
    {
        for (int i = 0; i < 4; ++i)
        {
            delete children[i];
            children[i] = nullptr;
        }

        // Task construction
        // TODO
        //????????????????
        Envelope subBoxes[4];
        double midX = (bbox.getMinX() + bbox.getMaxX()) / 2;
        double midY = (bbox.getMinY() + bbox.getMaxY()) / 2;
        subBoxes[0] = Envelope(bbox.getMinX(), midX, bbox.getMinY(), midY);
        subBoxes[1] = Envelope(midX, bbox.getMaxX(), bbox.getMinY(), midY);
        subBoxes[2] = Envelope(bbox.getMinX(), midX, midY, bbox.getMaxY());
        subBoxes[3] = Envelope(midX, bbox.getMaxX(), midY, bbox.getMaxY());
        for (int i = 0;i < 4;i++)
        {
            children[i] = new QuadNode(subBoxes[i]);
        }
        //????????????��?????????????
        for (int i = 0;i < features.size();i++)
        {
            for (int j = 0;j < 4;j++)
            {
                if (children[j]->getEnvelope().intersect(features[i].getGeom()->getEnvelope()))
                {
                    children[j]->add(features[i]);
                }
            }
        }
        //????????????????????
        // features.clear();
        //???????????????????????capacity?????????????
        for (int i = 0;i < 4;i++)
        {
            if (children[i]->getFeatureNum() > capacity)
            {
                children[i]->split(capacity);
            }
        }

    }

    void QuadNode::countNode(int& interiorNum, int& leafNum)
    {
        if (isLeafNode())
        {
            ++leafNum;
        }
        else
        {
            ++interiorNum;
            for (int i = 0; i < 4; ++i)
                children[i]->countNode(interiorNum, leafNum);
        }
    }

    int QuadNode::countHeight(int height)
    {
        ++height;
        if (!isLeafNode())
        {
            int cur = height;
            for (int i = 0; i < 4; ++i)
            {
                height = std::max(height, children[i]->countHeight(cur));
            }
        }
        return height;
    }


    void QuadNode::rangeQuery(const Envelope& rect,
        std::vector<Feature>& features)
    {
        if (!bbox.intersect(rect))
            return;

        // Task range query
        // TODO
        //?????????rect??????????????��??features??
        // ???????????????????????????????????
        if (isLeafNode())
        {
            for (const Feature& feature : this->features)
            {
                if (feature.getGeom()->getEnvelope().intersect(rect))
                {
                    features.push_back(feature);
                }
            }
        }
        else
        {
            // ??????????????????????????
            for (int i = 0; i < 4; ++i)
            {
                if (children[i] != nullptr && children[i]->getFeatureNum() > 0 && children[i]->getEnvelope().intersect(rect))
                    children[i]->rangeQuery(rect, features);
            }
        }

    }

    //     QuadNode* QuadNode::pointInLeafNode(double x, double y)
    //     {
    //         // Task NN query
    //         // TODO
    //         if (isLeafNode())
    //         {
    //             return this;
    //         }
    //         else
    //         {
    //             for (int i = 0;i < 4;i++)
    //             {
    //                 if (children[i]->bbox.contain(x, y))
    //                 {
    //                     return children[i]->pointInLeafNode(x, y);
    //                 }
    //             }
    //         }
    // 
    //         return nullptr;
    //     }
    QuadNode* QuadNode::pointInLeafNode(double x, double y)
    {
        // Task NN query
        // TODO
        if (isLeafNode() && this->getFeatureNum() > 0)
        {
            return this;
        }
        else
        {
            for (int i = 0;i < 4;i++)
            {
                if (children[i]->bbox.contain(x, y) && children[i]->getFeatureNum() > 0)
                {
                    QuadNode* node = children[i]->pointInLeafNode(x, y);
                    if (node != nullptr)
                    {
                        return children[i]->pointInLeafNode(x, y);
                    }
                    // return children[i]->pointInLeafNode(x, y);
                }
            }
            for (int i = 0;i < 4;i++)
            {
                if (children[i]->getFeatureNum() > 0)
                {
                    QuadNode* node = children[i]->pointInLeafNode(x, y);
                    if (node != nullptr)
                    {
                        return children[i]->pointInLeafNode(x, y);
                    }
                    // return children[i]->pointInLeafNode(x, y);
                }
            }
        }

        return nullptr;
    }
    void QuadNode::draw()
    {
        if (isLeafNode())
        {
            bbox.draw();
        }
        else
        {
            for (int i = 0; i < 4; ++i)
                children[i]->draw();
        }
    }

    /*
     * QuadTree
     */
    bool QuadTree::constructTree(const std::vector<Feature>& features)
    {
        if (features.empty())
            return false;

        // Task construction
        // TODO
        //????????��??
        bbox = Envelope(features[0].getGeom()->getEnvelope());
        for (int i = 0;i < features.size();i++)
        {
            bbox = bbox.unionEnvelope(features[i].getGeom()->getEnvelope());
        }
        //?????????
        root = new QuadNode(bbox);
        //????????????????
        root->add(features);
        //???features?????????capacity????????
        if (root->getFeatureNum() > capacity)
        {
            root->split(capacity);
        }

        return true;
    }

    void QuadTree::countNode(int& interiorNum, int& leafNum)
    {
        interiorNum = 0;
        leafNum = 0;
        if (root)
            root->countNode(interiorNum, leafNum);
    }

    void QuadTree::countHeight(int& height)
    {
        height = 0;
        if (root)
            height = root->countHeight(0);
    }

    void QuadTree::rangeQuery(const Envelope& rect,
        std::vector<Feature>& features)
    {
        features.clear();

        // Task range query
        // TODO

        // filter step (????????????��????��????????��???)
        if (root != nullptr)
        {
            root->rangeQuery(rect, features);
        }
        // ??????????????????????????????????hw6??rangeQuery?????
    }


    bool QuadTree::NNQuery(double x, double y, std::vector<Feature>& features)
    {
        if (!root || !(root->getEnvelope().contain(x, y)))
            return false;

        // Task NN query
        // TODO
        features.clear();
        QuadNode* leafNode = pointInLeafNode(x, y);
        // if (leafNode == nullptr || leafNode->getFeatureNum() == 0)
        // {
        //     double lenbboxx = 2 * (leafNode->getEnvelope().getMaxX() - leafNode->getEnvelope().getMinX());
        //     double lenbboxy = 2 * (leafNode->getEnvelope().getMaxY() - leafNode->getEnvelope().getMinY());
        //     Envelope rect = Envelope(x - lenbboxx, x + lenbboxx, y - lenbboxy, y + lenbboxy);
        //     rangeQuery(rect, features);
        //     return true;
        // }

        // filter step
        // 使用maxDistance2Envelope函数得到最短距离的最大值
        double minDist = INFINITY;
        for (int i = 0; i < leafNode->getFeatureNum(); i++)
        {
            minDist = std::min(minDist, leafNode->getFeature(i).maxDistance2Envelope(x, y));
        }
        Envelope rect = Envelope(x - minDist, x + minDist, y - minDist, y + minDist);
        rangeQuery(rect, features);

        // const Envelope &envelope = root->getEnvelope();
        // double minDist = std::max(envelope.getWidth(), envelope.getHeight());

        // 注意四叉树邻近查询仅返回候选集，精炼步在hw6的NNQuery中完成

        return true;
    }


    void QuadTree::draw()
    {
        if (root)
            root->draw();
    }
    Feature& QuadTree::pstNN(double x, double y, std::vector<Feature>& features)
    {
        double dist = INFINITY;
        int index = 0;
        for (int i = 0;i < features.size();i++)
        {
            double d = features[i].distance(x, y);
            if (d < dist)
            {
                index = i;
                dist = d;
            }
            // return;
        }
        return features[index];
    }

} // namespace hw6
