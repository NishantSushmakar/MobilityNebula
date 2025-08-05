# MEOS Function Integration Summary

Generated 39 MEOS functions for integration.

## Generated Components:
- 39 Logical Function classes (header + implementation)
- 39 Physical Function classes (header + implementation)
- 39 MEOSWrapper method declarations and implementations
- 39 SQL grammar tokens
- 39 SQL parser cases
- 39 Test files

## Integration Steps:

### 1. Add Logical Functions to Build
Add generated logical function files to `nes-logical-operators/CMakeLists.txt`:
- `src/Functions/Meos/ExternBoolGeogIntersectsLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomContainsLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomIntersects2dLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomIntersects3dLogicalFunction.cpp`
- `src/Functions/Meos/TcontainsGeoTgeoLogicalFunction.cpp`
- `src/Functions/Meos/TcontainsTgeoGeoLogicalFunction.cpp`
- `src/Functions/Meos/TcontainsTgeoTgeoLogicalFunction.cpp`
- `src/Functions/Meos/TintersectsGeoTgeoLogicalFunction.cpp`
- `src/Functions/Meos/TintersectsTgeoGeoLogicalFunction.cpp`
- `src/Functions/Meos/TintersectsTgeoTgeoLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetBigintLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetDateLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetFloatLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetIntLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetSetLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetTextLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetTimestamptzLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanBigintLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanDateLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanFloatLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanIntLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanSpanLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanSpansetLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanTimestamptzLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetBigintLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetDateLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetFloatLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetIntLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetSpanLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetSpansetLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetTimestamptzLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsNumspanTnumberLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTboxTnumberLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTemporalTstzspanLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTemporalTemporalLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberNumspanLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberTboxLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberTnumberLogicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTstzspanTemporalLogicalFunction.cpp`

### 2. Add Physical Functions to Build
Add generated physical function files to `nes-physical-operators/CMakeLists.txt`:
- `src/Functions/Meos/ExternBoolGeogIntersectsPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomContainsPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomIntersects2dPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolGeomIntersects3dPhysicalFunction.cpp`
- `src/Functions/Meos/TcontainsGeoTgeoPhysicalFunction.cpp`
- `src/Functions/Meos/TcontainsTgeoGeoPhysicalFunction.cpp`
- `src/Functions/Meos/TcontainsTgeoTgeoPhysicalFunction.cpp`
- `src/Functions/Meos/TintersectsGeoTgeoPhysicalFunction.cpp`
- `src/Functions/Meos/TintersectsTgeoGeoPhysicalFunction.cpp`
- `src/Functions/Meos/TintersectsTgeoTgeoPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetBigintPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetDatePhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetFloatPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetIntPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetSetPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetTextPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSetTimestamptzPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanBigintPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanDatePhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanFloatPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanIntPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanSpanPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanSpansetPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpanTimestamptzPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetBigintPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetDatePhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetFloatPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetIntPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetSpanPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetSpansetPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsSpansetTimestamptzPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsNumspanTnumberPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTboxTnumberPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTemporalTstzspanPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTemporalTemporalPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberNumspanPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberTboxPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTnumberTnumberPhysicalFunction.cpp`
- `src/Functions/Meos/ExternBoolContainsTstzspanTemporalPhysicalFunction.cpp`

### 3. Update MEOSWrapper
- Add content from `generated_methods.hpp` to `nes-plugins/MEOS/include/MEOSWrapper.hpp`
- Add content from `generated_methods.cpp` to `nes-plugins/MEOS/MEOSWrapper.cpp`

### 4. Update SQL Grammar
- Add tokens from `generated_tokens.g4` to `nes-sql-parser/AntlrSQL.g4`
- Add includes from `required_includes.cpp` to `nes-sql-parser/src/AntlrSQLQueryPlanCreator.cpp`
- Add parser cases from `generated_parser_cases.cpp` to the function_call switch statement

### 5. Register Functions
- Add logical function registrations to appropriate registry
- Add physical function registrations to appropriate registry

