void Meos::TemporalGeometry::externBoolContainsSpanSpan({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanSpan called" << std::endl;        
    void result = extern bool contains_span_span({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}