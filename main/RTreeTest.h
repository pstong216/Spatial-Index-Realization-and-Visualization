#ifndef RTREE_TEST_H_INCLUDED
#define RTREE_TEST_H_INCLUDED

#include "Common.h"
#include "RTreeSrc.h"
#include <ctime>
#include <iomanip> 
#include "CMakeIn.h"
#include <fstream>


using namespace hw6;

extern int mode;
extern std::vector<Geometry*> readGeom(const char* filename);
extern std::vector<std::string> readName(const char* filename);
extern void transformValue(double& res, const char* format);
extern void wrongMessage(Envelope e1, Envelope e2, bool cal);
extern void wrongMessage(const Point& pt1, const Point& pt2, double dis,
    double res);
extern void wrongMessage(Envelope e1, Envelope e2, Envelope cal, Envelope res);

namespace hw6 {

    template <uint8_t M> void RTree<M>::test(int t)
    {
        using namespace std;

        std::cout << "*********************Start*********************" << std::endl;
        if (t == TEST1)
        {
            std::cout << "锟斤拷锟斤拷1: Envelope Contain, Intersect, and Union" << endl;

            int failedCase = 0;
            Envelope e1(-1, 1, -1, 1);
            vector<Envelope> tests;
            tests.push_back(Envelope(-0.5, 0.5, -0.5, 0.5));
            tests.push_back(Envelope(-0.5, 0.5, 0.5, 1.5));
            tests.push_back(Envelope(0.5, 1.5, -0.5, 0.5));
            tests.push_back(Envelope(-1.5, -0.5, -1.5, -0.5));
            tests.push_back(Envelope(-2, -1, -0.5, 0.5));
            tests.push_back(Envelope(1, 1.5, 1, 1.5));
            tests.push_back(Envelope(-2, -1.5, -0.5, 0.5));
            tests.push_back(Envelope(-0.5, 0.5, 1.5, 2));
            tests.push_back(Envelope(-2, -1.5, 0.5, 1.5));
            tests.push_back(Envelope(0.5, 1.5, 1.5, 2));

            for (size_t i = 0; i < tests.size(); ++i)
            {
                if (e1.contain(tests[i]) != (i == 0))
                {
                    failedCase += 1;
                    wrongMessage(e1, tests[i], (i != 0));
                }
                if (tests[i].contain(e1) == true)
                {
                    failedCase += 1;
                    wrongMessage(tests[i], e1, true);
                }
            }
            cout << "Envelope Contain: " << tests.size() * 2 - failedCase << " / "
                << tests.size() * 2 << " tests are passed" << endl;

            failedCase = 0;
            for (size_t i = 0; i < tests.size(); ++i)
            {
                if (e1.intersect(tests[i]) != (i < 6))
                {
                    failedCase += 1;
                    wrongMessage(e1, tests[i], (i < 6));
                }
                if (tests[i].intersect(e1) != (i < 6))
                {
                    failedCase += 1;
                    wrongMessage(tests[i], e1, (i < 6));
                }
            }
            cout << "Envelope Intersect: " << tests.size() * 2 - failedCase << " / "
                << tests.size() * 2 << " tests are passed" << endl;

            failedCase = 0;
            vector<Envelope> results;
            results.push_back(Envelope(-1, 1, -1, 1));
            results.push_back(Envelope(-1, 1, -1, 1.5));
            results.push_back(Envelope(-1, 1.5, -1, 1));
            results.push_back(Envelope(-1.5, 1, -1.5, 1));
            results.push_back(Envelope(-2, 1, -1, 1));
            results.push_back(Envelope(-1, 1.5, -1, 1.5));
            results.push_back(Envelope(-2, 1, -1, 1));
            results.push_back(Envelope(-1, 1, -1, 2));
            results.push_back(Envelope(-2, 1, -1, 1.5));
            results.push_back(Envelope(-1, 1.5, -1, 2));
            for (size_t i = 0; i < tests.size(); ++i)
            {
                if (e1.unionEnvelope(tests[i]) != results[i])
                {
                    failedCase += 1;
                    wrongMessage(e1, tests[i], e1.unionEnvelope(tests[i]),
                        results[i]);
                }
                if (tests[i].unionEnvelope(e1) != results[i])
                {
                    failedCase += 1;
                    wrongMessage(tests[i], e1, e1.unionEnvelope(tests[i]),
                        results[i]);
                }
            }
            cout << "Envelope Union: " << tests.size() * 2 - failedCase << " / "
                << tests.size() * 2 << " tests are passed" << endl;
        }
        else if (t == TEST2)
        {
            cout << "锟斤拷锟斤拷2: Distance between Point and LineString" << endl;

            vector<Point> points;
            points.push_back(Point(0, 0));
            points.push_back(Point(10, 10));
            LineString line(points);

            points.push_back(Point(-10, -10));
            points.push_back(Point(20, 20));
            points.push_back(Point(5, 5));
            points.push_back(Point(10, 0));
            points.push_back(Point(10, -10));
            points.push_back(Point(0, 10));
            points.push_back(Point(0, 20));
            points.push_back(Point(20, 0));

            double dists[] = { 0,       0,       14.1421, 14.1421, 0,
                              7.07107, 14.1421, 7.07107, 14.1421, 14.1421 };

            int failedCase = 0;
            for (size_t i = 0; i < points.size(); ++i)
            {
                double dist = points[i].distance(&line);
                if (fabs(dist - dists[i]) > 0.0001)
                {
                    failedCase += 1;
                    cout << "Your answer is " << dist << " for test between ";
                    line.print();
                    cout << " and ";
                    points[i].print();
                    cout << ", but the answer is " << dists[i] << endl;
                }
            }
            cout << "Distance between Point and LineString: "
                << points.size() - failedCase << " / " << points.size()
                << " tests are passed" << endl;
        }
        else if (t == TEST3)
        {
            cout << "锟斤拷锟斤拷3: Distance between Point and Polygon" << endl;

            vector<Point> points;
            points.push_back(Point(5, 0));
            points.push_back(Point(3, 6));
            points.push_back(Point(2, 4));
            points.push_back(Point(-2, 4));
            points.push_back(Point(-3, 5));
            points.push_back(Point(-5, 0));
            points.push_back(Point(0, -3));
            points.push_back(Point(5, 0));
            LineString line(points);
            Polygon poly(line);

            points.clear();
            points.push_back(Point(5, 4));
            points.push_back(Point(3, 4));
            points.push_back(Point(0, 4));
            points.push_back(Point(-3, 4));
            points.push_back(Point(-5, 4));
            points.push_back(Point(5, 5));
            points.push_back(Point(3, 5));
            points.push_back(Point(0, 5));
            points.push_back(Point(-3, 5));
            points.push_back(Point(0, 0));

            double dists[] = { 1.26491, 0, 0, 0, 1.48556, 1.58114, 0, 1, 0, 0 };

            int failedCase = 0;
            for (size_t i = 0; i < points.size(); ++i)
            {
                double dist = points[i].distance(&poly);
                if (fabs(dist - dists[i]) > 0.00001)
                {
                    failedCase += 1;
                    cout << "Your answer is " << dist << " for test between ";
                    poly.print();
                    cout << " and ";
                    points[i].print();
                    cout << ", but the answer is " << dists[i] << endl;
                }
            }
            cout << "Distance between Point and Polygon: "
                << points.size() - failedCase << " / " << points.size()
                << " tests are passed" << endl;
        }
        else if (t == TEST4)
        {
            cout << "锟斤拷锟斤拷4: RTree Construction" << endl;
            int ncase, cct;
            ncase = cct = 2;

            RTree<8> rtree;
            vector<Geometry*> geom = readGeom(PROJ_SRC_DIR "/data/station");
            vector<Feature> features;

            for (size_t i = 0; i < geom.size(); ++i)
                features.push_back(Feature("", geom[i]));

            rtree.constructTree(features);

            int height, interiorNum, leafNum;
            rtree.countHeight(height);
            rtree.countNode(interiorNum, leafNum);

            if (!(height == 4 && interiorNum == 18 && leafNum == 86))
            {
                cout << "Case 1: "
                    << "Your answer is height: " << height
                    << ", interiorNum: " << interiorNum << ", leafNum: " << leafNum
                    << ". One possible answer is height: 4, interiorNum: "
                    "18, "
                    "leafNum: 86\n";
                --cct;
            }

            features.clear();
            for (size_t i = 0; i < geom.size(); ++i)
                delete geom[i];
            geom.clear();

            vector<Geometry*> geom2 = readGeom(PROJ_SRC_DIR "/data/highway");
            vector<Feature> features2;
            RTree<8> rtree2;

            for (size_t i = 0; i < geom2.size(); ++i)
                features2.push_back(Feature("", geom2[i]));

            rtree2.constructTree(features2);

            int height2, interiorNum2, leafNum2;
            rtree2.countHeight(height2);
            rtree2.countNode(interiorNum2, leafNum2);

            if (!(height2 == 6 && interiorNum2 == 484 && leafNum2 == 2305))
            {
                cout << "Case 2: "
                    << "Your answer is height: " << height2
                    << ", interiorNum: " << interiorNum2
                    << ", leafNum: " << leafNum2
                    << ". One possible answer is height: 6, interiorNum: "
                    "484, leafNum: 2305\n";
                --cct;
            }

            features2.clear();
            for (size_t i = 0; i < geom2.size(); ++i)
                delete geom2[i];
            geom2.clear();

            //cout << "RTree Construction: " << cct << " / " << ncase
            //     << " tests are passed" << endl;
        }
        else if (t == TEST8)
        {
            cout << "锟斤拷锟斤拷8: RTreeAnalysis" << endl;
            analyse();
        }

        cout << "**********************End**********************" << endl;
    }

