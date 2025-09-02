/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

grammar AntlrSQL;

@lexer::postinclude {
#include <Util/DisableWarningsPragma.hpp>
DISABLE_WARNING_PUSH
DISABLE_WARNING(-Wlogical-op-parentheses)
DISABLE_WARNING(-Wunused-parameter)
}

@parser::postinclude {
#include <Util/DisableWarningsPragma.hpp>
DISABLE_WARNING_PUSH
DISABLE_WARNING(-Wlogical-op-parentheses)
DISABLE_WARNING(-Wunused-parameter)
}

@parser::members {
      bool SQL_standard_keyword_behavior = false;
      bool legacy_exponent_literal_as_decimal_enabled = false;
}

@lexer::members {
  bool isValidDecimal() {
    int nextChar = _input->LA(1);
    if (nextChar >= 'A' && nextChar <= 'Z' || nextChar >= '0' && nextChar <= '9' ||
      nextChar == '_') {
      return false;
    } else {
      return true;
    }
  }

  bool isHint() {
    int nextChar = _input->LA(1);
    if (nextChar == '+') {
      return true;
    } else {
      return false;
    }
  }
}

singleStatement: statement ';'* EOF;

statement: query;

query : queryTerm queryOrganization;

queryOrganization:
         (ORDER BY order+=sortItem (',' order+=sortItem)*)?
         (LIMIT (ALL | limit=INTEGER_VALUE))?
         (OFFSET offset=INTEGER_VALUE)?
         ;

queryTerm: queryPrimary #primaryQuery
         |  left=queryTerm setoperator=UNION right=queryTerm  #setOperation
         ;

queryPrimary
    : querySpecification                                                    #queryPrimaryDefault
    | fromStatement                                                         #fromStmt
    | TABLE multipartIdentifier                                             #table
    | inlineTable                                                           #inlineTableDefault1
    | '(' query ')'                                                         #subquery
    ;
/// new layout to be closer to traditional SQL
querySpecification: selectClause fromClause whereClause? windowedAggregationClause? havingClause? sinkClause?;


fromClause: FROM relation (',' relation)*;

relation
    : relationPrimary joinRelation*
    ;

joinRelation
    : (joinType) JOIN right=relationPrimary joinCriteria? windowClause
    | NATURAL joinType JOIN right=relationPrimary windowClause
    ;

joinType
    : INNER?
    ;

joinCriteria
    : ON booleanExpression
    ;

relationPrimary
    : multipartIdentifier tableAlias          #tableName
    | '(' query ')'  tableAlias               #aliasedQuery
    | '(' relation ')' tableAlias             #aliasedRelation
    | inlineTable                             #inlineTableDefault2
    | functionTable                           #tableValuedFunction
    ;

functionTable
    : funcName=errorCapturingIdentifier '(' (expression (',' expression)*)? ')' tableAlias
    ;

fromStatement: fromClause fromStatementBody+;

fromStatementBody: selectClause whereClause? groupByClause?;

selectClause : SELECT (hints+=hint)* namedExpressionSeq;

whereClause: WHERE booleanExpression;

havingClause: HAVING booleanExpression;

inlineTable
    : VALUES expression (',' expression)* tableAlias
    ;

tableAlias
    : (AS? identifier identifierList?)?
    ;

multipartIdentifier
    : parts+=errorCapturingIdentifier ('.' parts+=errorCapturingIdentifier)*
    ;

namedExpression
    : expression AS name=identifier
    | expression
    ;

identifier: strictIdentifier;

strictIdentifier
    : IDENTIFIER #unquotedIdentifier
    | quotedIdentifier #quotedIdentifierAlternative;

quotedIdentifier
    : BACKQUOTED_IDENTIFIER
    ;

BACKQUOTED_IDENTIFIER
    : '`' ( ~'`' | '``' )* '`'
    ;

identifierList
    : '(' identifierSeq ')'
    ;

identifierSeq
    : ident+=errorCapturingIdentifier (',' ident+=errorCapturingIdentifier)*
    ;

errorCapturingIdentifier
    : identifier errorCapturingIdentifierExtra
    ;

errorCapturingIdentifierExtra
    : (MINUS identifier)+    #errorIdent
    |                        #realIdent
    ;

namedExpressionSeq
    : namedExpression (',' namedExpression)*
    ;

expression
    : valueExpression
    | booleanExpression
    | identifier
    ;

booleanExpression
    : NOT booleanExpression                                        #logicalNot
    | EXISTS '(' query ')'                                         #exists
    | valueExpression predicate?                                   #predicated
    | left=booleanExpression op=AND right=booleanExpression  #logicalBinary
    | left=booleanExpression op=OR right=booleanExpression   #logicalBinary
    ;

