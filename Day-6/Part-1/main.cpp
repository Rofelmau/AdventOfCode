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

QList<int> readNumbersFromLine(QString const& line) {
    QList<int> numbers;

    QString number;
    for (QChar const& letter : line) {
        if (letter.isDigit()) {
            number.append(letter);
        }
        else if (!number.isEmpty()) {
            numbers.append(number.toInt());
            number.clear();
        }
    }
    if (!number.isEmpty()) {
        numbers.append(number.toInt());
        number.clear();
    }

    return numbers;
}

int main(int, char *[])
{
    QList<int> time;
    QList<int> distance;

    QFile inputFile("input.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        time = readNumbersFromLine(in.readLine());
        distance = readNumbersFromLine(in.readLine());

        inputFile.close();
    }

    struct Race {
        int time;
        int distance;
    };

    QList<Race> races;
    for (int i = 0; i < time.length(); i++) {
        races.append( Race{.time = time[i], .distance = distance[i]});
    }

    QList<int> options = executeMultiThreaded<int, Race>(races, [](Race const& race) -> int {
        int distance = 0;
        int holdingTime = -1;
        do {
            holdingTime++;
            distance = (race.time - holdingTime) * holdingTime;
        } while (distance <= race.distance);

        const int minimumHoldingTime = holdingTime;


        holdingTime = race.time + 1;
        do {
            holdingTime--;
            distance = (race.time - holdingTime) * holdingTime;
        } while (distance <= race.distance);
        const int maxHoldingTime = holdingTime;

        return maxHoldingTime - minimumHoldingTime + 1;
    });

    int result = options.first();
    for (int i = 1; i < options.length(); i++) {
        result *= options[i];
    }

    std::cout << "result = " << QString::number(result).toStdString() << std::endl;

    return 0;
}
