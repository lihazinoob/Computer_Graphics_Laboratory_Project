#pragma once
#include<vector>
void generateUnitCubeUsingPositionAndNormal(std::vector<float>& vertices, std::vector<unsigned int>& indices);


void generateTerrainFromHeightMap(
    const char* heightMapPath,
    float yScale,
    float yShift,
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices,
    std::vector<float>& heightData,
    int& imageWidth,
    int& imageHeight
);
void generateCylinder(
    float radius,
    float height,
    int sectorCount,
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices
);

void generateTorus(
    float mainRadius,
    float tubeRadius,
    int mainSegments,
    int tubeSegments,
    std::vector<float>&vertices,
    std::vector<unsigned int>& indices
);

void generateCone(
    float radius,
    float height,
    int sectorCount,
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices
);

