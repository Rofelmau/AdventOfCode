#include <QtConcurrent>
#include <QFuture>
#include <QFile>
#include <QTextStream>

#include <functional>
#include <iostream>

void parseFileLineByLine(QString const& fileName, std::function<void(QString const&)> func)
{
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            func(in.readLine());
        }
        inputFile.close();
    }
}

template < typename ResultType, typename InputType >
QList<ResultType> executeMultiThreaded(QList<InputType> inputs, std::function<ResultType(InputType const&)> func)
{
    QList<QFuture<ResultType>> threads;

    for (InputType const& input : inputs) {
        QFuture<ResultType> future = QtConcurrent::run(QThreadPool::globalInstance(), func, input);
        threads.append(future);
    }

    QList<ResultType> results;
    for (QFuture<ResultType> thread : threads) {
        thread.waitForFinished();
        results.append(thread.result());
    }

    return results;
}

long long int readNumberFromLine(QString const& line) {

    QString number;
    for (QChar const& letter : line) {
        if (letter.isDigit()) {
            number.append(letter);
        }
    }

    return number.toLongLong();
}

int main(int, char *[])
{
    long long int time;
    long long int distance;

    QFile inputFile("input.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        time = readNumberFromLine(in.readLine());
        distance = readNumberFromLine(in.readLine());

        inputFile.close();
    }

    struct Race {
        long long int time;
        long long int distance;
    };

    Race race({.time = time, .distance = distance});
    QList<Race> races;
    races.append(race);

    QList<long long int> options = executeMultiThreaded<long long int, Race>(races, [](Race const& race) -> long long int {
        long long int distance = 0;
        long long int holdingTime = -1;
        do {
            holdingTime++;
            distance = (race.time - holdingTime) * holdingTime;
        } while (distance <= race.distance);

        const long long int minimumHoldingTime = holdingTime;


        holdingTime = race.time + 1;
        do {
            holdingTime--;
            distance = (race.time - holdingTime) * holdingTime;
        } while (distance <= race.distance);
        const long long int maxHoldingTime = holdingTime;

        return maxHoldingTime - minimumHoldingTime + 1;
    });

    long long int result = options.first();
    for (int i = 1; i < options.length(); i++) {
        result *= options[i];
    }

    std::cout << "result = " << QString::number(result).toStdString() << std::endl;

    return 0;
}
