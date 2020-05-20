#include <QString>
#include <QRegExp>

#include "Version.h"

class VersionPrivate
{
public:
    VersionPrivate()
        : majorNo(MajorNo), minorNo(MinorNo), subMinorNo(SubMinorNo)
    {}

    VersionPrivate(int theMajor, int theMinor, int theSubMinor)
        : majorNo(theMajor), minorNo(theMinor), subMinorNo(theSubMinor)
    {}

    int majorNo;
    int minorNo;
    int subMinorNo;

    static constexpr int MajorNo = 0;
    static constexpr int MinorNo = 9;
    static constexpr int SubMinorNo = 0;
    static const QString CodeName;
    static const QDate BuildDate;
};

const QString VersionPrivate::CodeName = QString("Anarchic Amoeba");

// Public

Version::Version()
    : d(new VersionPrivate())
{}

Version::Version(int majorNo, int minorNo, int subMinorNo)
    : d(new VersionPrivate(majorNo, minorNo, subMinorNo))
{}

Version::Version(const QString &version)
    : d(new VersionPrivate())
{
    QRegExp versionRegExp("^(\\d+)\\.(\\d+)\\.(\\d+)$");
    if (versionRegExp.indexIn(version) != -1) {
        d->majorNo = versionRegExp.cap(1).toInt();
        d->minorNo = versionRegExp.cap(2).toInt();
        d->subMinorNo = versionRegExp.cap(3).toInt();
    }
}

Version::~Version()
{
    delete d;
}

int Version::getMajor()
{
    return d->majorNo;
}

int Version::getMinor()
{
    return d->minorNo;
}

int Version::getSubminor()
{
    return d->subMinorNo;
}

QString Version::toString()
{
    return QString("%1.%2.%3").arg(d->majorNo).arg(d->minorNo).arg(d->subMinorNo);
}

bool Version::operator==(const Version &other)
{
    bool result;
    result = d->majorNo == other.d->majorNo && d->minorNo == other.d->minorNo && d->subMinorNo == other.d->subMinorNo;
    return result;
}

bool Version::operator>=(const Version &other)
{
    bool result;
    if (d->majorNo > other.d->majorNo) {
        result = true;
    } else if (d->majorNo < other.d->majorNo) {
        result = false;
    } else {
        if (d->minorNo > other.d->minorNo) {
            result = true;
        } else if (d->minorNo < other.d->minorNo) {
            result = false;
        } else {
            if (d->subMinorNo >= other.d->subMinorNo) {
                result = true;
            } else {
                result = false;
            }
        }
    }
    return result;
}

bool Version::operator<(const Version &other)
{
    return !(*this >= other);
}

QString Version::getCodeName()
{
    return VersionPrivate::CodeName;
}

QString Version::getApplicationVersion()
{
    Version version;
    return version.toString();
}


