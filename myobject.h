#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <iostream>
#include <qstring.h>

using namespace std;


class MyObject : public QObject
{

public:
    enum printMode{lineBreak,noLineBreak};
    enum listMode{noBreak,breakLines};

    void print(int val, printMode mode = printMode::lineBreak)
    {
        {
            if(mode == printMode::lineBreak)
                cout << val << endl;
            else
                cout << val;
        }
    }
    void print(QString text, printMode mode = printMode::lineBreak)
    {
        if(mode == printMode::lineBreak)
            cout << text.toStdString() << endl;
        else
            cout << text.toStdString();
    }
    void print(QStringList textList, listMode breakBetweenElemenets = listMode::noBreak ,printMode mode = printMode::lineBreak)
    {
        for(QString str : textList)
        {
            cout << str.toStdString();
            if(breakBetweenElemenets == listMode::breakLines)
                cout << endl;
        }
        if(mode == printMode::lineBreak)
            cout << endl;
    }

    void addEntry(const QString &entry)
    {
        entries << entry;
    }
    void extractEntries()
    {
        print("###Extracting string entries###");
        for (QString entry : entries)
        {
            print(entry);
        }
        print("###Done extracting###");

        entries.clear();
    }

signals:

public slots:

private:
    QStringList entries;
};

#endif // MYOBJECT_H
