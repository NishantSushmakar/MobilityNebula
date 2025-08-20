# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#    https://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

add_plugin(TemporalIntersects PhysicalFunction nes-physical-operators TemporalIntersectsPhysicalFunction.cpp)
add_plugin(TemporalIntersectsGeometry PhysicalFunction nes-physical-operators TemporalIntersectsGeometryPhysicalFunction.cpp)
add_plugin(TemporalAIntersectsGeometry PhysicalFunction nes-physical-operators TemporalAIntersectsGeometryPhysicalFunction.cpp)
add_plugin(EintersectsTgeoTgeo PhysicalFunction nes-physical-operators EintersectsTgeoTgeoPhysicalFunction.cpp)

add_plugin(EintersectsTgeoGeo PhysicalFunction nes-physical-operators EintersectsTgeoGeoPhysicalFunction.cpp)

add_plugin(AintersectsTgeoTgeo PhysicalFunction nes-physical-operators AintersectsTgeoTgeoPhysicalFunction.cpp)

add_plugin(AintersectsTgeoGeo PhysicalFunction nes-physical-operators AintersectsTgeoGeoPhysicalFunction.cpp)

add_plugin(AcontainsGeoTgeo PhysicalFunction nes-physical-operators AcontainsGeoTgeoPhysicalFunction.cpp)

add_plugin(AcontainsTgeoTgeo PhysicalFunction nes-physical-operators AcontainsTgeoTgeoPhysicalFunction.cpp)

add_plugin(AcontainsTgeoGeo PhysicalFunction nes-physical-operators AcontainsTgeoGeoPhysicalFunction.cpp)

add_plugin(AdisjointTgeoGeo PhysicalFunction nes-physical-operators AdisjointTgeoGeoPhysicalFunction.cpp)

add_plugin(AdisjointTgeoTgeo PhysicalFunction nes-physical-operators AdisjointTgeoTgeoPhysicalFunction.cpp)

add_plugin(AtouchesTgeoGeo PhysicalFunction nes-physical-operators AtouchesTgeoGeoPhysicalFunction.cpp)

add_plugin(AtouchesTgeoTgeo PhysicalFunction nes-physical-operators AtouchesTgeoTgeoPhysicalFunction.cpp)

add_plugin(AtouchesTpointGeo PhysicalFunction nes-physical-operators AtouchesTpointGeoPhysicalFunction.cpp)

add_plugin(EtouchesTpointGeo PhysicalFunction nes-physical-operators EtouchesTpointGeoPhysicalFunction.cpp)

add_plugin(EtouchesTgeoGeo PhysicalFunction nes-physical-operators EtouchesTgeoGeoPhysicalFunction.cpp)

add_plugin(EtouchesTgeoTgeo PhysicalFunction nes-physical-operators EtouchesTgeoTgeoPhysicalFunction.cpp)

add_plugin(EdwithinTgeoGeo PhysicalFunction nes-physical-operators EdwithinTgeoGeoPhysicalFunction.cpp)

add_plugin(AdwithinTgeoGeo PhysicalFunction nes-physical-operators AdwithinTgeoGeoPhysicalFunction.cpp)

add_plugin(AdwithinTgeoTgeo PhysicalFunction nes-physical-operators AdwithinTgeoTgeoPhysicalFunction.cpp)

add_plugin(EcontainsGeoTgeo PhysicalFunction nes-physical-operators EcontainsGeoTgeoPhysicalFunction.cpp)

add_plugin(EcontainsTgeoGeo PhysicalFunction nes-physical-operators EcontainsTgeoGeoPhysicalFunction.cpp)

add_plugin(EcontainsTgeoTgeo PhysicalFunction nes-physical-operators EcontainsTgeoTgeoPhysicalFunction.cpp)

add_plugin(EcoversGeoTgeo PhysicalFunction nes-physical-operators EcoversGeoTgeoPhysicalFunction.cpp)

add_plugin(EcoversTgeoTgeo PhysicalFunction nes-physical-operators EcoversTgeoTgeoPhysicalFunction.cpp)

add_plugin(EcoversTgeoGeo PhysicalFunction nes-physical-operators EcoversTgeoGeoPhysicalFunction.cpp)

add_plugin(EdisjointTgeoGeo PhysicalFunction nes-physical-operators EdisjointTgeoGeoPhysicalFunction.cpp)

add_plugin(EdisjointTgeoTgeo PhysicalFunction nes-physical-operators EdisjointTgeoTgeoPhysicalFunction.cpp)

add_plugin(EdwithinTgeoTgeo PhysicalFunction nes-physical-operators EdwithinTgeoTgeoPhysicalFunction.cpp)
