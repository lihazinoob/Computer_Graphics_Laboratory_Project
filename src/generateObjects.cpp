
#include<vector>
#include<iostream>
#include "../include/stb_image.h"
#include<glm/glm.hpp>


const float PI = 3.14159265359f;


// Function to generate a Unit-Cube using harcoded vertices
// Here each vertice will contain the info of point corrdinate and normal coordinate
void generateUnitCubeUsingPositionAndNormal(
    std::vector<float>& vertices, 
    std::vector<unsigned int>& indices
) {
    float data[] = {
        // Back face  (normal: 0,0,-1)
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        // Front face (normal: 0,0,1)
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        // Left face  (normal: -1,0,0)
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        // Right face (normal: 1,0,0)
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         // Bottom face (normal: 0,-1,0)
         -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
         // Top face    (normal: 0,1,0)
         -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
    };

    unsigned int idx[] = {
        0,1,2,  2,3,0,       // back
        4,5,6,  6,7,4,       // front
        8,9,10, 10,11,8,     // left
        12,13,14, 14,15,12,  // right
        16,17,18, 18,19,16,  // bottom
        20,21,22, 22,23,20   // top
    };

    int verticesArraySize = sizeof(data) / sizeof(float);
    int indicesArraySize = sizeof(idx) / sizeof(unsigned int);

    // Assign this value to the incoming vector from the calling function
    vertices.assign(data, data + verticesArraySize);
    indices.assign(idx, idx + indicesArraySize);
}

void generateTerrainFromHeightMap(
    const char* heightMapPath,
    float yScale,
    float yShift,
    std::vector<float>&vertices,
    std::vector<unsigned int>&indices,
    std::vector<float>& heightData,
    int& imageWidth,
    int& imageHeight

) {


    int width, height, numberOfChannels;
    // Load the image using the stbi library
    unsigned char* data =  stbi_load(heightMapPath, &width, &height, &numberOfChannels, 0);
    if (!data) {
        std::cout << "Failed to load the heightmap: " << heightMapPath;
        return;
    }
    imageHeight = height;
    imageWidth = width;

    // Now the render through the heightMap. Number of vertices of the terrain = height * width

    //Generate Vertices
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            // Extract the height from the image.Using the first channel will be enough as for the grayscale image the values are same in all channels
            unsigned char* texels = data + (z * width + x) * numberOfChannels;
            float y = (texels[0] / 255.0f) * yScale - yShift;

            heightData.push_back(y);

            // Center the grid around origin (0,0,0) in world space
            float vx = x - width / 2.0f;
            float vz = z - height / 2.0f;

            // Calculate the Normal Using Central Finite Difference Method
            // Read neighboring heights to compute slopes. If at the edge, mirror the current height.
            float hL = (x > 0) ? ((data[(z * width + (x - 1)) * numberOfChannels] / 255.0f) * yScale - yShift) : y;
            float hR = (x < width - 1) ? ((data[(z * width + (x + 1)) * numberOfChannels] / 255.0f) * yScale - yShift) : y;
            float hD = (z > 0) ? ((data[((z - 1) * width + x) * numberOfChannels] / 255.0f) * yScale - yShift) : y;
            float hU = (z < height - 1) ? ((data[((z + 1) * width + x) * numberOfChannels] / 255.0f) * yScale - yShift) : y;

            // Slope vectors
            glm::vec3 norm = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));

            // Push back to the incoming vector vertices with this vx,y and vz and normals
            vertices.push_back(vx);
            vertices.push_back(y);
            vertices.push_back(vz);
            vertices.push_back(norm.x);
            vertices.push_back(norm.y);
            vertices.push_back(norm.z);
        }   
    }
    stbi_image_free(data);

    // Generate the indices. In that order the square made up of two traingle will be created
    // Loop through all the points from 0....width-1 and 0....height - 1
    // For every point I need to grab four points
    // 1-----2
    // |     |
    // |     |
    // 3-----4
    // and create the order of the indices in cc manner(1 -> 3 -> 2 and 2 -> 3 -> 4)
    
    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int topLeft = (z * width) + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = ((z + 1) * width) + x;
            unsigned int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            //Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);

        }
    }

}