/// Problem fixed that the querySpecification rule could match an empty string
windowedAggregationClause:
    groupByClause? windowClause watermarkClause?
    | windowClause groupByClause? watermarkClause?;

groupByClause
    : GROUP BY groupingExpressions+=expression (',' groupingExpressions+=expression)* (
      WITH kind=ROLLUP
    | WITH kind=CUBE
    | kind=GROUPING SETS '(' groupingSet (',' groupingSet)* ')')?
    | GROUP BY kind=GROUPING SETS '(' groupingSet (',' groupingSet)* ')'
    ;

groupingSet
    : '(' (expression (',' expression)*)? ')'
    | expression
    ;

windowClause
    : WINDOW windowSpec
    ;

watermarkClause: WATERMARK '(' watermarkParameters ')';

watermarkParameters: watermarkIdentifier=identifier ',' watermark=INTEGER_VALUE watermarkTimeUnit=timeUnit;
/// Adding Threshold Windows
windowSpec:
    timeWindow #timeBasedWindow
    | countWindow #countBasedWindow
    | conditionWindow #thresholdBasedWindow
    ;

timeWindow
    : TUMBLING '(' (timestampParameter ',')?  sizeParameter ')'                       #tumblingWindow
    | SLIDING '(' (timestampParameter ',')? sizeParameter ',' advancebyParameter ')' #slidingWindow
    ;

countWindow:
    TUMBLING '(' INTEGER_VALUE ')'    #countBasedTumbling
    ;

conditionWindow
    : THRESHOLD '(' conditionParameter (',' thresholdMinSizeParameter)? ')' #thresholdWindow
    ;

conditionParameter: expression;
thresholdMinSizeParameter: INTEGER_VALUE;

sizeParameter: SIZE INTEGER_VALUE timeUnit;

advancebyParameter: ADVANCE BY INTEGER_VALUE timeUnit;

timeUnit: MS
        | SEC
        | MINUTE
        | HOUR
        | DAY
        ;

timestampParameter: IDENTIFIER;

