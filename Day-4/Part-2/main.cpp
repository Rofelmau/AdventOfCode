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

    struct Card {

        explicit Card (const int points) : points(points), instances(1) {}

        int points = 0;
        int instances = 0;
    };

    QList<Card> cards;

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
                points++;
            }
        }

        cards.append(Card(points));
    });

    for (int index = 0; index < cards.length(); index++) {
        Card const& card = cards[index];

        for (int subIndex = (index + 1); (subIndex <= (index + card.points)) && (subIndex < cards.length()); subIndex++) {
            cards[subIndex].instances += card.instances;
        }

        fileValue += card.instances;
    }

    std::cout << "fileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
