#include <QFile>
#include <QTextStream>

#include <iostream>

int main(int, char *[])
{
    int fileValue = 0;
    QFile inputFile("input.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QList<QChar> digitsInLine;
            QString const& line = in.readLine();
            for (QChar const& letter : line)
            {
                if (letter.isDigit())
                {
                    digitsInLine.append(letter);
                }
            }

            if (digitsInLine.size() <= 0)
            {
                continue;
            }
            else if (digitsInLine.size() == 1)
            {
                const int digit = digitsInLine.first().digitValue();
                fileValue += (digit * 10 + digit);
            }
            else
            {
                const int firstDigit = digitsInLine.first().digitValue();
                const int lastDigit = digitsInLine.last().digitValue();
                fileValue += (firstDigit * 10 + lastDigit);
            }
        }
        inputFile.close();
    }

    std::cout << "FileValue = " << QString::number(fileValue).toStdString() << std::endl;

    return 0;
}
