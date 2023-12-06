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

struct Mapping {
    uint sourceStartValue;
    uint destinationStartValue;
    uint rangeLength;
};

struct Seed {
    uint startValue;
    uint range;
};

int main(int, char *[])
{

    QList<Seed> seeds;
    QList<QList<Mapping>> mappings;

    QFile inputFile("input.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        // Read Seeds
        QString line = in.readLine();

        QStringList seedsFound = line.split(" ");
        for (int index = 1; index < seedsFound.length(); index++) {

            Seed seed;
            seed.startValue = seedsFound[index].toUInt();
            index++;
            seed.range = seedsFound[index].toUInt();

            seeds.append(seed);
        }

        //skip Empty Line
        in.readLine();

        while (!in.atEnd()) {
            //skip  line text line
            line = in.readLine();
            std::cout << line.toStdString() << std::endl;

            QList<Mapping> m;
            //read mappings
            line = in.readLine();
            while (!line.isEmpty()) {
                Mapping mapping;
                QStringList parts = line.split(" ");

                mapping.destinationStartValue = parts[0].toUInt();
                mapping.sourceStartValue = parts[1].toUInt();
                mapping.rangeLength = parts[2].toUInt();

                m.append(mapping);

                if (in.atEnd()) {
                    break;
                }
                line = in.readLine();
            }

            std::cout << "Mappings: " << QString::number(mappings.length()).toStdString() << std::endl;

            mappings.append(m);
        }

        inputFile.close();
    }



    auto getLowestValueForSeedGroup = [](Seed const& seed, QList<QList<Mapping>> const& mappings) -> uint {
        QList<uint> numbers;

        for (uint i = 0; i < seed.range; i++) {
            numbers.append(seed.startValue + i);
        }

        for (QList<Mapping> maps : mappings) {
            for (uint& number : numbers) {
                for (Mapping const& mapping : maps) {
                    if ((number >= mapping.sourceStartValue) && (number < (mapping.sourceStartValue + mapping.rangeLength))) {
                        number += (mapping.destinationStartValue - mapping.sourceStartValue);
                        break;
                    }
                }
            }
        }

        uint lowestNumber = numbers.first();
        for (const uint number : numbers) {
            if (number < lowestNumber) {
                lowestNumber = number;
            }
        }

        return lowestNumber;
    };

    QList<QFuture<uint>> threads;

    for (Seed seed : seeds) {
        QFuture<uint> future = QtConcurrent::run(QThreadPool::globalInstance(), getLowestValueForSeedGroup, seed, mappings);
        threads.append(future);
    }

    QList<uint> lowestNumbers;
    for (QFuture<uint> thread : threads) {
        thread.waitForFinished();
        lowestNumbers.append(thread.result());
    }

    uint lowestNumber = lowestNumbers.first();
    for (const uint number : lowestNumbers) {
        if (number < lowestNumber) {
            lowestNumber = number;
        }
    }

    std::cout << "lowestNumber = " << QString::number(lowestNumber).toStdString() << std::endl;

    return 0;
}
