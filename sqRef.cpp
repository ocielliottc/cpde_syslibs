/*
** Author: Carsten Arnholm, January 2010
** This code was written for my weather station project
**
** This code follows the sqlite3 license model, i.e.:
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

#include "sqRef.h"

sqRef::sqRef()
: m_table(0)
, m_id(-1)
{}

sqRef::sqRef(sqTable* table)
: m_table(table)
, m_id(-1)
{}

sqRef::sqRef(sqTable* table, IDint64 id)
: m_table(table)
, m_id(id)
{}

sqRef::sqRef(const sqRef& other)
: m_table(other.m_table)
, m_id(other.m_id)
{}

sqRef::~sqRef()
{
}
