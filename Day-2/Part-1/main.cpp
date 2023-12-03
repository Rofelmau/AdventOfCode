#include <QFile>
#include <QTextStream>

#include <functional>
#include <iostream>

static const QStringList colors = {
    "green",
    "red",
    "blue"
};

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

    const int redAvailable = 12;
    const int greenAvailable = 13;
    const int blueAvailable = 14;

    parseFileLineByLine("input.txt", [&](QString const& line) -> void
    {
        const QString gameId = line.split("Game ").last().split(":").first();

        auto areColorPullsValid = [&] (QString const& searchColor, const int limit) -> bool {
            for (QString const& a: line.split(" " + searchColor)) {
                const QString number = a.split(" ").last();
                if (number.toInt() > limit) {
                    return false;
                }
            }
            return true;
        };

        const bool redCubesFound = areColorPullsValid("red", redAvailable);
        const bool greenCubesFound = areColorPullsValid("green", greenAvailable);
        const bool blueCubesFound = areColorPullsValid("blue", blueAvailable);

        if (redCubesFound && greenCubesFound && blueCubesFound) {
            fileValue += gameId.toInt();
        }

    });

    std::cout << "FileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
