void Meos::TemporalGeometry::externBoolContainsSpansetDate({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetDate called" << std::endl;        
    void result = extern bool contains_spanset_date({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}