#ifndef _COUCH_H_
#define _COUCH_H_

#include "cocr.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QTextCodec>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace cocr {
    class StrokeData {
    private:
        unsigned short int strokePointNum;
        typedef cv::Point Point;
        std::vector<Point> points;
    public:
        void transform(const cv::Mat &affineTransform) {
//            vector<cv::Point> src={
//
//            },dst={
//
//            };
//            auto t= cv::getAffineTransform(src,dst);
            std::vector<cv::Point2f> src = {
                    cv::Point2f(0, 0),
                    cv::Point2f(0, 20),
                    cv::Point2f(10, 10)
            }, dst = {
                    cv::Point2f(5, 0),
                    cv::Point2f(10, 5),
                    cv::Point2f(10, 0)
            };
            auto t = cv::getAffineTransform(src.data(), dst.data());
            cv::Mat res;
            cv::multiply(t, cv::Vec3f(5, 5, 1).t(), res);
            show(res);
        }

        decltype(points.size()) getPointSize() const {
            return points.size();
        }

        const decltype(points) &getPoints() const {
            return points;
        }

        void setStrokePointNum(unsigned short strokePointNum) {
            StrokeData::strokePointNum = strokePointNum;
        }

        StrokeData() : strokePointNum(0) {}

        StrokeData(unsigned short int _strokePointNum) :
                strokePointNum(_strokePointNum) {
            points.resize(strokePointNum);
        }

        friend class SampleData;

        friend inline std::istream &operator>>(std::istream &in, StrokeData &sd) {
//              in >> sd.strokePointNum;
            in.read((char *) &sd.strokePointNum, 2);
            sd.points.resize(sd.strokePointNum);
            for (auto &point:sd.points) {
//                  in >> point.first >> point.second;
                in.read((char *) &point.x, 2);
                in.read((char *) &point.y, 2);
            }
            return in;
        }

        friend inline std::ostream &operator<<(std::ostream &out, const StrokeData &sd) {
            out << "next stroke with " << sd.strokePointNum << " points" << std::endl;
            for (const auto &point:sd.points) {
                out << "(" << point.x << "," << point.y << "), ";
            }
            out << std::endl;
            return out;
        }

        friend inline QDataStream &operator<<(QDataStream &s, const StrokeData &o) {
            s << o.strokePointNum;
            for (auto &point:o.points) {
                s << point.x << point.y;
            }
            return s;
        }

        friend inline QDataStream &operator>>(QDataStream &s, StrokeData &o) {
            s >> o.strokePointNum;
            o.points.resize(o.strokePointNum);
            for (auto &point:o.points) {
                s >> point.x >> point.y;
            }
            return s;
        }
    };

    class SampleData {
    private:
        unsigned short int lineNum;
        std::vector<StrokeData> strokes;
        std::string label;
        int width, height;
    public:
        int getWidth() const {
            return width;
        }

        void setWidth(int width) {
            SampleData::width = width;
        }

        int getHeight() const {
            return height;
        }

        void setHeight(int height) {
            SampleData::height = height;
        }

        decltype(strokes.size()) getStrokeSize() const {
            return strokes.size();
        }

        const decltype(strokes) &getStrokes() const {
            return strokes;
        }

        const std::string &getLabel() const {
            return label;
        }

        void setLabel(const std::string &label) {
            SampleData::label = label;
        }

        void setLineNum(unsigned short lineNum) {
            SampleData::lineNum = lineNum;
        }

        void wash() {
            auto it = strokes.begin();
            while (it != strokes.end()) {
                if (it->getPointSize() < 1) {
                    it = strokes.erase(it);
                    lineNum--;
                } else {
                    it++;
                }
            }
        }

        void visualize(int thickness = 3, const std::string &dumpToFile = "") {
            auto getOffsetPoint = [&](const StrokeData::Point &p) -> cv::Point {
                return p + cv::Point(floor(thickness / 2), floor(thickness / 2));
            };
            auto getBoundingSize = [&](int w, int h, int k = 1) -> cv::Size {
                return cv::Size(w, h) * k + cv::Size(thickness, thickness);
            };
            const auto black = cv::Scalar(0), white = cv::Scalar(255), gray = cv::Scalar(127.5);
            cv::Mat image(getBoundingSize(getWidth(), getHeight()), CV_8UC1, white);
            for (const auto &stroke:strokes) {
                const auto &points = stroke.points;
                if (points.size() == 0)continue;
                cv::line(image, getOffsetPoint(points[0]), getOffsetPoint(points[0]),
                         black, thickness, cv::LINE_AA);
                for (auto i = 1; i < points.size(); i++) {
                    cv::line(image, getOffsetPoint(points[i - 1]), getOffsetPoint(points[i]),
                             black, thickness, cv::LINE_AA);
                }
            }
            cv::Mat panel(getBoundingSize(getWidth(), getHeight(), 3), CV_8UC1, white);
            auto rect = cv::Rect(cv::Point(getWidth(), getHeight()),
                                 getBoundingSize(getWidth(), getHeight()));
            image.copyTo(panel(rect));
            cv::rectangle(panel, rect, gray, 1);
            LogPressKey2Continue();
            if (dumpToFile.empty()) {
                cv::imshow("display", panel);
                cv::waitKey(0);
            } else {
                cv::imwrite(dumpToFile, panel);
                getchar();
            }
        }

        bool normalize() {
            auto xmax = std::numeric_limits<int>::min(),
                    xmin = std::numeric_limits<int>::max();
            auto ymax = xmax, ymin = xmin;
            for (const auto &stroke:strokes) {
                const auto &points = stroke.points;
                for (auto &point:points) {
                    xmax = xmax > point.x ? xmax : point.x;
                    ymax = ymax > point.y ? ymax : point.y;
                    xmin = xmin < point.x ? xmin : point.x;
                    ymin = ymin < point.y ? ymin : point.y;
                }
            }
            for (auto &stroke:strokes) {
                auto &points = stroke.points;
                for (auto &point:points) {
                    point.x -= xmin;
                    point.y -= ymin;
                }
            }
            setWidth(xmax - xmin);
            setHeight(ymax - ymin);
            if (getWidth() < 0 || getWidth() > 2000 || getHeight() < 0 || getHeight() > 2000) {
                return false;
            }
            return true;
        }

        SampleData() : lineNum(0), width(0), height(0) {}

        SampleData(unsigned short int _lineNum) : lineNum(_lineNum) {
            strokes.resize(_lineNum);
        }

        friend inline std::istream &operator>>(std::istream &in, SampleData &s) {
//              in>>s.lineNum;
            s.strokes.resize(s.lineNum);
            for (auto &stroke:s.strokes) {
                in >> stroke;
            }
            return in;
        }

        friend inline std::ostream &operator<<(std::ostream &out, const SampleData &s) {
            out << "sample \"" << s.label << "\" has " << s.lineNum << " strokes" << std::endl;
            for (auto &stroke:s.strokes) {
                out << stroke;
            }
            return out;
        }

        friend inline QDataStream &operator<<(QDataStream &s, const SampleData &o) {
            s << o.height << o.width << o.lineNum << QString(o.getLabel().c_str());
            for (auto &stroke:o.strokes) {
                s << stroke;
            }
            return s;
        }

        friend inline QDataStream &operator>>(QDataStream &s, SampleData &o) {
            QString index;
            s >> o.height >> o.width >> o.lineNum >> index;
            o.setLabel(index.toStdString());
            o.strokes.resize(o.lineNum);
            for (auto &stroke:o.strokes) {
                s >> stroke;
            }
            return s;
        }
    };

    class CouchReader {
    public:
        static CouchReader &getInstance() {
            static CouchReader cr;
            return cr;
        }

        void washDataSet(std::vector<SampleData> &src) {
            auto it = src.begin();
            while (it != src.end()) {
                if (it->getStrokeSize() < 1) {// 删除笔迹数量为空的样本
                    it = src.erase(it);
                } else {
                    it->wash();// 删除包含点数为空的笔迹
                    if (it->normalize()) {// 移动到坐标原点，记录边框大小
                        it++;
                    } else { // 边框取值异常
                        it = src.erase(it);
                    }
                }
            }
        }

        void readCouchTopDir(const std::string &couchTopDir, std::vector<SampleData> &dst) {
            QDir dir(couchTopDir.c_str());
            dir.setFilter(QDir::Files | QDir::Hidden | QDir::AllDirs);
            QFileInfoList fileInfoList = dir.entryInfoList();
//            qDebug() << dir << fileInfoList;
            while (!fileInfoList.isEmpty()) {
                QFileInfo tempFileInfo = fileInfoList.last();
                if (!tempFileInfo.isDir()) {
                    if (tempFileInfo.suffix() == "idx") {
                        readIdxFile(tempFileInfo.absoluteFilePath().toStdString(), dst);
                    }
                    fileInfoList.removeLast();
                } else if (tempFileInfo.fileName() != "." && tempFileInfo.fileName() != "..") {
                    QDir subDir(tempFileInfo.filePath());
                    fileInfoList.removeLast();
                    fileInfoList.append(subDir.entryInfoList());
//                  qDebug() << tempFileInfo;
                } else {
                    fileInfoList.removeLast();
//                  qDebug() << tempFileInfo;
                }
            }
        }

        void readIdxFile(const std::string &idxFileName, std::vector<SampleData> &dst) {
            using namespace std;
            if (idxFileName.find(".idx") == string::npos) {
                cerr << "fail to open: " << idxFileName << std::endl;
                return;
            }
            ifstream idxIfsm(idxFileName, ios::in | ios::binary);
            if (!idxIfsm.is_open()) {
                cerr << "fail to open: " << idxFileName << std::endl;
                return;
            }
//          idxIfsm >> sampleSum;
            idxIfsm.read((char *) &sampleSum, 4);
            string datFileName = idxFileName;
            auto suffixIndex = datFileName.find_last_of(".");
            datFileName.replace(suffixIndex + 1, 3, "dat");
            LogName(datFileName);
            ifstream datIfsm(datFileName, ios::in | ios::binary);
            if (!datIfsm.is_open()) {
                datFileName.replace(suffixIndex + 1, 3, "DAT");
                datIfsm.open(datFileName, ios::in | ios::binary);
                if (!datIfsm.is_open()) {
                    cerr << "fail to open: " << datFileName << std::endl;
                    return;
                }
            }
            while (idxIfsm.read((char *) &sampleState, 1)) {
//              idxIfsm >> oswIndex >> idxIndex >> datOffset;
                idxIfsm.read((char *) &oswIndex, 4);
                idxIfsm.read((char *) &idxIndex, 4);
                idxIfsm.read((char *) &datOffset, 4);
                datIfsm.seekg(datOffset, ios::beg);
//              datIfsm >> wordLength;
                datIfsm.read((char *) &wordLength, 1);
                wordCode = new unsigned char[1 * (size_t) wordLength + 1];
                datIfsm.read((char *) wordCode, 1 * (size_t) wordLength);
                wordCode[1 * (size_t) wordLength] = '\0';
//              datIfsm >> pointNum >> lineNum >> getTimePointNum;
                datIfsm.read((char *) &pointNum, 2);
                datIfsm.read((char *) &lineNum, 2);
                datIfsm.read((char *) &getTimePointNum, 2);
                dst.emplace_back(SampleData(lineNum));
                SampleData &sample = dst.back();
                sample.setLabel((char *) wordCode);
                getTimePointIndex = new unsigned short int[2 * (size_t) getTimePointNum];
                elapsedTime = new int[4 * (size_t) getTimePointNum];
                datIfsm.read((char *) getTimePointIndex, 2 * (size_t) getTimePointNum);
                datIfsm.read((char *) elapsedTime, 4 * (size_t) getTimePointNum);
                datIfsm >> sample;
                freeUselessParm();
                LogLine(sample);
            }
            datIfsm.close();
            idxIfsm.close();
            LogLine("done.");
        }

    private:
        CouchReader() : wordCode(nullptr),
                        getTimePointIndex(nullptr),
                        elapsedTime(nullptr) {}

        ~CouchReader() { freeUselessParm(); }

        CouchReader(const CouchReader &) = delete;

        const CouchReader &operator=(const CouchReader &) = delete;

        void freeUselessParm() {
            if (getTimePointIndex) {
                delete getTimePointIndex;
                getTimePointIndex = nullptr;
            }
            if (elapsedTime) {
                delete elapsedTime;
                elapsedTime = nullptr;
            }
            if (wordCode) {
                delete wordCode;
                wordCode = nullptr;
            }
        }

        long sampleSum;                         //4
        unsigned char sampleState;              //1
        int oswIndex;                           //4
        int idxIndex;                           //4
        int datOffset;                          //4
        unsigned char wordLength;               //1
        unsigned char *wordCode;                //1*WordLength
        unsigned short int pointNum;            //2
        unsigned short int lineNum;             //2
        unsigned short int getTimePointNum;     //2
        unsigned short int *getTimePointIndex;  //2*getTimePointNum
        int *elapsedTime;                       //4*getTimePointNum
    };

    inline bool loadCouchFromFile(const std::string &saveDir, std::vector<SampleData> &samples) {
        using namespace std;
        auto &couchReader = CouchReader::getInstance();
        // 从序列化文件加载数据
        QFile f((saveDir + "/couch.txt").c_str());
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen()) { return false; }
        QDataStream reader(&f);
        int size;
        reader >> size;
        samples.resize(size);
        for (auto &s:samples) { reader >> s; }
        f.close();