### 6. Build and Test
```bash
ninja -C build-docker
# Run generated tests
ctest --test-dir build-docker -R "externBoolGeogIntersects"
ctest --test-dir build-docker -R "externBoolGeomContains"
ctest --test-dir build-docker -R "externBoolGeomIntersects2d"
ctest --test-dir build-docker -R "externBoolGeomIntersects3d"
ctest --test-dir build-docker -R "tcontainsGeoTgeo"
ctest --test-dir build-docker -R "tcontainsTgeoGeo"
ctest --test-dir build-docker -R "tcontainsTgeoTgeo"
ctest --test-dir build-docker -R "tintersectsGeoTgeo"
ctest --test-dir build-docker -R "tintersectsTgeoGeo"
ctest --test-dir build-docker -R "tintersectsTgeoTgeo"
ctest --test-dir build-docker -R "externBoolContainsSetBigint"
ctest --test-dir build-docker -R "externBoolContainsSetDate"
ctest --test-dir build-docker -R "externBoolContainsSetFloat"
ctest --test-dir build-docker -R "externBoolContainsSetInt"
ctest --test-dir build-docker -R "externBoolContainsSetSet"
ctest --test-dir build-docker -R "externBoolContainsSetText"
ctest --test-dir build-docker -R "externBoolContainsSetTimestamptz"
ctest --test-dir build-docker -R "externBoolContainsSpanBigint"
ctest --test-dir build-docker -R "externBoolContainsSpanDate"
ctest --test-dir build-docker -R "externBoolContainsSpanFloat"
ctest --test-dir build-docker -R "externBoolContainsSpanInt"
ctest --test-dir build-docker -R "externBoolContainsSpanSpan"
ctest --test-dir build-docker -R "externBoolContainsSpanSpanset"
ctest --test-dir build-docker -R "externBoolContainsSpanTimestamptz"
ctest --test-dir build-docker -R "externBoolContainsSpansetBigint"
ctest --test-dir build-docker -R "externBoolContainsSpansetDate"
ctest --test-dir build-docker -R "externBoolContainsSpansetFloat"
ctest --test-dir build-docker -R "externBoolContainsSpansetInt"
ctest --test-dir build-docker -R "externBoolContainsSpansetSpan"
ctest --test-dir build-docker -R "externBoolContainsSpansetSpanset"
ctest --test-dir build-docker -R "externBoolContainsSpansetTimestamptz"
ctest --test-dir build-docker -R "externBoolContainsNumspanTnumber"
ctest --test-dir build-docker -R "externBoolContainsTboxTnumber"
ctest --test-dir build-docker -R "externBoolContainsTemporalTstzspan"
ctest --test-dir build-docker -R "externBoolContainsTemporalTemporal"
ctest --test-dir build-docker -R "externBoolContainsTnumberNumspan"
ctest --test-dir build-docker -R "externBoolContainsTnumberTbox"
ctest --test-dir build-docker -R "externBoolContainsTnumberTnumber"
ctest --test-dir build-docker -R "externBoolContainsTstzspanTemporal"
```

## Generated Functions:
- `extern bool geog_intersects` (spatial_intersection)
- `extern bool geom_contains` (spatial_containment)
- `extern bool geom_intersects2d` (spatial_intersection)
- `extern bool geom_intersects3d` (spatial_intersection)
- `tcontains_geo_tgeo` (spatial_containment)
- `tcontains_tgeo_geo` (spatial_containment)
- `tcontains_tgeo_tgeo` (spatial_containment)
- `tintersects_geo_tgeo` (spatial_intersection)
- `tintersects_tgeo_geo` (spatial_intersection)
- `tintersects_tgeo_tgeo` (spatial_intersection)
- `extern bool contains_set_bigint` (spatial_containment)
- `extern bool contains_set_date` (spatial_containment)
- `extern bool contains_set_float` (spatial_containment)
- `extern bool contains_set_int` (spatial_containment)
- `extern bool contains_set_set` (spatial_containment)
- `extern bool contains_set_text` (spatial_containment)
- `extern bool contains_set_timestamptz` (spatial_containment)
- `extern bool contains_span_bigint` (spatial_containment)
- `extern bool contains_span_date` (spatial_containment)
- `extern bool contains_span_float` (spatial_containment)
- `extern bool contains_span_int` (spatial_containment)
- `extern bool contains_span_span` (spatial_containment)
- `extern bool contains_span_spanset` (spatial_containment)
- `extern bool contains_span_timestamptz` (spatial_containment)
- `extern bool contains_spanset_bigint` (spatial_containment)
- `extern bool contains_spanset_date` (spatial_containment)
- `extern bool contains_spanset_float` (spatial_containment)
- `extern bool contains_spanset_int` (spatial_containment)
- `extern bool contains_spanset_span` (spatial_containment)
- `extern bool contains_spanset_spanset` (spatial_containment)
- `extern bool contains_spanset_timestamptz` (spatial_containment)
- `extern bool contains_numspan_tnumber` (spatial_containment)
- `extern bool contains_tbox_tnumber` (spatial_containment)
- `extern bool contains_temporal_tstzspan` (spatial_containment)
- `extern bool contains_temporal_temporal` (spatial_containment)
- `extern bool contains_tnumber_numspan` (spatial_containment)
- `extern bool contains_tnumber_tbox` (spatial_containment)
- `extern bool contains_tnumber_tnumber` (spatial_containment)
- `extern bool contains_tstzspan_temporal` (spatial_containment)
