#pragma once
#include<vector>
float getTerrainHeight(float worldX, float worldZ, const std::vector<float>& heightData, int width, int height, float scale);
unsigned int loadTexture2D(const char* texturePath, bool flipVertically = true);
