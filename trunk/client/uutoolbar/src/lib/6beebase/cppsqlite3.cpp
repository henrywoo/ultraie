// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "cppsqlite3.h"
#include <cstdlib>
#include "uulogging.h"
#include "6bees_util.h"
// Named constant for passing to CppSQLite3Exception when passing it a string that cannot be deleted.
static const bool DONT_DELETE_MSG=false;
// Prototypes for SQLite functions not included in SQLite DLL, but copied below from SQLite encode.c
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out);
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out);
CppSQLite3Exception::CppSQLite3Exception(const int nErrCode, char* szErrMess, bool bDeleteMsg/*=true*/):
mnErrCode(nErrCode)
{
  mpszErrMess = sqlite3_mprintf("%s[%d]: %s", errorCodeAsString(nErrCode), nErrCode, szErrMess ? szErrMess : "");
  if (bDeleteMsg && szErrMess){
    sqlite3_free(szErrMess);
  }
}
CppSQLite3Exception::CppSQLite3Exception(const CppSQLite3Exception&  e):
mnErrCode(e.mnErrCode)
{
  mpszErrMess = 0;
  if (e.mpszErrMess){
    mpszErrMess = sqlite3_mprintf("%s", e.mpszErrMess);
  }
}
const char* CppSQLite3Exception::errorCodeAsString(int nErrCode){
  switch (nErrCode){
    case SQLITE_OK          : return "SQLITE_OK";
    case SQLITE_ERROR       : return "SQLITE_ERROR";
    case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
    case SQLITE_PERM        : return "SQLITE_PERM";
    case SQLITE_ABORT       : return "SQLITE_ABORT";
    case SQLITE_BUSY        : return "SQLITE_BUSY";
    case SQLITE_LOCKED      : return "SQLITE_LOCKED";
    case SQLITE_NOMEM       : return "SQLITE_NOMEM";
    case SQLITE_READONLY    : return "SQLITE_READONLY";
    case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
    case SQLITE_IOERR       : return "SQLITE_IOERR";
    case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
    case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
    case SQLITE_FULL        : return "SQLITE_FULL";
    case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
    case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
    case SQLITE_EMPTY       : return "SQLITE_EMPTY";
    case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
    case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
    case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
    case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
    case SQLITE_MISUSE      : return "SQLITE_MISUSE";
    case SQLITE_NOLFS       : return "SQLITE_NOLFS";
    case SQLITE_AUTH        : return "SQLITE_AUTH";
    case SQLITE_FORMAT      : return "SQLITE_FORMAT";
    case SQLITE_RANGE       : return "SQLITE_RANGE";
    case SQLITE_ROW         : return "SQLITE_ROW";
    case SQLITE_DONE        : return "SQLITE_DONE";
    case CPPSQLITE_ERROR    : return "CPPSQLITE_ERROR";
    default: return "UNKNOWN_ERROR";
  }
}
CppSQLite3Exception::~CppSQLite3Exception(){
  if (mpszErrMess){
    sqlite3_free(mpszErrMess);
    mpszErrMess = 0;
  }
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Buffer::CppSQLite3Buffer(){
  mpBuf = 0;
}
CppSQLite3Buffer::~CppSQLite3Buffer(){
  clear();
}
void CppSQLite3Buffer::clear(){
  if (mpBuf){
    sqlite3_free(mpBuf);
    mpBuf = 0;
  }
}
const char* CppSQLite3Buffer::format(const char* szFormat, ...){
  clear();
  va_list va;
  va_start(va, szFormat);
  mpBuf = sqlite3_vmprintf(szFormat, va);
  va_end(va);
  return mpBuf;
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Binary::CppSQLite3Binary() : mpBuf(0), mnBinaryLen(0), mnBufferLen(0), mnEncodedLen(0), mbEncoded(false){ }
CppSQLite3Binary::~CppSQLite3Binary(){
  clear();
}
void CppSQLite3Binary::setBinary(const unsigned char* pBuf, int nLen){
  mpBuf = allocBuffer(nLen);
  memcpy(mpBuf, pBuf, nLen);
}
void CppSQLite3Binary::setEncoded(const unsigned char* pBuf){
  clear();
  mnEncodedLen = (int)strlen((const char*)pBuf);
  mnBufferLen = mnEncodedLen + 1; // Allow for NULL terminator
  mpBuf = (unsigned char*)malloc(mnBufferLen);
  if (!mpBuf){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Cannot allocate memory", DONT_DELETE_MSG);
  }
  memcpy(mpBuf, pBuf, mnBufferLen);
  mbEncoded = true;
}
const unsigned char* CppSQLite3Binary::getEncoded(){
  if (!mbEncoded){
    unsigned char* ptmp = (unsigned char*)malloc(mnBinaryLen);
    memcpy(ptmp, mpBuf, mnBinaryLen);
    mnEncodedLen = sqlite3_encode_binary(ptmp, mnBinaryLen, mpBuf);
    free(ptmp);
    mbEncoded = true;
  }
  return mpBuf;
}
const unsigned char* CppSQLite3Binary::getBinary(){
  if (mbEncoded){
    // in/out buffers can be the same
    mnBinaryLen = sqlite3_decode_binary(mpBuf, mpBuf);
    if (mnBinaryLen == -1){
      throw CppSQLite3Exception(CPPSQLITE_ERROR, "Cannot decode binary", DONT_DELETE_MSG);
    }
    mbEncoded = false;
  }
  return mpBuf;
}
int CppSQLite3Binary::getBinaryLength(){
  getBinary();
  return mnBinaryLen;
}
unsigned char* CppSQLite3Binary::allocBuffer(int nLen){
  clear();
  // Allow extra space for encoded binary as per comments in
  // SQLite encode.c See bottom of this file for implementation
  // of SQLite functions use 3 instead of 2 just to be sure ;-)
  mnBinaryLen = nLen;
  mnBufferLen = 3 + (257*nLen)/254;
  mpBuf = (unsigned char*)malloc(mnBufferLen);
  if (!mpBuf){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Cannot allocate memory", DONT_DELETE_MSG);
  }
  mbEncoded = false;
  return mpBuf;
}
void CppSQLite3Binary::clear(){
  if (mpBuf){
    mnBinaryLen = 0;
    mnBufferLen = 0;
    free(mpBuf);
    mpBuf = 0;
  }
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Query::CppSQLite3Query(){
  mpVM = 0;
  mbEof = true;
  mnCols = 0;
  mbOwnVM = false;
}
CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery){
  mpVM = rQuery.mpVM;
  // Only one object can own the VM
  const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
  mbEof = rQuery.mbEof;
  mnCols = rQuery.mnCols;
  mbOwnVM = rQuery.mbOwnVM;
}
CppSQLite3Query::CppSQLite3Query(sqlite3* pDB, sqlite3_stmt* pVM, bool bEof, bool bOwnVM/*=true*/){
  mpDB = pDB;
  mpVM = pVM;
  mbEof = bEof;
  mnCols = sqlite3_column_count(mpVM);
  mbOwnVM = bOwnVM;
}
CppSQLite3Query::~CppSQLite3Query(){
  try {
    finalize();
  } catch (...){ }
}
CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery){
  try {
    finalize();
  } catch (...){ }
  mpVM = rQuery.mpVM;
  // Only one object can own the VM
  const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
  mbEof = rQuery.mbEof;
  mnCols = rQuery.mnCols;
  mbOwnVM = rQuery.mbOwnVM;
  return *this;
}
int CppSQLite3Query::numFields(){
  checkVM();
  return mnCols;
}
const char* CppSQLite3Query::fieldValue(int nField){
  checkVM();
  if (nField < 0 || nField > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  return (const char*)sqlite3_column_text(mpVM, nField);
}
const char* CppSQLite3Query::fieldValue(const char* szField){
  int nField = fieldIndex(szField);
  return (const char*)sqlite3_column_text(mpVM, nField);
}
int CppSQLite3Query::getIntField(int nField, int nNullValue/*=0*/){
  if (fieldDataType(nField) == SQLITE_NULL){
    return nNullValue;
  } else {
    return sqlite3_column_int(mpVM, nField);
  }
}
int CppSQLite3Query::getIntField(const char* szField, int nNullValue/*=0*/){
  int nField = fieldIndex(szField);
  return getIntField(nField, nNullValue);
}
double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/){
  if (fieldDataType(nField) == SQLITE_NULL){
    return fNullValue;
  } else {
    return sqlite3_column_double(mpVM, nField);
  }
}
double CppSQLite3Query::getFloatField(const char* szField, double fNullValue/*=0.0*/){
  int nField = fieldIndex(szField);
  return getFloatField(nField, fNullValue);
}
const char* CppSQLite3Query::getStringField(int nField, const char* szNullValue/*=""*/){
  if (fieldDataType(nField) == SQLITE_NULL){
    return szNullValue;
  } else {
    return (const char*)sqlite3_column_text(mpVM, nField);
  }
}
const char* CppSQLite3Query::getStringField(const char* szField, const char* szNullValue/*=""*/){
  int nField = fieldIndex(szField);
  return getStringField(nField, szNullValue);
}
const unsigned char* CppSQLite3Query::getBlobField(int nField, int& nLen){
  checkVM();
  if (nField < 0 || nField > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  nLen = sqlite3_column_bytes(mpVM, nField);
  return (const unsigned char*)sqlite3_column_blob(mpVM, nField);
}
const unsigned char* CppSQLite3Query::getBlobField(const char* szField, int& nLen){
  int nField = fieldIndex(szField);
  return getBlobField(nField, nLen);
}
bool CppSQLite3Query::fieldIsNull(int nField){
  return (fieldDataType(nField) == SQLITE_NULL);
}
bool CppSQLite3Query::fieldIsNull(const char* szField){
  int nField = fieldIndex(szField);
  return (fieldDataType(nField) == SQLITE_NULL);
}
int CppSQLite3Query::fieldIndex(const char* szField){
  checkVM();
  if (szField){
    for (int nField = 0; nField < mnCols; nField++){
      const char* szTemp = sqlite3_column_name(mpVM, nField);
      if (strcmp(szField, szTemp) == 0){
        return nField;
      }
    }
  }
  throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field name requested", DONT_DELETE_MSG);
}
const char* CppSQLite3Query::fieldName(int nCol){
  checkVM();
  if (nCol < 0 || nCol > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  return sqlite3_column_name(mpVM, nCol);
}
const char* CppSQLite3Query::fieldDeclType(int nCol){
  checkVM();
  if (nCol < 0 || nCol > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  return sqlite3_column_decltype(mpVM, nCol);
}
int CppSQLite3Query::fieldDataType(int nCol){
  checkVM();
  if (nCol < 0 || nCol > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  return sqlite3_column_type(mpVM, nCol);
}
bool CppSQLite3Query::eof(){
  checkVM();
  return mbEof;
}
void CppSQLite3Query::nextRow(){
  checkVM();
  int nRet = sqlite3_step(mpVM);
  if (nRet == SQLITE_DONE){
    // no rows
    mbEof = true;
  }else if (nRet == SQLITE_ROW){
    // more rows, nothing to do
  }else{
    nRet = sqlite3_finalize(mpVM);
    mpVM = 0;
    const char* szError = sqlite3_errmsg(mpDB);
    throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
  }
}
void CppSQLite3Query::finalize(){
  if (mpVM && mbOwnVM){
    int nRet = sqlite3_finalize(mpVM);
    mpVM = 0;
    if (nRet != SQLITE_OK){
      const char* szError = sqlite3_errmsg(mpDB);
      throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
  }
}
void CppSQLite3Query::checkVM(){
  if (mpVM == 0){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Null Virtual Machine pointer", DONT_DELETE_MSG);
  }
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Table::CppSQLite3Table(){
  mpaszResults = 0;
  mnRows = 0;
  mnCols = 0;
  mnCurrentRow = 0;
}
CppSQLite3Table::CppSQLite3Table(const CppSQLite3Table& rTable){
  mpaszResults = rTable.mpaszResults;
  // Only one object can own the results
  const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
  mnRows = rTable.mnRows;
  mnCols = rTable.mnCols;
  mnCurrentRow = rTable.mnCurrentRow;
}
CppSQLite3Table::CppSQLite3Table(char** paszResults, int nRows, int nCols){
  mpaszResults = paszResults;
  mnRows = nRows;
  mnCols = nCols;
  mnCurrentRow = 0;
}
CppSQLite3Table::~CppSQLite3Table(){
  try{
    finalize();
  }catch (...){
  }
}
CppSQLite3Table& CppSQLite3Table::operator=(const CppSQLite3Table& rTable){
  try{
    finalize();
  }catch (...){
  }
  mpaszResults = rTable.mpaszResults;
  // Only one object can own the results
  const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
  mnRows = rTable.mnRows;
  mnCols = rTable.mnCols;
  mnCurrentRow = rTable.mnCurrentRow;
  return *this;
}
void CppSQLite3Table::finalize(){
  if (mpaszResults){
    sqlite3_free_table(mpaszResults);
    mpaszResults = 0;
  }
}
int CppSQLite3Table::numFields(){
  checkResults();
  return mnCols;
}
int CppSQLite3Table::numRows(){
  checkResults();
  return mnRows;
}
const char* CppSQLite3Table::fieldValue(int nField){
  checkResults();
  if (nField < 0 || nField > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
  return mpaszResults[nIndex];
}
const char* CppSQLite3Table::fieldValue(const char* szField){
  checkResults();
  if (szField){
    for (int nField = 0; nField < mnCols; nField++){
      if (strcmp(szField, mpaszResults[nField]) == 0){
        int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
        return mpaszResults[nIndex];
      }
    }
  }
  throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field name requested", DONT_DELETE_MSG);
}
int CppSQLite3Table::getIntField(int nField, int nNullValue/*=0*/){
  if (fieldIsNull(nField)){
    return nNullValue;
  }else{
    return atoi(fieldValue(nField));
  }
}
int CppSQLite3Table::getIntField(const char* szField, int nNullValue/*=0*/){
  if (fieldIsNull(szField)){
    return nNullValue;
  }else{
    return atoi(fieldValue(szField));
  }
}
double CppSQLite3Table::getFloatField(int nField, double fNullValue/*=0.0*/){
  if (fieldIsNull(nField)){
    return fNullValue;
  }else{
    return atof(fieldValue(nField));
  }
}
double CppSQLite3Table::getFloatField(const char* szField, double fNullValue/*=0.0*/){
  if (fieldIsNull(szField)){
    return fNullValue;
  }else{
    return atof(fieldValue(szField));
  }
}
const char* CppSQLite3Table::getStringField(int nField, const char* szNullValue/*=""*/){
  if (fieldIsNull(nField)){
    return szNullValue;
  }else{
    return fieldValue(nField);
  }
}
const char* CppSQLite3Table::getStringField(const char* szField, const char* szNullValue/*=""*/){
  if (fieldIsNull(szField)){
    return szNullValue;
  }else{
    return fieldValue(szField);
  }
}
bool CppSQLite3Table::fieldIsNull(int nField){
  checkResults();
  return (fieldValue(nField) == 0);
}
bool CppSQLite3Table::fieldIsNull(const char* szField){
  checkResults();
  return (fieldValue(szField) == 0);
}
const char* CppSQLite3Table::fieldName(int nCol){
  checkResults();
  if (nCol < 0 || nCol > mnCols-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid field index requested", DONT_DELETE_MSG);
  }
  return mpaszResults[nCol];
}
void CppSQLite3Table::setRow(int nRow){
  checkResults();
  if (nRow < 0 || nRow > mnRows-1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid row index requested", DONT_DELETE_MSG);
  }
  mnCurrentRow = nRow;
}
void CppSQLite3Table::checkResults(){
  if (mpaszResults == 0){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Null Results pointer", DONT_DELETE_MSG);
  }
}
////////////////////////////////////////////////////////////////////////////////
CppSQLite3Statement::CppSQLite3Statement(){
  mpDB = 0;
  mpVM = 0;
}
CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement){
  mpDB = rStatement.mpDB;
  mpVM = rStatement.mpVM;
  // Only one object can own VM
  const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
}
CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM){
  mpDB = pDB;
  mpVM = pVM;
}
CppSQLite3Statement::~CppSQLite3Statement(){
  try{
    finalize();
  }catch (...){
  }
}
CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement){
  mpDB = rStatement.mpDB;
  mpVM = rStatement.mpVM;
  // Only one object can own VM
  const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
  return *this;
}
int CppSQLite3Statement::execDML(){
  checkDB();
  checkVM();
  const char* szError=0;
  int nRet = sqlite3_step(mpVM);
  if (nRet == SQLITE_DONE){
    int nRowsChanged = sqlite3_changes(mpDB);
    nRet = sqlite3_reset(mpVM);
    if (nRet != SQLITE_OK){
      szError = sqlite3_errmsg(mpDB);
      throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
    return nRowsChanged;
  }else{
    nRet = sqlite3_reset(mpVM);
    szError = sqlite3_errmsg(mpDB);
    throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
  }
}
CppSQLite3Query CppSQLite3Statement::execQuery(){
  checkDB();
  checkVM();
  int nRet = sqlite3_step(mpVM);
  if (nRet == SQLITE_DONE){
    // no rows
    return CppSQLite3Query(mpDB, mpVM, true/*eof*/, false);
  }else if (nRet == SQLITE_ROW){
    // at least 1 row
    return CppSQLite3Query(mpDB, mpVM, false/*eof*/, false);
  }else{
    nRet = sqlite3_reset(mpVM);
    const char* szError = sqlite3_errmsg(mpDB);
    throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::bind(int nParam, const char* szValue){
  checkVM();
  int nRes = sqlite3_bind_text(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);
  if (nRes != SQLITE_OK){
    throw CppSQLite3Exception(nRes, "Error binding string param", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::bind(int nParam, const int nValue){
  checkVM();
  int nRes = sqlite3_bind_int(mpVM, nParam, nValue);
  if (nRes != SQLITE_OK){
    throw CppSQLite3Exception(nRes, "Error binding int param", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::bind(int nParam, const double dValue){
  checkVM();
  int nRes = sqlite3_bind_double(mpVM, nParam, dValue);
  if (nRes != SQLITE_OK){
    throw CppSQLite3Exception(nRes, "Error binding double param", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::bind(int nParam, const unsigned char* blobValue, int nLen){
  checkVM();
  int nRes = sqlite3_bind_blob(mpVM, nParam,
    (const void*)blobValue, nLen, SQLITE_TRANSIENT);
  if (nRes != SQLITE_OK){
    throw CppSQLite3Exception(nRes, "Error binding blob param", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::bindNull(int nParam){
  checkVM();
  int nRes = sqlite3_bind_null(mpVM, nParam);
  if (nRes != SQLITE_OK){
    throw CppSQLite3Exception(nRes, "Error binding NULL param", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::reset(){
  if (mpVM){
    int nRet = sqlite3_reset(mpVM);
    if (nRet != SQLITE_OK){
      const char* szError = sqlite3_errmsg(mpDB);
      throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
  }
}
void CppSQLite3Statement::finalize(){
  if (mpVM){
    int nRet = sqlite3_finalize(mpVM);
    mpVM = 0;
    if (nRet != SQLITE_OK){
      const char* szError = sqlite3_errmsg(mpDB);
      throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
  }
}
void CppSQLite3Statement::checkDB(){
  if (mpDB == 0){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Database not open", DONT_DELETE_MSG);
  }
}
void CppSQLite3Statement::checkVM(){
  if (mpVM == 0){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Null Virtual Machine pointer", DONT_DELETE_MSG);
  }
}
////////////////////////////////////////////////////////////////////////////////
bool CppSQLite3DB::openalready=false;
CppSQLite3DB& CppSQLite3DB::instance(const char* s){
  static CppSQLite3DB sqlitedb;
  if (!openalready){
    sqlitedb.open(s);
  }
  return sqlitedb;
}
CppSQLite3DB::CppSQLite3DB(){
  mpDB = NULL;
  openalready = false;
  mnBusyTimeoutMs = 60000; // 60 seconds
}
CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db){
  mpDB = db.mpDB;
  mnBusyTimeoutMs = 60000; // 60 seconds
}
CppSQLite3DB::~CppSQLite3DB(){
  close();
}
CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db){
  mpDB = db.mpDB;
  mnBusyTimeoutMs = 60000; // 60 seconds
  return *this;
}
void CppSQLite3DB::open(const char* szFile){
  int nRet = sqlite3_open(szFile, &mpDB);
  if (nRet != SQLITE_OK){
    string error="Cannot open database:";
    error.append(sqlite3_errmsg(mpDB));
    UUDEBUG((LOG_ERROR,error.c_str()));
    _6bees_util::quitfatalerror(error.c_str());
  }
  openalready= true;
  setBusyTimeout(mnBusyTimeoutMs);
}
void CppSQLite3DB::close(){
  if (mpDB){
    sqlite3_close(mpDB);
    openalready = false;
    mpDB = NULL;
  }
}
CppSQLite3Statement CppSQLite3DB::compileStatement(const char* szSQL){
  checkDB();
  sqlite3_stmt* pVM = compile(szSQL);
  return CppSQLite3Statement(mpDB, pVM);
}
bool CppSQLite3DB::tableExists(const char* szTable){
  char szSQL[128];
  sprintf_s(szSQL,128,"select count(*) from sqlite_master where type='table' and name='%s'",szTable);
  int nRet = execScalar(szSQL);
  return (nRet > 0);
}
int CppSQLite3DB::execDML(const char* szSQL){
  checkDB();
  char* szError=0;
  int nRet = sqlite3_exec(mpDB, szSQL, 0, 0, &szError);
  if (nRet == SQLITE_OK){
    return sqlite3_changes(mpDB);
  }else{
#if 1
    UUDEBUG((LOG_ERROR,szError));
    return -1;
#else
    throw CppSQLite3Exception(nRet, szError);
#endif
  }
}
CppSQLite3Query CppSQLite3DB::execQuery(const char* szSQL){
  checkDB();
  sqlite3_stmt* pVM = compile(szSQL);
  int nRet = sqlite3_step(pVM);
  if (nRet == SQLITE_DONE){
    // no rows
    return CppSQLite3Query(mpDB, pVM, true/*eof*/);
  }else if (nRet == SQLITE_ROW){
    // at least 1 row
    return CppSQLite3Query(mpDB, pVM, false/*eof*/);
  }else{
    nRet = sqlite3_finalize(pVM);
    const char* szError= sqlite3_errmsg(mpDB);
    throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
  }
}
int CppSQLite3DB::execScalar(const char* szSQL){
  CppSQLite3Query q = execQuery(szSQL);
  if (q.eof() || q.numFields() < 1){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Invalid scalar query", DONT_DELETE_MSG);
  }
  return atoi(q.fieldValue(0));
}
CppSQLite3Table CppSQLite3DB::getTable(const char* szSQL){
  checkDB();
  char* szError=0;
  char** paszResults=0;
  int nRet;
  int nRows(0);
  int nCols(0);
  nRet = sqlite3_get_table(mpDB, szSQL, &paszResults, &nRows, &nCols, &szError);
  if (nRet == SQLITE_OK){
    return CppSQLite3Table(paszResults, nRows, nCols);
  }else{
    throw CppSQLite3Exception(nRet, szError);
  }
}
sqlite_int64 CppSQLite3DB::lastRowId(){
  return sqlite3_last_insert_rowid(mpDB);
}
void CppSQLite3DB::setBusyTimeout(int nMillisecs){
  mnBusyTimeoutMs = nMillisecs;
  sqlite3_busy_timeout(mpDB, mnBusyTimeoutMs);
}
void CppSQLite3DB::checkDB(){
  if (!mpDB){
    throw CppSQLite3Exception(CPPSQLITE_ERROR, "Database not open", DONT_DELETE_MSG);
  }
}
sqlite3_stmt* CppSQLite3DB::compile(const char* szSQL){
  checkDB();
  char* szError=0;
  const char* szTail=0;
  sqlite3_stmt* pVM;
  int nRet = sqlite3_prepare(mpDB, szSQL, -1, &pVM, &szTail);
  if (nRet != SQLITE_OK){
    throw CppSQLite3Exception(nRet, szError);
  }
  return pVM;
}
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out){
  int i, j, e, m;
  int cnt[256];
  if( n<=0 ){
    out[0] = 'x';
    out[1] = 0;
    return 1;
  }
  memset(cnt, 0, sizeof(cnt));
  for(i=n-1; i>=0; i--){ cnt[in[i]]++; }
  m = n;
  for(i=1; i<256; i++){
    int sum;
    if( i=='\'' ) continue;
    sum = cnt[i] + cnt[(i+1)&0xff] + cnt[(i+'\'')&0xff];
    if( sum<m ){
      m = sum;
      e = i;
      if( m==0 ) break;
    }
  }
  out[0] = e;
  j = 1;
  for(i=0; i<n; i++){
    int c = (in[i] - e)&0xff;
    if( c==0 ){
      out[j++] = 1;
      out[j++] = 1;
    }else if( c==1 ){
      out[j++] = 1;
      out[j++] = 2;
    }else if( c=='\'' ){
      out[j++] = 1;
      out[j++] = 3;
    }else{
      out[j++] = c;
    }
  }
  out[j] = 0;
  return j;
}
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out){
  int i, c, e;
  e = *(in++);
  i = 0;
  while( (c = *(in++))!=0 ){
    if( c==1 ){
      c = *(in++);
      if( c==1 ){
        c = 0;
      }else if( c==2 ){
        c = 1;
      }else if( c==3 ){
        c = '\'';
      }else{
        return -1;
      }
    }
    out[i++] = (c + e)&0xff;
  }
  return i;
}