//        cout << "load " << samples.size() << " samples from " << saveDir <<std::endl;
        random_shuffle(samples.begin(), samples.end());
        couchReader.washDataSet(samples);
//        cout << "after validation, " << samples.size() << " samples exist" <<std::endl;
        return true;
    }

    inline bool dumpCouchToFile(const std::string &couchTopDir, const std::string &saveDir) {
        using namespace std;
        vector<SampleData> samples;
        auto &couchReader = CouchReader::getInstance();
        // 读原始数据集，序列化成单独文件
        couchReader.readCouchTopDir(couchTopDir, samples);
        couchReader.washDataSet(samples);
        // 转换标签
        unordered_set<string> labelSet;
        unordered_map<string, int> labelMap;
        for (auto &s : samples) { labelSet.insert(s.getLabel()); }
        int i = labelSet.size();
        for (const auto &l:labelSet) { labelMap.insert(make_pair(l, i--)); }
        for (auto &s:samples) { s.setLabel(to_string(labelMap.find(s.getLabel())->second)); }
        // 写点序列数据
        QFile f((saveDir + "/couch.txt").c_str());
        f.open(QIODevice::WriteOnly);
        if (!f.isOpen()) { return false; }
        QDataStream writter(&f);
        writter << (int) samples.size();
        for (auto &s:samples) { writter << s; }
        f.close();
        // 写标签数据
        ofstream ofsm(saveDir + "/labelSet.txt", ios::out);
        if (!ofsm.is_open()) { return false; }
        for (auto &l : labelMap) { ofsm << l.first << " " << l.second << std::endl; }
        ofsm.close();
        return true;
    }
}
#endif //_COUCH_H_