    template <uint8_t I, uint8_t Last, uint8_t Step>
    void forConstCapAnalyseRTree(const std::vector<Feature>& features)
    {
        if constexpr (I <= Last)
        {
            RTree<I> rtree;
            rtree.constructTree(features);

            // TODO

            forConstCapAnalyseRTree<I + Step, Last, Step>(features);
        }
    }

    template <uint8_t I, uint8_t Last, uint8_t Step>
    void rtreeTest(std::vector<Feature>& features, std::ofstream& outFile)
    {

        // outFile << "data size: " << features.size() << '\n';

        int continueCount = 0;
        //   统计建树时间
        std::cout << "data size: " << features.size() << std::endl;
        if constexpr (I <= Last)
        {
            clock_t start_time = clock();
            RTree<I>* rtree = new RTree<I>();
            rtree->constructTree(features);
            clock_t end_time = clock();


            double constructionTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            std::cout << "construct time: " << constructionTime << std::endl;

            int height = 0, interiorNum = 0, leafNum = 0;
            // TODO 统计四叉树的高度，内部节点数目和叶子节点数目
            rtree->countHeight(height);
            rtree->countNode(interiorNum, leafNum);
            std::cout << "height: " << height;
            std::cout << "  interiorNum: " << interiorNum;
            std::cout << "  leafNum: " << leafNum << std::endl;
            //测试 nnqury
            double x, y;
            std::vector<Feature> candidateFeatures;
            Feature nearestFeature;
            start_time = clock();
            for (int i = 0; i < 10000; ++i)
            {
                x = -((rand() % 225) / 10000.0 + 73.9812);
                y = (rand() % 239) / 10000.0 + 40.7247;
                rtree->NNQuery(x, y, candidateFeatures);
                // refine step
                if (candidateFeatures.empty())
                {
                    continueCount++;
                    continue;
                }
                Point p(x, y);
                double minDist = candidateFeatures[0].getGeom()->distance(&p);
                nearestFeature = candidateFeatures[0];
                for (int i = 1;i < candidateFeatures.size();i++)
                {
                    double dist = candidateFeatures[i].getGeom()->distance(&p);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        nearestFeature = candidateFeatures[i];
                    }
                }
            }
            end_time = clock();
            double queryTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            std::cout << "NNQuery time: " << std::setprecision(3) << queryTime << "s" << std::endl;

            //测试 rangequery
            //随机生成矩形
            double rx, ry, radius;
            std::vector<Feature> selectedFeatures;
            start_time = clock();
            for (int t = 0;t < 1000;t++)
            {
                candidateFeatures.clear();
                rx = -((rand() % 225) / 10000.0 + 73.9812);//-73.9812到-73.9588
                ry = (rand() % 239) / 10000.0 + 40.7247;//40.7247到40.7485
                radius = (rand() % 100) / 10000.0 + 0.0001;//0.0001到0.01
                Envelope rect(rx - radius, rx + radius, ry - radius, ry + radius);
                // std::vector<Feature> features;
                rtree->rangeQuery(rect, candidateFeatures);//filter
                selectedFeatures.clear();//refine
                for (int i = 0;i < candidateFeatures.size();i++)
                {
                    if (candidateFeatures[i].getGeom()->intersects(rect))
                    {
                        selectedFeatures.push_back(candidateFeatures[i]);
                    }
                }

            }
            end_time = clock();
            double rangeQueryTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            std::cout << "RangeQuery time: " << std::setprecision(3) << rangeQueryTime << "s"
                << std::endl
                << std::endl;
            outFile << "|" << features.size() << "|" << (int)I << " | " << height << " |" << interiorNum << " |" << leafNum << "|" << std::setprecision(3) << constructionTime << "|" << std::setprecision(3) << queryTime << "|" << std::setprecision(3) << rangeQueryTime << " |\n";
            outFile << "continueCount: " << continueCount << std::endl;
            delete rtree;


            rtreeTest<I + Step, Last, Step>(features, outFile);
        }
    }
    template <uint8_t M> void RTree<M>::analyse()
    {
        std::ofstream outFile("./RTree_result.md");
        if (!outFile.is_open())
        {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }
        else
        {
            std::cout << "File opened for writing." << std::endl;
        }
        outFile << "| Data size |M| Height | Interior node number | Leaf node number | Construction time| NNQuery time|RangeQuery time|\n";
        outFile << "|----------|----------|----------|----------|----------|----------|----------|----------|\n";



        using namespace std;
        //test taxi
        std::cout << "test taxi:" << std::endl;
        outFile << "test taxi:" << std::endl;
        vector<Feature> features;
        vector<Geometry*> geom = readGeom(PROJ_SRC_DIR "/data/taxi");
        vector<string> name = readName(PROJ_SRC_DIR "/data/taxi");

        features.clear();
        features.reserve(geom.size());
        for (size_t i = 0; i < geom.size(); ++i)
            features.push_back(Feature(name[i], geom[i]));

        rtreeTest<70, 200, 10>(features, outFile);

        //test highway
        std::cout << "highway test:" << std::endl;
        outFile << "highway test:" << std::endl;
        features.clear();
        geom.clear();
        name.clear();
        geom = readGeom(PROJ_SRC_DIR "/data/highway");
        // name = readName(PROJ_SRC_DIR "/data/highway");

        features.reserve(geom.size());
        for (size_t i = 0; i < geom.size(); ++i)
            features.push_back(hw6::Feature(to_string(i), geom[i]));

        rtreeTest<70, 200, 10>(features, outFile);

        //test station
        std::cout << "station test:" << std::endl;
        outFile << "station test:" << std::endl;
        features.clear();
        geom.clear();
        name.clear();
        geom = readGeom(PROJ_SRC_DIR "/data/station");
        name = readName(PROJ_SRC_DIR "/data/station");

        features.reserve(geom.size());
        for (size_t i = 0; i < geom.size(); ++i)
            features.push_back(Feature(name[i], geom[i]));

        rtreeTest<70, 200, 10>(features, outFile);

        outFile.close();

    }


} // namespace hw6

#endif // !RTREE_TEST_H_INCLUDED