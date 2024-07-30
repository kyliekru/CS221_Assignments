# Image Block Manipulator

This project is a C++ program designed to manipulate images by dividing them into blocks, performing various transformations on these blocks, and reassembling them into a final image. The program leverages classes such as `Block` and `Chain` for handling and processing image data.

## Features

- **Block Division and Manipulation**: The `Block` class divides images into smaller blocks and provides functionalities to manipulate these blocks, such as flipping them horizontally or vertically and averaging colors.
- **Chaining Blocks**: The `Chain` class manages sequences of blocks, allowing for complex manipulations and reassembling them into a final image.
- **Rendering**: Reassemble manipulated blocks into a final image.

## Dependencies

- Standard C++ libraries
- PNG image handling via `cs221util::PNG`
