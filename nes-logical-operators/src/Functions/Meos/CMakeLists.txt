# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#    https://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

add_plugin(Intersect LogicalFunction nes-logical-operators IntersectLogicalFunction.cpp)
add_plugin(TemporalIntersects LogicalFunction nes-logical-operators TemporalIntersectsFunction.cpp)
add_plugin(TemporalIntersectsGeometry LogicalFunction nes-logical-operators TemporalIntersectsGeometryLogicalFunction.cpp)
add_plugin(TemporalAIntersectsGeometry LogicalFunction nes-logical-operators TemporalAIntersectsGeometryLogicalFunction.cpp)
add_plugin(EintersectsTgeoTgeo LogicalFunction nes-logical-operators EintersectsTgeoTgeoLogicalFunction.cpp)

add_plugin(EintersectsTgeoGeo LogicalFunction nes-logical-operators EintersectsTgeoGeoLogicalFunction.cpp)

add_plugin(AintersectsTgeoTgeo LogicalFunction nes-logical-operators AintersectsTgeoTgeoLogicalFunction.cpp)

add_plugin(AintersectsTgeoGeo LogicalFunction nes-logical-operators AintersectsTgeoGeoLogicalFunction.cpp)

add_plugin(AcontainsGeoTgeo LogicalFunction nes-logical-operators AcontainsGeoTgeoLogicalFunction.cpp)

add_plugin(AcontainsTgeoTgeo LogicalFunction nes-logical-operators AcontainsTgeoTgeoLogicalFunction.cpp)

add_plugin(AcontainsTgeoGeo LogicalFunction nes-logical-operators AcontainsTgeoGeoLogicalFunction.cpp)

add_plugin(AdisjointTgeoGeo LogicalFunction nes-logical-operators AdisjointTgeoGeoLogicalFunction.cpp)

add_plugin(AdisjointTgeoTgeo LogicalFunction nes-logical-operators AdisjointTgeoTgeoLogicalFunction.cpp)

add_plugin(AtouchesTgeoGeo LogicalFunction nes-logical-operators AtouchesTgeoGeoLogicalFunction.cpp)

add_plugin(AtouchesTgeoTgeo LogicalFunction nes-logical-operators AtouchesTgeoTgeoLogicalFunction.cpp)

add_plugin(AtouchesTpointGeo LogicalFunction nes-logical-operators AtouchesTpointGeoLogicalFunction.cpp)

add_plugin(EtouchesTpointGeo LogicalFunction nes-logical-operators EtouchesTpointGeoLogicalFunction.cpp)

add_plugin(EtouchesTgeoGeo LogicalFunction nes-logical-operators EtouchesTgeoGeoLogicalFunction.cpp)

add_plugin(EtouchesTgeoTgeo LogicalFunction nes-logical-operators EtouchesTgeoTgeoLogicalFunction.cpp)

add_plugin(EdwithinTgeoGeo LogicalFunction nes-logical-operators EdwithinTgeoGeoLogicalFunction.cpp)

add_plugin(AdwithinTgeoGeo LogicalFunction nes-logical-operators AdwithinTgeoGeoLogicalFunction.cpp)

add_plugin(AdwithinTgeoTgeo LogicalFunction nes-logical-operators AdwithinTgeoTgeoLogicalFunction.cpp)

add_plugin(EcontainsGeoTgeo LogicalFunction nes-logical-operators EcontainsGeoTgeoLogicalFunction.cpp)

add_plugin(EcontainsTgeoGeo LogicalFunction nes-logical-operators EcontainsTgeoGeoLogicalFunction.cpp)

add_plugin(EcontainsTgeoTgeo LogicalFunction nes-logical-operators EcontainsTgeoTgeoLogicalFunction.cpp)

add_plugin(EcoversGeoTgeo LogicalFunction nes-logical-operators EcoversGeoTgeoLogicalFunction.cpp)

add_plugin(EcoversTgeoTgeo LogicalFunction nes-logical-operators EcoversTgeoTgeoLogicalFunction.cpp)

add_plugin(EcoversTgeoGeo LogicalFunction nes-logical-operators EcoversTgeoGeoLogicalFunction.cpp)

add_plugin(EdisjointTgeoGeo LogicalFunction nes-logical-operators EdisjointTgeoGeoLogicalFunction.cpp)

add_plugin(EdisjointTgeoTgeo LogicalFunction nes-logical-operators EdisjointTgeoTgeoLogicalFunction.cpp)

add_plugin(EdwithinTgeoTgeo LogicalFunction nes-logical-operators EdwithinTgeoTgeoLogicalFunction.cpp)
