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

    parseFileLineByLine("input.txt", [&](QString const& line) -> void {

        const QString card = line.split(":").last();
        const QString winningNumbersString = card.split("|").first();
        const QString myNumbersString = card.split("|").last();

        auto getNumbersFromString = [](QString const& string) -> QList<int> {
            QList<int> numbers;

            QString numberAsString;
            for (QChar const& letter : string)
            {
                if (letter.isDigit()) {
                    numberAsString.append(letter);
                }
                else {
                    if (!numberAsString.isEmpty()) {
                        numbers.append(numberAsString.toInt());
                        numberAsString.clear();
                    }
                }
            }
            if (!numberAsString.isEmpty()) {
                numbers.append(numberAsString.toInt());
                numberAsString.clear();
            }

            return numbers;
        };

        QList<int> const& winningNumbers = getNumbersFromString(winningNumbersString);
        QList<int> const& myNumbers = getNumbersFromString(myNumbersString);

        int points = 0;
        for (int const myNumber: myNumbers) {
            if (winningNumbers.contains(myNumber)) {
                if (points == 0) {
                    points++;
                } else {
                    points *= 2;
                }
            }
        }

        fileValue += points;
    });

    std::cout << "fileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
