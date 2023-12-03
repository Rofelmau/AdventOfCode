#include <QFile>
#include <QTextStream>

#include <functional>
#include <iostream>

struct NumberAppearance {

    explicit NumberAppearance (const int value, const int line, const int index) : value(value), line (line), index(index) {}
    explicit NumberAppearance (QString const& value, const int line, const int index) : value(value.toUInt()), line(line), index(index) {}

    int value;
    int line;
    int index;
};

struct SymboleAppearance {

    explicit SymboleAppearance (QString const& symbole, const int line, const int index) : symbole(symbole), line (line), index(index) {}

    QString symbole;
    int line;
    int index;
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
    QList<NumberAppearance> numbersFound;
    QList<SymboleAppearance> symbolesFound;

    int lineIndex = 0;

    parseFileLineByLine("input.txt", [&](QString const& line) -> void
                        {
                            int letterIndex = 0;
                            QString number;
                            for (QChar const& letter : line)
                            {
                                if (letter.isDigit()) {
                                    number.append(letter);
                                }
                                else {
                                    if (!number.isEmpty()) {
                                        const int startIndex = letterIndex - number.length();
                                        const NumberAppearance numberAppearance(number, lineIndex, startIndex);
                                        numbersFound.append(numberAppearance);
                                        number.clear();
                                    }

                                    if (letter != QChar('.') && letter != QChar(' ')) {
                                        const SymboleAppearance symboleAppearance(letter, lineIndex, letterIndex);
                                        symbolesFound.append(symboleAppearance);
                                    }
                                }

                                letterIndex++;
                            }
                            if (!number.isEmpty()) {
                                const int startIndex = letterIndex - number.length();
                                const NumberAppearance numberAppearance(number, lineIndex, startIndex);
                                numbersFound.append(numberAppearance);
                                number.clear();
                            }
                            lineIndex++;
                        });

    for (SymboleAppearance const& symbole : symbolesFound) {
        if (symbole.symbole != "*") {
            continue;
        }
        QList<NumberAppearance> adjacentNumbers;
        for (NumberAppearance const& number : numbersFound) {
            if ((symbole.line >= (number.line - 1)) && (symbole.line <= (number.line + 1))) {
                if ((symbole.index >= (number.index - 1)) && (symbole.index <= (number.index + QString::number(number.value).length())) ) {
                    adjacentNumbers.append(number);
                }
            }
        }
        if (adjacentNumbers.count() == 2) {
            const int gearNumber = adjacentNumbers.first().value * adjacentNumbers.last().value;
            fileValue += gearNumber;
        }
    }

    std::cout << "numbers found = " << QString::number(numbersFound.size()).toStdString() << std::endl;
    std::cout << "symboles found = " << QString::number(symbolesFound.size()).toStdString() << std::endl;
    std::cout << "fileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
