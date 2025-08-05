void Meos::TemporalGeometry::externBoolContainsSpansetSpanset({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetSpanset called" << std::endl;        
    void result = extern bool contains_spanset_spanset({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}