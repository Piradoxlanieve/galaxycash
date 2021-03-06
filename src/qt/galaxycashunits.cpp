#include "galaxycashunits.h"
#include "chainparams.h"
#include <QStringList>
#include <QSettings>

GalaxyCashUnits::GalaxyCashUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<GalaxyCashUnits::Unit> GalaxyCashUnits::availableUnits()
{
    QList<GalaxyCashUnits::Unit> unitlist;
    unitlist.append(GCH);
    unitlist.append(mGCH);
    unitlist.append(uGCH);
    return unitlist;
}

bool GalaxyCashUnits::valid(int unit)
{
    switch(unit)
    {
    case GCH:
    case mGCH:
    case uGCH:
        return true;
    default:
        return false;
    }
}

QString GalaxyCashUnits::name(int unit)
{
    switch(unit)
    {
    case GCH: return QString("GCH");
    case mGCH: return QString("mGCH");
    case uGCH: return QString::fromUtf8("μGCH");
    default: return QString("???");
    }
}

QString GalaxyCashUnits::description(int unit)
{
    switch(unit)
    {
    case GCH: return QString("GalaxyCashs");
    case mGCH: return QString("Milli-GalaxyCashs (1 / 1,000)");
    case uGCH: return QString("Micro-GalaxyCashs (1 / 1,000,000)");
    default: return QString("???");
    }
}

qint64 GalaxyCashUnits::factor(int unit)
{
    switch(unit)
    {
    case GCH:  return 100000000;
    case mGCH: return 100000;
    case uGCH: return 100;
    default:   return 100000000;
    }
}

int GalaxyCashUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case GCH: return 8; // 21,000,000 (# digits, without commas)
    case mGCH: return 11; // 21,000,000,000
    case uGCH: return 14; // 21,000,000,000,000
    default: return 0;
    }
}

int GalaxyCashUnits::decimals(int unit)
{
    switch(unit)
    {
    case GCH: return 8;
    case mGCH: return 5;
    case uGCH: return 2;
    default: return 0;
    }
}

QString GalaxyCashUnits::format(int unit, qint64 n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Right-trim excess zeros after the decimal point
    int nTrim = 0;
    for (int i = remainder_str.size()-1; i>=2 && (remainder_str.at(i) == '0'); --i)
        ++nTrim;
    remainder_str.chop(nTrim);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}

QString GalaxyCashUnits::formatWithUnit(int unit, qint64 amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

QString GalaxyCashUnits::format2(int unit, const int64_t& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Right-trim excess zeros after the decimal point
    int nTrim = 0;
    for (int i = remainder_str.size()-1; i>=2 && (remainder_str.at(i) == '0'); --i)
        ++nTrim;
    remainder_str.chop(nTrim);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}

// TODO: Review all remaining calls to GalaxyCashUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: BtcoinUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString GalaxyCashUnits::formatWithUnit2(int unit, const int64_t& amount, bool plussign, SeparatorStyle separators)
{
    return format2(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString GalaxyCashUnits::formatHtmlWithUnit(int unit, const int64_t& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit2(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString GalaxyCashUnits::floorWithUnit(int unit, const int64_t& amount, bool plussign, SeparatorStyle separators)
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

    QString result = format2(unit, amount, plussign, separators);
    if(decimals(unit) > digits) result.chop(decimals(unit) - digits);

    return result + QString(" ") + name(unit);
}

QString GalaxyCashUnits::floorHtmlWithUnit(int unit, const int64_t& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool GalaxyCashUnits::parse(int unit, const QString &value, qint64 *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);
    QStringList parts = value.split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    qint64 retvalue = str.toLongLong(&ok);
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

int GalaxyCashUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant GalaxyCashUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
