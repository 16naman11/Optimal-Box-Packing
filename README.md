# Optimal-Box-Packing
A C++ program for optimal packing of products in export trading. The software determines the best fitting outer carton for different shaped products (cuboid, cylinder, sphere, cube) while minimizing space and ensuring protection. Features customizable carton sizes and efficient packing algorithms.

# Optimal Product Packing Software

## Description

This C++ program is designed for optimal packing of products in an export trading company. It determines the best fitting outer carton for various shaped products (cuboid, cylinder, sphere, cube) while minimizing the number of cartons used and ensuring that there is sufficient space for packing materials. The software features customizable carton sizes and efficient packing algorithms to handle various product dimensions and quantities.

## Features

- Supports multiple product shapes: cuboid, cylinder, sphere, and cube.
- Customizable inventory of outer carton sizes.
- Ensures at least 1 inch of space around products for packing materials.
- Calculates the optimal number of cartons required for given product dimensions and quantities.
- Determines the best orientation for packing products within the cartons.


## Usage

1. Run the program:
    ```sh
    ./packing
    ```
2. Follow the prompts to enter the shape and dimensions of the product.
3. Enter the number of products to be shipped.
4. The program will output the optimal packing strategy, including the best orientation, the size of the cartons, and the number of cartons required.

## Example

```sh
Box Size Detector
What is Shape of the product: 
1. Cuboid 
2. Cylinder 
3. Sphere 
4. Cube
3
Enter Dimensions of the Product to be Shipped: 
Enter the Radius: 2

Enter the Number of products to be Shipped: 23
Optimal Packing Strategy:
Best orientation for packing: Length-wise
Products per row (lengthwise): 4
Products per column (breadthwise): 3
Products per stack (heightwise): 3
Best carton size: 18 x 16 x 14
Number of cartons required: 1
All products have been packed.
1 inch of space has been left through the height, width, and length for you to stuff packing.
