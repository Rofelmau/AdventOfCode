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

int main(int, char *[])
{

    QList<uint> numbers;

    QFile inputFile("input.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        // Read Seeds
        QString line = in.readLine();

        QStringList seeds = line.split(" ");
        for (int index = 1; index < seeds.length(); index++) {
            const uint number = seeds[index].toUInt();
            index++;
            const uint range = seeds[index].toUInt();
            for (uint i = 0; i < range; i++) {
                numbers.append(number + i);
            }
        }

        //skip Empty Line
        in.readLine();

        while (!in.atEnd()) {
            //skip  line text line
            line = in.readLine();
            std::cout << line.toStdString() << std::endl;

            QList<Mapping> mappings;
            //read mappings
            line = in.readLine();
            while (!line.isEmpty()) {
                Mapping mapping;
                QStringList parts = line.split(" ");

                mapping.destinationStartValue = parts[0].toUInt();
                mapping.sourceStartValue = parts[1].toUInt();
                mapping.rangeLength = parts[2].toUInt();

                mappings.append(mapping);

                if (in.atEnd()) {
                    break;
                }
                line = in.readLine();
            }

            std::cout << "Mappings: " << QString::number(mappings.length()).toStdString() << std::endl;

            for (uint& number : numbers) {
                for (Mapping const& mapping : mappings) {
                    if ((number >= mapping.sourceStartValue) && (number < (mapping.sourceStartValue + mapping.rangeLength))) {
                        number += (mapping.destinationStartValue - mapping.sourceStartValue);
                        break;
                    }
                }
            }

        }

        inputFile.close();
    }

    uint lowestNumber = numbers.first();
    for (const uint number : numbers) {
        if (number < lowestNumber) {
            lowestNumber = number;
        }
    }

    std::cout << "lowestNumber = " << QString::number(lowestNumber).toStdString() << std::endl;

    return 0;
}