functionName:  IDENTIFIER | AVG | MAX | MIN | SUM | COUNT | MEDIAN | ARRAY_AGG | VAR | TEMPORAL_SEQUENCE | TEMPORAL_EINTERSECTS_GEOMETRY | TEMPORAL_AINTERSECTS_GEOMETRY | EINTERSECTS_TGEO_TGEO | EINTERSECTS_TGEO_GEO | AINTERSECTS_TGEO_TGEO | AINTERSECTS_TGEO_GEO | ACONTAINS_GEO_TGEO | ACONTAINS_TGEO_TGEO | ACONTAINS_TGEO_GEO | ADISJOINT_TGEO_GEO | ADISJOINT_TGEO_TGEO | ATOUCHES_TGEO_GEO | ATOUCHES_TGEO_TGEO | ATOUCHES_TPOINT_GEO | ETOUCHES_TPOINT_GEO | ETOUCHES_TGEO_GEO | ETOUCHES_TGEO_TGEO | EDWITHIN_TGEO_GEO | ADWITHIN_TGEO_GEO | ADWITHIN_TGEO_TGEO | ECONTAINS_GEO_TGEO | ECONTAINS_TGEO_GEO | ECONTAINS_TGEO_TGEO | ECOVERS_GEO_TGEO | ECOVERS_TGEO_TGEO | ECOVERS_TGEO_GEO | EDISJOINT_TGEO_GEO | EDISJOINT_TGEO_TGEO | EDWITHIN_TGEO_TGEO | ABOVE_STBOX_TSPATIAL | ABOVE_TSPATIAL_STBOX | ABOVE_TSPATIAL_TSPATIAL | AFTER_STBOX_TSPATIAL | AFTER_TSPATIAL_STBOX | AFTER_TSPATIAL_TSPATIAL | BACK_STBOX_TSPATIAL | BACK_TSPATIAL_STBOX | BACK_TSPATIAL_TSPATIAL | BEFORE_STBOX_TSPATIAL | BEFORE_TSPATIAL_STBOX | BEFORE_TSPATIAL_TSPATIAL | BELOW_STBOX_TSPATIAL | BELOW_TSPATIAL_STBOX | BELOW_TSPATIAL_TSPATIAL | FRONT_STBOX_TSPATIAL | FRONT_TSPATIAL_STBOX | FRONT_TSPATIAL_TSPATIAL | LEFT_STBOX_TSPATIAL | LEFT_TSPATIAL_STBOX | LEFT_TSPATIAL_TSPATIAL | OVERABOVE_STBOX_TSPATIAL | OVERABOVE_TSPATIAL_STBOX | OVERABOVE_TSPATIAL_TSPATIAL | OVERAFTER_STBOX_TSPATIAL | OVERAFTER_TSPATIAL_STBOX | OVERAFTER_TSPATIAL_TSPATIAL | OVERBACK_STBOX_TSPATIAL | OVERBACK_TSPATIAL_STBOX | OVERBACK_TSPATIAL_TSPATIAL | OVERBEFORE_STBOX_TSPATIAL | OVERBEFORE_TSPATIAL_STBOX | OVERBEFORE_TSPATIAL_TSPATIAL | OVERBELOW_STBOX_TSPATIAL | OVERBELOW_TSPATIAL_STBOX | OVERBELOW_TSPATIAL_TSPATIAL | OVERFRONT_STBOX_TSPATIAL | OVERFRONT_TSPATIAL_STBOX | OVERFRONT_TSPATIAL_TSPATIAL | OVERLEFT_STBOX_TSPATIAL | OVERLEFT_TSPATIAL_STBOX | OVERLEFT_TSPATIAL_TSPATIAL | OVERRIGHT_STBOX_TSPATIAL | OVERRIGHT_TSPATIAL_STBOX | OVERRIGHT_TSPATIAL_TSPATIAL | RIGHT_TSPATIAL_TSPATIAL | RIGHT_TSPATIAL_STBOX | RIGHT_STBOX_TSPATIAL | ADJACENT_STBOX_TSPATIAL | ADJACENT_TSPATIAL_STBOX | ADJACENT_TSPATIAL_TSPATIAL | CONTAINED_STBOX_TSPATIAL | CONTAINED_TSPATIAL_STBOX | CONTAINED_TSPATIAL_TSPATIAL | CONTAINS_STBOX_TSPATIAL | CONTAINS_TSPATIAL_STBOX | CONTAINS_TSPATIAL_TSPATIAL | OVERLAPS_STBOX_TSPATIAL | OVERLAPS_TSPATIAL_STBOX | OVERLAPS_TSPATIAL_TSPATIAL | SAME_STBOX_TSPATIAL | SAME_TSPATIAL_STBOX | SAME_TSPATIAL_TSPATIAL | ALWAYS_EQ_GEO_TGEO | ALWAYS_EQ_TGEO_GEO | ALWAYS_EQ_TGEO_TGEO | ALWAYS_NE_GEO_TGEO | ALWAYS_NE_TGEO_GEO | ALWAYS_NE_TGEO_TGEO | EVER_EQ_GEO_TGEO | EVER_EQ_TGEO_GEO | EVER_EQ_TGEO_TGEO | EVER_NE_GEO_TGEO | EVER_NE_TGEO_GEO | EVER_NE_TGEO_TGEO | TCONTAINS_GEO_TGEO| TEMPORAL_ECONTAINS_GEOMETRY;

sinkClause: INTO sink (',' sink)*;

sink: identifier;

nullNotnull
    : NOT? NULLTOKEN
    ;

streamName: IDENTIFIER;

fileFormat: CSV_FORMAT;

sortItem
    : expression ordering=(ASC | DESC)? (NULLS nullOrder=(LAST | FIRST))?
    ;

predicate
    : NOT? kind=BETWEEN lower=valueExpression AND upper=valueExpression
    | NOT? kind=IN '(' expression (',' expression)* ')'
    | NOT? kind=IN '(' query ')'
    | NOT? kind=RLIKE pattern=valueExpression
    | NOT? kind=LIKE quantifier=(ANY | SOME | ALL) ('('')' | '(' expression (',' expression)* ')')
    | NOT? kind=LIKE pattern=valueExpression (ESCAPE escapeChar=STRING)?
    | IS nullNotnull
    | IS NOT? kind=(TRUE | FALSE | UNKNOWN)
    | IS NOT? kind=DISTINCT FROM right=valueExpression
    ;


valueExpression
    : functionName '(' (argument+=expression (',' argument+=expression)*)? ')'                 #functionCall
    | op=(MINUS | PLUS | TILDE) valueExpression                                        #arithmeticUnary
    | left=valueExpression op=(ASTERISK | SLASH | PERCENT | DIV) right=valueExpression #arithmeticBinary
    | left=valueExpression op=(PLUS | MINUS | CONCAT_PIPE) right=valueExpression       #arithmeticBinary
    | left=valueExpression op=AMPERSAND right=valueExpression                          #arithmeticBinary
    | left=valueExpression op=HAT right=valueExpression                                #arithmeticBinary
    | left=valueExpression op=PIPE right=valueExpression                               #arithmeticBinary
    | left=valueExpression comparisonOperator right=valueExpression                          #comparison
    | INFER_MODEL '(' IDENTIFIER ',' inferModelInputFields ')'       #inference
    | primaryExpression                                                                      #valueExpressionDefault
    ;

