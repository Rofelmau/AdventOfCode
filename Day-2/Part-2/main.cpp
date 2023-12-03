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

int main(int, char *[])
{
    int fileValue = 0;

    parseFileLineByLine("input.txt", [&](QString const& line) -> void
                        {
                            auto getHighestValue = [&] (QString const& searchColor) -> int {
                                int highesValue = 0;
                                for (QString const& a: line.split(" " + searchColor)) {
                                    const QString number = a.split(" ").last();
                                    if (number.toInt() > highesValue) {
                                        highesValue = number.toInt();
                                    }
                                }

                                return highesValue;
                            };

                            const int redCubesNeeded = getHighestValue("red");
                            const int greenCubesNeeded = getHighestValue("green");
                            const int blueCubesNeeded = getHighestValue("blue");

                            fileValue += redCubesNeeded*greenCubesNeeded*blueCubesNeeded;

                        });

    std::cout << "FileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
