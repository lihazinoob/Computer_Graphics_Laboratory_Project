#pragma once
#include<vector>
void generateUnitCubeUsingPositionAndNormal(std::vector<float>& vertices, std::vector<unsigned int>& indices);

void generateTerrainFromHeightMap(const char* heightMapPath, float yScale, float yShift, std::vector<float>& vertices, std::vector<unsigned int>&indices);