comparisonOperator
    : EQ | NEQ | NEQJ | LT | LTE | GT | GTE | NSEQ
    ;

hint
    : '/*+' hintStatements+=hintStatement (','? hintStatements+=hintStatement)* '*/'
    ;

hintStatement
    : hintName=identifier
    | hintName=identifier '(' parameters+=primaryExpression (',' parameters+=primaryExpression)* ')'
    ;

primaryExpression
    : ASTERISK                                                                                 #star
    | qualifiedName '.' ASTERISK                                                               #star
    | base=primaryExpression '.' fieldName=identifier                                          #dereference
    | '(' query ')'                                                                            #subqueryExpression
    | '(' namedExpression (',' namedExpression)+ ')'                                           #rowConstructor
    | '(' expression ')'                                                                       #parenthesizedExpression
    | constant                                                                                 #constantDefault
    | identifier                                                                               #columnReference
    ;

inferModelInputFields: primaryExpression;

qualifiedName
    : identifier ('.' identifier)*
    ;

number
    : MINUS? INTEGER_VALUE              #integerLiteral
    | MINUS? FLOAT_LITERAL              #floatLiteral
    ;

constant
    : NULLTOKEN                                                                                #nullLiteral
    | identifier STRING                                                                        #typeConstructor
    | number                                                                                   #numericLiteral
    | booleanValue                                                                             #booleanLiteral
    | STRING+                                                                                  #stringLiteral
    ;

booleanValue
    : TRUE | FALSE
    ;


