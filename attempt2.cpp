#include <iostream>  // Include the iostream library for input/output
#include <limits>    // Include the limits library to use numeric limits
#include <algorithm> // Include the algorithm library for potential future use
using namespace std;

int main() {
    // Print the initial message
    cout << "Box Size Detector" << endl;
    
    // Ask the user for the shape of the product
    cout << "What is Shape of the product: " << endl;
    cout << "1. Cuboid \n2. Cylinder \n3. Sphere \n4. Cube" << endl;
    
    // Variable to store user's choice
    int choice;
    cin >> choice;
    
    // Prompt to enter dimensions of the product
    cout << "Enter Dimensions of the Product to be Shipped: " << endl;
    
    // Variables to store dimensions of the product
    int len = 0, br = 0, ht = 0, rd = 0;
    
    // Use switch-case to handle different shapes and input dimensions accordingly
    switch (choice) {
        case 1:
            // For Cuboid
            cout << "Enter the Length: ";
            cin >> len;
            cout << "Enter the Breadth: ";
            cin >> br;
            cout << "Enter the Height: ";
            cin >> ht;
            break;
        case 4:
            // For Cube
            cout << "Enter the Length: ";
            cin >> len;
            br = len;
            ht = len;
            break;
        case 2:
            // For Cylinder
            cout << "Enter the Height: ";
            cin >> ht;
            cout << "Enter the Radius: ";
            cin >> rd;
            len = rd * 2;
            br = rd * 2;
            break;
        case 3:
            // For Sphere
            cout << "Enter the Radius: ";
            cin >> rd;
            len = rd * 2;
            br = rd * 2;
            ht = rd * 2;
            break;
    }

    // Check if dimensions are valid (positive values)
    if (len <= 0 || br <= 0 || ht <= 0) {
        cout << "Invalid dimensions provided. Please enter positive values." << endl;
        return 1;
    }

    // Prompt to enter the number of products to be shipped
    cout << "\nEnter the Number of products to be Shipped: ";
    int quant;
    cin >> quant;
    
    // Check if the quantity is valid (positive value)
    if (quant <= 0) {
        cout << "Invalid quantity. Please enter a positive number." << endl;
        return 1;
    }

    // Inventory of available carton sizes
    int inventory[3][5] = {
        {10, 12, 14, 16, 18}, // Lengths
        {8, 10, 12, 14, 16},  // Breadths
        {6, 8, 10, 12, 14}    // Heights
    };

    // Variables to keep track of the best carton configuration
    int min_outer_cartons = numeric_limits<int>::max();
    int best_carton_index = -1;
    int best_orientation = 0;
    int best_fit_lengthwise = 0, best_fit_breadthwise = 0, best_fit_heightwise = 0;

    // Iterate over each available carton size
    // Iterate over each available carton size
    for (int i = 0; i < 5; ++i) {
        // Check three different orientations
        for (int o = 1; o <= 3; ++o) {
            int num_boxes_lengthwise = 0, num_boxes_breadthwise = 0, num_boxes_heightwise = 0;

            // Adjust dimension comparisons based on orientation
            if (o == 1) { // Lengthwise
                num_boxes_lengthwise = (inventory[0][i] > len + 2) ? (inventory[0][i] - 2) / len : 0;
                num_boxes_breadthwise = (inventory[1][i] > br + 2) ? (inventory[1][i] - 2) / br : 0;
                num_boxes_heightwise = (inventory[2][i] > ht + 2) ? (inventory[2][i] - 2) / ht : 0;
            } else if (o == 2) { // Breadthwise
                num_boxes_lengthwise = (inventory[0][i] > br + 2) ? (inventory[0][i] - 2) / br : 0;
                num_boxes_breadthwise = (inventory[1][i] > len + 2) ? (inventory[1][i] - 2) / len : 0;
                num_boxes_heightwise = (inventory[2][i] > ht + 2) ? (inventory[2][i] - 2) / ht : 0;
            } else if (o == 3) { // Heightwise
                num_boxes_lengthwise = (inventory[0][i] > ht + 2) ? (inventory[0][i] - 2) / ht : 0;
                num_boxes_breadthwise = (inventory[1][i] > len + 2) ? (inventory[1][i] - 2) / len : 0;
                num_boxes_heightwise = (inventory[2][i] > br + 2) ? (inventory[2][i] - 2) / br : 0;
            }

            // Skip if any dimension cannot fit the product
            if (num_boxes_lengthwise == 0 || num_boxes_breadthwise == 0 || num_boxes_heightwise == 0) {
                continue; // Skip this carton as it cannot fit the product in the current orientation
            }

            // Calculate the maximum number of products that can be packed based on orientation
            int max_products_packed = num_boxes_lengthwise * num_boxes_breadthwise * num_boxes_heightwise;

            // Calculate the number of cartons needed
            int needed_cartons = (quant + max_products_packed - 1) / max_products_packed;
            if (needed_cartons < min_outer_cartons && max_products_packed > 0) {
                // Update the best carton configuration if this one is better
                min_outer_cartons = needed_cartons;
                best_carton_index = i;
                best_orientation = o;
                best_fit_lengthwise = num_boxes_lengthwise;
                best_fit_breadthwise = num_boxes_breadthwise;
                best_fit_heightwise = num_boxes_heightwise;
            }
        }
    }

    // Check if a suitable carton was found
    if (min_outer_cartons == numeric_limits<int>::max()) {
        cout << "No suitable carton found. Please check the dimensions and try again." << endl;
        return 1;
    }

    // Output the optimal packing strategy
    cout << "Optimal Packing Strategy:" << endl;
    cout << "Best orientation for packing: ";
    if (best_orientation == 1) {
        cout << "Length-wise" << endl;
        cout << "Products per row (lengthwise): " << best_fit_lengthwise << endl;
        cout << "Products per column (breadthwise): " << best_fit_breadthwise << endl;
        cout << "Products per stack (heightwise): " << best_fit_heightwise << endl;
    } else if (best_orientation == 2) {
        cout << "Breadth-wise" << endl;
        cout << "Products per row (breadthwise): " << best_fit_breadthwise << endl;
        cout << "Products per column (lengthwise): " << best_fit_lengthwise << endl;
        cout << "Products per stack (heightwise): " << best_fit_heightwise << endl;
    } else {
        cout << "Height-wise" << endl;
        cout << "Products per row (heightwise): " << best_fit_heightwise << endl;
        cout << "Products per column (lengthwise): " << best_fit_lengthwise << endl;
        cout << "Products per stack (breadthwise): " << best_fit_breadthwise << endl;
    }

    // Output the best carton size and number of cartons required
    cout << "Best carton size: " << inventory[0][best_carton_index] << " x " << inventory[1][best_carton_index] << " x " << inventory[2][best_carton_index] << endl;
    cout << "Number of cartons required: " << min_outer_cartons << endl;
    cout << "All products have been packed." << endl;
    cout << "1 inch of space has been left through the height, width, and length for you to stuff packing." << endl;

    return 0;
}