void generateCylinder(float radius, float height, int sectorCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    // Clear the vectors just in case they have old data
    vertices.clear();
    indices.clear();

    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // ==========================================
    // 1. GENERATE VERTICES (Positions & Normals)
    // ==========================================

    // A. Side Vertices
    for (int i = 0; i <= sectorCount; ++i) {
        sectorAngle = i * sectorStep;

        // Calculate the X and Z coordinates using basic trig
        float x = radius * cos(sectorAngle);
        float z = radius * sin(sectorAngle);

        // The normal for the side points straight out from the center
        float nx = cos(sectorAngle);
        float ny = 0.0f;
        float nz = sin(sectorAngle);

        // Top edge vertex (y = height / 2)
        vertices.push_back(x); vertices.push_back(height / 2.0f); vertices.push_back(z);
        vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);

        // Bottom edge vertex (y = -height / 2)
        vertices.push_back(x); vertices.push_back(-height / 2.0f); vertices.push_back(z);
        vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);
    }

    // B. Top and Bottom Center Vertices (for the caps)
    // Top center index will be: (sectorCount + 1) * 2
    int topCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f); vertices.push_back(height / 2.0f); vertices.push_back(0.0f);
    vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f); // Normal points UP

    // Bottom center index will be: topCenterIndex + 1
    int bottomCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f); vertices.push_back(-height / 2.0f); vertices.push_back(0.0f);
    vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f); // Normal points DOWN

    // ==========================================
    // 2. GENERATE INDICES (Connecting the dots)
    // ==========================================

    // A. Side Triangles
    for (int i = 0; i < sectorCount; ++i) {
        int k1 = i * 2;       // Top vertex of current sector
        int k2 = k1 + 1;      // Bottom vertex of current sector
        int k3 = k1 + 2;      // Top vertex of NEXT sector
        int k4 = k1 + 3;      // Bottom vertex of NEXT sector

        // Triangle 1 (Top-Left, Bottom-Left, Top-Right)
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k3);

        // Triangle 2 (Top-Right, Bottom-Left, Bottom-Right)
        indices.push_back(k3);
        indices.push_back(k2);
        indices.push_back(k4);
    }

    // B. Top Cap Triangles
    for (int i = 0; i < sectorCount; ++i) {
        int k1 = i * 2;       // Edge vertex of current sector
        int k2 = (i + 1) * 2; // Edge vertex of NEXT sector

        indices.push_back(topCenterIndex);
        indices.push_back(k1);
        indices.push_back(k2);
    }

    // C. Bottom Cap Triangles
    for (int i = 0; i < sectorCount; ++i) {
        int k1 = i * 2 + 1;       // Edge vertex of current sector
        int k2 = (i + 1) * 2 + 1; // Edge vertex of NEXT sector

        // Note: Winding order is reversed here (k2 then k1) so the bottom faces outward!
        indices.push_back(bottomCenterIndex);
        indices.push_back(k2);
        indices.push_back(k1);
    }
}


void generateTorus(
    float mainRadius,
    float tubeRadius,
    int mainSegments,
    int tubeSegments,
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices
) {
    vertices.clear();
    indices.clear();

    float mainStep = 2.0f * PI / mainSegments;
    float tubeStep = 2.0f * PI / tubeSegments;

    for(int i = 0; i <= mainSegments; i++) {
        float u = i * mainStep;
        float cosU = cos(u);
        float sinU = sin(u);
        for (int j = 0; j <= tubeSegments; j++) {
            float v = j * tubeStep;
            float cosV = cos(v);
            float sinV = sin(v);

            // Using the formula of creating the torus calculate x, y and z
            float x = (mainRadius + tubeRadius * cosV) * cosU;
            float y = tubeRadius * sinV;
            float z = (mainRadius + tubeRadius * cosV) * sinU;

            float nx = cosV * cosU;
            float ny = sinV;
            float nz = cosV * sinU;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }



    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    for (int i = 0; i < mainSegments; ++i) {
        for (int j = 0; j < tubeSegments; ++j) {
            int k1 = i * (tubeSegments + 1) + j;
            int k2 = k1 + tubeSegments + 1;
            int k3 = k1 + 1;
            int k4 = k2 + 1;

            // Triangle 1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k3);

            // Triangle 2
            indices.push_back(k3);
            indices.push_back(k2);
            indices.push_back(k4);
        }
    }


}