ALL: 'ALL' | 'all';
AND: 'AND' | 'and';
ANY: 'ANY';
AS: 'AS' | 'as';
ASC: 'ASC' | 'asc';
AT: 'AT';
BETWEEN: 'BETWEEN' | 'between';
BY: 'BY' | 'by';
COMMENT: 'COMMENT';
CUBE: 'CUBE';
DELETE: 'DELETE';
DESC: 'DESC' | 'desc';
DISTINCT: 'DISTINCT';
DIV: 'DIV';
DROP: 'DROP';
ELSE: 'ELSE';
END: 'END';
ESCAPE: 'ESCAPE';
EXISTS: 'EXISTS';
FALSE: 'FALSE';
FIRST: 'FIRST';
FOR: 'FOR';
FROM: 'FROM' | 'from';
FULL: 'FULL';
GROUP: 'GROUP' | 'group';
GROUPING: 'GROUPING';
HAVING: 'HAVING' | 'having';
IF: 'IF';
IN: 'IN' | 'in';
INFER_MODEL: 'INFER_MODEL' | 'infer_model';
INNER: 'INNER' | 'inner';
INSERT: 'INSERT' | 'insert';
INTO: 'INTO' | 'into';
IS: 'IS'  'is';
JOIN: 'JOIN' | 'join';
LAST: 'LAST';
LEFT: 'LEFT';
LIKE: 'LIKE';
LIMIT: 'LIMIT' | 'limit';
LIST: 'LIST';
ARRAY_AGG: 'ARRAY_AGG' | 'array_agg';
NATURAL: 'NATURAL';
NOT: 'NOT' | 'not' | '!';
NULLTOKEN:'NULL';
NULLS: 'NULLS';
OF: 'OF';
ON: 'ON' | 'on';
OR: 'OR' | 'or';
ORDER: 'ORDER' | 'order';
QUERY: 'QUERY';
RECOVER: 'RECOVER';
RIGHT: 'RIGHT';
RLIKE: 'RLIKE' | 'REGEXP';
ROLLUP: 'ROLLUP';
SELECT: 'SELECT' | 'select';
SETS: 'SETS';
SOME: 'SOME';
START: 'START';
TABLE: 'TABLE';
TO: 'TO';
TRUE: 'TRUE';
TYPE: 'TYPE';
UNION: 'UNION' | 'union';
UNKNOWN: 'UNKNOWN';
USE: 'USE';
USING: 'USING';
VALUES: 'VALUES';
WHEN: 'WHEN';
WHERE: 'WHERE' | 'where';
WINDOW: 'WINDOW' | 'window';
WITH: 'WITH';
TUMBLING: 'TUMBLING' | 'tumbling';
SLIDING: 'SLIDING' | 'sliding';
THRESHOLD : 'THRESHOLD'|'threshold';
SIZE: 'SIZE' | 'size';
ADVANCE: 'ADVANCE' | 'advance';
MS: 'MS' | 'ms';
SEC: 'SEC' | 'sec';
MINUTE: 'MINUTE' | 'minute' | 'MINUTES' | 'minutes';
HOUR: 'HOUR' | 'hour' | 'HOURS' | 'hours';
DAY: 'DAY' | 'day' | 'DAYS' | 'days';
MIN: 'MIN' | 'min';
MAX: 'MAX' | 'max';
AVG: 'AVG' | 'avg';
SUM: 'SUM' | 'sum';
COUNT: 'COUNT' | 'count';
MEDIAN: 'MEDIAN' | 'median';
VAR: 'VAR' | 'var';
TEMPORAL_SEQUENCE: 'TEMPORAL_SEQUENCE' | 'temporal_sequence';
TEMPORAL_EINTERSECTS_GEOMETRY: 'TEMPORAL_EINTERSECTS_GEOMETRY' | 'temporal_eintersects_geometry';
TEMPORAL_AINTERSECTS_GEOMETRY: 'TEMPORAL_AINTERSECTS_GEOMETRY' | 'temporal_aintersects_geometry';
EINTERSECTS_TGEO_TGEO: 'EINTERSECTS_TGEO_TGEO' | 'eintersects_tgeo_tgeo';
EINTERSECTS_TGEO_GEO: 'EINTERSECTS_TGEO_GEO' | 'eintersects_tgeo_geo';
AINTERSECTS_TGEO_TGEO: 'AINTERSECTS_TGEO_TGEO' | 'aintersects_tgeo_tgeo';
AINTERSECTS_TGEO_GEO: 'AINTERSECTS_TGEO_GEO' | 'aintersects_tgeo_geo';
ACONTAINS_GEO_TGEO: 'ACONTAINS_GEO_TGEO' | 'acontains_geo_tgeo';
ACONTAINS_TGEO_TGEO: 'ACONTAINS_TGEO_TGEO' | 'acontains_tgeo_tgeo';
ACONTAINS_TGEO_GEO: 'ACONTAINS_TGEO_GEO' | 'acontains_tgeo_geo';
ADISJOINT_TGEO_GEO: 'ADISJOINT_TGEO_GEO' | 'adisjoint_tgeo_geo';
ADISJOINT_TGEO_TGEO: 'ADISJOINT_TGEO_TGEO' | 'adisjoint_tgeo_tgeo';
ATOUCHES_TGEO_GEO: 'ATOUCHES_TGEO_GEO' | 'atouches_tgeo_geo';
ATOUCHES_TGEO_TGEO: 'ATOUCHES_TGEO_TGEO' | 'atouches_tgeo_tgeo';
ATOUCHES_TPOINT_GEO: 'ATOUCHES_TPOINT_GEO' | 'atouches_tpoint_geo';
ETOUCHES_TPOINT_GEO: 'ETOUCHES_TPOINT_GEO' | 'etouches_tpoint_geo';
ETOUCHES_TGEO_GEO: 'ETOUCHES_TGEO_GEO' | 'etouches_tgeo_geo';
ETOUCHES_TGEO_TGEO: 'ETOUCHES_TGEO_TGEO' | 'etouches_tgeo_tgeo';
EDWITHIN_TGEO_GEO: 'EDWITHIN_TGEO_GEO' | 'edwithin_tgeo_geo';
ADWITHIN_TGEO_GEO: 'ADWITHIN_TGEO_GEO' | 'adwithin_tgeo_geo';
ADWITHIN_TGEO_TGEO: 'ADWITHIN_TGEO_TGEO' | 'adwithin_tgeo_tgeo';
ECONTAINS_GEO_TGEO: 'ECONTAINS_GEO_TGEO' | 'econtains_geo_tgeo';
ECONTAINS_TGEO_GEO: 'ECONTAINS_TGEO_GEO' | 'econtains_tgeo_geo';
ECONTAINS_TGEO_TGEO: 'ECONTAINS_TGEO_TGEO' | 'econtains_tgeo_tgeo';
ECOVERS_GEO_TGEO: 'ECOVERS_GEO_TGEO' | 'ecovers_geo_tgeo';
ECOVERS_TGEO_TGEO: 'ECOVERS_TGEO_TGEO' | 'ecovers_tgeo_tgeo';
ECOVERS_TGEO_GEO: 'ECOVERS_TGEO_GEO' | 'ecovers_tgeo_geo';
EDISJOINT_TGEO_GEO: 'EDISJOINT_TGEO_GEO' | 'edisjoint_tgeo_geo';
EDISJOINT_TGEO_TGEO: 'EDISJOINT_TGEO_TGEO' | 'edisjoint_tgeo_tgeo';
EDWITHIN_TGEO_TGEO: 'EDWITHIN_TGEO_TGEO' | 'edwithin_tgeo_tgeo';
ABOVE_STBOX_TSPATIAL: 'ABOVE_STBOX_TSPATIAL' | 'above_stbox_tspatial';
ABOVE_TSPATIAL_STBOX: 'ABOVE_TSPATIAL_STBOX' | 'above_tspatial_stbox';
ABOVE_TSPATIAL_TSPATIAL: 'ABOVE_TSPATIAL_TSPATIAL' | 'above_tspatial_tspatial';
AFTER_STBOX_TSPATIAL: 'AFTER_STBOX_TSPATIAL' | 'after_stbox_tspatial';
AFTER_TSPATIAL_STBOX: 'AFTER_TSPATIAL_STBOX' | 'after_tspatial_stbox';
AFTER_TSPATIAL_TSPATIAL: 'AFTER_TSPATIAL_TSPATIAL' | 'after_tspatial_tspatial';
BACK_STBOX_TSPATIAL: 'BACK_STBOX_TSPATIAL' | 'back_stbox_tspatial';
BACK_TSPATIAL_STBOX: 'BACK_TSPATIAL_STBOX' | 'back_tspatial_stbox';
BACK_TSPATIAL_TSPATIAL: 'BACK_TSPATIAL_TSPATIAL' | 'back_tspatial_tspatial';
BEFORE_STBOX_TSPATIAL: 'BEFORE_STBOX_TSPATIAL' | 'before_stbox_tspatial';
BEFORE_TSPATIAL_STBOX: 'BEFORE_TSPATIAL_STBOX' | 'before_tspatial_stbox';
BEFORE_TSPATIAL_TSPATIAL: 'BEFORE_TSPATIAL_TSPATIAL' | 'before_tspatial_tspatial';
BELOW_STBOX_TSPATIAL: 'BELOW_STBOX_TSPATIAL' | 'below_stbox_tspatial';
BELOW_TSPATIAL_STBOX: 'BELOW_TSPATIAL_STBOX' | 'below_tspatial_stbox';
BELOW_TSPATIAL_TSPATIAL: 'BELOW_TSPATIAL_TSPATIAL' | 'below_tspatial_tspatial';
FRONT_STBOX_TSPATIAL: 'FRONT_STBOX_TSPATIAL' | 'front_stbox_tspatial';
FRONT_TSPATIAL_STBOX: 'FRONT_TSPATIAL_STBOX' | 'front_tspatial_stbox';
FRONT_TSPATIAL_TSPATIAL: 'FRONT_TSPATIAL_TSPATIAL' | 'front_tspatial_tspatial';
LEFT_STBOX_TSPATIAL: 'LEFT_STBOX_TSPATIAL' | 'left_stbox_tspatial';
LEFT_TSPATIAL_STBOX: 'LEFT_TSPATIAL_STBOX' | 'left_tspatial_stbox';
LEFT_TSPATIAL_TSPATIAL: 'LEFT_TSPATIAL_TSPATIAL' | 'left_tspatial_tspatial';
OVERABOVE_STBOX_TSPATIAL: 'OVERABOVE_STBOX_TSPATIAL' | 'overabove_stbox_tspatial';
OVERABOVE_TSPATIAL_STBOX: 'OVERABOVE_TSPATIAL_STBOX' | 'overabove_tspatial_stbox';
OVERABOVE_TSPATIAL_TSPATIAL: 'OVERABOVE_TSPATIAL_TSPATIAL' | 'overabove_tspatial_tspatial';
OVERAFTER_STBOX_TSPATIAL: 'OVERAFTER_STBOX_TSPATIAL' | 'overafter_stbox_tspatial';
OVERAFTER_TSPATIAL_STBOX: 'OVERAFTER_TSPATIAL_STBOX' | 'overafter_tspatial_stbox';
OVERAFTER_TSPATIAL_TSPATIAL: 'OVERAFTER_TSPATIAL_TSPATIAL' | 'overafter_tspatial_tspatial';
OVERBACK_STBOX_TSPATIAL: 'OVERBACK_STBOX_TSPATIAL' | 'overback_stbox_tspatial';
OVERBACK_TSPATIAL_STBOX: 'OVERBACK_TSPATIAL_STBOX' | 'overback_tspatial_stbox';
OVERBACK_TSPATIAL_TSPATIAL: 'OVERBACK_TSPATIAL_TSPATIAL' | 'overback_tspatial_tspatial';
OVERBEFORE_STBOX_TSPATIAL: 'OVERBEFORE_STBOX_TSPATIAL' | 'overbefore_stbox_tspatial';
OVERBEFORE_TSPATIAL_STBOX: 'OVERBEFORE_TSPATIAL_STBOX' | 'overbefore_tspatial_stbox';
OVERBEFORE_TSPATIAL_TSPATIAL: 'OVERBEFORE_TSPATIAL_TSPATIAL' | 'overbefore_tspatial_tspatial';
OVERBELOW_STBOX_TSPATIAL: 'OVERBELOW_STBOX_TSPATIAL' | 'overbelow_stbox_tspatial';
OVERBELOW_TSPATIAL_STBOX: 'OVERBELOW_TSPATIAL_STBOX' | 'overbelow_tspatial_stbox';
OVERBELOW_TSPATIAL_TSPATIAL: 'OVERBELOW_TSPATIAL_TSPATIAL' | 'overbelow_tspatial_tspatial';
OVERFRONT_STBOX_TSPATIAL: 'OVERFRONT_STBOX_TSPATIAL' | 'overfront_stbox_tspatial';
OVERFRONT_TSPATIAL_STBOX: 'OVERFRONT_TSPATIAL_STBOX' | 'overfront_tspatial_stbox';
OVERFRONT_TSPATIAL_TSPATIAL: 'OVERFRONT_TSPATIAL_TSPATIAL' | 'overfront_tspatial_tspatial';
OVERLEFT_STBOX_TSPATIAL: 'OVERLEFT_STBOX_TSPATIAL' | 'overleft_stbox_tspatial';
OVERLEFT_TSPATIAL_STBOX: 'OVERLEFT_TSPATIAL_STBOX' | 'overleft_tspatial_stbox';
OVERLEFT_TSPATIAL_TSPATIAL: 'OVERLEFT_TSPATIAL_TSPATIAL' | 'overleft_tspatial_tspatial';
OVERRIGHT_STBOX_TSPATIAL: 'OVERRIGHT_STBOX_TSPATIAL' | 'overright_stbox_tspatial';
OVERRIGHT_TSPATIAL_STBOX: 'OVERRIGHT_TSPATIAL_STBOX' | 'overright_tspatial_stbox';
OVERRIGHT_TSPATIAL_TSPATIAL: 'OVERRIGHT_TSPATIAL_TSPATIAL' | 'overright_tspatial_tspatial';
RIGHT_TSPATIAL_TSPATIAL: 'RIGHT_TSPATIAL_TSPATIAL' | 'right_tspatial_tspatial';
RIGHT_TSPATIAL_STBOX: 'RIGHT_TSPATIAL_STBOX' | 'right_tspatial_stbox';
RIGHT_STBOX_TSPATIAL: 'RIGHT_STBOX_TSPATIAL' | 'right_stbox_tspatial';
ADJACENT_STBOX_TSPATIAL: 'ADJACENT_STBOX_TSPATIAL' | 'adjacent_stbox_tspatial';
ADJACENT_TSPATIAL_STBOX: 'ADJACENT_TSPATIAL_STBOX' | 'adjacent_tspatial_stbox';
ADJACENT_TSPATIAL_TSPATIAL: 'ADJACENT_TSPATIAL_TSPATIAL' | 'adjacent_tspatial_tspatial';
CONTAINED_STBOX_TSPATIAL: 'CONTAINED_STBOX_TSPATIAL' | 'contained_stbox_tspatial';
CONTAINED_TSPATIAL_STBOX: 'CONTAINED_TSPATIAL_STBOX' | 'contained_tspatial_stbox';
CONTAINED_TSPATIAL_TSPATIAL: 'CONTAINED_TSPATIAL_TSPATIAL' | 'contained_tspatial_tspatial';
CONTAINS_STBOX_TSPATIAL: 'CONTAINS_STBOX_TSPATIAL' | 'contains_stbox_tspatial';
CONTAINS_TSPATIAL_STBOX: 'CONTAINS_TSPATIAL_STBOX' | 'contains_tspatial_stbox';
CONTAINS_TSPATIAL_TSPATIAL: 'CONTAINS_TSPATIAL_TSPATIAL' | 'contains_tspatial_tspatial';
OVERLAPS_STBOX_TSPATIAL: 'OVERLAPS_STBOX_TSPATIAL' | 'overlaps_stbox_tspatial';
OVERLAPS_TSPATIAL_STBOX: 'OVERLAPS_TSPATIAL_STBOX' | 'overlaps_tspatial_stbox';
OVERLAPS_TSPATIAL_TSPATIAL: 'OVERLAPS_TSPATIAL_TSPATIAL' | 'overlaps_tspatial_tspatial';
SAME_STBOX_TSPATIAL: 'SAME_STBOX_TSPATIAL' | 'same_stbox_tspatial';
SAME_TSPATIAL_STBOX: 'SAME_TSPATIAL_STBOX' | 'same_tspatial_stbox';
SAME_TSPATIAL_TSPATIAL: 'SAME_TSPATIAL_TSPATIAL' | 'same_tspatial_tspatial';
ALWAYS_EQ_GEO_TGEO: 'ALWAYS_EQ_GEO_TGEO' | 'always_eq_geo_tgeo';
ALWAYS_EQ_TGEO_GEO: 'ALWAYS_EQ_TGEO_GEO' | 'always_eq_tgeo_geo';
ALWAYS_EQ_TGEO_TGEO: 'ALWAYS_EQ_TGEO_TGEO' | 'always_eq_tgeo_tgeo';
ALWAYS_NE_GEO_TGEO: 'ALWAYS_NE_GEO_TGEO' | 'always_ne_geo_tgeo';
ALWAYS_NE_TGEO_GEO: 'ALWAYS_NE_TGEO_GEO' | 'always_ne_tgeo_geo';
ALWAYS_NE_TGEO_TGEO: 'ALWAYS_NE_TGEO_TGEO' | 'always_ne_tgeo_tgeo';
EVER_EQ_GEO_TGEO: 'EVER_EQ_GEO_TGEO' | 'ever_eq_geo_tgeo';
EVER_EQ_TGEO_GEO: 'EVER_EQ_TGEO_GEO' | 'ever_eq_tgeo_geo';
EVER_EQ_TGEO_TGEO: 'EVER_EQ_TGEO_TGEO' | 'ever_eq_tgeo_tgeo';
EVER_NE_GEO_TGEO: 'EVER_NE_GEO_TGEO' | 'ever_ne_geo_tgeo';
EVER_NE_TGEO_GEO: 'EVER_NE_TGEO_GEO' | 'ever_ne_tgeo_geo';
EVER_NE_TGEO_TGEO: 'EVER_NE_TGEO_TGEO' | 'ever_ne_tgeo_tgeo';
TCONTAINS_GEO_TGEO: 'TCONTAINS_GEO_TGEO' | 'tcontains_geo_tgeo';
TEMPORAL_ECONTAINS_GEOMETRY: 'TEMPORAL_ECONTAINS_GEOMETRY' | 'temporal_econtains_geometry';
WATERMARK: 'WATERMARK' | 'watermark';
OFFSET: 'OFFSET' | 'offset';
LOCALHOST: 'LOCALHOST' | 'localhost';
CSV_FORMAT : 'CSV_FORMAT';
AT_MOST_ONCE : 'AT_MOST_ONCE';
AT_LEAST_ONCE : 'AT_LEAST_ONCE';
///--NebulaSQL-KEYWORD-LIST-END
///****************************
/// End of the keywords list
///****************************



