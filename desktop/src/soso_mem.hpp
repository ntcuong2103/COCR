#ifndef _SOSO_MEM_HPP_
#define _SOSO_MEM_HPP_
#include <opencv2/core/mat.hpp>
#include <QWidget>
#include <QThread>
#include <QSharedMemory>
#include <QPointer>
#include <vector>
#include <string>

class MemDataGenerator : public QThread {
Q_OBJECT

    MemDataGenerator(const MemDataGenerator &) = delete;

    MemDataGenerator &operator=(const MemDataGenerator &) = delete;

    void updateParm();

    void reAllocateMem();

    std::string memKey;
    QSharedMemory mem;
    std::vector<QSharedMemory *> locks;
    bool stop;
    size_t sampleNum, imgWidth, imgHeight, imgChannel, labelSize;
    size_t sampleSize, imgSize, memSize, idx, sleepTime,count;
    std::unordered_map<std::string, size_t> jpgMap, txtMap;
public:

    size_t getSleepTime() const;

    void setSleepTime(size_t sleepTime);

    bool isStop() const;

    const std::string &getMemKey() const;

    void setMemKey(const std::string &memKey);

    MemDataGenerator();

    ~MemDataGenerator();

    void set(const size_t &_sampleNum, const size_t &_imgWidth, const size_t &_imgHeight, const size_t &_imgChannel,
             const size_t &_labelSize);

    void run() override;
    std::optional<cv::Mat> readImageFromMem(const char *_filename);
    std::optional<std::vector<std::tuple<int, float, float, float, float, float>>>
    readLabelFromMem(const char *_filename);
public slots:

    void exit_run();
};

class MCWidget : public QWidget {
Q_OBJECT
    MemDataGenerator *dg;
public:
    explicit MCWidget(QWidget *_parent = nullptr);

    void closeEvent(QCloseEvent *e) override;

signals:

    void sig_closed();
};


#endif//_SOSO_MEM_HPP_
