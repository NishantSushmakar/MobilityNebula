#include <iostream>
#include <string>
#include <vector>
extern "C" {
#include <meos.h>
}

int main() {
    // Initialize MEOS
    meos_initialize();
    
    // Test data from meos_intersects_temporal_geometry_string.test
    std::vector<std::pair<std::string, std::string>> test_cases;
    
    // Test 1: Multiple temporal geometry intersect operations
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9847 40.7494)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9867 40.7494)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7504)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9877 40.7504)@2021-01-01 00:20:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9867 40.7514)@2021-01-01 00:20:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9887 40.7514)@2021-01-01 00:30:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9877 40.7524)@2021-01-01 00:30:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9800 40.7450)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9790 40.7460)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9810 40.7460)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9800 40.7470)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9820 40.7470)@2021-01-01 00:20:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9810 40.7480)@2021-01-01 00:20:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9830 40.7480)@2021-01-01 00:30:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9820 40.7490)@2021-01-01 00:30:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});

    // Test 2: Mixed temporal and static geometries
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9855 40.7483)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9867 40.7494)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9865 40.7493)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9877 40.7504)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9875 40.7503)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9798 40.7449)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9810 40.7460)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9808 40.7459)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9820 40.7470)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9818 40.7469)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9900 40.7500)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9898 40.7499)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9910 40.7510)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9908 40.7509)"});
    test_cases.push_back({"SRID=4326;POINT(-73.9920 40.7520)@2021-01-01 00:10:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9918 40.7519)"});

    // Test 3: Multiple geometry checks with same coordinates
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:01:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9858 40.7485)@2021-01-01 00:01:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9859 40.7486)@2021-01-01 00:02:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9860 40.7487)@2021-01-01 00:02:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9861 40.7488)@2021-01-01 00:03:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:03:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:04:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9900 40.7500)@2021-01-01 00:04:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9910 40.7510)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});

    // Test 4: Simple temporal points for validation
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:01:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9860 40.7487)@2021-01-01 00:02:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:03:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9900 40.7500)@2021-01-01 00:04:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});

    // Test 5: Sequential temporal points
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9858 40.7485)@2021-01-01 00:01:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:02:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9860 40.7487)@2021-01-01 00:03:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:04:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9862 40.7489)@2021-01-01 00:05:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});
    test_cases.push_back({"SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:06:00", "SRID=4326;POINT(-73.9857 40.7484)@2021-01-01 00:00:00"});

    std::cout << "Testing MEOS eintersects_tgeo_tgeo and eintersects_tgeo_geo functions:" << std::endl;
    std::cout << "Results format: 1=intersects, 0=no intersection, -1=no temporal overlap/error" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < test_cases.size(); i++) {
        const auto& left = test_cases[i].first;
        const auto& right = test_cases[i].second;
        
        std::cout << "Test " << (i + 1) << ":" << std::endl;
        std::cout << "  Left:  " << left << std::endl;
        std::cout << "  Right: " << right << std::endl;
        
        try {
            // Detect if geometries are temporal or static
            bool isLeftTemporal = left.find('@') != std::string::npos;
            bool isRightTemporal = right.find('@') != std::string::npos;
            
            int result = -1;
            
            if (isLeftTemporal && isRightTemporal) {
                // Both temporal - use eintersects_tgeo_tgeo
                Temporal* leftTemporal = tgeometry_in(left.c_str());
                Temporal* rightTemporal = tgeometry_in(right.c_str());
                if (leftTemporal && rightTemporal) {
                    result = eintersects_tgeo_tgeo(leftTemporal, rightTemporal);
                    free(leftTemporal);
                    free(rightTemporal);
                }
                std::cout << "  Type: temporal-temporal (eintersects_tgeo_tgeo)" << std::endl;
            } else if (isLeftTemporal && !isRightTemporal) {
                // Left temporal, right static - use eintersects_tgeo_geo
                Temporal* leftTemporal = tgeometry_in(left.c_str());
                GSERIALIZED* rightStatic = geom_in(right.c_str(), -1);
                if (leftTemporal && rightStatic) {
                    result = eintersects_tgeo_geo(leftTemporal, rightStatic);
                    free(leftTemporal);
                    free(rightStatic);
                }
                std::cout << "  Type: temporal-static (eintersects_tgeo_geo)" << std::endl;
            } else {
                std::cout << "  Type: INVALID - left geometry must be temporal" << std::endl;
                result = -1;
            }
            
            std::cout << "  Result: " << result << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "  Exception: " << e.what() << std::endl;
            std::cout << "  Result: -1" << std::endl;
        } catch (...) {
            std::cout << "  Unknown exception" << std::endl;
            std::cout << "  Result: -1" << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    // Finalize MEOS
    meos_finalize();
    return 0;
}