#include "QuadTree.h"
#include "Common.h"
#include <ctime>
#include "CMakeIn.h"
#include <fstream>
#include <iomanip> 
#include <cmath>

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

    void QuadTree::analyse()
    {

        using namespace std;


        std::ofstream outFile("./QTree_resultnew.md");


        if (!outFile.is_open())
        {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }
        else
        {
            std::cout << "File opened for writing." << std::endl;
        }

        vector<Feature> features;
        vector<Geometry*> geom;
        vector<string> name;
        for (int dataSet = 0;dataSet < 3;dataSet++)
        {


            geom.clear();
            features.clear();
            name.clear();
            outFile << "*************************" << std::endl;
            if (dataSet == 0)
            {
                geom = readGeom(PROJ_SRC_DIR "/data/taxi");
                name = readName(PROJ_SRC_DIR "/data/taxi");
                std::cout << "- - - - - - - - - - - - texi test- - - - - - - - - - - - " << std::endl;
                // outFile << "texi test" << std::endl;
            }
            if (dataSet == 1)
            {
                geom = readGeom(PROJ_SRC_DIR "/data/highway");
                std::cout << "- - - - - - - - - - - - highway test- - - - - - - - - - - - " << std::endl;
                outFile << "highway test" << std::endl;
            }
            else if (dataSet == 2)
            {
                geom = readGeom(PROJ_SRC_DIR "/data/station");
                name = readName(PROJ_SRC_DIR "/data/station");
                std::cout << "- - - - - - - - - - - - station test- - - - - - - - - - - - " << std::endl;
                outFile << "station test" << std::endl;
            }

            features.reserve(geom.size());
            if (dataSet != 1)
            {
                for (size_t i = 0; i < geom.size(); ++i)
                    features.push_back(Feature(name[i], geom[i]));
            }
            else
            {
                for (size_t i = 0; i < geom.size(); ++i)
                    features.push_back(hw6::Feature(to_string(i), geom[i]));
            }
            cout << "data size: " << geom.size() << endl;

            // 写入表头
            outFile << "data size: " << geom.size() << endl;
            outFile << "| Capacity | Height | Interior node number | Leaf node number | Construction time| NNQuery time|RangeQuery time|\n";
            outFile << "|----------|----------|----------|----------|----------|----------|----------|\n";
            // srand(time(nullptr));
            for (int cap = 70; cap <= 200; cap += 10)
            {
                int continueCount = 0;
                QuadTree* qtree = new QuadTree();
                // Task 构造四叉树，输出四叉树的节点数目和高度
                // TODO 读入出租车数据

                clock_t start_time = clock();
                // TODO  统计建树时间
                qtree->setCapacity(cap);
                qtree->constructTree(features);
                clock_t end_time = clock();

                int height = 0, interiorNum = 0, leafNum = 0;
                // TODO 统计四叉树的高度，内部节点数目和叶子节点数目
                qtree->countHeight(height);
                qtree->countNode(interiorNum, leafNum);


                cout << "Capacity " << cap << "\n";
                cout << "Height: " << height
                    << " \tInterior node number: " << interiorNum
                    << " \tLeaf node number: " << leafNum << "\n";
                // CLOCKS_PER_SEC;
                double constructionTime = double(end_time - start_time) / CLOCKS_PER_SEC;
                cout << "Construction time: " << std::setprecision(3) << constructionTime << "s" << endl;


                double x, y;
                vector<Feature> candidateFeatures;
                Feature nearestFeature;
                start_time = clock();
                int cc = 0;
                int ec = 0;
                for (int i = 0; i < 10000; ++i)
                {
                    candidateFeatures.clear();
                    x = -((rand() % 225) / 10000.0 + 73.9812);
                    y = (rand() % 239) / 10000.0 + 40.7247;
                    qtree->NNQuery(x, y, candidateFeatures);
                    // Feature f = qtree->pstNN(x, y, features);

                    // refine step
                    // TODO
                    if (candidateFeatures.empty())
                    {
                        continueCount++;
                        continue;
                    }
                    Point p(x, y);
                    double minDist = candidateFeatures[0].getGeom()->distance(&p);
                    nearestFeature = candidateFeatures[0];
                    cc++;
                    for (int j = 1;j < candidateFeatures.size();j++)
                    {
                        double dist = candidateFeatures[j].getGeom()->distance(&p);
                        if (dist < minDist)
                        {
                            minDist = dist;
                            nearestFeature = candidateFeatures[j];
                        }
                    }

                    // double nx = nearestFeature.getEnvelope().getMinX();
                    // double ny = nearestFeature.getEnvelope().getMinY();

                    //                     double px = f.getEnvelope().getMinX();
                    //                     double py = f.getEnvelope().getMinY();
                    // 
                    // 
                    //                     if (fabs(nx - px) > 0.0000000001 || fabs(ny - py) > 0.0000000001)
                    //                     {
                    //                         cout << "相差较大" << endl;
                    //                         ec++;
                    //                     }
                    //                     std::cout << " " << std::setprecision(10) << x << " " << std::setprecision(10) << y << std::endl;
                    //                     nearestFeature.print();

                }
                cout << "cc: " << cc << endl;
                cout << "error count : " << ec << endl;
                end_time = clock();
                double queryTime = double(end_time - start_time) / CLOCKS_PER_SEC;
                cout << "NNQuery time: " << std::setprecision(3) << queryTime << "s"
                    << endl;

                // 测试 rangequery
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
                    qtree->rangeQuery(rect, candidateFeatures);//filter
                    selectedFeatures.clear();//refine
                    for (int j = 0;j < candidateFeatures.size();j++)
                    {
                        if (candidateFeatures[j].getGeom()->intersects(rect))
                        {
                            selectedFeatures.push_back(candidateFeatures[j]);
                        }
                    }

                }
                end_time = clock();
                double rangeQueryTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
                std::cout << "Range query time: " << std::setprecision(3) << rangeQueryTime << "s" << std::endl;;
                // outFile << "NNQuery time: " << std::setprecision(3) << double(end_time - start_time) / CLOCKS_PER_SEC << "s"
                //     << endl
                //     << endl;
                outFile << "|" << cap << " | " << height << " |" << interiorNum << " |" << leafNum << "|" << std::setprecision(3) << constructionTime << "|" << std::setprecision(3) << queryTime << "|" << std::setprecision(3) << rangeQueryTime << " |\n";
                // outFile << "continueCount: " << continueCount << std::endl;
                delete qtree;

            }
        }
        outFile.close();

    }

    void QuadTree::test(int t)
    {
        using namespace std;

        cout << "*********************Start*********************" << endl;
        if (t == TEST1)
        {
            cout << "测试1: Envelope Contain, Intersect, and Union" << endl;

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
            cout << "测试2: Distance between Point and LineString" << endl;

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
            cout << "测试3: Distance between Point and Polygon" << endl;

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
            cout << "测试4: QuadTree Construction" << endl;
            int ncase, cct;
            ncase = cct = 2;
            QuadTree qtree;
            vector<Geometry*> geom = readGeom(PROJ_SRC_DIR "/data/polygon");
            vector<Feature> features;

            for (size_t i = 0; i < geom.size(); ++i)
                features.push_back(Feature("", geom[i]));

            qtree.setCapacity(1);
            qtree.constructTree(features);

            int height, interiorNum, leafNum;
            qtree.countHeight(height);
            qtree.countNode(interiorNum, leafNum);

            if (!(height == 6 && interiorNum == 8 && leafNum == 25))
            {
                cout << "Case 1: "
                    << "Your answer is height: " << height
                    << ", interiorNum: " << interiorNum << ", leafNum: " << leafNum
                    << " for case1, but the answer is height: 6, interiorNum: 8, "
                    "leafNum: 25\n";
                --cct;
            }

            features.clear();
            for (size_t i = 0; i < geom.size(); ++i)
                delete geom[i];
            geom.clear();

            vector<Geometry*> geom2 = readGeom(PROJ_SRC_DIR "/data/highway");
            vector<Feature> features2;
            QuadTree qtree2;

            for (size_t i = 0; i < geom2.size(); ++i)
                features2.push_back(Feature("", geom2[i]));

            qtree2.setCapacity(20);
            qtree2.constructTree(features2);

            int height2, interiorNum2, leafNum2;
            qtree2.countHeight(height2);
            qtree2.countNode(interiorNum2, leafNum2);

            if (!(height2 == 11 && interiorNum2 == 1386 && leafNum2 == 4159))
            {
                cout << "Case 2: "
                    << "Your answer is height: " << height2
                    << ", interiorNum: " << interiorNum2
                    << ", leafNum: " << leafNum2
                    << " for case2, but the answer is height: 11, interiorNum: "
                    "1386, leafNum: 4159\n";
                --cct;
            }

            features2.clear();
            for (size_t i = 0; i < geom2.size(); ++i)
                delete geom2[i];
            geom2.clear();

            cout << "QuadTree Construction: " << cct << " / " << ncase
                << " tests are passed" << endl;
        }
        else if (t == TEST8)
        {
            cout << "测试8: QuadTreeAnalysis" << endl;
            analyse();
        }

        cout << "**********************End**********************" << endl;
    }

} // namespace hw6