#include <QFile>
#include <QMap>
#include <QTextStream>

#include <functional>
#include <iostream>

static const QStringList numbersAsWord = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

static const QMap<QString, int> wordToNumber = {
    QPair<QString, int>("one", 1),
    QPair<QString, int>("two", 2),
    QPair<QString, int>("three", 3),
    QPair<QString, int>("four", 4),
    QPair<QString, int>("five", 5),
    QPair<QString, int>("six", 6),
    QPair<QString, int>("seven", 7),
    QPair<QString, int>("eight", 8),
    QPair<QString, int>("nine", 9),
};

struct NumberAppearance {

    explicit NumberAppearance (const int value, const int index) : value(value), index(index) {}
    explicit NumberAppearance (QString const& value, const int index) : value(wordToNumber.value(value)), index(index) {}

    int index;
    int value;
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

    parseFileLineByLine("input.txt", [&](QString const& line) -> void
    {
        QList<NumberAppearance> numberInLine;

        for (QString const& numberAsWord : numbersAsWord)
        {
            for (int i = 0; i < line.length(); ) {
                const int index = line.indexOf(numberAsWord, i);
                if (index != -1) {
                    NumberAppearance numberAppearance(numberAsWord, index);
                    numberInLine.append(numberAppearance);
                    i = index + numberAsWord.length();
                }
                i ++;
            }
        }

        for (int number = 1; number <= 9; number++) {
            for (int i = 0; i < line.length(); i++) {
                const int index = line.indexOf(QString::number(number), i);
                if (index != -1) {
                    NumberAppearance numberAppearance(number, index);
                    numberInLine.append(numberAppearance);
                }
            }
        }

        std::sort(numberInLine.begin(), numberInLine.end(),
                  [](NumberAppearance const& first, NumberAppearance const& second) -> bool {
                      return first.index < second.index;
                  }
        );
        const int firstDigit = numberInLine.first().value;
        const int lastDigit = numberInLine.last().value;
        fileValue += (firstDigit * 10 + lastDigit);
    });

    std::cout << "FileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
