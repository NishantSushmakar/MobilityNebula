// Generated SQL parser cases for MEOS functions
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL GEOG_INTERSECTS_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolGeogIntersectsLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolGeogIntersectsLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL GEOG_INTERSECTS_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL GEOM_CONTAINS_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolGeomContainsLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolGeomContainsLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL GEOM_CONTAINS_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL GEOM_INTERSECTS2D_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolGeomIntersects2dLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolGeomIntersects2dLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL GEOM_INTERSECTS2D_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL GEOM_INTERSECTS3D_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolGeomIntersects3dLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolGeomIntersects3dLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL GEOM_INTERSECTS3D_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TCONTAINS_GEO_TGEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TcontainsGeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TcontainsGeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TCONTAINS_GEO_TGEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TCONTAINS_TGEO_GEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TcontainsTgeoGeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TcontainsTgeoGeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TCONTAINS_TGEO_GEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TCONTAINS_TGEO_TGEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TcontainsTgeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TcontainsTgeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TCONTAINS_TGEO_TGEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TINTERSECTS_GEO_TGEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TintersectsGeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TintersectsGeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TINTERSECTS_GEO_TGEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TINTERSECTS_TGEO_GEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TintersectsTgeoGeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TintersectsTgeoGeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TINTERSECTS_TGEO_GEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_TINTERSECTS_TGEO_TGEO_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<TintersectsTgeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<TintersectsTgeoTgeoLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_TINTERSECTS_TGEO_TGEO_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_BIGINT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_BIGINT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_DATE_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_DATE_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_FLOAT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_FLOAT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_INT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_INT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_SET_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetSetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetSetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_SET_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_TEXT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetTextLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetTextLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_TEXT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SET_TIMESTAMPTZ_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSetTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSetTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SET_TIMESTAMPTZ_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_BIGINT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_BIGINT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_DATE_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_DATE_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_FLOAT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_FLOAT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_INT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_INT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_SPAN_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanSpanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanSpanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_SPAN_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_SPANSET_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanSpansetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanSpansetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_SPANSET_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPAN_TIMESTAMPTZ_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpanTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpanTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_TIMESTAMPTZ_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_BIGINT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetBigintLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_BIGINT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_DATE_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetDateLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_DATE_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_FLOAT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetFloatLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_FLOAT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_INT_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetIntLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_INT_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_SPAN_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetSpanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetSpanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_SPAN_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_SPANSET_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetSpansetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetSpansetLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_SPANSET_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_TIMESTAMPTZ_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsSpansetTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsSpansetTimestamptzLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_SPANSET_TIMESTAMPTZ_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_NUMSPAN_TNUMBER_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsNumspanTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsNumspanTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_NUMSPAN_TNUMBER_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TBOX_TNUMBER_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTboxTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTboxTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TBOX_TNUMBER_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TEMPORAL_TSTZSPAN_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTemporalTstzspanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTemporalTstzspanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TEMPORAL_TSTZSPAN_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TEMPORAL_TEMPORAL_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTemporalTemporalLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTemporalTemporalLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TEMPORAL_TEMPORAL_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_NUMSPAN_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTnumberNumspanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTnumberNumspanLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_NUMSPAN_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_TBOX_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTnumberTboxLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTnumberTboxLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_TBOX_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_TNUMBER_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTnumberTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTnumberTnumberLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TNUMBER_TNUMBER_GEOMETRY requires either 4 or 6 arguments");
            }
        }
        case AntlrSQLParser::TEMPORAL_EXTERN BOOL CONTAINS_TSTZSPAN_TEMPORAL_GEOMETRY: {
            auto arguments = ctx->argument();
            if (arguments.size() == 4) {
                // 4-parameter version: temporal-static intersection
                return std::make_shared<ExternBoolContainsTstzspanTemporalLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), visit(arguments[3])
                );
            } else if (arguments.size() == 6) {
                // 6-parameter version: temporal-temporal intersection  
                return std::make_shared<ExternBoolContainsTstzspanTemporalLogicalFunction>(
                    visit(arguments[0]), visit(arguments[1]), visit(arguments[2]), 
                    visit(arguments[3]), visit(arguments[4]), visit(arguments[5])
                );
            } else {
                throw std::invalid_argument("TEMPORAL_EXTERN BOOL CONTAINS_TSTZSPAN_TEMPORAL_GEOMETRY requires either 4 or 6 arguments");
            }
        }
