#pragma once

/// @file storages/postgres/sql_state.hpp
/// @brief PostgreSQL error codes

#include <cstdint>
#include <string>

namespace storages {
namespace postgres {

// PostgreSQL error classes
/// Docs: https://www.postgresql.org/docs/10/static/errcodes-appendix.html
/// Enumeration was generated by userver/scripts/postgres/pg_sql_codes.py
enum class SqlStateClass : std::int64_t {
  kSuccessfulCompletion = 0x01,
  kWarning = 0x02,
  kNoData = 0x10,
  kSqlStatementNotYetComplete = 0x20,
  kConnectionException = 0x40,
  kTriggeredActionException = 0x80,
  kFeatureNotSupported = 0x100,
  kInvalidTransactionInitiation = 0x200,
  kLocatorException = 0x400,
  kInvalidGrantor = 0x800,
  kInvalidRoleSpecification = 0x1000,
  kDiagnosticsException = 0x2000,
  kCaseNotFound = 0x4000,
  kCardinalityViolation = 0x8000,
  kDataException = 0x10000,
  kIntegrityConstraintViolation = 0x20000,
  kInvalidCursorState = 0x40000,
  kInvalidTransactionState = 0x80000,
  kInvalidSqlStatementName = 0x100000,
  kTriggeredDataChangeViolation = 0x200000,
  kInvalidAuthorizationSpecification = 0x400000,
  kDependentPrivilegeDescriptorsStillExist = 0x800000,
  kInvalidTransactionTermination = 0x1000000,
  kSqlRoutineException = 0x2000000,
  kInvalidCursorName = 0x4000000,
  kExternalRoutineException = 0x8000000,
  kExternalRoutineInvocationException = 0x10000000,
  kSavepointException = 0x20000000,
  kInvalidCatalogName = 0x40000000,
  kInvalidSchemaName = 0x80000000,
  kTransactionRollback = 0x100000000,
  kSyntaxErrorOrAccessRuleViolation = 0x200000000,
  kWithCheckOptionViolation = 0x400000000,
  kInsufficientResources = 0x800000000,
  kProgramLimitExceeded = 0x1000000000,
  kObjectNotInPrerequisiteState = 0x2000000000,
  kOperatorIntervention = 0x4000000000,
  kSystemError = 0x8000000000,
  kSnapshotTooOld = 0x10000000000,
  kConfigFileError = 0x20000000000,
  kFdwError = 0x40000000000,
  kPlpgsqlError = 0x80000000000,
  kInternalError = 0x100000000000,
};

/// PostgreSQL error codes
/// Docs: https://www.postgresql.org/docs/10/static/errcodes-appendix.html
/// Enumeration was generated by userver/scripts/postgres/pg_sql_codes.py
enum class SqlState : std::int64_t {
  kUnknownState,  ///!< Unknown state, not in PostgreSQL docs
  //@{
  /** @name Class 00 — Successful Completion */
  kSuccessfulCompletion = static_cast<std::int64_t>(
      SqlStateClass::kSuccessfulCompletion),  //!< 00000
  //@}
  //@{
  /** @name Class 01 — Warning */
  kWarning = static_cast<std::int64_t>(SqlStateClass::kWarning),  //!< 01000
  kDynamicResultSetsReturned,                                     //!< 0100C
  kImplicitZeroBitPadding,                                        //!< 01008
  kNullValueEliminatedInSetFunction,                              //!< 01003
  kPrivilegeNotGranted,                                           //!< 01007
  kPrivilegeNotRevoked,                                           //!< 01006
  kStringDataRightTruncationWarn,                                 //!< 01004
  kDeprecatedFeature,                                             //!< 01P01
  //@}
  //@{
  /** @name Class 02 — No Data (this is also a warning class per the SQL
     standard) */
  kNoData = static_cast<std::int64_t>(SqlStateClass::kNoData),  //!< 02000
  kNoAdditionalDynamicResultSetsReturned,                       //!< 02001
  //@}
  //@{
  /** @name Class 03 — SQL Statement Not Yet Complete */
  kSqlStatementNotYetComplete = static_cast<std::int64_t>(
      SqlStateClass::kSqlStatementNotYetComplete),  //!< 03000
  //@}
  //@{
  /** @name Class 08 — Connection Exception */
  kConnectionException = static_cast<std::int64_t>(
      SqlStateClass::kConnectionException),        //!< 08000
  kConnectionDoesNotExist,                         //!< 08003
  kConnectionFailure,                              //!< 08006
  kSqlclientUnableToEstablishSqlconnection,        //!< 08001
  kSqlserverRejectedEstablishmentOfSqlconnection,  //!< 08004
  kTransactionResolutionUnknown,                   //!< 08007
  kProtocolViolation,                              //!< 08P01
  //@}
  //@{
  /** @name Class 09 — Triggered Action Exception */
  kTriggeredActionException = static_cast<std::int64_t>(
      SqlStateClass::kTriggeredActionException),  //!< 09000
  //@}
  //@{
  /** @name Class 0A — Feature Not Supported */
  kFeatureNotSupported = static_cast<std::int64_t>(
      SqlStateClass::kFeatureNotSupported),  //!< 0A000
  //@}
  //@{
  /** @name Class 0B — Invalid Transaction Initiation */
  kInvalidTransactionInitiation = static_cast<std::int64_t>(
      SqlStateClass::kInvalidTransactionInitiation),  //!< 0B000
  //@}
  //@{
  /** @name Class 0F — Locator Exception */
  kLocatorException =
      static_cast<std::int64_t>(SqlStateClass::kLocatorException),  //!< 0F000
  kInvalidLocatorSpecification,                                     //!< 0F001
  //@}
  //@{
  /** @name Class 0L — Invalid Grantor */
  kInvalidGrantor =
      static_cast<std::int64_t>(SqlStateClass::kInvalidGrantor),  //!< 0L000
  kInvalidGrantOperation,                                         //!< 0LP01
  //@}
  //@{
  /** @name Class 0P — Invalid Role Specification */
  kInvalidRoleSpecification = static_cast<std::int64_t>(
      SqlStateClass::kInvalidRoleSpecification),  //!< 0P000
  //@}
  //@{
  /** @name Class 0Z — Diagnostics Exception */
  kDiagnosticsException = static_cast<std::int64_t>(
      SqlStateClass::kDiagnosticsException),        //!< 0Z000
  kStackedDiagnosticsAccessedWithoutActiveHandler,  //!< 0Z002
  //@}
  //@{
  /** @name Class 20 — Case Not Found */
  kCaseNotFound =
      static_cast<std::int64_t>(SqlStateClass::kCaseNotFound),  //!< 20000
  //@}
  //@{
  /** @name Class 21 — Cardinality Violation */
  kCardinalityViolation = static_cast<std::int64_t>(
      SqlStateClass::kCardinalityViolation),  //!< 21000
  //@}
  //@{
  /** @name Class 22 — Data Exception */
  kDataException =
      static_cast<std::int64_t>(SqlStateClass::kDataException),  //!< 22000
  kArraySubscriptError,                                          //!< 2202E
  kCharacterNotInRepertoire,                                     //!< 22021
  kDatetimeFieldOverflow,                                        //!< 22008
  kDivisionByZero,                                               //!< 22012
  kErrorInAssignment,                                            //!< 22005
  kEscapeCharacterConflict,                                      //!< 2200B
  kIndicatorOverflow,                                            //!< 22022
  kIntervalFieldOverflow,                                        //!< 22015
  kInvalidArgumentForLogarithm,                                  //!< 2201E
  kInvalidArgumentForNtileFunction,                              //!< 22014
  kInvalidArgumentForNthValueFunction,                           //!< 22016
  kInvalidArgumentForPowerFunction,                              //!< 2201F
  kInvalidArgumentForWidthBucketFunction,                        //!< 2201G
  kInvalidCharacterValueForCast,                                 //!< 22018
  kInvalidDatetimeFormat,                                        //!< 22007
  kInvalidEscapeCharacter,                                       //!< 22019
  kInvalidEscapeOctet,                                           //!< 2200D
  kInvalidEscapeSequence,                                        //!< 22025
  kNonstandardUseOfEscapeCharacter,                              //!< 22P06
  kInvalidIndicatorParameterValue,                               //!< 22010
  kInvalidParameterValue,                                        //!< 22023
  kInvalidRegularExpression,                                     //!< 2201B
  kInvalidRowCountInLimitClause,                                 //!< 2201W
  kInvalidRowCountInResultOffsetClause,                          //!< 2201X
  kInvalidTablesampleArgument,                                   //!< 2202H
  kInvalidTablesampleRepeat,                                     //!< 2202G
  kInvalidTimeZoneDisplacementValue,                             //!< 22009
  kInvalidUseOfEscapeCharacter,                                  //!< 2200C
  kMostSpecificTypeMismatch,                                     //!< 2200G
  kNullValueNotAllowed,                                          //!< 22004
  kNullValueNoIndicatorParameter,                                //!< 22002
  kNumericValueOutOfRange,                                       //!< 22003
  kSequenceGeneratorLimitExceeded,                               //!< 2200H
  kStringDataLengthMismatch,                                     //!< 22026
  kStringDataRightTruncation,                                    //!< 22001
  kSubstringError,                                               //!< 22011
  kTrimError,                                                    //!< 22027
  kUnterminatedCString,                                          //!< 22024
  kZeroLengthCharacterString,                                    //!< 2200F
  kFloatingPointException,                                       //!< 22P01
  kInvalidTextRepresentation,                                    //!< 22P02
  kInvalidBinaryRepresentation,                                  //!< 22P03
  kBadCopyFileFormat,                                            //!< 22P04
  kUntranslatableCharacter,                                      //!< 22P05
  kNotAnXmlDocument,                                             //!< 2200L
  kInvalidXmlDocument,                                           //!< 2200M
  kInvalidXmlContent,                                            //!< 2200N
  kInvalidXmlComment,                                            //!< 2200S
  kInvalidXmlProcessingInstruction,                              //!< 2200T
  //@}
  //@{
  /** @name Class 23 — Integrity Constraint Violation */
  kIntegrityConstraintViolation = static_cast<std::int64_t>(
      SqlStateClass::kIntegrityConstraintViolation),  //!< 23000
  kRestrictViolation,                                 //!< 23001
  kNotNullViolation,                                  //!< 23502
  kForeignKeyViolation,                               //!< 23503
  kUniqueViolation,                                   //!< 23505
  kCheckViolation,                                    //!< 23514
  kExclusionViolation,                                //!< 23P01
  //@}
  //@{
  /** @name Class 24 — Invalid Cursor State */
  kInvalidCursorState =
      static_cast<std::int64_t>(SqlStateClass::kInvalidCursorState),  //!< 24000
  //@}
  //@{
  /** @name Class 25 — Invalid Transaction State */
  kInvalidTransactionState = static_cast<std::int64_t>(
      SqlStateClass::kInvalidTransactionState),      //!< 25000
  kActiveSqlTransaction,                             //!< 25001
  kBranchTransactionAlreadyActive,                   //!< 25002
  kHeldCursorRequiresSameIsolationLevel,             //!< 25008
  kInappropriateAccessModeForBranchTransaction,      //!< 25003
  kInappropriateIsolationLevelForBranchTransaction,  //!< 25004
  kNoActiveSqlTransactionForBranchTransaction,       //!< 25005
  kReadOnlySqlTransaction,                           //!< 25006
  kSchemaAndDataStatementMixingNotSupported,         //!< 25007
  kNoActiveSqlTransaction,                           //!< 25P01
  kInFailedSqlTransaction,                           //!< 25P02
  kIdleInTransactionSessionTimeout,                  //!< 25P03
  //@}
  //@{
  /** @name Class 26 — Invalid SQL Statement Name */
  kInvalidSqlStatementName = static_cast<std::int64_t>(
      SqlStateClass::kInvalidSqlStatementName),  //!< 26000
  //@}
  //@{
  /** @name Class 27 — Triggered Data Change Violation */
  kTriggeredDataChangeViolation = static_cast<std::int64_t>(
      SqlStateClass::kTriggeredDataChangeViolation),  //!< 27000
  //@}
  //@{
  /** @name Class 28 — Invalid Authorization Specification */
  kInvalidAuthorizationSpecification = static_cast<std::int64_t>(
      SqlStateClass::kInvalidAuthorizationSpecification),  //!< 28000
  kInvalidPassword,                                        //!< 28P01
  //@}
  //@{
  /** @name Class 2B — Dependent Privilege Descriptors Still Exist */
  kDependentPrivilegeDescriptorsStillExist = static_cast<std::int64_t>(
      SqlStateClass::kDependentPrivilegeDescriptorsStillExist),  //!< 2B000
  kDependentObjectsStillExist,                                   //!< 2BP01
  //@}
  //@{
  /** @name Class 2D — Invalid Transaction Termination */
  kInvalidTransactionTermination = static_cast<std::int64_t>(
      SqlStateClass::kInvalidTransactionTermination),  //!< 2D000
  //@}
  //@{
  /** @name Class 2F — SQL Routine Exception */
  kSqlRoutineException = static_cast<std::int64_t>(
      SqlStateClass::kSqlRoutineException),  //!< 2F000
  kFunctionExecutedNoReturnStatement,        //!< 2F005
  kModifyingSqlDataNotPermitted,             //!< 2F002
  kProhibitedSqlStatementAttempted,          //!< 2F003
  kReadingSqlDataNotPermitted,               //!< 2F004
  //@}
  //@{
  /** @name Class 34 — Invalid Cursor Name */
  kInvalidCursorName =
      static_cast<std::int64_t>(SqlStateClass::kInvalidCursorName),  //!< 34000
  //@}
  //@{
  /** @name Class 38 — External Routine Exception */
  kExternalRoutineException = static_cast<std::int64_t>(
      SqlStateClass::kExternalRoutineException),  //!< 38000
  kContainingSqlNotPermitted,                     //!< 38001
  kModifyingSqlDataNotPermittedEx,                //!< 38002
  kProhibitedSqlStatementAttemptedEx,             //!< 38003
  kReadingSqlDataNotPermittedEx,                  //!< 38004
  //@}
  //@{
  /** @name Class 39 — External Routine Invocation Exception */
  kExternalRoutineInvocationException = static_cast<std::int64_t>(
      SqlStateClass::kExternalRoutineInvocationException),  //!< 39000
  kInvalidSqlstateReturned,                                 //!< 39001
  kNullValueNotAllowedEx,                                   //!< 39004
  kTriggerProtocolViolated,                                 //!< 39P01
  kSrfProtocolViolated,                                     //!< 39P02
  kEventTriggerProtocolViolated,                            //!< 39P03
  //@}
  //@{
  /** @name Class 3B — Savepoint Exception */
  kSavepointException =
      static_cast<std::int64_t>(SqlStateClass::kSavepointException),  //!< 3B000
  kInvalidSavepointSpecification,                                     //!< 3B001
  //@}
  //@{
  /** @name Class 3D — Invalid Catalog Name */
  kInvalidCatalogName =
      static_cast<std::int64_t>(SqlStateClass::kInvalidCatalogName),  //!< 3D000
  //@}
  //@{
  /** @name Class 3F — Invalid Schema Name */
  kInvalidSchemaName =
      static_cast<std::int64_t>(SqlStateClass::kInvalidSchemaName),  //!< 3F000
  //@}
  //@{
  /** @name Class 40 — Transaction Rollback */
  kTransactionRollback = static_cast<std::int64_t>(
      SqlStateClass::kTransactionRollback),  //!< 40000
  kTransactionIntegrityConstraintViolation,  //!< 40002
  kSerializationFailure,                     //!< 40001
  kStatementCompletionUnknown,               //!< 40003
  kDeadlockDetected,                         //!< 40P01
  //@}
  //@{
  /** @name Class 42 — Syntax Error or Access Rule Violation */
  kSyntaxErrorOrAccessRuleViolation = static_cast<std::int64_t>(
      SqlStateClass::kSyntaxErrorOrAccessRuleViolation),  //!< 42000
  kSyntaxError,                                           //!< 42601
  kInsufficientPrivilege,                                 //!< 42501
  kCannotCoerce,                                          //!< 42846
  kGroupingError,                                         //!< 42803
  kWindowingError,                                        //!< 42P20
  kInvalidRecursion,                                      //!< 42P19
  kInvalidForeignKey,                                     //!< 42830
  kInvalidName,                                           //!< 42602
  kNameTooLong,                                           //!< 42622
  kReservedName,                                          //!< 42939
  kDatatypeMismatch,                                      //!< 42804
  kIndeterminateDatatype,                                 //!< 42P18
  kCollationMismatch,                                     //!< 42P21
  kIndeterminateCollation,                                //!< 42P22
  kWrongObjectType,                                       //!< 42809
  kGeneratedAlways,                                       //!< 428C9
  kUndefinedColumn,                                       //!< 42703
  kUndefinedFunction,                                     //!< 42883
  kUndefinedTable,                                        //!< 42P01
  kUndefinedParameter,                                    //!< 42P02
  kUndefinedObject,                                       //!< 42704
  kDuplicateColumn,                                       //!< 42701
  kDuplicateCursor,                                       //!< 42P03
  kDuplicateDatabase,                                     //!< 42P04
  kDuplicateFunction,                                     //!< 42723
  kDuplicatePreparedStatement,                            //!< 42P05
  kDuplicateSchema,                                       //!< 42P06
  kDuplicateTable,                                        //!< 42P07
  kDuplicateAlias,                                        //!< 42712
  kDuplicateObject,                                       //!< 42710
  kAmbiguousColumn,                                       //!< 42702
  kAmbiguousFunction,                                     //!< 42725
  kAmbiguousParameter,                                    //!< 42P08
  kAmbiguousAlias,                                        //!< 42P09
  kInvalidColumnReference,                                //!< 42P10
  kInvalidColumnDefinition,                               //!< 42611
  kInvalidCursorDefinition,                               //!< 42P11
  kInvalidDatabaseDefinition,                             //!< 42P12
  kInvalidFunctionDefinition,                             //!< 42P13
  kInvalidPreparedStatementDefinition,                    //!< 42P14
  kInvalidSchemaDefinition,                               //!< 42P15
  kInvalidTableDefinition,                                //!< 42P16
  kInvalidObjectDefinition,                               //!< 42P17
  //@}
  //@{
  /** @name Class 44 — WITH CHECK OPTION Violation */
  kWithCheckOptionViolation = static_cast<std::int64_t>(
      SqlStateClass::kWithCheckOptionViolation),  //!< 44000
  //@}
  //@{
  /** @name Class 53 — Insufficient Resources */
  kInsufficientResources = static_cast<std::int64_t>(
      SqlStateClass::kInsufficientResources),  //!< 53000
  kDiskFull,                                   //!< 53100
  kOutOfMemory,                                //!< 53200
  kTooManyConnections,                         //!< 53300
  kConfigurationLimitExceeded,                 //!< 53400
  //@}
  //@{
  /** @name Class 54 — Program Limit Exceeded */
  kProgramLimitExceeded = static_cast<std::int64_t>(
      SqlStateClass::kProgramLimitExceeded),  //!< 54000
  kStatementTooComplex,                       //!< 54001
  kTooManyColumns,                            //!< 54011
  kTooManyArguments,                          //!< 54023
  //@}
  //@{
  /** @name Class 55 — Object Not In Prerequisite State */
  kObjectNotInPrerequisiteState = static_cast<std::int64_t>(
      SqlStateClass::kObjectNotInPrerequisiteState),  //!< 55000
  kObjectInUse,                                       //!< 55006
  kCantChangeRuntimeParam,                            //!< 55P02
  kLockNotAvailable,                                  //!< 55P03
  //@}
  //@{
  /** @name Class 57 — Operator Intervention */
  kOperatorIntervention = static_cast<std::int64_t>(
      SqlStateClass::kOperatorIntervention),  //!< 57000
  kQueryCanceled,                             //!< 57014
  kAdminShutdown,                             //!< 57P01
  kCrashShutdown,                             //!< 57P02
  kCannotConnectNow,                          //!< 57P03
  kDatabaseDropped,                           //!< 57P04
  //@}
  //@{
  /** @name Class 58 — System Error (errors external to Postgres itself) */
  kSystemError =
      static_cast<std::int64_t>(SqlStateClass::kSystemError),  //!< 58000
  kIoError,                                                    //!< 58030
  kUndefinedFile,                                              //!< 58P01
  kDuplicateFile,                                              //!< 58P02
  //@}
  //@{
  /** @name Class 72 — Snapshot Failure */
  kSnapshotTooOld =
      static_cast<std::int64_t>(SqlStateClass::kSnapshotTooOld),  //!< 72000
  //@}
  //@{
  /** @name Class F0 — Configuration File Error */
  kConfigFileError =
      static_cast<std::int64_t>(SqlStateClass::kConfigFileError),  //!< F0000
  kLockFileExists,                                                 //!< F0001
  //@}
  //@{
  /** @name Class HV — Foreign Data Wrapper Error (SQL/MED) */
  kFdwError = static_cast<std::int64_t>(SqlStateClass::kFdwError),  //!< HV000
  kFdwColumnNameNotFound,                                           //!< HV005
  kFdwDynamicParameterValueNeeded,                                  //!< HV002
  kFdwFunctionSequenceError,                                        //!< HV010
  kFdwInconsistentDescriptorInformation,                            //!< HV021
  kFdwInvalidAttributeValue,                                        //!< HV024
  kFdwInvalidColumnName,                                            //!< HV007
  kFdwInvalidColumnNumber,                                          //!< HV008
  kFdwInvalidDataType,                                              //!< HV004
  kFdwInvalidDataTypeDescriptors,                                   //!< HV006
  kFdwInvalidDescriptorFieldIdentifier,                             //!< HV091
  kFdwInvalidHandle,                                                //!< HV00B
  kFdwInvalidOptionIndex,                                           //!< HV00C
  kFdwInvalidOptionName,                                            //!< HV00D
  kFdwInvalidStringLengthOrBufferLength,                            //!< HV090
  kFdwInvalidStringFormat,                                          //!< HV00A
  kFdwInvalidUseOfNullPointer,                                      //!< HV009
  kFdwTooManyHandles,                                               //!< HV014
  kFdwOutOfMemory,                                                  //!< HV001
  kFdwNoSchemas,                                                    //!< HV00P
  kFdwOptionNameNotFound,                                           //!< HV00J
  kFdwReplyHandle,                                                  //!< HV00K
  kFdwSchemaNotFound,                                               //!< HV00Q
  kFdwTableNotFound,                                                //!< HV00R
  kFdwUnableToCreateExecution,                                      //!< HV00L
  kFdwUnableToCreateReply,                                          //!< HV00M
  kFdwUnableToEstablishConnection,                                  //!< HV00N
  //@}
  //@{
  /** @name Class P0 — PL/pgSQL Error */
  kPlpgsqlError =
      static_cast<std::int64_t>(SqlStateClass::kPlpgsqlError),  //!< P0000
  kRaiseException,                                              //!< P0001
  kNoDataFound,                                                 //!< P0002
  kTooManyRows,                                                 //!< P0003
  kAssertFailure,                                               //!< P0004
  //@}
  //@{
  /** @name Class XX — Internal Error */
  kInternalError =
      static_cast<std::int64_t>(SqlStateClass::kInternalError),  //!< XX000
  kDataCorrupted,                                                //!< XX001
  kIndexCorrupted,                                               //!< XX002
  //@}
};

SqlStateClass GetSqlStateClass(SqlState);
// TODO Replace with string_view
SqlState SqlStateFromString(const std::string&);

bool IsWhitelistedState(SqlState);

}  // namespace postgres
}  // namespace storages

namespace std {
/// @brief Specialisation of std::hash template for SqlState.
template <>
struct hash<storages::postgres::SqlState> {
  std::size_t operator()(storages::postgres::SqlState v) const {
    return static_cast<std::size_t>(v);
  }
};
}  // namespace std
