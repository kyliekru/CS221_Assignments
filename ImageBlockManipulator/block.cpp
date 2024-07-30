/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include "block.h"

/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
**/
void Block::Build(PNG& img, unsigned int x, unsigned int y, unsigned int dimension) {
	data.resize(dimension, vector<RGBAPixel>(dimension));

	for (unsigned int i = 0; i < dimension; i++) {
		for (unsigned int j = 0; j < dimension; j++) {
			data[i][j] = *img.getPixel(x + i, y + j);
		}
	}
    
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 * 
 * @param full - whether to render the entire contents to the target PNG,
 *               or the average color to a single pixel in the target PNG.
**/
void Block::Render(PNG& img, unsigned int x, unsigned int y, bool full) const {
	if (full) {
		for (unsigned char i = 0; i < data.size(); i++) {
			for (unsigned char j = 0; j < data[i].size(); j++) {
				*img.getPixel(x + i, y + j) = data[i][j];
			}
		}
	} else {
		RGBAPixel avgPixel = GetAverageColor();
		*img.getPixel(x, y) = avgPixel;
	}
    
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
**/
void Block::FlipHorizontal() {
	if (!data.empty()) {
        unsigned int numRows = data.size();
        unsigned int numCols = data[0].size();

        for (unsigned int j = 0; j < numCols; j++) {
            for (unsigned int i = 0; i < numRows / 2; i++) {
                RGBAPixel temp = data[i][j];
                data[i][j] = data[numRows - 1 - i][j];
                data[numRows - 1 - i][j] = temp;
            }
        }
    }
}

/**
 * Flips the Block's pixel data vertically by a simple reflection
 * across the horizontal axis through its middle.
**/
void Block::FlipVertical() {
	if (!data.empty()) {
        unsigned int numRows = data.size();
        unsigned int numCols = data[0].size();

        for (unsigned int i = 0; i < numRows; i++) {
            for (unsigned int j = 0; j < numCols / 2; j++) {
                RGBAPixel temp = data[i][j];
                data[i][j] = data[i][numCols - 1 - j];
                data[i][numCols - 1 - j] = temp;
            }
        }
    }
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const {
	// replace the statement below with your implementation
	return data.size();
}

/**
 * Returns the average color of the pixels in the block's data.
**/
RGBAPixel Block::GetAverageColor() const {
	// replace the statement below with your implementation
	double totalRed = 0;
	double totalBlue = 0;
	double totalGreen = 0;
	double totalAlpha = 0;
	int pixelCount = 0;

		for (unsigned int i=0; i < data.size(); i++) {
		for (unsigned int j=0; j < data[i].size(); j++) {
			totalRed += data[i][j].r;
			totalBlue += data[i][j].b;
			totalGreen += data[i][j].g;
			totalAlpha += data[i][j].a;
			pixelCount++;
		}
	} 
	
	unsigned char avgRed = static_cast<unsigned char>(totalRed / pixelCount);
	unsigned char avgBlue = static_cast<unsigned char>(totalBlue / pixelCount);
	unsigned char avgGreen = static_cast<unsigned char>(totalGreen / pixelCount);
	double avgAlpha = static_cast<unsigned char>(totalAlpha / pixelCount);

	RGBAPixel avgPixel(avgRed, avgGreen, avgBlue, avgAlpha);
	return avgPixel;

}

/**
 * Replaces all pixel data in the block with the block's average color.
**/
void Block::FillAverage() {
	RGBAPixel avgPixel = GetAverageColor();

	for (unsigned int i = 0; i < data.size(); i++) {
		for (unsigned int j = 0; j < data[i].size(); j++) {
			data[i][j] = avgPixel;
		}
	}
    
}