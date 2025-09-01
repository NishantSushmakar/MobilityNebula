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

#ifndef NES_PLUGINS_MEOS_HPP
#define NES_PLUGINS_MEOS_HPP

#include <string>
#include <vector>

extern "C" {
    #include <meos.h>
    #include <meos_geo.h>
}

namespace MEOS {

class Meos {
  public:
    /**
     * @brief Initialize MEOS library
     */
    Meos();

    /**
     * @brief Finalize MEOS library, free the timezone cache
     */
    ~Meos();

    class SpatioTemporalBox {
    public:
        /**
         * @brief Create SpatioTemporal from WKT string
         * @param wkt_string String in format "SRID=4326;SpatioTemporal X((3.5, 50.5),(4.5, 51.5))"
         */
        explicit SpatioTemporalBox(const std::string& wkt_string);
        ~SpatioTemporalBox();


    private:
        void* stbox_ptr;
    };

    class TemporalInstant {
    public:
        explicit TemporalInstant(double lon, double lat, long long ts, int srid=4326);
        ~TemporalInstant();

        bool intersects(const TemporalInstant& point) const;

    private:
        Temporal* instant;
    };

    class StaticGeometry;
    class TemporalGeometry;
    class StaticGeometry {
    public:
        explicit StaticGeometry(const std::string& wkt_string);
        ~StaticGeometry();

        GSERIALIZED* getGeometry() const;

        int containsTemporal(const TemporalGeometry& temporal_geom) const;
        // int coversTemporal(const TemporalGeometry& temporal_geom) const;

    private:
        GSERIALIZED* geometry;
    };

    class TemporalGeometry {
    public:
        explicit TemporalGeometry(const std::string& wkt_string);
        ~TemporalGeometry();

        Temporal* getGeometry() const;

        int intersects(const TemporalGeometry& geom) const;
        int intersectsStatic(const StaticGeometry& static_geom) const;
        
        int aintersects(const TemporalGeometry& geom) const;
        int aintersectsStatic(const StaticGeometry& static_geom) const;

        int contains(const TemporalGeometry& geom) const;
        int containsStatic(const StaticGeometry& static_geom) const;

        // int covers(const TemporalGeometry& geom) const;
        // int coversStatic(const StaticGeometry& static_geom) const;

        // int disjoint(const TemporalGeometry& geom) const;
        // int disjointStatic(const StaticGeometry& static_geom) const;
        
        // int dwithin(const TemporalGeometry& geom, double dist) const;
        // int dwithinStatic(const StaticGeometry& static_geom, double dist) const;

        // int touches(const TemporalGeometry& geom) const;
        // int touchesStatic(const StaticGeometry& static_geom) const;

    private:
        Temporal* geometry;
    };
    
    class TemporalSequence {
    public:
        // Constructor for creating trajectory from multiple points
        explicit TemporalSequence(const std::vector<TemporalInstant*>& instants);
        
        // Constructor for creating trajectory from coordinate arrays
        TemporalSequence(const std::vector<double>& longitudes, 
                        const std::vector<double>& latitudes, 
                        const std::vector<long long>& timestamps, 
                        int srid = 4326);
        
        ~TemporalSequence();

        
        // bool intersects(const TemporalInstant& point) const;
        // double distance(const TemporalInstant& point) const;
        double length(const TemporalInstant& instant) const;

    private:
        Temporal* sequence;
    };


    static std::string convertSecondsToTimestamp(long long seconds);
    
    /**
     * @brief Parse a temporal point string into a MEOS Temporal object
     * @param trajStr String representation of temporal point (e.g., "{Point(1.0 2.0)@2023-01-01 00:00:00}")
     * @return Void pointer to Temporal object, nullptr on failure. Caller must free with freeTemporalObject()
     */
    static void* parseTemporalPoint(const std::string& trajStr);
    
    /**
     * @brief Free a MEOS Temporal object
     * @param temporal Pointer to Temporal object to free
     */
    static void freeTemporalObject(void* temporal);
    
    /**
     * @brief Convert a Temporal object to WKB format
     * @param temporal Pointer to Temporal object
     * @param size Output parameter for the size of the WKB data
     * @return Pointer to WKB data, nullptr on failure. Caller must free the returned pointer.
     */
    static uint8_t* temporalToWKB(void* temporal, size_t& size);
    
    /**
     * @brief Ensure MEOS is initialized
     */
    static void ensureMeosInitialized();
    
    bool finalized=false;

};

}
#endif // NES_PLUGINS_MEOS_HPP