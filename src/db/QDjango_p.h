/*
 * Copyright (C) 2010-2012 Jeremy Lainé
 * Contact: http://code.google.com/p/qdjango/
 *
 * This file is part of the QDjango Library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QDJANGO_P_H
#define QDJANGO_P_H

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QSharedDataPointer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#if defined(QDJANGO_SHARED)
#  if defined(QDJANGO_BUILD)
#    define QDJANGO_EXPORT Q_DECL_EXPORT
#  else
#    define QDJANGO_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QDJANGO_EXPORT
#endif

class QDjangoMetaModelPrivate;

/** \brief The QDjangoMetaField class holds the database schema for a field.
 *
 * \internal
 */
class QDJANGO_EXPORT QDjangoMetaField
{
public:
    QDjangoMetaField();
    QVariant toDatabase(const QVariant &value) const;

    QByteArray name;
    QVariant::Type type;
    bool autoIncrement;
    bool index;
    int maxLength;
    bool null;
    bool unique;
    QString foreignModel;
};

/** \brief The QDjangoMetaModel class holds the database schema for a model.
 *
 *  It manages table creation and deletion operations as well as row
 *  serialisation, deserialisation and deletion operations.
 *
 * \internal
 */
class QDJANGO_EXPORT QDjangoMetaModel
{
public:
    QDjangoMetaModel(const QObject *model = 0);
    QDjangoMetaModel(const QDjangoMetaModel &other);
    ~QDjangoMetaModel();
    QDjangoMetaModel& operator=(const QDjangoMetaModel &other);

    bool createTable() const;
    bool dropTable() const;

    void load(QObject *model, const QVariantList &props, int &pos) const;
    bool remove(QObject *model) const;
    bool save(QObject *model) const;

    QObject *foreignKey(const QObject *model, const char *name) const;
    void setForeignKey(QObject *model, const char *name, QObject *value) const;

    QList<QDjangoMetaField> localFields() const;
    QMap<QByteArray, QString> foreignFields() const;
    QByteArray primaryKey() const;
    QString table() const;

private:
    QSharedDataPointer<QDjangoMetaModelPrivate> d;
};

/** \brief The QDjangoDatabase class represents a set of connections to a
 *  database.
 *
 * \internal
 */
class QDjangoDatabase : public QObject
{
    Q_OBJECT

public:
    QDjangoDatabase(QObject *parent = 0);

    QSqlDatabase reference;
    QMutex mutex;
    QMap<QThread*, QSqlDatabase> copies;
    qint64 connectionId;

private slots:
    void threadFinished();
};

class QDJANGO_EXPORT QDjangoQuery : public QSqlQuery
{
public:
    QDjangoQuery(QSqlDatabase db);
    void addBindValue(const QVariant &val, QSql::ParamType paramType = QSql::In);
    bool exec();
    bool exec(const QString &query);
};

#endif
