/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include <math.h>
#include <iostream>

#include "chain.h"

/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
**/
Chain::Chain(PNG& img, unsigned int nodedimension) {


	unsigned int cols = img.width() / nodedimension;
	unsigned int rows = img.height() / nodedimension;

	if (img.width() % nodedimension != 0 || img.height() % nodedimension != 0) {
        NW = nullptr;
        SE = nullptr;
        length_ = 0;
        return;
	}
    std::cout << "line 28" << std::endl;
	length_ = 0;
	NW = nullptr;
	SE = nullptr;

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			Block block;
			block.Build(img, c * nodedimension, r * nodedimension, nodedimension);
			InsertBack(block);
		}
	}
    
}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 * @param full whether to render all pixel data into a full-size image,
 *             or block averages into a tiny image
**/
PNG Chain::Render(unsigned int cols, bool full) {
    if (NW == nullptr || length_ == 0) {
        return PNG();
    }

    if (cols == 0 || cols > length_) {
        return PNG();
    }

	unsigned int rows = (length_ + cols - 1)/ cols;
	unsigned int blockWidth = NW->data.Dimension();
	unsigned int blockHeight = NW->data.Dimension();
	unsigned int width = full ? cols * blockWidth : cols;
	unsigned int height = full ? rows * blockHeight : rows;

	PNG output(width, height);


	Node* curr = NW;
	unsigned int x = 0;
	unsigned int y = 0;

	for (unsigned int row = 0; row < rows; row++) {
		for (unsigned int col = 0; col < cols; col++) {
			if (curr != nullptr) {
				curr->data.Render(output, x, y, full);
				curr = curr->next;
			}
			x += full ? blockWidth : 1;
		}
		x = 0;
		y += full ? blockHeight : 1;
	}

	return output;
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
**/
void Chain::InsertBack(const Block& ndata) {
    Node* newNode = new Node(ndata);

    if (length_ == 0) {
   
        NW = newNode;
        SE = newNode;
    } else {

        SE->next = newNode;
        newNode->prev = SE;
        SE = newNode;
    }

    length_++;
}


/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *  
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
**/
void Chain::Reverse() {
    if (NW == nullptr || length_ == 0) {
        return;
    }

    Node* currentNode = NW;
    while (currentNode != nullptr) {

        Node* temp = currentNode->next;
        currentNode->next = currentNode->prev;
        currentNode->prev = temp;


        currentNode = currentNode->prev; 
    }

    
    Node* temp = NW;
    NW = SE;
    SE = temp;
}


/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 columns (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/
void Chain::FlipHorizontal(unsigned int cols) {
    if (NW == nullptr || length_ == 0) {
        return;
    }

    if (cols < 2 || length_ < cols || length_ % cols != 0) {
        return;
    }

    unsigned int rows = length_ / cols;

    
    Node** nodeArray = new Node*[length_];
    Node* curr = NW;
    for (unsigned int i = 0; i < length_; ++i) {
        nodeArray[i] = curr;
        curr = curr->next;
    }

    
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < cols / 2; ++col) {
            unsigned int leftIndex = row * cols + col;
            unsigned int rightIndex = row * cols + (cols - 1 - col);

            
            Node* tempNode = nodeArray[leftIndex];
            nodeArray[leftIndex] = nodeArray[rightIndex];
            nodeArray[rightIndex] = tempNode;

            for (unsigned int col = 0; col < cols; ++col) {
                unsigned int index = row * cols + col;
                nodeArray[index]->data.FlipHorizontal();
            }
        }
    }

    
    for (unsigned int i = 0; i < length_; ++i) {
        if (i > 0) {
            nodeArray[i]->prev = nodeArray[i - 1];
        } else {
            nodeArray[i]->prev = nullptr;
            NW = nodeArray[i]; 
        }
        if (i < length_ - 1) {
            nodeArray[i]->next = nodeArray[i + 1];
        } else {
            nodeArray[i]->next = nullptr;
            SE = nodeArray[i];  
        }
    }

    delete[] nodeArray;
    
}


/**
 * Rearranges the Node structure and internal pixel data to be flipped over a horizontal axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, flipping with 3 columns (2 rows):
 *                         
 *      NW -> A <-> B <-> C <->
 *            D <-> E <-> F <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *      NW -> D <-> E <-> F <->
 *            A <-> B <-> C <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/
void Chain::FlipVertical(unsigned int cols) {
    if (NW == nullptr || length_ == 0) {
        return;
    }

    if (length_ < cols || cols == 0 || length_ % cols != 0) {
        return;
    }


    unsigned int rows = length_ / cols;

    
    Node** nodeArray = new Node*[length_];
    Node* curr = NW;
    for (unsigned int i = 0; i < length_; ++i) {
        nodeArray[i] = curr;
        curr = curr->next;
    }

    
    for (unsigned int col = 0; col < cols; ++col) {
        for (unsigned int row = 0; row < rows / 2; ++row) {
            unsigned int topIndex = row * cols + col;
            unsigned int botIndex = (rows - 1 - row) * cols + col;

            
            Node* tempNode = nodeArray[topIndex];
            nodeArray[topIndex] = nodeArray[botIndex];
            nodeArray[botIndex] = tempNode;

            nodeArray[topIndex]->data.FlipVertical();
            nodeArray[botIndex]->data.FlipVertical();
        }
    }

    
    for (unsigned int i = 0; i < length_; ++i) {
        if (i > 0) {
            nodeArray[i]->prev = nodeArray[i - 1];
        } else {
            nodeArray[i]->prev = nullptr;
            NW = nodeArray[i];  
        }
        if (i < length_ - 1) {
            nodeArray[i]->next = nodeArray[i + 1];
        } else {
            nodeArray[i]->next = nullptr;
            SE = nodeArray[i];  
        }
    }

    delete[] nodeArray; 
}


/**
 * Produces a blocky, pixellated effect (in a rendered image) by replacing each
 * block's pixel data with the average color of the block.
**/
void Chain::Blockify() {
    if (NW == nullptr || length_ == 0) {
        return;
    }
    Node* curr = NW;
    while (curr != nullptr) {
        curr->data.FillAverage();
        curr = curr->next;
    }
}


/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
**/
void Chain::Clear() {
    if (NW == nullptr) {
        SE = nullptr;
        length_ = 0;
        return;
    }
    Node* curr = NW;
    while (curr != nullptr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp; 
    }
    NW = nullptr;
    SE = nullptr;
    length_ = 0;
}



/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
**/
void Chain::Copy(const Chain& other) {
    if (other.NW == nullptr || other.length_ == 0) {
        NW = nullptr;
        SE = nullptr;
        length_ = 0;
        return;
    }
    
    length_ = other.length_;

    Node* otherCurr = other.NW;
    Node* thisCurr = new Node(otherCurr->data);
    NW = thisCurr;

    otherCurr = otherCurr->next;

    while (otherCurr != nullptr) {
        Node* newNode = new Node(otherCurr->data);
        thisCurr->next = newNode;
        newNode->prev = thisCurr;
        thisCurr = newNode;
        otherCurr = otherCurr->next;
    }

    SE = thisCurr;
}




/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
**/

