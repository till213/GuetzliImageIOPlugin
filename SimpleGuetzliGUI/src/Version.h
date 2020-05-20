#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QDate>

class VersionPrivate;

/*!
 * The default constructor creates an instance which represents the current \em application version.
 * Other constructors exist to construct \em document versions, for example, which can be compared against
 * the \em application version.
 *
 * The version string has the form "[major].[minor].[subminor]", for example "1.2.3".
 */
class Version
{
public:
    /*!
     * Creates this Version with the current values of the application version.
     */
    Version();

    /*!
     * Creates this Version with \p majorNo, \p minorNo and \p subMinorNo values. This
     * Version can then be compared against the application version, for example.
     *
     * \param majorNo
     *        major version number
     * \param minorNo
     *        minor version number
     * \param subMinorNo
     *        sub-minor version number
     */
    Version(int majorNo, int minorNo, int subMinorNo);

    /*!
     * Creates this Version by parsing the \p version string value. This
     * Version can then be compared against the application version, for example.
     *
     * \param version
     *        a QString containing the version of the expected [major].[minor].[subminor]
     * \sa #operator<
     */
    Version(const QString &version);
    ~Version();

    /*!
     * Returns the major version number.
     *
     * \return the major version number
     */
    int getMajor();

    /*!
     * Returns the minor version number.
     *
     * \return the minor version number
     */
    int getMinor();

    /*!
     * Returns the subminor version number.
     *
     * \return the subminor version number
     */
    int getSubminor();

    /*!
     * Returns a string representation of the form "[major].[minor].[subminor]" of this Version
     * which can be used to construct another Version instance.
     *
     * \return a QString containing a user-friendly version value
     * \sa Version(const QString &)
     */
    QString toString();

    bool operator==(const Version &other);

    bool operator>=(const Version &other);

    bool operator<(const Version &other);

    /*!
     * A cool code name - every application needs this ;)
     *
     * \return a QString containing a cool code name
     */
    static QString getCodeName();

    /*!
     * The version number which follows the pattern version.minor.subminor.
     *
     * Example: 1.2.3
     *
     * \return a QString containing the application version.
     */
    static QString getApplicationVersion();

    /*!
     * The date on which the application was built.
     *
     * \return a QDate containing the build date
     */
    static QDate getBuildDate();

private:
    Q_DISABLE_COPY(Version)

    VersionPrivate *d;
};

#endif // VERSION_H
