#ifndef GCHUNITS_H
#define GCHUNITS_H

#include <QString>
#include <QAbstractListModel>

// U+2009 THIN SPACE = UTF-8 E2 80 89
#define REAL_THIN_SP_CP 0x2009
#define REAL_THIN_SP_UTF8 "\xE2\x80\x89"
#define REAL_THIN_SP_HTML "&thinsp;"

// U+200A HAIR SPACE = UTF-8 E2 80 8A
#define HAIR_SP_CP 0x200A
#define HAIR_SP_UTF8 "\xE2\x80\x8A"
#define HAIR_SP_HTML "&#8202;"

// U+2006 SIX-PER-EM SPACE = UTF-8 E2 80 86
#define SIXPEREM_SP_CP 0x2006
#define SIXPEREM_SP_UTF8 "\xE2\x80\x86"
#define SIXPEREM_SP_HTML "&#8198;"

// U+2007 FIGURE SPACE = UTF-8 E2 80 87
#define FIGURE_SP_CP 0x2007
#define FIGURE_SP_UTF8 "\xE2\x80\x87"
#define FIGURE_SP_HTML "&#8199;"

// QMessageBox seems to have a bug whereby it doesn't display thin/hair spaces
// correctly.  Workaround is to display a space in a small font.  If you
// change this, please test that it doesn't cause the parent span to start
// wrapping.
#define HTML_HACK_SP "<span style='white-space: nowrap; font-size: 6pt'> </span>"

// Define THIN_SP_* variables to be our preferred type of thin space
#define THIN_SP_CP   REAL_THIN_SP_CP
#define THIN_SP_UTF8 REAL_THIN_SP_UTF8
#define THIN_SP_HTML HTML_HACK_SP
/** GalaxyCash unit definitions. Encapsulates parsing and formatting
   and serves as list model for drop-down selection boxes.
*/
class GalaxyCashUnits: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GalaxyCashUnits(QObject *parent);

    /** GalaxyCash units.
      @note Source: https://en.galaxycash.it/wiki/Units . Please add only sensible ones
     */
    enum Unit
    {
        GCH,
        mGCH,
        uGCH
    };
    enum SeparatorStyle
    {
        separatorNever,
        separatorStandard,
        separatorAlways
    };
    //! @name Static API
    //! Unit conversion and formatting
    ///@{

    //! Get list of units, for drop-down box
    static QList<Unit> availableUnits();
    //! Is unit ID valid?
    static bool valid(int unit);
    //! Short name
    static QString name(int unit);
    //! Longer description
    static QString description(int unit);
    //! Number of Satoshis (1e-8) per unit
    static qint64 factor(int unit);
    //! Number of amount digits (to represent max number of coins)
    static int amountDigits(int unit);
    //! Number of decimals left
    static int decimals(int unit);
    //! Format as string
    static QString format(int unit, qint64 amount, bool plussign=false);
    //! Format as string (with unit)
    static QString formatWithUnit(int unit, qint64 amount, bool plussign=false);
    //! Format as string
    static QString format2(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);
    static QString simpleFormat(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);
    //! Format as string (with unit)
    static QString formatWithUnit2(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);
    static QString formatHtmlWithUnit(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);
    //! Format as string (with unit) but floor value up to "digits" settings
    static QString floorWithUnit(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);
    static QString floorHtmlWithUnit(int unit, const int64_t& amount, bool plussign=false, SeparatorStyle separators=separatorStandard);

    //! Parse string to coin amount
    static bool parse(int unit, const QString &value, qint64 *val_out);
    ///@}

    //! @name AbstractListModel implementation
    //! List model for unit drop-down selection box.
    ///@{
    enum RoleIndex {
        /** Unit identifier */
        UnitRole = Qt::UserRole
    };
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    ///@}

private:
    QList<GalaxyCashUnits::Unit> unitlist;
};
typedef GalaxyCashUnits::Unit GalaxyCashUnit;

#endif // GCHUNITS_H