BOOLEAN_VALUE: 'true' | 'false';
EQ  : '=' | '==';
NSEQ: '<=>';
NEQ : '<>';
NEQJ: '!=';
LT  : '<';
LTE : '<=' | '!>';
GT  : '>';
GTE : '>=' | '!<';

PLUS: '+';
MINUS: '-';
ASTERISK: '*';
SLASH: '/';
PERCENT: '%';
TILDE: '~';
AMPERSAND: '&';
PIPE: '|';
CONCAT_PIPE: '||';
HAT: '^';

STRING
    : '\'' ( ~('\''|'\\') | ('\\' .) )* '\''
    | '"' ( ~('"'|'\\') | ('\\' .) )* '"'
    ;

INTEGER_VALUE
    : DIGIT+
    ;

FLOAT_LITERAL
    : DIGIT+ EXPONENT?
    | DECIMAL_DIGITS EXPONENT? {isValidDecimal()}?
    ;

IDENTIFIER
    : (LETTER | DIGIT | '_')+
    ;

fragment DECIMAL_DIGITS
    : DIGIT+ '.' DIGIT*
    | '.' DIGIT+
    ;

fragment EXPONENT
    : 'E' [+-]? DIGIT+
    ;

fragment DIGIT
    : [0-9]
    ;

fragment LETTER
    : ('a'..'z'|'A'..'Z'|'_')
    ;

WS
    : [ \r\n\t]+ -> channel(HIDDEN)
    ;

/// Catch-all for anything we can't recognize.
/// We use this to be able to ignore and recover all the text
/// when splitting statements with DelimiterLexer
UNRECOGNIZED
    : .
    ;

//Make sure that you add lexer rules for keywords before the identifier rule,
//otherwise it will take priority and your grammars will not work

SIMPLE_COMMENT
    : '--' ('\\\n' | ~[\r\n])* '\r'? '\n'? -> channel(HIDDEN)
    ;

BRACKETED_COMMENT
    : '/*' {!isHint()}? (BRACKETED_COMMENT|.)*? '*/' -> channel(HIDDEN)
    ;

