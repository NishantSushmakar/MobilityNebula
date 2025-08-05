// Generated MEOS function implementations
void Meos::TemporalGeometry::externBoolGeogIntersects({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolGeogIntersects called" << std::endl;        
    void result = extern bool geog_intersects({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolGeomContains({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolGeomContains called" << std::endl;        
    void result = extern bool geom_contains({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolGeomIntersects2d({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolGeomIntersects2d called" << std::endl;        
    void result = extern bool geom_intersects2d({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolGeomIntersects3d({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolGeomIntersects3d called" << std::endl;        
    void result = extern bool geom_intersects3d({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tcontainsGeoTgeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tcontainsGeoTgeo called" << std::endl;        
    extern Temporal * result = tcontains_geo_tgeo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tcontainsTgeoGeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tcontainsTgeoGeo called" << std::endl;        
    extern Temporal * result = tcontains_tgeo_geo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tcontainsTgeoTgeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tcontainsTgeoTgeo called" << std::endl;        
    extern Temporal * result = tcontains_tgeo_tgeo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tintersectsGeoTgeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tintersectsGeoTgeo called" << std::endl;        
    extern Temporal * result = tintersects_geo_tgeo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tintersectsTgeoGeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tintersectsTgeoGeo called" << std::endl;        
    extern Temporal * result = tintersects_tgeo_geo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

extern Temporal * Meos::TemporalGeometry::tintersectsTgeoTgeo({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::tintersectsTgeoTgeo called" << std::endl;        
    extern Temporal * result = tintersects_tgeo_tgeo({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetBigint({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetBigint called" << std::endl;        
    void result = extern bool contains_set_bigint({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetDate({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetDate called" << std::endl;        
    void result = extern bool contains_set_date({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetFloat({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetFloat called" << std::endl;        
    void result = extern bool contains_set_float({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetInt({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetInt called" << std::endl;        
    void result = extern bool contains_set_int({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetSet({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetSet called" << std::endl;        
    void result = extern bool contains_set_set({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetText({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetText called" << std::endl;        
    void result = extern bool contains_set_text({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSetTimestamptz({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSetTimestamptz called" << std::endl;        
    void result = extern bool contains_set_timestamptz({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanBigint({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanBigint called" << std::endl;        
    void result = extern bool contains_span_bigint({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanDate({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanDate called" << std::endl;        
    void result = extern bool contains_span_date({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanFloat({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanFloat called" << std::endl;        
    void result = extern bool contains_span_float({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanInt({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanInt called" << std::endl;        
    void result = extern bool contains_span_int({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanSpan({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanSpan called" << std::endl;        
    void result = extern bool contains_span_span({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanSpanset({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanSpanset called" << std::endl;        
    void result = extern bool contains_span_spanset({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpanTimestamptz({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpanTimestamptz called" << std::endl;        
    void result = extern bool contains_span_timestamptz({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetBigint({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetBigint called" << std::endl;        
    void result = extern bool contains_spanset_bigint({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetDate({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetDate called" << std::endl;        
    void result = extern bool contains_spanset_date({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetFloat({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetFloat called" << std::endl;        
    void result = extern bool contains_spanset_float({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetInt({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetInt called" << std::endl;        
    void result = extern bool contains_spanset_int({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetSpan({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetSpan called" << std::endl;        
    void result = extern bool contains_spanset_span({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetSpanset({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetSpanset called" << std::endl;        
    void result = extern bool contains_spanset_spanset({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsSpansetTimestamptz({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsSpansetTimestamptz called" << std::endl;        
    void result = extern bool contains_spanset_timestamptz({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsNumspanTnumber({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsNumspanTnumber called" << std::endl;        
    void result = extern bool contains_numspan_tnumber({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTboxTnumber({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTboxTnumber called" << std::endl;        
    void result = extern bool contains_tbox_tnumber({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTemporalTstzspan({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTemporalTstzspan called" << std::endl;        
    void result = extern bool contains_temporal_tstzspan({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTemporalTemporal({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTemporalTemporal called" << std::endl;        
    void result = extern bool contains_temporal_temporal({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTnumberNumspan({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTnumberNumspan called" << std::endl;        
    void result = extern bool contains_tnumber_numspan({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTnumberTbox({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTnumberTbox called" << std::endl;        
    void result = extern bool contains_tnumber_tbox({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTnumberTnumber({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTnumberTnumber called" << std::endl;        
    void result = extern bool contains_tnumber_tnumber({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

void Meos::TemporalGeometry::externBoolContainsTstzspanTemporal({{#parameters}}{{#isConst}}const {{/isConst}}{{type}}{{#isPointer}}*{{/isPointer}}{{#isReference}}&{{/isReference}} {{name}}{{#notLast}}, {{/notLast}}{{/parameters}}) const {
    std::cout << "TemporalGeometry::externBoolContainsTstzspanTemporal called" << std::endl;        
    void result = extern bool contains_tstzspan_temporal({{#parameters}}{{#needsCast}}({{castType}}){{/needsCast}}{{#passPointer}}{{/passPointer}}{{name}}{{#notLast}}, {{/notLast}}{{/parameters}});
    return result;
}

