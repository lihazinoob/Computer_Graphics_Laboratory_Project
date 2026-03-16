#include<vector>

// Mathematical function to get terrain height at any X,Z position
float getTerrainHeight(float worldX, float worldZ, const std::vector<float>& heightData, int width, int height, float scale) {
    // 1. Reverse the scaling applied by the model matrix
    float localX = worldX / scale;
    float localZ = worldZ / scale;

    // 2. Reverse the origin centering (vx = x - width/2.0)
    float gridX = localX + width / 2.0f;
    float gridZ = localZ + height / 2.0f;

    // 3. Boundary check (if off the grid, stay at height 0)
    if (gridX < 0 || gridX >= width - 1 || gridZ < 0 || gridZ >= height - 1) {
        return 0.0f;
    }

    // 4. Find which grid square we are in
    int xInt = (int)gridX;
    int zInt = (int)gridZ;

    // 5. Find exactly where we are inside that square (fraction between 0.0 and 1.0)
    float dx = gridX - xInt;
    float dz = gridZ - zInt;

    // 6. Get the height of the 4 corners of this specific square
    float hTopLeft = heightData[zInt * width + xInt];
    float hTopRight = heightData[zInt * width + (xInt + 1)];
    float hBottomLeft = heightData[(zInt + 1) * width + xInt];
    float hBottomRight = heightData[(zInt + 1) * width + (xInt + 1)];

    float localHeight = 0.0f;

    // 7. Barycentric Interpolation (Determine which of the 2 triangles we are over)
    if (dx + dz <= 1.0f) {
        // Top-Left Triangle
        localHeight = hTopLeft + dx * (hTopRight - hTopLeft) + dz * (hBottomLeft - hTopLeft);
    }
    else {
        // Bottom-Right Triangle
        localHeight = hBottomRight + (1.0f - dx) * (hBottomLeft - hBottomRight) + (1.0f - dz) * (hTopRight - hBottomRight);
    }

    // 8. Re-apply the scale to get actual world height
    return localHeight;
}