
#include<vector>
#include<iostream>
#include "../include/stb_image.h"
#include<glm/glm.hpp>
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
