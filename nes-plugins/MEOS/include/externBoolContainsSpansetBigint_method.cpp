void Meos::TemporalGeometry::externBoolContainsSpansetBigint({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetBigint called" << std::endl;        
    void result = extern bool contains_spanset_bigint({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}